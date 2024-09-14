OS_DETECTION_ENABLE = yes
CAPS_WORD_ENABLE = yes
OLED_DRIVER = ssd1306
OLED_TRANSPORT = i2c
LTO_ENABLE = yes
OLED_ENABLE = yes

SRC += ./lib/layer_status/layer_status.c
SRC += ./lib/logo.c
SRC += csteamengine.c
SRC += features/select_word.c
SRC += features/caps_word.c
CAPS_WORD_ENABLE = no

VPATH += users/csteamengine/features
