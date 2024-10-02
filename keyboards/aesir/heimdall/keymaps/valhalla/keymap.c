// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// #include "secrets.h"
#include "csteamengine.h"
#include "lib/layer_status/layer_status.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
                 SELECT_WORD,   TO(_BASE),   TO(_FN0),   TO(_FN1),   TO(_FN2),   MO(_FN2),   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,
        QK_BOOT,   KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13, KC_DEL,    KC_NO,      KC_END,      KC_HOME,    KC_NUM,
        QK_RBT,   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,    KC_BSPC,         KC_PPLS,     KC_PMNS,     KC_PAST,    KC_PSLS,
        KC_NO,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS,         KC_KP_7,     KC_KP_8,     KC_KP_9,    KC_KP_DOT,
        KC_NO,   KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,          KC_KP_4,     KC_KP_5,     KC_KP_6,    KC_UP,
        RGB_HUI,   KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,            KC_RSFT,         KC_KP_1,     KC_KP_2,     KC_KP_3,    KC_DOWN,
        RGB_HUD,   KC_LCTL, KC_LALT, KC_LGUI,                            KC_SPC,                             KC_RGUI, KC_RALT, KC_APP,    KC_RCTL,         KC_PENT,     KC_KP_0,     KC_LEFT,    KC_RIGHT
    )
};
// clang-format on

static uint32_t key_timer;               // timer for last keyboard activity, use 32bit value and function to make longer idle time possible
static void     refresh_rgb(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void     check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
bool            is_rgb_timeout = false;  // store if RGB has timed out or not in a boolean

void refresh_rgb(void) {
    key_timer = timer_read32(); // store time of last refresh
    if (is_rgb_timeout) {
        is_rgb_timeout = false;
        rgb_matrix_enable_noeeprom();
    }
}

void check_rgb_timeout(void) {
    if (!is_rgb_timeout && timer_elapsed32(key_timer) > RGB_MATRIX_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    {
        rgb_matrix_disable_noeeprom();
        is_rgb_timeout = true;
    }
}
/* Then, call the above functions from QMK's built in post processing functions like so */
/* Runs at the end of each scan loop, check if RGB timeout has occured or not */
void housekeeping_task_user(void) {
#ifdef RGB_MATRIX_TIMEOUT
    check_rgb_timeout();
#endif
}

/* Runs after each encoder tick, check if activity occurred */
void post_encoder_update_user(uint8_t index, bool clockwise) {
#ifdef RGB_MATRIX_TIMEOUT
    refresh_rgb();
#endif
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
}

// Use this to clear eeprom
// void eeconfig_init_user(void) {
//     eeconfig_update_rgb_matrix_default();
//     rgb_matrix_enable_noeeprom();
// }

bool rgb_matrix_indicators_user(void) {
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(8, RGB_BLUE);
    } else {
        rgb_matrix_set_color(8, RGB_BLACK);
    }
    return false;
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
// Define a type for as many tap dance states as you need
typedef enum { TD_NONE, TD_UNKNOWN, TD_SINGLE_TAP, TD_SINGLE_HOLD, TD_DOUBLE_TAP } td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
    uint16_t   single_tap_keycode;
    uint16_t   double_tap_keycode;
    uint16_t   held;
    uint16_t   hold;
} td_tap_t;

enum {
    TASK_EMOJI_LAYER
    // TODO add more Tap Dance keycodes here.
};

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

void ql_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_on(_BASE);
        layer_off(_FN0);
        layer_off(_FN1);
        layer_off(_FN2);
    }
    ql_tap_state.state = TD_NONE;
}

// clang-format off
// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [TASK_EMOJI_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, task_layer_finished, ql_reset),
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
