// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"
// #include "secrets.h"
#include "csteamengine.h"
#include "lib/layer_status/layer_status.h"
// #include "lib/bongocat/bongocat.h"
#include <qp.h>
#include "graphics/layout-1-left.qgf.h"

painter_device_t lcd;
static uint8_t last_backlight = 255;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * This is a standard version of this same keyboard. I added optional 2u keys for 0, + and Enter on the numpad
     * For my personal board, I will be splitting them up to have more macros, but if you want a more standard layout
     * You can install this one. The one downside with this, is you won't have arrow keys on the main layer anymore.
     *       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     *       │M0 │M1 │M2 │M3 │M4 │M5 │M6 │M7 │M8 │M9 │M10│M11│M12│M13│M14│
     *       └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     * ┌───┐ ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐ ┌───┬───┬───┬───┐
     * │M17│ │Esc│F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│F13│M15│ │Del│End│Hom│M16│
     * ├───┤ ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┴───┤ ┌───┬───┬───┬───┐
     * │M18│ │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│ │ + │ - │ * │ / │
     * ├───┤ ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┼───┤
     * │M19│ │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │ │ 7 │ 8 │ 9 │   │ <-- KC_NO filler
     * ├───┤ ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ ├───┼───┼───┤ + │
     * │M20│ │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │ │ 4 │ 5 │ 6 │   │
     * ├───┤ ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤ ├───┼───┼───┼───┤
     * │M21│ │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │    Shift │ │ 1 │ 2 │ 3 │   │ <-- KC_NO filler
     * ├───┤ ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ├───┴───┼───┤Ent│
     * │M22│ │Ctrl│GUI │Alt │                        │ Alt│ GUI│Menu│Ctrl│ │   0   │ . │   │
     * └───┘ └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───────┴───┴───┘
     *                                                                       ^-- KC_NO filler
     */
    /*  Row:    0         1        2        3         4      */
    [_BASE] = LAYOUT(
                BL_DOWN, KC_ENTER, BL_UP
            ),

    /*  Row:    0        1        2        3        4       */
    [_FN0] = LAYOUT(

                TO(_BASE), CW_TOGG, TO(_FN1)
            ),

    /*  Row:    0        1        2        3        4       */
    [_FN1] = LAYOUT(
                TO(_FN0), SC_LSPO, TO(_FN2)
                ),

    /*  Row:    0        1        2        3        4        */
    [_FN2] = LAYOUT(
                TO(_FN1), SC_RSPC, TO(_BASE)
            ),
};

// clang-format on
#ifdef OLED_ENABLE
bool oled_task_keymap(void) {
    render_layer_status();
    // render_bongocat();

    return true;
}
#endif

#ifdef LCD_ACTIVITY_TIMEOUT
static uint32_t lcd_key_timer;
static void     refresh_lcd(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void     check_lcd_timeout(void); // checks if enough time has passed for RGB to timeout
bool            is_lcd_timeout = false;  // store if RGB has timed out or not in a boolean

void refresh_lcd(void) {
    lcd_key_timer = timer_read32(); // store time of last refresh
    if (is_lcd_timeout) {
        qp_power(lcd, true);
        if (last_backlight != 255) {
            backlight_set(last_backlight);
        }
        last_backlight = 255;
        is_lcd_timeout = false;
    }
}

void check_lcd_timeout(void) {
    if (!is_lcd_timeout && timer_elapsed32(lcd_key_timer) > LCD_ACTIVITY_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    {
        if (last_backlight == 255) {
            last_backlight = get_backlight_level();
        }

        backlight_set(0);

        qp_power(lcd, false);
        is_lcd_timeout = true;
    }
}
#endif

static painter_image_handle_t my_image;

void keyboard_post_init_keymap(void) {
    // Let the LCD get some power...
    wait_ms(200);

    // Initialize the LCD
    lcd = qp_ili9341_make_spi_device(240, 320, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 4, 0);
    qp_init(lcd, QP_ROTATION_180);  // Try different rotations

    backlight_enable();
    backlight_set(255);
    // Turn on the LCD and clear the display
    qp_power(lcd, true);
    // qp_rect(lcd, 0, 0, 239, 319, 0, 0 ,255, true);
    // qp_circle(lcd, 120, 150, 20, 8, 255, 255, true);
    // my_image = qp_load_image_mem(gfx_layout_1_left);
    // if (my_image != NULL) {
    //     qp_drawimage(lcd, 0, 20, my_image);
    // }
}

void housekeeping_task_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
        check_lcd_timeout();
    #endif

    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > 33) { // Throttle to 30fps
        last_draw = timer_read32();
        // Draw r=4 filled circles down the left side of the display
        for (int i = 0; i < 239; i+=8) {
            qp_circle(lcd, 4, 4+i, 4, i, 255, 255, true);
        }
        qp_flush(lcd);
    }
}

void suspend_power_down_keymap(void) {
    if (last_backlight == 255) {
        last_backlight = get_backlight_level();
    }
    backlight_set(0);
    qp_power(lcd, false);
}

void suspend_wakeup_init_keymap(void) {
    qp_power(lcd, true);
    if (last_backlight != 255) {
        backlight_set(last_backlight);
    }
    last_backlight = 255;
    qp_flush(lcd);
}

bool process_record_keymap (uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        #ifdef LCD_ACTIVITY_TIMEOUT
            refresh_lcd();

        #endif
    }

  return true;
}
