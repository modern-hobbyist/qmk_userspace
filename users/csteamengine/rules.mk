OS_DETECTION_ENABLE = yes
CAPS_WORD_ENABLE = yes
OLED_DRIVER = ssd1306
OLED_TRANSPORT = i2c
LTO_ENABLE = yes
OLED_ENABLE = yes
BOOTMAGIC_ENABLE = yes

SRC += ./lib/layer_status/layer_status.c
SRC += ./lib/logo.c
SRC += csteamengine.c
SRC += features/select_word.c
# SRC += graphics/face.qgf.c
CAPS_WORD_ENABLE = yes

VPATH += users/csteamengine/features
# VPATH += users/csteamengine/graphics
