DEBOUNCE_TYPE = asym_eager_defer_pk
TAP_DANCE_ENABLE = yes

VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes

# firmware size reduction
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no 
QMK_SETTINGS = no

COMBO_ENABLE = no
KEY_OVERRIDE_ENABLE = no

USER_NAME := bryan065

SRC += \
	rgb.c\
	bryan065.c