#pragma once

enum userspace_keycodes {
    SELECT_WORD = SAFE_RANGE,
    PWONE,
    PWTWO,
    PWTHREE,
    PWFOUR,
};

enum layer_names {
    _BASE,
    _FN0,
    #ifdef EXTRA_LAYERS
    _FN1,
    _FN2
    #endif
};
