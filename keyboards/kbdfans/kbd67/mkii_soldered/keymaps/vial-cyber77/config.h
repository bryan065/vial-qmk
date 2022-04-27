/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#undef BACKLIGHT_PIN
#undef DEBOUNCE

// User settings
#define USB_POLLING_INTERVAL_MS 2
#define QMK_KEYS_PER_SCAN 8
#define DEBOUNCE 2

#undef PRODUCT
#define PRODUCT Cyber77
#define FORCE_NKRO
#define LAYER_STATE_8BIT

#define RGB_CUSTOM_TIMEOUT_DELAY 900000     // Timout delay in ms. Set to 15 minutes
#define PREDEFINED_TAP_DANCE
#define LOST_ARK

/* Custom user specific features
*
*  Startup animation duration time and fade duration can be set here.
*
*  Defaults:
*  STARTUP_ANIM_TIME = 2200
*  FADE_TIME = 4    (Fast = 3 / Medium = 6 / Slow = 10)
*/
//#define STARTUP_ANIM_TIME 2200
//#define FADE_TIME 4

//Encoder pins
#ifdef ENCODER_ENABLE
    #define ENCODERS_PAD_A { B3 } // MISO
    #define ENCODERS_PAD_B { B1 } // SCLK / SCK
    #define ENCODER_DEFAULT_POS 0x3 // Fix encoder skips when changing direction
#endif

// Vial config
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
#define VIAL_TAP_DANCE_ENTRIES 2
#define PREDEFINED_TAP_DANCE

#define VIAL_KEYBOARD_UID {0x95, 0x31, 0xB4, 0xAD, 0xD9, 0x7A, 0xE9, 0x07}

/* Vial unlock keycombo: ESC + Return */
#define VIAL_UNLOCK_COMBO_ROWS { 0, 2 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 13}

// Vial firmware size tweaks
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define NO_ACTION_ONESHOT

//RGB Matrix Effects
#define RGBW
#define RGB_DI_PIN B2                       // Assign to pin B2 / MOSI on board
#define DRIVER_LED_TOTAL 76                 // The number of LEDs connected
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 190
#define RGB_MATRIX_STARTUP_SPD 64
#define RGB_DISABLE_WHEN_USB_SUSPENDED

#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CUSTOM_HUE_PENDULUM2
#define RGB_MATRIX_STARTUP_HUE 170

#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL

#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH

#define RGB_MATRIX_KEYPRESSES

#define RGB_MATRIX_STARTUP_SPD 64
#define RGB_MATRIX_LED_PROCESS_LIMIT 10
#define RGB_MATRIX_LED_FLUSH_LIMIT 26

//SK6812 Tweak
#define WS2812_TRST_US 80