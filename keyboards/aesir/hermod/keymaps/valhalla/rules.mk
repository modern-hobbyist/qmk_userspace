USER_NAME := csteamengine
VIA_ENABLE = no
OLED_ENABLE = no
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += ili9341_spi
BACKLIGHT_ENABLE = no
PWM_ENABLE = yes
SERIAL_DRIVER = usart
SRC += ./fonts/norse20.qff.c
SRC += ./graphics/hermod-logo.qgf.c
SRC += ./graphics/left-base-layout.qgf.c
SRC += ./graphics/right-base-layout.qgf.c
