// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// #include "secrets.h"
#include "csteamengine.h"
#ifdef SECRETS_H
#include "secrets.h"
#endif
#include "lib/layer_status/layer_status.h"

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

bool CAPS_WORD_STATE = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
                                KC_ESC,             SCREEN_RECORDING_1, SCREEN_SHOT_TO_CLIPBOARD,   SCREEN_SHOT_AREA,   SCREEN_SHOT_SCREEN, KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   MAC_EMOJIS,    TD(TD_PW_THREE),        TD(TD_PW_TWO),  TD(TD_PW_ONE),
        REFACTOR,               KC_ESC,             KC_F1,              KC_F2,                      KC_F3,              KC_F4,              KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,        KC_F12,                 KC_F13,         KC_NO,          KC_PPLS,    KC_END,      KC_HOME,           KC_NUM,
        EXTRACT_METHOD,         KC_GRV,             KC_1,               KC_2,                       KC_3,               KC_4,               KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,       KC_EQL,                 KC_BSPC,                        KC_DEL,     KC_PMNS,     KC_PAST,           KC_PSLS,
        SELECT_WORD,            KC_TAB,             KC_Q,               KC_W,                       KC_E,               KC_R,               KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,       KC_RBRC,                KC_BSLS,                        KC_KP_DOT,  KC_KP_7,     KC_KP_8,           KC_KP_9,
        TD(TD_COPY_FILE),       KC_CAPS,            KC_A,               KC_S,                       KC_D,               KC_F,               KC_G,       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,       KC_ENT,                                                 KC_UP,      KC_KP_4,     KC_KP_5,           KC_KP_6,
        TD(TD_OBSIDIAN),        KC_LSFT,            KC_Z,               KC_X,                       KC_C,               KC_V,               KC_B,       KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                                                               KC_DOWN,    KC_KP_1,     KC_KP_2,           KC_KP_3,
        TD(TD_TICK_TICK),       KC_LCTL,            KC_LALT,            KC_LGUI,                                        KC_SPC,                                                    KC_RGUI, KC_RALT, KC_APP,        KC_RCTL,                                                KC_LEFT,    KC_RIGHT,    KC_P0,             TD(TD_ENTER_SUPER)
    ),
    [_FN0] = LAYOUT(
                                KC_ESC,             TO(_FN1),           KC_NO,              KC_NO,              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,       KC_NO,      KC_NO,      KC_NO,
        RGB_M_P,                KC_ESC,             KC_F1,              KC_F2,              KC_F3,              KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,      KC_F12,     KC_F13,     KC_NO,  KC_PPLS,            KC_END,      KC_HOME,     KC_NUM,
        RGB_TOG,                KC_GRV,             KC_1,               KC_2,               KC_3,               KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,     KC_EQL,     KC_BSPC,            KC_DEL,             KC_PMNS,     KC_PAST,     KC_PSLS,
        RGB_MOD,                KC_TAB,             KC_Q,               KC_W,               KC_E,               KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,     KC_RBRC,    KC_BSLS,            KC_MUTE,            KC_KP_7,     KC_KP_8,     KC_KP_9,
        RGB_RMOD,               KC_CAPS,            KC_A,               KC_S,               KC_D,               KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,     KC_ENT,                         KC_KB_VOLUME_UP,    KC_KP_4,     KC_KP_5,     KC_KP_6,
        RGB_HUI,                KC_LSFT,            KC_Z,               KC_X,               KC_C,               KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                                     KC_KB_VOLUME_DOWN,  KC_KP_1,     KC_KP_2,     KC_KP_3,
        RGB_HUD,                KC_LCTL,            KC_LALT,            KC_LGUI,                                KC_SPC,                             KC_RGUI, KC_RALT, KC_APP,  KC_RCTL,                                     KC_MPRV,            KC_MNXT,     KC_MPLY,     KC_PENT
    ),
    [_FN1] = LAYOUT(
                                KC_ESC,             TO(_FN2),           KC_NO,              KC_NO,              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,       KC_NO,      KC_NO,      KC_NO,
        KC_NO,                  KC_ESC,             KC_F1,              KC_F2,              KC_F3,              KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,      KC_F12,     KC_F13,     KC_NO,  KC_PPLS,    KC_END,      KC_HOME,     KC_NUM,
        KC_NO,                  KC_GRV,             KC_1,               KC_2,               KC_3,               KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,     KC_EQL,     KC_BSPC,            KC_DEL,     KC_PMNS,     KC_PAST,     KC_PSLS,
        SELECT_WORD,            KC_TAB,             KC_Q,               KC_W,               KC_E,               KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,     KC_RBRC,    KC_BSLS,            KC_KP_DOT,  KC_KP_7,     KC_KP_8,     KC_KP_9,
        CW_TOGG,                KC_CAPS,            KC_A,               KC_S,               KC_D,               KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,     KC_ENT,                         KC_UP,      KC_KP_4,     KC_KP_5,     KC_KP_6,
        TD(TD_OBSIDIAN),        KC_LSFT,            KC_Z,               KC_X,               KC_C,               KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                                     KC_DOWN,    KC_KP_1,     KC_KP_2,     KC_KP_3,
        TD(TD_TICK_TICK),       KC_LCTL,            KC_LALT,            KC_LGUI,                                KC_SPC,                             KC_RGUI, KC_RALT, KC_APP,  KC_RCTL,                                     KC_LEFT,    KC_RIGHT,    KC_KP_0,     KC_PENT
    ),
    [_FN2] = LAYOUT(
                                KC_ESC,             TO(_BASE),           KC_NO,              KC_NO,              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,       KC_NO,      KC_NO,      KC_NO,
        KC_NO,                  KC_ESC,             KC_F1,              KC_F2,              KC_F3,              KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,      KC_F12,     KC_F13,     KC_NO,  KC_PPLS,    KC_END,      KC_HOME,     KC_NUM,
        KC_NO,                  KC_GRV,             KC_1,               KC_2,               KC_3,               KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,     KC_EQL,     KC_BSPC,            KC_DEL,     KC_PMNS,     KC_PAST,     KC_PSLS,
        SELECT_WORD,            KC_TAB,             KC_Q,               KC_W,               KC_E,               KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,     KC_RBRC,    KC_BSLS,            KC_KP_DOT,  KC_KP_7,     KC_KP_8,     KC_KP_9,
        CW_TOGG,                KC_CAPS,            KC_A,               KC_S,               KC_D,               KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,     KC_ENT,                         KC_UP,      KC_KP_4,     KC_KP_5,     KC_KP_6,
        TD(TD_OBSIDIAN),        KC_LSFT,            KC_Z,               KC_X,               KC_C,               KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                                     KC_DOWN,    KC_KP_1,     KC_KP_2,     KC_KP_3,
        TD(TD_TICK_TICK),       KC_LCTL,            KC_LALT,            KC_LGUI,                                KC_SPC,                             KC_RGUI, KC_RALT, KC_APP,  KC_RCTL,                                     KC_LEFT,    KC_RIGHT,    KC_KP_0,     KC_PENT
    ),
};
// clang-format on

void suspend_power_down_keymap(void) {
    // code will run multiple times while keyboard is suspended
}

void suspend_wakeup_init_keymap(void) {
    // code will run on keyboard wakeup
}

// Use this to clear eeprom
// void eeconfig_init_user(void) {
//     eeconfig_update_rgb_matrix_default();
//     rgb_matrix_enable_noeeprom();
// }

bool rgb_matrix_indicators_user(void) {
    if (host_keyboard_led_state().caps_lock || CAPS_WORD_STATE) {
        rgb_matrix_set_color(8, RGB_BLUE);
    } else {
        rgb_matrix_set_color(8, RGB_BLACK);
    }
    return false;
}

void caps_word_set_user(bool active) {
    if (active || host_keyboard_led_state().caps_lock) {
        CAPS_WORD_STATE = true;
    } else {
        CAPS_WORD_STATE = false;
    }
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {

    return true;
}

void matrix_scan_user(void) {
    // Other tasks...
}

/**
 * Default OLED driver, should show the default layer states, though may need to share those across keymaps?
 */
#ifdef OLED_ENABLE
oled_rotation_t oled_init_keymap(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

bool oled_task_keymap(void) {
    render_layer_status();
    // render_bongocat();

    return true;
}
#endif

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
