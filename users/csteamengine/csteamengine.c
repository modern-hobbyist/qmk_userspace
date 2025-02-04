// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "csteamengine.h"
#include "features/select_word.h"

#include "quantum.h"

// OLED animation
#ifdef OLED_ENABLE
#include "lib/logo.h"
#endif

#ifdef RGB_MATRIX_TIMEOUT
static uint32_t key_timer;               // timer for last keyboard activity, use 32bit value and function to make longer idle time possible
static void     refresh_rgb(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void     check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
bool            is_rgb_timeout = false;  // store if RGB has timed out or not in a boolean

void refresh_rgb(void) {
    #ifdef RGB_MATRIX_TIMEOUT
    key_timer = timer_read32(); // store time of last refresh
    if (is_rgb_timeout) {
        is_rgb_timeout = false;
        rgb_matrix_enable_noeeprom();
    }
    #endif
}

void check_rgb_timeout(void) {
    #ifdef RGB_MATRIX_TIMEOUT
    if (!is_rgb_timeout && timer_elapsed32(key_timer) > RGB_MATRIX_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    {
        rgb_matrix_disable_noeeprom();
        is_rgb_timeout = true;
    }
    #endif
}
#endif

__attribute__ ((weak))
void housekeeping_task_keymap(void) {
}

// /* Then, call the above functions from QMK's built in post processing functions like so */
// /* Runs at the end of each scan loop, check if RGB timeout has occured or not */
void housekeeping_task_user(void) {
    housekeeping_task_keymap();
#ifdef RGB_MATRIX_TIMEOUT
    check_rgb_timeout();
#endif
}


__attribute__ ((weak))
void post_encoder_update_keymap(uint8_t index, bool clockwise) {
}


__attribute__ ((weak))
void keyboard_post_init_keymap(void) {
}

void keyboard_post_init_user(void) {
    debug_enable=true;
    keyboard_post_init_keymap();
}


__attribute__ ((weak))
void keyboard_pre_init_keymap(void) {
}

void keyboard_pre_init_user(void) {
    keyboard_pre_init_keymap();
}

/* Runs after each encoder tick, check if activity occurred */
void post_encoder_update_user(uint8_t index, bool clockwise) {
#ifdef RGB_MATRIX_TIMEOUT
    refresh_rgb();
#endif
}

__attribute__ ((weak))
void post_process_record_keymap(uint16_t keycode, keyrecord_t *record) {
}

/* Runs after each key press, check if activity occurred */
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGB_MATRIX_TIMEOUT
    if (record->event.pressed) refresh_rgb();
#endif
}
__attribute__ ((weak))
void suspend_power_down_keymap(void) {
    // code will run multiple times while keyboard is suspended
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    suspend_power_down_keymap();
}

__attribute__ ((weak))
void suspend_wakeup_init_keymap(void) {
    // code will run on keyboard wakeup
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    suspend_wakeup_init_keymap();
}

// Default timeout for displaying boot logo.
#ifndef OLED_LOGO_TIMEOUT
#    define OLED_LOGO_TIMEOUT 3500
#endif

__attribute__ ((weak))
bool process_record_keymap (uint16_t keycode, keyrecord_t *record) {
  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_select_word(keycode, record, SELECT_WORD)) { return false; }
    if (!process_caps_word(keycode, record)) { return false; }

    if (!process_record_keymap(keycode, record)) {
        return false;
    }

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
        case OPEN_TICK_TICK:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_E))));
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
        case NEW_OBSIDIAN_NOTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_LOPT(SS_LCTL(SS_TAP(X_O))))));
            } else {
            }
            break;
        case OPEN_OBSIDIAN:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LOPT(SS_LSFT(SS_TAP(X_O)))));
            } else {
            }
            break;
        case MOVE_OBSIDIAN_FILE:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_M))));
            } else {
            }
            break;
        case MAC_EMOJIS:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LCTL(SS_TAP(X_SPACE))));
            } else {
            }
            break;
        case SCREEN_RECORDING_1:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_1))));
            } else {
            }
            break;
        case SCREEN_SHOT_TO_CLIPBOARD:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_2))));
            } else {
            }
            break;
        case SCREEN_SHOT_AREA:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_3))));
            } else {
            }
            break;
        case SCREEN_SHOT_SCREEN:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_4))));
            } else {
            }
            break;
    }
    // other macros
    return true;
}

#ifdef OLED_ENABLE
uint16_t startup_timer;

__attribute__ ((weak))
oled_rotation_t oled_init_keymap (oled_rotation_t rotation) {
  return false;
}

__attribute__ ((weak))
bool oled_task_keymap (void) {
  return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    startup_timer = timer_read();
    // oled_init(rotation);
    return oled_init_keymap(rotation);
}

bool oled_task_user(void) {
    static bool finished_logo = false;

    if (!finished_logo) {
        finished_logo = render_logo();
    } else {
        if (!oled_task_keymap()) {
            return false;
        }
    }

    return true;
}
#endif
