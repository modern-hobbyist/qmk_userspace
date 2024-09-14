// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "csteamengine.h"
#include "secrets.h"

#include "quantum.h"
#include "common.h"

#include "features/select_word.h"

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
    // other macros
    return true;
}

// #ifdef OLED_ENABLE
uint16_t startup_timer;

__attribute__ ((weak))
bool oled_task_keymap (void) {
  return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    startup_timer = timer_read();
    return rotation;
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
// #endif
