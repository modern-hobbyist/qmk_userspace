/* Copyright 2024 Modern Hobbyist <https://github.com/modern-hobbyist>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

/* Use the custom font */
#define OLED_FONT_H "./lib/glcdfont.c"

#ifdef OLED_ENABLE
/* Mapping I2C2 for OLED */
#    define I2C1_SCL_PIN B6
#    define I2C1_SDA_PIN B7
#    define I2C_DRIVER I2CD1
#endif

#define SPI_DRIVER SPID2
#define SPI_SCK_PIN B13
// #define SPI_SCK_PAL_MODE 5
#define SPI_MOSI_PIN B15
// #define SPI_MOSI_PAL_MODE 5
#define SPI_MISO_PIN B14
// #define SPI_MISO_PAL_MODE 5

#define LCD_RST_PIN B5
#define LCD_CS_PIN A0
#define LCD_DC_PIN A1
#ifndef LCD_ACTIVITY_TIMEOUT
#    define LCD_ACTIVITY_TIMEOUT 30000
#endif

// Backlight driver (to control LCD backlight)
#define BACKLIGHT_PWM_DRIVER PWMD4
#define BACKLIGHT_PWM_CHANNEL 3
#define BACKLIGHT_LEVELS 10
// #define BACKLIGHT_PAL_MODE 2
#define BACKLIGHT_PIN B8
