// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "csteamengine.h"
#include "lib/layer_status/layer_status.h"
#include <qp.h>
// #include "graphics/face.qgf.h"

painter_device_t lcd;
#ifdef BACKLIGHT_ENABLE
static uint8_t last_backlight = 255;
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
                // Left Half Numpad                           Left Half                                                                                 Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          KC_NO,  KC_ESC,     KC_NO,    KC_NO,  KC_NO,  KC_NO,              KC_NO,                  KC_NO,              KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_DEL,     KC_BSPC,                KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          KC_NO,  KC_GRV,     KC_1,     KC_2,   KC_3,   KC_4,               KC_5,                   KC_6,               KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          KC_NO,  KC_TAB,     KC_Q,     KC_W,   KC_E,   KC_R,               KC_T,                   KC_Y,               KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,                KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            KC_NO,  KC_LSFT,    KC_A,     KC_S,   KC_D,   KC_F,               KC_G,                   KC_H,               KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,    KC_NO,                  KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          KC_NO,  KC_CAPS,    KC_Z,     KC_X,   KC_C,   KC_V,               KC_B,                   KC_N,               KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_MINS,    KC_RSFT,                KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
                                                                                                            KC_LALT,            KC_LCTL,                KC_LALT,            KC_LCTL,                KC_UP,
                                                                                                            LT(_FN0, KC_LGUI),  KC_SPACE,               LT(_FN0, KC_RGUI),  KC_SPACE,   KC_LEFT,    KC_DOWN,    KC_RIGHT
            ),
    [_FN0] = LAYOUT(
                // Left Half Numpad                           Left Half                                                                                         Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          KC_NO,  KC_ESC,     KC_NO,      KC_NO,       KC_NO,       KC_NO,      KC_NO,                      KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,      KC_DEL,     G(KC_BSPC),             KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          KC_NO,  KC_GRV,     KC_1,       KC_2,        KC_3,        KC_4,       KC_5,                       KC_6,       KC_7,       KC_8,           KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          KC_NO,  G(KC_TAB),  G(KC_Q),    G(KC_W),     G(KC_E),     G(KC_R),    G(KC_T),                    KC_Y,       G(KC_U),    G(KC_I),        KC_O,       G(KC_P),    KC_LBRC,    KC_RBRC,                KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            KC_NO,  KC_LSFT,    G(KC_A),    G(KC_S),     KC_D,        KC_F,       KC_G,                       KC_H,       KC_J,       KC_K,           G(KC_L),    KC_SCLN,    KC_QUOT,    KC_NO,                  KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          KC_NO,  KC_CAPS,    G(KC_Z),    G(KC_X),     G(KC_C),     G(KC_V),    G(KC_B),                    G(KC_N),    KC_M,       KC_COMM,        KC_DOT,     G(KC_SLSH), KC_MINS,    KC_RSFT,                KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
                                                                                                                        KC_LALT,    KC_LCTL,                    KC_MPRV,    KC_MNXT,                    KC_VOLU,
                                                                                                                        KC_LGUI,    G(KC_SPACE),                KC_RGUI,    KC_MPLY,    LAG(KC_LEFT),   KC_VOLD,    LAG(KC_RIGHT)
            ),
    [_FN1] = LAYOUT(
                // Left Half Numpad                           Left Half                                                                                 Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          KC_NO,  KC_ESC,     KC_NO,    KC_NO,  KC_NO,  KC_NO,              KC_NO,                  KC_NO,              KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_DEL,     KC_BSPC,            KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          KC_NO,  KC_GRV,     KC_1,     KC_2,   KC_3,   KC_4,               KC_5,                   KC_6,               KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,             KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          KC_NO,  KC_TAB,     KC_Q,     KC_W,   KC_E,   KC_R,               KC_T,                   KC_Y,               KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,            KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            KC_NO,  KC_LSFT,    KC_A,     KC_S,   KC_D,   KC_F,               KC_G,                   KC_H,               KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,    KC_NO,              KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          KC_NO,  KC_CAPS,    KC_Z,     KC_X,   KC_C,   KC_V,               KC_B,                   KC_N,               KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_MINS,    KC_RSFT,            KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
                                                                                                            KC_LALT,            KC_LCTL,                KC_LALT,            KC_LCTL,                KC_UP,
                                                                                                            KC_LGUI,            KC_SPACE,               KC_RGUI,            KC_SPACE,   KC_LEFT,    KC_DOWN,    KC_RIGHT
            ),
    [_FN2] = LAYOUT(
                // Left Half Numpad                           Left Half                                                                                 Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          KC_NO,  KC_ESC,     KC_NO,    KC_NO,  KC_NO,  KC_NO,              KC_NO,                  KC_NO,              KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_DEL,     KC_BSPC,            KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          KC_NO,  KC_GRV,     KC_1,     KC_2,   KC_3,   KC_4,               KC_5,                   KC_6,               KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,             KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          KC_NO,  KC_TAB,     KC_Q,     KC_W,   KC_E,   KC_R,               KC_T,                   KC_Y,               KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,            KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            KC_NO,  KC_LSFT,    KC_A,     KC_S,   KC_D,   KC_F,               KC_G,                   KC_H,               KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,    KC_NO,              KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          KC_NO,  KC_CAPS,    KC_Z,     KC_X,   KC_C,   KC_V,               KC_B,                   KC_N,               KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_MINS,    KC_RSFT,            KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
                                                                                                            KC_LALT,            KC_LCTL,                KC_LALT,            KC_LCTL,                KC_UP,
                                                                                                            KC_LGUI,            KC_SPACE,               KC_RGUI,            KC_SPACE,   KC_LEFT,    KC_DOWN,    KC_RIGHT
            ),
};

// clang-format on

#ifdef LCD_ACTIVITY_TIMEOUT
static uint32_t lcd_key_timer;
static void     refresh_lcd(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void     check_lcd_timeout(void); // checks if enough time has passed for RGB to timeout
bool            is_lcd_timeout = false;  // store if RGB has timed out or not in a boolean

void refresh_lcd(void) {
    lcd_key_timer = timer_read32(); // store time of last refresh
    if (is_lcd_timeout) {
        qp_power(lcd, true);

        #ifdef BACKLIGHT_ENABLE
        if (last_backlight != 255) {
            backlight_set(last_backlight);
        }
        last_backlight = 255;
        #endif

        is_lcd_timeout = false;
    }
}

void check_lcd_timeout(void) {
    if (!is_lcd_timeout && timer_elapsed32(lcd_key_timer) > LCD_ACTIVITY_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    {
        #ifdef BACKLIGHT_ENABLE
        if (last_backlight == 255) {
            last_backlight = get_backlight_level();
        }

        backlight_set(0);
        #endif

        qp_power(lcd, false);
        is_lcd_timeout = true;
    }
}
#endif

// static painter_image_handle_t my_image;

void keyboard_post_init_keymap(void) {
    // Let the LCD get some power...
    wait_ms(200);

    // Initialize the LCD
    lcd = qp_ili9341_make_spi_device(320, 240, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 1, 0);
    qp_init(lcd, QP_ROTATION_0);

    #ifdef BACKLIGHT_ENABLE
    backlight_enable();
    backlight_set(255);
    #endif
    // Turn on the LCD and clear the display
    qp_power(lcd, true);

    qp_rect(lcd, 0, 0, 240, 320, 0, 0, 0, true);

    // my_image = qp_load_image_mem(gfx_face);
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
    #ifdef BACKLIGHT_ENABLE
    if (last_backlight == 255) {
        last_backlight = get_backlight_level();
    }
    backlight_set(0);
    #endif

    qp_power(lcd, false);
}

void suspend_wakeup_init_keymap(void) {
    qp_power(lcd, true);

    #ifdef BACKLIGHT_ENABLE
    if (last_backlight != 255) {
        backlight_set(last_backlight);
    }
    last_backlight = 255;
    #endif

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
