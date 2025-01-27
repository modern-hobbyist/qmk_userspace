// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "csteamengine.h"
#include "lib/layer_status/layer_status.h"
#include <qp.h>
#include "./norse20.qff.h"

painter_device_t lcd;
#ifdef BACKLIGHT_ENABLE
static uint8_t last_backlight = 255;
#endif

#ifdef SECRETS_H
#include "secrets.h"
#endif

static painter_font_handle_t my_font;

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
    TD_ENTER_SUPER
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
                // Left Half Numpad                           Left Half                                                                                             Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          REFACTOR,         KC_ESC,     KC_NO,    KC_NO,  KC_NO,    KC_NO,              KC_NO,                  TD(TD_PW_ONE),      TD(TD_PW_TWO),  TD(TD_PW_THREE),      KC_NO,      KC_NO,      KC_DEL,     KC_BSPC,                KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          EXTRACT_METHOD,   KC_GRV,     KC_1,     KC_2,   KC_3,     KC_4,               KC_5,                   KC_6,               KC_7,           KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          TD(TD_COPY_FILE), KC_TAB,     KC_Q,     KC_W,   KC_E,     KC_R,               KC_T,                   KC_Y,               KC_U,           KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,                KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            TD(TD_OBSIDIAN),  KC_LSFT,    KC_A,     KC_S,   KC_D,     KC_F,               KC_G,                   KC_H,               KC_J,           KC_K,       KC_L,       KC_SCLN,    KC_QUOT,    KC_ENT,                  KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          TD(TD_TICK_TICK), KC_CAPS,    KC_Z,     KC_X,   KC_C,     KC_V,               KC_B,                   KC_N,               KC_M,           KC_COMM,    KC_DOT,     KC_SLSH,    KC_MINS,    KC_RSFT,                KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
                                                                                                                        KC_LALT,            KC_LCTL,                KC_LALT,            KC_LCTL,                    KC_UP,
                                                                                                                        LT(_FN0, KC_LGUI),  KC_SPACE,               LT(_FN0, KC_RGUI),  KC_SPACE,       KC_LEFT,    KC_DOWN,    KC_RIGHT
            ),
    [_FN0] = LAYOUT(
                // Left Half Numpad                           Left Half                                                                                             Right Half                                                                                          Right Half Numpad
                KC_NUM, KC_PSLS, KC_PAST,   KC_PMNS,          RGB_TOG,      KC_ESC,     RGB_VAD,    RGB_VAI,     KC_NO,         KC_NO,      KC_NO,                      KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,      KC_DEL,     G(KC_BSPC),             KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
                KC_P7,  KC_P8,   KC_P9,     KC_PPLS,          RGB_MOD,      G(KC_GRV),  KC_1,       KC_2,        KC_3,          KC_4,       KC_5,                       KC_6,       KC_7,       KC_8,           KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_P7,    KC_P8,   KC_P9,     KC_PPLS,
                KC_P4,  KC_P5,   KC_P6,     KC_PEQL,          RGB_RMOD,     G(KC_TAB),  G(KC_Q),    G(KC_W),     G(KC_E),       G(KC_R),    G(KC_T),                    KC_Y,       G(KC_U),    G(KC_I),        KC_O,       G(KC_P),    KC_LBRC,    KC_RBRC,                KC_P4,    KC_P5,   KC_P6,     KC_PEQL,
                KC_P1,  KC_P2,   KC_P3,     KC_NO,            RGB_HUI,      KC_LSFT,    G(KC_A),    G(KC_S),     KC_D,          KC_F,       KC_G,                       KC_H,       KC_J,       KC_K,           G(KC_L),    KC_SCLN,    KC_QUOT,    KC_NO,                  KC_P1,    KC_P2,   KC_P3,     KC_NO,
                KC_NO,  KC_P0,   KC_PDOT,   KC_PENT,          RGB_HUD,      KC_CAPS,    G(KC_Z),    G(KC_X),     G(KC_C),       G(KC_V),    G(KC_B),                    G(KC_N),    KC_M,       KC_COMM,        KC_DOT,     G(KC_SLSH), KC_MINS,    KC_RSFT,                KC_PENT,  KC_P0,   KC_PDOT,   KC_PENT,
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

        gpio_write_pin_low(LCD_ENABLE_PIN);
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


        gpio_write_pin_low(LCD_ENABLE_PIN);
        qp_power(lcd, false);
        is_lcd_timeout = true;
    }
}
#endif

void keyboard_pre_init_keymap(void) {
    gpio_set_pin_output(LCD_ENABLE_PIN);

    gpio_write_pin_low(LCD_ENABLE_PIN);
}

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
    qp_rect(lcd, 0, 0, 240, 320, 6, 0, 0, true);

    my_font = qp_load_font_mem(font_norse20);
    if (my_font != NULL) {
        static const char *text = "Aesir | Project Hermod";
        int16_t width = qp_textwidth(my_font, text);

        qp_drawtext(lcd, (120 - width/2), (160 - my_font->line_height/2), my_font, text);
    }

    if (is_keyboard_left()) {
        static const char *text = "Left";
        int16_t width = qp_textwidth(my_font, text);

        qp_drawtext(lcd, (120 - width/2), 0, my_font, text);
    } else {
        static const char *text = "Right";
        int16_t width = qp_textwidth(my_font, text);

        qp_drawtext(lcd, (120 - width/2), 0, my_font, text);
    }

    gpio_write_pin_high(LCD_ENABLE_PIN);
    qp_power(lcd, true);
}

void housekeeping_task_keymap(void) {
    #ifdef LCD_ACTIVITY_TIMEOUT
        check_lcd_timeout();
    #endif
}

void suspend_power_down_keymap(void) {
    #ifdef BACKLIGHT_ENABLE
    if (last_backlight == 255) {
        last_backlight = get_backlight_level();
    }
    backlight_set(0);
    #endif

    gpio_write_pin_low(LCD_ENABLE_PIN);
    qp_power(lcd, false);
}

void suspend_wakeup_init_keymap(void) {
    gpio_write_pin_low(LCD_ENABLE_PIN);
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

void ql_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_on(_BASE);
        layer_off(_FN0);
        layer_off(_FN1);
        layer_off(_FN2);
        unregister_code(KC_LSFT);
        unregister_code(KC_RSFT);
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
