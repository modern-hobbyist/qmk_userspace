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
#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B8
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PIN B9
#define I2C1_SDA_PAL_MODE 1
#endif

#define CAPSLOCK_LED 8
#define SELECT_WORD_TIMEOUT 2000 // When idle, clear state after 2 seconds.
#define RGB_MATRIX_TIMEOUT 900000 // number of milliseconds to wait until rgb automatically turns off
#define RGB_MATRIX_SLEEP
#define RGB_MATRIX_DEFAULT_HUE 100
#define RGB_MATRIX_DEFAULT_SAT 255
#define RGB_MATRIX_DEFAULT_VAL 200
#define RGBLIGHT_LIMIT_VAL 125
#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 1
