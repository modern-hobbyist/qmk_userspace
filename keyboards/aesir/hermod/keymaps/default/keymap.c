// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <hal.h>
#include "common.h"
#include "lib/layer_status/layer_status.h"

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
    /*  Row:    0         1        2        3         4      */
    [_BASE] = LAYOUT(
                TO(_FN2), MO(_FN0), TO(_FN0)
            ),

    /*  Row:    0        1        2        3        4       */
    [_FN0] = LAYOUT(

                TO(_BASE), MO(_FN1), TO(_FN1)
            ),

    /*  Row:    0        1        2        3        4       */
    [_FN1] = LAYOUT(
                TO(_FN0), MO(_FN2), TO(_FN2)
                ),

    /*  Row:    0        1        2        3        4        */
    [_FN2] = LAYOUT(
                TO(_FN1), MO(_BASE), TO(_BASE)
            ),
};

// clang-format on
#ifdef OLED_ENABLE
bool oled_task_keymap(void) {
    render_layer_status();

    return true;
}
#endif
