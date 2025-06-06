// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * This is a standard version of this same keyboard. I added optional 2u keys for 0, + and Enter on the numpad
     * For my personal board, I will be splitting them up to have more macros, but if you want a more standard layout
     * You can install this one. The one downside with this, is you won't have arrow keys on the main layer anymore.
     *       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     *       │M0 │M1 │M2 │M3 │M4 │M5 │M6 │M7 │M8 │M9 │M10│M11│M12│M13│M14│
     *       └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     * ┌───┐ ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐ ┌───┬───┬───┬───┐
     * │M17│ │Esc│F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│F13│M15│ │Del│End│Hom│M16│
     * ├───┤ ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┴───┤ ┌───┬───┬───┬───┐
     * │M18│ │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│ │ + │ - │ * │ / │
     * ├───┤ ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┼───┤
     * │M19│ │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │ │ 7 │ 8 │ 9 │   │ <-- KC_NO filler
     * ├───┤ ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ ├───┼───┼───┤ + │
     * │M20│ │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │ │ 4 │ 5 │ 6 │   │
     * ├───┤ ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤ ├───┼───┼───┼───┤
     * │M21│ │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │    Shift │ │ 1 │ 2 │ 3 │   │ <-- KC_NO filler
     * ├───┤ ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ├───┴───┼───┤Ent│
     * │M22│ │Ctrl│GUI │Alt │                        │ Alt│ GUI│Menu│Ctrl│ │   0   │ . │   │
     * └───┘ └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───────┴───┴───┘
     *                                                                       ^-- KC_NO filler
     */
    [0] = LAYOUT(
                 KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,
        KC_NO,   KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13, KC_NO,    KC_INS,    KC_HOME,   KC_HOME,  KC_PSLS,
        KC_NO,   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,    KC_BSPC,         KC_DEL,    KC_END,    KC_PAST,  KC_PMNS,
        KC_NO,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS,         KC_P7,     KC_P8,     KC_P9,    KC_NO,
        KC_NO,   KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,          KC_P4,     KC_P5,     KC_P6,    KC_PPLS,
        KC_NO,   KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,            KC_RSFT,         KC_P1,     KC_P2,     KC_P3,    KC_NO,
        KC_NO,   KC_LCTL, KC_LALT, KC_LGUI,                            KC_SPC,                             KC_RGUI, KC_RALT, KC_APP,    KC_RCTL,         KC_NO,     KC_P0,     KC_PDOT,  KC_PENT
    )
};
// clang-format on
