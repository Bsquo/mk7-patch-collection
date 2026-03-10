# The VERSION_* symbols correspond to the defines in `include/versions.h`

ifeq ($(VERSION),chn_dlp)
VERSION_SYM := VERSION_CHN_DLP
endif

ifeq ($(VERSION),chn_rev1)
VERSION_SYM := VERSION_CHN_REV1
endif

ifeq ($(VERSION),eur_dlp)
VERSION_SYM := VERSION_EUR_DLP
endif

ifeq ($(VERSION),eur_kiosk)
VERSION_SYM := VERSION_EUR_KIOSK
endif

ifeq ($(VERSION),eur_rev0)
VERSION_SYM := VERSION_EUR_REV0
endif

ifeq ($(VERSION),eur_rev0_v11)
VERSION_SYM := VERSION_EUR_REV0_V11
endif

ifeq ($(VERSION),eur_rev1)
VERSION_SYM := VERSION_EUR_REV1
endif

ifeq ($(VERSION),eur_rev2)
VERSION_SYM := VERSION_EUR_REV2
endif

ifeq ($(VERSION),jpn_dlp)
VERSION_SYM := VERSION_JPN_DLP
endif

ifeq ($(VERSION),jpn_kiosk)
VERSION_SYM := VERSION_JPN_KIOSK
endif

ifeq ($(VERSION),jpn_rev0)
VERSION_SYM := VERSION_JPN_REV0
endif

ifeq ($(VERSION),jpn_rev0_v11)
VERSION_SYM := VERSION_JPN_REV0_V11
endif

ifeq ($(VERSION),jpn_rev1)
VERSION_SYM := VERSION_JPN_REV1
endif

ifeq ($(VERSION),jpn_rev2)
VERSION_SYM := VERSION_JPN_REV2
endif

ifeq ($(VERSION),kor_dlp)
VERSION_SYM := VERSION_KOR_DLP
endif

ifeq ($(VERSION),kor_rev1)
VERSION_SYM := VERSION_KOR_REV1
endif

ifeq ($(VERSION),kor_rev2)
VERSION_SYM := VERSION_KOR_REV2
endif

ifeq ($(VERSION),twn_dlp)
VERSION_SYM := VERSION_TWN_DLP
endif

ifeq ($(VERSION),twn_rev1)
VERSION_SYM := VERSION_TWN_REV1
endif

ifeq ($(VERSION),twn_rev2)
VERSION_SYM := VERSION_TWN_REV2
endif

ifeq ($(VERSION),usa_dlp)
VERSION_SYM := VERSION_USA_DLP
endif

ifeq ($(VERSION),usa_kiosk)
VERSION_SYM := VERSION_USA_KIOSK
endif

ifeq ($(VERSION),usa_rev0)
VERSION_SYM := VERSION_USA_REV0
endif

ifeq ($(VERSION),usa_rev0_v11)
VERSION_SYM := VERSION_USA_REV0_V11
endif

ifeq ($(VERSION),usa_rev1)
VERSION_SYM := VERSION_USA_REV1
endif

ifeq ($(VERSION),usa_rev2)
VERSION_SYM := VERSION_USA_REV2
endif
