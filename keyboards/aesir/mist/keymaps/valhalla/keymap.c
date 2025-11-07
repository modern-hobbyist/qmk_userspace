// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "csteamengine.h"
#include "lib/layer_status/layer_status.h"
#include "print.h"

#ifdef LCD_ACTIVITY_TIMEOUT
#include "backlight.h"
#include <qp.h>
#include "./fonts/norse20.qff.h"
#include "./graphics/hermod-logo.qgf.h"
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

enum custom_keycodes {
    CS_MPLY = SAFE_RANGE,
    CS_MNXT,
    CS_MPRV,
    CS_UP,
    CS_DOWN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,     KC_NO,   KC_NO, KC_NO,   KC_NO,    KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,  KC_NO,     KC_DEL,
        KC_GRV,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,      KC_9,    KC_0,    KC_MINS, KC_EQL,                 KC_BSPC,
        KC_TAB,   KC_Q,     KC_W,   KC_E,    KC_R,    KC_T,     KC_Y,    KC_U,    KC_I,      KC_O,    KC_P,    KC_LBRC, KC_RBRC,                KC_BSLS,                        KC_NUM,     KC_PSLS, KC_PAST, KC_PMNS,
        KC_CAPS,    KC_A,     KC_S,   KC_D,    KC_F,    KC_G,     KC_H,    KC_J,    KC_K,      KC_L,    KC_SCLN, KC_QUOT,                       KC_ENT,                         KC_P7,      KC_P8,   KC_P9,
            KC_LSFT,        KC_Z,     KC_X,   KC_C,    KC_V,    KC_B,     KC_N,    KC_M,    KC_COMM,   KC_DOT,  KC_SLSH,                        KC_RSFT,                        KC_P4,      KC_P5,   KC_P6,   KC_PPLS,
        KC_NO,  KC_NO,    MAC_EMOJIS, KC_LOPT,  KC_LGUI,      MO(_FN0),      KC_SPC, KC_RCTL,            KC_UP,                       KC_NO,  KC_RCTL,                        KC_P1,      KC_P2,   KC_P3,
        KC_NO,   KC_NO,      KC_NO,                                       KC_LEFT,    KC_DOWN,    KC_RIGHT,        KC_NO,  KC_NO,                          KC_P0,               KC_PDOT, KC_PENT
    ),
    [_FN0] = LAYOUT(
        QK_BOOT,     RGB_TOG, RGB_VAD,    RGB_VAI,    RGB_HUI,    RGB_RMOD,        RGB_MOD,   _______,   _______,   _______,   _______,  _______,  _______,  _______,
        _______,     _______,    _______,    _______,    _______,    _______,     _______,    _______,    _______,      _______,    _______,    _______,   _______,     _______,
        _______,     _______,     _______,   _______,    _______,    _______,     _______,    _______,    _______,      _______,    _______,    _______,   _______,     _______,    _______,    _______,    _______,    _______,
        _______,        _______,     _______,   _______,    _______,    _______,     _______,    _______,    _______,      _______,    _______,    _______,                         _______,    _______,    _______,    _______,
                _______,        _______,     _______,   _______,    _______,    _______,     _______,    _______,    _______,   _______,  _______,     _______,                     _______,    _______,    _______,    _______,
        _______,    _______,    _______,    _______,  _______,  _______,    KC_HOME,     KC_END,            KC_VOLU,                      _______,      _______,                    _______,    _______,    _______,
        _______,    _______,    _______,                                                        KC_MPRV,    KC_VOLD,    KC_MNXT,    _______,      _______,                          _______,                _______,    _______
    ),
};

// clang-format on

#ifdef LCD_ACTIVITY_TIMEOUT
static void     check_lcd_timeout(void); // checks if enough time has passed for RGB to timeout
bool            is_lcd_timeout = false;  // store if RGB has timed out or not in a boolean
bool            peripherals_on = true;
static void     render_lcd(bool force);
static void     init_lcd(void);
static void     power_off_lcd(void);
static void     render_static_text(void);
static void     render_rgb_text(void);
static void     render_caps_text(void);
static painter_image_handle_t hermod_logo;
// static painter_image_handle_t layer_1;
// static painter_image_handle_t layer_1_full;
// static painter_image_handle_t layer_2_full;
// static painter_image_handle_t left_1_layout;
static uint32_t last_layer_state = -1;
uint8_t last_rgb_mode = -1;
bool last_caps = true;
// static bool show_alt_layer = false;

const char *current_rgb_mode(void) {
    switch (rgb_matrix_get_mode()) {
        case RGB_MATRIX_BREATHING:
            return "Breathing";
        case RGB_MATRIX_CYCLE_ALL:
            return "Cycle All";
        case RGB_MATRIX_CYCLE_LEFT_RIGHT:
            return "Cycle L-R";
        case RGB_MATRIX_HUE_BREATHING:
            return "Hue Breath";
        case RGB_MATRIX_JELLYBEAN_RAINDROPS:
            return "Jellybean";
        case RGB_MATRIX_MULTISPLASH:
            return "MultiSplash";
        case RGB_MATRIX_PIXEL_FLOW:
            return "Pixel Flow";
        case RGB_MATRIX_PIXEL_RAIN:
            return "Pixel Rain";
        case RGB_MATRIX_RAINBOW_BEACON:
            return "RB Beacon";
        case RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
            return "RB Chevron";
        case RGB_MATRIX_RAINBOW_PINWHEELS:
            return "RB Pinwheel";
        case RGB_MATRIX_RAINDROPS:
            return "Raindrops";
        case RGB_MATRIX_SOLID_COLOR:
            return "Solid Color";
        case RGB_MATRIX_SOLID_MULTISPLASH:
            return "Solid MSplash";
        case RGB_MATRIX_SOLID_REACTIVE:
            return "Solid React";
        case RGB_MATRIX_SOLID_REACTIVE_CROSS:
            return "Solid R Cross";
        case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
            return "Solid R Simple";
        case RGB_MATRIX_SOLID_REACTIVE_WIDE:
            return "Solid R Wide";
        case RGB_MATRIX_SOLID_SPLASH:
            return "Solid Splash";
        case RGB_MATRIX_SPLASH:
            return "Splash";
        case RGB_MATRIX_TYPING_HEATMAP:
            return "Heatmap";
        default:
            return "Unknown";
    }
}


const char *current_layer_name(void) {
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            return "Base";
        case _FN0:
            return "Layer 1";
    }
    return "unknown";
}

void init_lcd(void) {
    qp_init(lcd, QP_ROTATION_90);

    // Turn LCD On
    qp_power(lcd, true);

    backlight_set(10);

    // Turn on the LCD and clear the display
    qp_rect(lcd, 0, 0, 320, 240, 6, 0, 0, true);
}

void render_static_text(void) {
    // if(layer_1 != NULL) {
    //     qp_drawimage(lcd, 0, 20, layer_1);
    // }

    if (hermod_logo != NULL) {
        qp_drawimage(lcd, 10, (240 - hermod_logo->height - 10), hermod_logo);
    }

    if (my_font != NULL) {
        static const char *title = "Project Aesir | Mist";
        static const char *caps_title = "Caps";
        static const char *layer_title = "Layer";
        static const char *rgb_title = "RGB";


        qp_drawtext(lcd, (320 - qp_textwidth(my_font, title) - 2), (240 - 20 - 2), my_font, title);

        qp_drawtext(lcd, 2, 2, my_font, caps_title);
        qp_drawtext(lcd, (160 - qp_textwidth(my_font, layer_title)/2), 2, my_font, layer_title);
        qp_drawtext(lcd, (320 - qp_textwidth(my_font, rgb_title) - 2), 2, my_font, rgb_title);
    }
}

void render_rgb_text(void) {
    const uint8_t curr_rgb_mode = rgblight_get_mode();

    render_static_text();
    last_rgb_mode = curr_rgb_mode;
    const char * rgb_mode = current_rgb_mode();

    // Rect to clear that area
    qp_rect(lcd, 200, 22, 320, 50, 6, 0, 0, true);

    // RGB Profile
    qp_drawtext(lcd, (320 - qp_textwidth(my_font, rgb_mode) - 2), 27, my_font, rgb_mode);
}

void render_caps_text(void) {
    bool curr_caps = host_keyboard_led_state().caps_lock;

    render_static_text();
    last_caps = curr_caps;

    // Rect to clear that area
    qp_rect(lcd, 0, 22, 100, 50, 6, 0, 0, true);

    if (curr_caps) {
        // Caps Lock is on
        qp_drawtext(lcd, 2, 27, my_font, "On");
    } else {
        // Caps Lock is off
        qp_drawtext(lcd, 2, 27, my_font, "Off");
    }
}

void power_off_lcd(void) {
    // Turn on the LCD and clear the display
    // qp_power(lcd, false);
    qp_rect(lcd, 0, 0, 320, 240, 0, 0, 0, true); // fill with black
    qp_flush(lcd);
    qp_power(lcd, false);

    backlight_set(0);

    // TODO Disable LCD backlight
}

void render_lcd(bool force) {
    const uint8_t curr_rgb_mode = rgblight_get_mode();
    bool curr_caps = host_keyboard_led_state().caps_lock;

    if(last_layer_state != layer_state || force) {
        last_layer_state   = layer_state;
        const char      *layer_name = current_layer_name();

        // if(get_highest_layer(layer_state) == _BASE) {
        //     if(layer_1_full != NULL) {
        //         qp_drawimage(lcd, 0, 5, layer_1_full);
        //     }
        // } else if (get_highest_layer(layer_state) == _FN0){
        //     if(layer_2_full != NULL) {
        //         qp_drawimage(lcd, 0, 5, layer_2_full);
        //     }
        // }

        render_static_text();
        render_caps_text();
        render_rgb_text();
        qp_rect(lcd, 100, 22, 200, 50, 6, 0, 0, true);
        qp_drawtext(lcd, (160 - qp_textwidth(my_font, layer_name)/2), 27, my_font, layer_name);
    }

    if(curr_rgb_mode != last_rgb_mode || force) {
        render_rgb_text();
    }

    // Caps
    if(curr_caps != last_caps || force) {
        render_caps_text();
    }
}

void check_lcd_timeout(void) {
    peripherals_on = last_input_activity_elapsed() < LCD_ACTIVITY_TIMEOUT;

    if (!is_lcd_timeout && !peripherals_on) // check if RGB has already timeout and if enough time has passed
    {
        #ifdef BACKLIGHT_ENABLE
        if (last_backlight == 255) {
            last_backlight = get_backlight_level();
        }

        backlight_set(0);
        #endif

        power_off_lcd();

        is_lcd_timeout = true;
    } else if(is_lcd_timeout && peripherals_on) {
        init_lcd();
        render_static_text();
        render_lcd(true);
        is_lcd_timeout = false;
    }
}
#endif

void keyboard_pre_init_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
        // TODO update LCD backlight PWM
        // Turn LCD Off for now
        backlight_set(0);
    #endif
}

void keyboard_post_init_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT

    // Initialize the LCD
    lcd = qp_ili9341_make_spi_device(320, 240, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 8, 0);
    my_font = qp_load_font_mem(font_norse20);
    hermod_logo = qp_load_image_mem(gfx_hermod_logo);
    // layer_1 = qp_load_image_mem(gfx_layer_1);
    // layer_1_full = qp_load_image_mem(gfx_layer_1_full);
    // layer_2_full = qp_load_image_mem(gfx_layer_2_full);
    // left_1_layout = qp_load_image_mem(gfx_left_1_layout);

    init_lcd();
    render_static_text();
    render_lcd(false);

    #ifdef BACKLIGHT_ENABLE
    backlight_enable();
    backlight_set(10);
    #endif
    #endif
}

void housekeeping_task_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
        check_lcd_timeout();

        if(!is_lcd_timeout && peripherals_on) {
            render_lcd(false);
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
        render_lcd(true);
    #endif

    #ifdef BACKLIGHT_ENABLE
    if (last_backlight != 255) {
        backlight_set(last_backlight);
    }
    last_backlight = 100;
    #endif
}

bool process_record_keymap (uint16_t keycode, keyrecord_t *record) {

      return true;
}
