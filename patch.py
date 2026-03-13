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

# The physical address of the very start of the executable
# 0xC00 for the E3 2010's .app
if version == "e3_2010":
    PHYSICAL_BASE_OFFSET = 0xC00
else:
    PHYSICAL_BASE_OFFSET = 0x0

result = subprocess.run([os.environ["DEVKITARM"] + r'/bin/arm-none-eabi-objdump', '--section-headers', elf], stdout=subprocess.PIPE)
lines = str(result.stdout).split('\\n')
sectionsInfo = [line.split()[1:6] for line in lines if line.split() and line.split()[0].isdigit()]
sections = ((sec[0], int(sec[2],16), int(sec[4],16), int(sec[1],16)) for sec in sectionsInfo if int(sec[2],16) != 0)

off = lambda vaddr: struct.pack(">I", (vaddr - 0x100000) + PHYSICAL_BASE_OFFSET)[1:]
sz = lambda size: struct.pack(">H", size)

ips = b'PATCH'
with open(elf, 'rb') as e:
    for name, vaddr, offset, size in sections:

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

print()
