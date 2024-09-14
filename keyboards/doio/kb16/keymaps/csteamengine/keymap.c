/* Copyright 2022 DOIO
 * Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
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

#include QMK_KEYBOARD_H
#include "csteamengine.h"

// OLED animation
#include "lib/layer_status/layer_status.h"
#include "secrets.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

// clang-format off
enum layer_names {
    _BASE,
    _EMOJIS,
    _FN1,
    _FN2
};

// enum layer_keycodes { };
enum custom_keycodes {
    HARDPASTE = SAFE_RANGE,
    LOCK_SCREEN,
    INTELLIJ_BACK,
    INTELLIJ_FORWARD,
    CMD_SHIFT_T,
    CMD_SHIFT_P,
    DRAG_SCROLL,
    COPY_PASTA,
    DRAGON_DROP,
    FRY_DANCE,
    BENDER_DANCE,
    ITS_DONE,
    MARGED,
    MVP,
    LOL,
    MONKEY_LOOK,
    COPY_PATH,
    COPY_FILENAME,
    TICK_TICK_TASK,
    TICK_TICK_MINI,
    REFACTOR,
    RUN,
    EXTRACT_METHOD,
    INSPECT,
    DISTRACTION_FREE_MODE,
};

// Define a type for as many tap dance states as you need
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

enum {
    TASK_EMOJI_LAYER, // Our custom tap dance key; add any other tap dance keys to this enum
    TD_PW_ONE,
    TD_PW_TWO,
    TD_PW_THREE,
    TD_PW_FOUR,
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
    // rgb_matrix_enable();
    // rgb_matrix_set_color_all(RGB_BLUE);
    // if (host_keyboard_led_state().caps_lock) {
    //     rgb_matrix_set_color(0, RGB_BLUE);
    // } else {
    //     rgb_matrix_set_color(0, RGB_BLACK);
    // }
    return false;
}

// Functions associated with individual tap dances
void ql_finished(tap_dance_state_t *state, void *user_data, uint16_t key_code);
void ql_reset(tap_dance_state_t *state, void *user_data);

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*  Row:    0         1        2        3         4      */
    [_BASE] = LAYOUT(
                TD(TD_PW_ONE),     TD(TD_PW_TWO),    TD(TD_PW_THREE),    TD(TD_PW_FOUR),     QK_BOOT,
                REFACTOR,     RUN,    EXTRACT_METHOD,    INSPECT,     TO(_EMOJIS),
                INTELLIJ_BACK,     INTELLIJ_FORWARD,    COPY_PATH,   COPY_FILENAME,  KC_CAPS,
                TD(TASK_EMOJI_LAYER), SELECT_WORD, DISTRACTION_FREE_MODE, CMD_SHIFT_T
            ),

    /*  Row:    0        1        2        3        4       */
    [_EMOJIS] = LAYOUT(
                _______, _______, _______, _______, _______,
                DRAGON_DROP, COPY_PASTA, _______, _______, TO(_FN1),
                ITS_DONE, MARGED, LOL, MONKEY_LOOK, _______,
                TD(TASK_EMOJI_LAYER),     FRY_DANCE,    BENDER_DANCE,    KC_ENT
            ),

    /*  Row:    0        1        2        3        4       */
    [_FN1] = LAYOUT(
                _______, _______, _______, _______, EE_CLR,
                _______, _______, _______, _______, TO(_FN2),
                _______, _______, _______, _______, _______,
                _______, _______, _______, _______
            ),

    /*  Row:    0        1        2        3        4        */
    [_FN2] = LAYOUT(
                RGB_SPI, RGB_SPD, _______, QK_BOOT, _______,
                RGB_SAI, RGB_SAD, _______, _______, TO(_BASE),
                RGB_TOG, RGB_MOD, RGB_HUI, _______, _______,
                _______, RGB_VAI, RGB_HUD, RGB_VAD
            ),
};
// clang-format on

#ifdef OLED_ENABLE
bool oled_task_keymap(void) {
    render_layer_status();

    return true;
}
#endif

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE]   = {ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_DOWN, KC_UP), ENCODER_CCW_CW(KC_PGDN, KC_PGUP)},
    [_EMOJIS] = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_MS_WH_RIGHT, KC_MS_WH_LEFT)},
    [_FN1]    = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [_FN2]    = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
};
#endif

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ITS_DONE:
            if (record->event.pressed) {
                SEND_STRING(":itsdoneintensifies:");
            } else {
            }
            break;
        case MARGED:
            if (record->event.pressed) {
                SEND_STRING(":marged:");
            } else {
            }
            break;
        case LOL:
            if (record->event.pressed) {
                SEND_STRING(":lol:");
            } else {
            }
            break;
        case MVP:
            if (record->event.pressed) {
                SEND_STRING(":mvp:");
            } else {
            }
            break;
        case COPY_PATH:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_C))));
            } else {
            }
            break;
        case COPY_FILENAME:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LOPT(SS_LCTL(SS_LSFT(SS_TAP(X_C))))));
            } else {
            }
            break;
        case TICK_TICK_TASK:
            if (record->event.pressed) {
                SEND_STRING(SS_LOPT(SS_LSFT(SS_TAP(X_A))));
            } else {
            }
            break;
        case TICK_TICK_MINI:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_O))));
            } else {
            }
            break;
        case FRY_DANCE:
            if (record->event.pressed) {
                SEND_STRING(":frydance:");
            } else {
            }
            break;
        case BENDER_DANCE:
            if (record->event.pressed) {
                SEND_STRING(":benderdance:");
            } else {
            }
            break;
        case MONKEY_LOOK:
            if (record->event.pressed) {
                SEND_STRING(":monkey-look:");
            } else {
            }
            break;
        case HARDPASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_DOWN(X_V))));
            } else {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_UP(X_V))));
            }
            break;
        case LOCK_SCREEN:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LCTL(SS_TAP(X_Q))));
            } else {
            }
            break;
        case INTELLIJ_BACK:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_TAP(X_LEFT_BRACKET)));
            } else {
            }
            break;
        case INTELLIJ_FORWARD:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_TAP(X_RIGHT_BRACKET)));
            } else {
            }
            break;
        case CMD_SHIFT_T:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_T))));
            } else {
            }
            break;
        case CMD_SHIFT_P:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_P))));
            } else {
            }
            break;
        case REFACTOR:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_F6)));
            } else {
            }
            break;
        case RUN:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_TAP(X_4)));
            } else {
            }
            break;
        case EXTRACT_METHOD:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LOPT(SS_TAP(X_M))));
            } else {
            }
            break;
        case INSPECT:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LOPT(SS_TAP(X_I))));
            } else {
            }
            break;
        case DISTRACTION_FREE_MODE:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_F12))));
            } else {
            }
            break;
        case DRAGON_DROP:
            if (record->event.pressed) {
                SEND_STRING(":dragon::droplet:");
            } else {
            }
            break;
        case COPY_PASTA:
            if (record->event.pressed) {
                SEND_STRING(":copyright::spaghetti:");
            } else {
            }
            break;
    }
    return true;
};

/* Runs after each key press, check if activity occurred */
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGB_MATRIX_TIMEOUT
    if (record->event.pressed) refresh_rgb();
#endif
}

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
            layer_on(_EMOJIS);
            break;
        case TD_DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(_EMOJIS)) {
                // If already set, then switch it off
                layer_off(_EMOJIS);
            } else {
                // If not already set, then switch the layer on
                layer_on(_EMOJIS);
            }
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
            SEND_STRING(SS_TAP(X_SPC));
            break;
        case TD_SINGLE_HOLD:
            // TODO define a hold action for pw one
            layer_on(_EMOJIS);
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(PW_ONE_STRING);
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
            layer_on(_EMOJIS);
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(PW_TWO_STRING);
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
            layer_on(_EMOJIS);
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(PW_THREE_STRING);
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
            layer_on(_EMOJIS);
            break;
        case TD_DOUBLE_TAP:
            SEND_STRING(PW_FOUR_STRING);
            break;
        default:
            break;
    }
}

void ql_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_on(_BASE);
        layer_off(_EMOJIS);
        layer_off(_FN1);
        layer_off(_FN2);
    }
    ql_tap_state.state = TD_NONE;
}

// clang-format off
// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [TASK_EMOJI_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, task_layer_finished, ql_reset),
    [TD_PW_ONE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_one_finished, ql_reset),
    [TD_PW_TWO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_two_finished, ql_reset),
    [TD_PW_THREE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_three_finished, ql_reset),
    [TD_PW_FOUR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pw_four_finished, ql_reset),
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
