// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "csteamengine.h"

#include "lib/layer_status/layer_status.h"

#include "print.h"


#ifdef LCD_ACTIVITY_TIMEOUT
#include <qp.h>
#include "./fonts/norse20.qff.h"
#include "./graphics/hermod-logo.qgf.h"
#include "./graphics/left-base-layout.qgf.h"
#include "./graphics/right-base-layout.qgf.h"
#define LCD_RENDER_TIMEOUT 100

static painter_font_handle_t my_font;
static uint16_t last_update = 0;

painter_device_t lcd;
#endif

#ifdef BACKLIGHT_ENABLE
static uint8_t last_backlight = 255;
#endif

#ifdef SECRETS_H
#include "secrets.h"
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                // Left Half Numpad                           Left Half                                                                                 Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          KC_NO,  KC_ESC,     KC_NO,      KC_NO,  KC_NO,    KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_DEL,     KC_BSPC,            KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          KC_NO,  KC_GRV,     KC_1,       KC_2,   KC_3,     KC_4,               KC_5,               KC_6,           KC_7,           KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,             KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          KC_NO,  KC_TAB,     KC_Q,       KC_W,   KC_E,     KC_R,               KC_T,               KC_Y,           KC_U,           KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,            KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            KC_NO,  KC_LSFT,    KC_A,       KC_S,   KC_D,     KC_F,               KC_G,               KC_H,           KC_J,           KC_K,       KC_L,       KC_SCLN,    KC_QUOT,    KC_NO,              KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          KC_NO,  KC_CAPS,    KC_Z,       KC_X,   KC_C,     KC_V,               KC_B,               KC_N,           KC_M,           KC_COMM,    KC_DOT,     KC_SLSH,    KC_MINS,    KC_RSFT,            KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
                                                                                                                KC_LALT,            KC_LCTL,            KC_LALT,        KC_LCTL,                                    KC_UP,
                                                                                                    LT(1, KC_LGUI),     KC_SPACE,   KC_NO,              LT(1, KC_LGUI), KC_SPACE,    KC_NO,      KC_LEFT,    KC_DOWN,    KC_RIGHT
            ),
    [1] = LAYOUT(
                // Left Half Numpad                           Left Half                                                                                 Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          KC_NO,  KC_ESC,     KC_NO,      KC_NO,       KC_NO,       KC_NO,      KC_NO,              KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,      KC_DEL,     G(KC_BSPC),             KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          KC_NO,  KC_GRV,     KC_1,       KC_2,        KC_3,        KC_4,       KC_5,               KC_6,       KC_7,       KC_8,           KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          KC_NO,  G(KC_TAB),  G(KC_Q),    G(KC_W),     G(KC_E),     G(KC_R),    G(KC_T),            KC_Y,       G(KC_U),    G(KC_I),        KC_O,       G(KC_P),    KC_LBRC,    KC_RBRC,                KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            KC_NO,  KC_LSFT,    G(KC_A),    G(KC_S),     KC_D,        KC_F,       KC_G,               KC_H,       KC_J,       KC_K,           G(KC_L),    KC_SCLN,    KC_QUOT,    KC_NO,                  KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          KC_NO,  KC_CAPS,    G(KC_Z),    G(KC_X),     G(KC_C),     G(KC_V),    G(KC_B),            G(KC_N),    KC_M,       KC_COMM,        KC_DOT,     G(KC_SLSH), KC_MINS,    KC_RSFT,                KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
                                                                                                                        KC_LALT,    KC_LCTL,            KC_MPRV,    KC_MNXT,                                KC_VOLU,
                                                                                                        KC_LGUI,    G(KC_SPACE),    KC_NO,              KC_LGUI,    KC_MPLY, KC_NO,         LAG(KC_LEFT),   KC_VOLD,    LAG(KC_RIGHT)
            ),
};

#ifdef LCD_ACTIVITY_TIMEOUT
static uint32_t lcd_key_timer;
static void     refresh_lcd(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void     check_lcd_timeout(void); // checks if enough time has passed for RGB to timeout
bool            is_lcd_timeout = false;  // store if RGB has timed out or not in a boolean
bool            peripherals_on = false;
static void     render_lcd(void);
static void     init_lcd(void);
static void     power_off_lcd(void);
static void     render_static_text(void);
static painter_image_handle_t hermod_logo;
static painter_image_handle_t left_base_layout;
static painter_image_handle_t right_base_layout;
static uint32_t last_layer_state = -1;
uint8_t last_rgb_mode = -1;
bool last_caps = true;

const char *current_rgb_mode(void) {
    switch(rgblight_get_mode()) {
        case 2:
            return "Cycle";
        default:
            return "Static";
    }
    return "Static";
}

const char *current_layer_name(void) {
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            return "Base";
        case _FN0:
            return "Layer 1";
        case _FN1:
            return "Layer 2";
        case _FN2:
            return "Layer 3";
    }
    return "unknown";
}

void init_lcd(void) {
    qp_init(lcd, QP_ROTATION_0);

    // gpio_write_pin_high(LCD_ENABLE_PIN);

    // Let the LCD get some power...
    wait_ms(200);

    // Turn on the LCD and clear the display
    qp_rect(lcd, 0, 0, 240, 320, 6, 0, 0, true);

    my_font = qp_load_font_mem(font_norse20);
    hermod_logo = qp_load_image_mem(gfx_hermod_logo);
    left_base_layout = qp_load_image_mem(gfx_left_base_layout);
    right_base_layout = qp_load_image_mem(gfx_right_base_layout);

    // Turn LCD On
    qp_power(lcd, true);
}

void render_static_text(void) {
    if (my_font != NULL) {
        static const char *title = "Project Aesir | Hermod";
        static const char *caps_title = "Caps";
        static const char *layer_title = "Layer";
        static const char *rgb_title = "RGB";


        if(is_keyboard_left()) {
            qp_drawtext(lcd, (240 - qp_textwidth(my_font, title) - 2), (320 - 20 - 2), my_font, title);
        } else {
            qp_drawtext(lcd, 2, (320 - 20 - 2), my_font, title);
        }

        qp_drawtext(lcd, 2, 2, my_font, caps_title);
        qp_drawtext(lcd, (120 - qp_textwidth(my_font, layer_title)/2), 2, my_font, layer_title);
        qp_drawtext(lcd, (240 - qp_textwidth(my_font, rgb_title) - 2), 2, my_font, rgb_title);
    }

    if (is_keyboard_left()) {
        if (left_base_layout != NULL) {
            qp_drawimage(lcd, 0, 55, left_base_layout);
        }

        if (hermod_logo != NULL) {
            qp_drawimage(lcd, 10, (320 - hermod_logo->height - 10), hermod_logo);
        }
    } else {
        if (left_base_layout != NULL) {
            qp_drawimage(lcd, 0, 55, right_base_layout);
        }

        if (hermod_logo != NULL) {
            qp_drawimage(lcd, (240 - hermod_logo->width -10), (320 - hermod_logo->height - 10), hermod_logo);
        }
    }
}

void power_off_lcd(void) {
    // Turn on the LCD and clear the display
    qp_power(lcd, false);

    // Disable power to display
    // gpio_write_pin_low(LCD_ENABLE_PIN);
}

void render_lcd() {
    const uint8_t curr_rgb_mode = rgblight_get_mode();
    bool curr_caps = host_keyboard_led_state().caps_lock;

    if(last_layer_state != layer_state) {
        last_layer_state   = layer_state;
        const char      *layer_name = current_layer_name();
        qp_rect(lcd, 80, 22, 160, 50, 6, 0, 0, true);
        qp_drawtext(lcd, (120 - qp_textwidth(my_font, layer_name)/2), 27, my_font, layer_name);
    }

    if(curr_rgb_mode != last_rgb_mode) {
        last_rgb_mode = curr_rgb_mode;
        const char * rgb_mode = current_rgb_mode();

        // Rect to clear that area
        qp_rect(lcd, 160, 22, 240, 50, 6, 0, 0, true);

        // RGB Profile
        qp_drawtext(lcd, (240 - qp_textwidth(my_font, rgb_mode) - 2), 27, my_font, rgb_mode);
    }

    // Caps
    if(curr_caps != last_caps) {
        last_caps = curr_caps;

        // Rect to clear that area
        qp_rect(lcd, 0, 22, 80, 50, 6, 0, 0, true);

        if (curr_caps) {
            // Caps Lock is on
            qp_drawtext(lcd, 2, 27, my_font, "On");
        } else {
            // Caps Lock is off
            qp_drawtext(lcd, 2, 27, my_font, "Off");
        }
    }
}

void refresh_lcd(void) {
    lcd_key_timer = timer_read32(); // store time of last refresh
    if (is_lcd_timeout) {
        // Turn LCD On
        init_lcd();
        render_static_text();
        render_lcd();

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
    peripherals_on = last_input_activity_elapsed() < LCD_ACTIVITY_TIMEOUT;

    if (!is_lcd_timeout && timer_elapsed32(lcd_key_timer) > LCD_ACTIVITY_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    {
        #ifdef BACKLIGHT_ENABLE
        if (last_backlight == 255) {
            last_backlight = get_backlight_level();
        }

        backlight_set(0);
        #endif

        power_off_lcd();

        is_lcd_timeout = true;
    }
}
#endif

void keyboard_pre_init_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
        // gpio_set_pin_output(LCD_ENABLE_PIN);

        // // Turn LCD Off for now
        // gpio_write_pin_low(LCD_ENABLE_PIN);
    #endif
}

void keyboard_post_init_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
    // Initialize the LCD
    lcd = qp_ili9341_make_spi_device(320, 240, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 1, 0);
    init_lcd();
    render_static_text();
    render_lcd();

    #ifdef BACKLIGHT_ENABLE
    backlight_enable();
    backlight_set(255);
    #endif
    #endif
}

void housekeeping_task_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
        check_lcd_timeout();

        if(!is_lcd_timeout && peripherals_on) {
            render_lcd();
            last_update = timer_read();
        }
    #endif
}

void suspend_power_down_keymap(void) {
    #ifdef BACKLIGHT_ENABLE
    if (last_backlight == 255) {
        last_backlight = get_backlight_level();
    }
    backlight_set(0);
    #endif

    #ifdef LCD_ACTIVITY_TIMEOUT
        // Turn LCD Off
        power_off_lcd();
    #endif
}

void suspend_wakeup_init_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
        // Turn LCD On
        init_lcd();
        render_static_text();
        render_lcd();
    #endif

    #ifdef BACKLIGHT_ENABLE
    if (last_backlight != 255) {
        backlight_set(last_backlight);
    }
    last_backlight = 255;
    #endif
}

bool process_record_keymap (uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        #ifdef LCD_ACTIVITY_TIMEOUT
            refresh_lcd();

        #endif
    }

  return true;
}
