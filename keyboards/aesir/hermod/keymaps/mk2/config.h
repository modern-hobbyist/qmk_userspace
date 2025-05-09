// #ifndef LCD_ACTIVITY_TIMEOUT
#undef LCD_ACTIVITY_TIMEOUT
#define LCD_ACTIVITY_TIMEOUT 300000
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT LCD_ACTIVITY_TIMEOUT
// #endif

// Uncomment this if you want to configure a secrets.h file
#define SECRETS_H
#define FEE_PAGE_COUNT 1
#define QUANTUM_PAINTER_PIXDATA_BUFFER_SIZE 512

#define TAPPING_TERM 150

// for keyboard-level data sync:
#define SPLIT_TRANSACTION_IDS_KB KEYBOARD_SYNC_A
