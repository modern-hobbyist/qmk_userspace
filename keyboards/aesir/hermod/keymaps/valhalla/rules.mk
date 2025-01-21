USER_NAME := csteamengine
VIA_ENABLE = yes
OLED_ENABLE = no
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += ili9341_spi
BACKLIGHT_ENABLE = yes
PWM_ENABLE = yes
BACKLIGHT_DRIVER = pwm
SERIAL_DRIVER = usart
SRC += ./graphics/layout-1-left.qgf.c
SRC += graphics/face.qgf.c

# RGB Matrix enabled
RGB_MATRIX_ENABLE = yes
