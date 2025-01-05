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

// TODO remove this once I add LEDs
#ifdef RGB_MATRIX_TIMEOUT
 #undef RGB_MATRIX_TIMEOUT
#endif
