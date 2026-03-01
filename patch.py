import os
import struct
import subprocess
import sys

print()

if len(sys.argv) < 3:
    print("Usage: python patch.py <elf_path> <version>")
    sys.exit(1)

elf = sys.argv[1]
version = sys.argv[2]

result = subprocess.run([os.environ["DEVKITARM"] + r'/bin/arm-none-eabi-objdump', '--section-headers', elf], stdout=subprocess.PIPE)
lines = str(result.stdout).split('\\n')
sectionsInfo = [line.split()[1:6] for line in lines if line.split() and line.split()[0].isdigit()]
sections = ((sec[0], int(sec[2],16), int(sec[4],16), int(sec[1],16)) for sec in sectionsInfo if int(sec[2],16) != 0)

off = lambda vaddr: struct.pack(">I",vaddr - 0x100000)[1:]
sz = lambda size: struct.pack(">H", size)

memoryExtension = 0

ips = b'PATCH'
with open(elf, 'rb') as e:
    for name, vaddr, offset, size in sections:

        if name == '.text':
            memoryExtension += size

        e.seek(offset, 0)
        while size > 65535:
            patch = e.read(65535)
            # print('{:0x} {:0x} {}'.format(vaddr, vaddr + 65535, name))
            ips += off(vaddr)
            ips += sz(65535)
            ips += patch
            vaddr += 65535
            offset += 65535
            size -= 65535

        patch =  e.read(size)
        if len(patch) != 0:
            # print('{:0x} {:0x} {}'.format(vaddr, vaddr + size, name))
            ips += off(vaddr)
            ips += sz(size)
            ips += patch
ips += b'EOF'

with open("code.ips", 'wb') as patchFile:
    patchFile.write(ips)

print("created code.ips")

# Create exheader file by adding code size info to the template
for citra in [False, True]:
    exhTemplatePath = os.path.join('exheader', version, 'exheader_template.bin')
    exhFileName = 'exheader.bin'
    with open(exhTemplatePath, 'rb') as exhTemplate, open(exhFileName, 'wb') as exh:
        exh.write(exhTemplate.read(0x34))

        numDataPages = struct.unpack("<I", exhTemplate.read(4))[0]
        dataSize = struct.unpack("<I", exhTemplate.read(4))[0]
        bssSize = struct.unpack("<I", exhTemplate.read(4))[0]

        # A buffer of 0x1000 bytes is added because the new code is injected at the first
        # multiple of 0x1000 after the BSS section.
        dataSize += bssSize + memoryExtension + 0x1000
        numDataPages = ((dataSize + 0xFFF) & ~0xFFF) >> 0xC
        bssSize = 0

        exh.write(struct.pack("<I", numDataPages))
        exh.write(struct.pack("<I", dataSize))
        exh.write(struct.pack("<I", bssSize))

        exh.write(exhTemplate.read())

    print("created %s" % exhFileName)

print()
