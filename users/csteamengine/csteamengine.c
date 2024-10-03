// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "csteamengine.h"
#include "features/select_word.h"

#include "quantum.h"

// OLED animation
#include "lib/logo.h"

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
