OLED_ENABLE = yes

# RGB Matrix enabled
RGB_MATRIX_ENABLE = yes
OS_DETECTION_ENABLE = yes

CAPS_WORD_ENABLE = yes
OLED_DRIVER = ssd1306
OLED_TRANSPORT = i2c
LTO_ENABLE = yes
VIA_ENABLE = yes
TAP_DANCE_ENABLE = yes
USER_NAME := csteamengine

SRC += ./lib/layer_status/layer_status.c
SRC += ./lib/logo.c
