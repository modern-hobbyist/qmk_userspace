#pragma once

#define EXTRA_LAYERS
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define CAPS_WORD_INVERT_ON_SHIFT
#define CAPS_WORD_IDLE_TIMEOUT 2000  // 3 seconds.
#define SELECT_WORD_TIMEOUT 2000 // When idle, clear state after 2 seconds.

#define RGB_MATRIX_TIMEOUT 900000 // number of milliseconds to wait until rgb automatically turns off
#define RGB_MATRIX_SLEEP
#define RGB_MATRIX_DEFAULT_HUE 100
#define RGB_MATRIX_DEFAULT_SAT 255
#define RGB_MATRIX_DEFAULT_VAL 200
#define RGBLIGHT_LIMIT_VAL 125
