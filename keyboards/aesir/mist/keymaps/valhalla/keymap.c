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
#include "./graphics/left-1-layout.qgf.h"
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

#ifdef TAP_DANCE_ENABLE

enum {
    TASK_EMOJI_LAYER, // Our custom tap dance key; add any other tap dance keys to this enum
    TD_PW_ONE,
    TD_PW_TWO,
    TD_PW_THREE,
    TD_PW_FOUR,
    TD_CAPS_LOCK,
    TD_SC_LSHIFT,
    TD_SC_RSHIFT,
    TD_TICK_TICK,
    TD_OBSIDIAN,
    TD_COPY_FILE,
    TD_VOL_UP,
    TD_VOL_DOWN,
    TD_ENTER_SUPER,
    TD_BRACKET_GUI,
    TD_LPAR_GUI,
    TD_RPAR_GUI,
    TD_LALT_PAR,
    TD_LCTL_PAR,
    TD_RALT_PAR,
    TD_RCTL_PAR,
    TD_ESC_CAPS,
    TD_GRV_LAYER,
    TD_REFACTOR
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
    uint16_t single_tap_keycode;
    uint16_t double_tap_keycode;
    uint16_t held;
    uint16_t hold;
} td_tap_t;
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,         KC_DEL,
        KC_GRV,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,      KC_9,    KC_0,    KC_MINS, KC_EQL,         KC_BSPC,
        KC_TAB,   KC_Q,     KC_W,   KC_E,    KC_R,    KC_T,     KC_Y,    KC_U,    KC_I,      KC_O,    KC_P,    KC_LBRC, KC_RBRC,        KC_BSLS,        KC_NUM,     KC_PSLS, KC_PAST, KC_PMNS,
        KC_CAPS,    KC_A,     KC_S,   KC_D,    KC_F,    KC_G,     KC_H,    KC_J,    KC_K,      KC_L,    KC_SCLN, KC_QUOT,               KC_ENT,         KC_P7,      KC_P8,   KC_P9,
            KC_LSFT,        KC_Z,     KC_X,   KC_C,    KC_V,    KC_B,     KC_N,    KC_M,    KC_COMM,   KC_DOT,  KC_SLSH,                KC_RSFT,        KC_P4,      KC_P5,   KC_P6,   KC_PPLS,
        KC_LCTL,  KC_NO,    KC_NO,      KC_LALT,  KC_LGUI,      KC_NO,      KC_SPC, KC_RALT,            KC_UP,                  KC_NO,  KC_RCTL,        KC_P1,      KC_P2,   KC_P3,
        KC_NO,    KC_NO,    KC_NO,                                                          KC_LEFT,    KC_DOWN,    KC_RIGHT,   KC_NO,  KC_NO,          KC_P0,               KC_PDOT, KC_PENT
    ),
    [_FN0] = LAYOUT(
        KC_ESC,     _______,  _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,  _______,  _______,  KC_DEL,
        KC_GRV,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,      KC_9,    KC_0,    KC_MINS,   KC_EQL,                 KC_BSPC,
        KC_TAB,     KC_Q,     KC_W,   KC_E,    KC_R,    KC_T,     KC_Y,    KC_U,    KC_I,      KC_O,    KC_P,    KC_LBRC,   KC_RBRC,                KC_BSLS,        KC_NUM,     KC_PSLS, KC_PAST, KC_PMNS,
        KC_CAPS,        KC_A,     KC_S,   KC_D,    KC_F,    KC_G,     KC_H,    KC_J,    KC_K,      KC_L,    KC_SCLN,    KC_QUOT,                    KC_ENT,         KC_P7,      KC_P8,   KC_P9,
                KC_LSFT,        KC_Z,     KC_X,   KC_C,    KC_V,    KC_B,     KC_N,    KC_M,    KC_COMM,   KC_DOT,  KC_SLSH,                        KC_RSFT,        KC_P4,      KC_P5,   KC_P6,   KC_PPLS,
        KC_LCTL,    _______,    _______,    KC_LALT,  KC_LGUI,  _______,    KC_SPC,     KC_RALT,            KC_UP,                      _______,    KC_RCTL,        KC_P1,      KC_P2,   KC_P3,
        _______,    _______,    _______,                                                  KC_LEFT,    KC_DOWN,    KC_RIGHT,             _______,    _______,        KC_P0,               KC_PDOT, KC_PENT
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
static painter_image_handle_t hermod_logo;
static painter_image_handle_t left_base_layout;
static painter_image_handle_t left_1_layout;
static uint32_t last_layer_state = -1;
uint8_t last_rgb_mode = -1;
bool last_caps = true;
static bool is_held = false;
static bool show_alt_layer = false;

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
    }
    return "unknown";
}

void init_lcd(void) {
    qp_init(lcd, QP_ROTATION_0);

    // TODO set backlight PWM to high
    // gpio_write_pin_high(LCD_ENABLE_PIN);

    // Turn on the LCD and clear the display
    qp_rect(lcd, 0, 0, 240, 320, 6, 0, 0, true);

    // Turn LCD On
    qp_power(lcd, true);
}

void render_static_text(void) {
    if (my_font != NULL) {
        static const char *title = "Project Aesir | Hermod";
        static const char *caps_title = "Caps";
        static const char *layer_title = "Layer";
        static const char *rgb_title = "RGB";


        qp_drawtext(lcd, (240 - qp_textwidth(my_font, title) - 2), (320 - 20 - 2), my_font, title);

        qp_drawtext(lcd, 2, 2, my_font, caps_title);
        qp_drawtext(lcd, (120 - qp_textwidth(my_font, layer_title)/2), 2, my_font, layer_title);
        qp_drawtext(lcd, (240 - qp_textwidth(my_font, rgb_title) - 2), 2, my_font, rgb_title);
    }
    if(show_alt_layer) {
        if (left_1_layout != NULL) {
            qp_drawimage(lcd, 0, 55, left_1_layout);
        }
    } else {
        if (left_base_layout != NULL) {
            qp_drawimage(lcd, 0, 55, left_base_layout);
        }
    }

    if (hermod_logo != NULL) {
        qp_drawimage(lcd, 10, (320 - hermod_logo->height - 10), hermod_logo);
    }
}

void power_off_lcd(void) {
    // Turn on the LCD and clear the display
    qp_power(lcd, false);

    // TODO Disable LCD backlight
}

void render_lcd(bool force) {
    const uint8_t curr_rgb_mode = rgblight_get_mode();
    bool curr_caps = host_keyboard_led_state().caps_lock;

    if(last_layer_state != layer_state || force) {
        last_layer_state   = layer_state;
        const char      *layer_name = current_layer_name();
        qp_rect(lcd, 80, 22, 160, 50, 6, 0, 0, true);
        qp_drawtext(lcd, (120 - qp_textwidth(my_font, layer_name)/2), 27, my_font, layer_name);
    }

    if(curr_rgb_mode != last_rgb_mode || force) {
        last_rgb_mode = curr_rgb_mode;
        const char * rgb_mode = current_rgb_mode();

        // Rect to clear that area
        qp_rect(lcd, 160, 22, 240, 50, 6, 0, 0, true);

        // RGB Profile
        qp_drawtext(lcd, (240 - qp_textwidth(my_font, rgb_mode) - 2), 27, my_font, rgb_mode);
    }

    // Caps
    if(curr_caps != last_caps || force) {
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
    #endif
}

void keyboard_post_init_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT

    // Initialize the LCD
    lcd = qp_ili9341_make_spi_device(320, 240, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 1, 0);
    my_font = qp_load_font_mem(font_norse20);
    hermod_logo = qp_load_image_mem(gfx_hermod_logo);
    left_base_layout = qp_load_image_mem(gfx_left_base_layout);
    left_1_layout = qp_load_image_mem(gfx_left_1_layout);

    init_lcd();
    render_static_text();
    render_lcd(false);

    #ifdef BACKLIGHT_ENABLE
    // TODO ensure this is right
    backlight_enable();
    backlight_set(255);
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
    last_backlight = 255;
    #endif
}

bool process_record_keymap (uint16_t keycode, keyrecord_t *record) {

      return true;
}

/// ================ TAP DANCE CONFIG =================
#ifdef TAP_DANCE_ENABLE
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(tap_dance_state_t *state, void *user_data, uint16_t key_code);
void ql_reset(tap_dance_state_t *state, void *user_data);

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed)
            return TD_SINGLE_TAP;
        else
            return TD_SINGLE_HOLD;
    } else if (state->count == 2)
        return TD_DOUBLE_TAP;
    else
        return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {.is_press_action = true, .state = TD_NONE};

// Functions that control what our tap dance key does
void task_layer_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(SS_LOPT(SS_LSFT(SS_TAP(X_A))));
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN0);
            break;
        case TD_DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(_FN0)) {
                // If already set, then switch it off
                layer_off(_FN0);
            } else {
                // If not already set, then switch the layer on
                layer_on(_FN0);
            }
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
// void task_caps_lock(tap_dance_state_t *state, void *user_data) {
//     ql_tap_state.state = cur_dance(state);
//     switch (ql_tap_state.state) {
//         case TD_SINGLE_TAP:
//             tap_code(CAPS_WORD);
//             break;
//         case TD_SINGLE_HOLD:
//             break;
//         case TD_DOUBLE_TAP:
//             tap_code(KC_CAPS);
//             break;
//         default:
//             break;
//     }
// }

// Functions that control what our tap dance key does
void task_sc_left_shift(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING("(");
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LSFT);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_LSFT);
            tap_code(KC_LSFT);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void task_sc_right_shift(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(")");
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_RSFT);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_RSFT);
            tap_code(KC_RSFT);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void pw_one_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_ESC);
            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_LGUI(SS_LCTL(SS_TAP(X_Q))));
            break;
        case TD_DOUBLE_TAP:
            #ifdef SECRETS_H
            SEND_STRING(PW_ONE_STRING);
            #endif
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void pw_two_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:

            break;
        case TD_SINGLE_HOLD:
            // TODO define a hold action for pw two
            layer_on(_FN0);
            break;
        case TD_DOUBLE_TAP:
            #ifdef SECRETS_H
            SEND_STRING(PW_TWO_STRING);
            #endif
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void pw_three_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:

            break;
        case TD_SINGLE_HOLD:
            // TODO define a hold action for pw three
            layer_on(_FN0);
            break;
        case TD_DOUBLE_TAP:
            #ifdef SECRETS_H
            SEND_STRING(PW_THREE_STRING);
            #endif
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void pw_four_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:

            break;
        case TD_SINGLE_HOLD:
            // TODO define a hold action for pw four
            layer_on(_FN0);
            break;
        case TD_DOUBLE_TAP:
            #ifdef SECRETS_H
            SEND_STRING(PW_FOUR_STRING);
            #endif
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void tick_tick_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(SS_LGUI(SS_LOPT(SS_LSFT(SS_TAP(X_A)))));
            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_O))));
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_E))));
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void obsidian_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(SS_LGUI(SS_LOPT(SS_LSFT(SS_TAP(X_O)))));
            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_M))));
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(SS_LGUI(SS_LSFT(SS_LOPT(SS_LCTL(SS_TAP(X_O))))));
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void copy_file_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(SS_LGUI(SS_LOPT(SS_LCTL(SS_LSFT(SS_TAP(X_C))))));
            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_P))));
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(SS_LGUI(SS_LSFT(SS_LCTL(SS_TAP(X_C)))));
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void enter_super_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_PENT);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN0);
            break;
        case TD_DOUBLE_TAP:
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void l_par_gui_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_LPRN);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN0);
            register_code(KC_LGUI);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_LBRC);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void r_par_gui_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_RPRN);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN0);
            register_code(KC_RGUI);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_RBRC);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void l_alt_par_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_LPRN);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LALT);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_LBRC);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void l_ctl_par_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_RPRN);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LCTL);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_RBRC);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void r_alt_par_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_LPRN);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_RALT);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_LBRC);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void r_ctl_par_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_RPRN);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_RCTL);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_RBRC);
            break;
        default:
            break;
    }
}

// Functions that control what our tap dance key does
void refactor_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            SEND_STRING(SS_LSFT(SS_TAP(X_F6)));
            break;
        case TD_SINGLE_HOLD:
            is_held = true;
            break;
        case TD_DOUBLE_TAP:
        default:
            break;
    }
}

void ql_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_on(_BASE);
        layer_off(_FN0);
        unregister_code(KC_LSFT);
        unregister_code(KC_RSFT);
        unregister_code(KC_LGUI);
        unregister_code(KC_RGUI);
        unregister_code(KC_LCTL);
        unregister_code(KC_RCTL);
        unregister_code(KC_LALT);
        unregister_code(KC_RALT);
        is_held = false;
    }
    ql_tap_state.state = TD_NONE;
}

// clang-format off
// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [TASK_EMOJI_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, task_layer_finished, ql_reset),
    // [TD_CAPS_LOCK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, task_caps_lock, ql_reset),
    [TD_SC_LSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, task_sc_left_shift, ql_reset),
    [TD_SC_RSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, task_sc_right_shift, ql_reset),
    [TD_PW_ONE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_one_finished, ql_reset),
    [TD_PW_TWO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_two_finished, ql_reset),
    [TD_PW_THREE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_three_finished, ql_reset),
    [TD_PW_FOUR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_four_finished, ql_reset),
    [TD_TICK_TICK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tick_tick_finished, ql_reset),
    [TD_OBSIDIAN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, obsidian_finished, ql_reset),
    [TD_COPY_FILE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, copy_file_finished, ql_reset),
    [TD_VOL_UP] = ACTION_TAP_DANCE_DOUBLE(KC_KB_VOLUME_UP, KC_MPLY),
    [TD_VOL_DOWN] = ACTION_TAP_DANCE_DOUBLE(KC_KB_VOLUME_DOWN, KC_KB_MUTE),
    [TD_ENTER_SUPER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, enter_super_finished, ql_reset),
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
    [TD_GRV_LAYER] = ACTION_TAP_DANCE_LAYER_MOVE(KC_GRV, _FN0),
    [TD_LPAR_GUI] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l_par_gui_finished, ql_reset),
    [TD_RPAR_GUI] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, r_par_gui_finished, ql_reset),
    [TD_LALT_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l_alt_par_finished, ql_reset),
    [TD_LCTL_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l_ctl_par_finished, ql_reset),
    [TD_RALT_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, r_alt_par_finished, ql_reset),
    [TD_RCTL_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, r_ctl_par_finished, ql_reset),
    [TD_REFACTOR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, refactor_finished, ql_reset),
    // TODO add more tap dance actions here.
};
// clang-format on

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
}
#endif
/// ================ END TAP DANCE CONFIG =================
