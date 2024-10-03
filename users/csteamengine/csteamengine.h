#pragma once

enum userspace_keycodes {
    SELECT_WORD = SAFE_RANGE,
    PWONE,
    PWTWO,
    PWTHREE,
    PWFOUR,
    HARDPASTE,
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
    OPEN_TICK_TICK,
    TICK_TICK_MINI,
    REFACTOR,
    RUN,
    EXTRACT_METHOD,
    INSPECT,
    DISTRACTION_FREE_MODE,
    OPEN_OBSIDIAN,
    NEW_OBSIDIAN_NOTE,
    MOVE_OBSIDIAN_FILE,
};

enum layer_names {
    _BASE,
    _FN0,
    #ifdef EXTRA_LAYERS
    _FN1,
    _FN2
    #endif
};
