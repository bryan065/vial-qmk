/*
  Copyright (c) 2022 Bryan065

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#pragma once

#include "quantum.h"

// Per layer RGB options
struct RGB_MATRIX_INDICATOR {
    bool flag;
    uint8_t hsv_h;
    uint8_t hsv_s;
    uint8_t hsv_v;
};

extern struct RGB_MATRIX_INDICATOR indicator_matrix[8];
extern struct RGB_MATRIX_INDICATOR indicator_underglow[8];

#if (PERKEY_HUE < 0) || (PERKEY_HUE > 255)
    #error "PERKEY_HUE needs to be within 0-255"
#elif !defined (PERKEY_HUE)
    #define PERKEY_HUE 0
#endif

#if (PERKEY_SAT < 0) || (PERKEY_SAT > 255)
    #error "PERKEY_SAT needs to be within 0-255"
#elif !defined (PERKEY_SAT)
    #define PERKEY_SAT 0
#endif

// Define custom values if not defined in config.h
#if STARTUP_ANIM_TIME < 0
#   error STARTUP_ANIM_TIME must be greater than 0
#elif !defined (STARTUP_ANIM_TIME)
#   define STARTUP_ANIM_TIME 2200
#endif

// Ensure the built in RGB_DISABLE_TIMEOUT is disabled if you enable RGB_CUSTOM_TIMEOUT_DELAY
#if (RGB_CUSTOM_TIMEOUT_DELAY > 0)
  #undef RGB_DISABLE_TIMEOUT
  #define RGB_DISABLE_TIMEOUT 0
#endif

// RGB_CUSTOM_TIMEOUT_DELAY setup
#if RGB_DISABLE_TIMEOUT == 0
  #if (RGB_CUSTOM_TIMEOUT_DELAY < 30000) && (RGB_CUSTOM_TIMEOUT_DELAY > 0)
    #undef RGB_CUSTOM_TIMEOUT_DELAY
    #define RGB_CUSTOM_TIMEOUT_DELAY 30000
  #endif

    uint32_t     rgb_anykey_timeout;
    bool         timeout;
#endif

// Fade time setup for RGB_CUSTOM_TIMEOUT_DELAY
#if FADE_TIME < 0
#   error FADE_TIME must be greater than 0
#elif !defined (FADE_TIME)
#   define FADE_TIME 4
#endif

// Boot animation start position
#ifndef BOOT_ANIM_X
  #define BOOT_ANIM_X 30
#endif
#ifndef BOOT_ANIM_Y
  #define BOOT_ANIM_Y 0
#endif

HSV SPLASH_math2(HSV, int16_t, int16_t, uint8_t, uint16_t);
void rgb_matrix_boot_anim(uint8_t );

void rgb_matrix_fade_in(void);
void rgb_matrix_fade_out(void);
void rgb_matrix_fade_out_noeeprom(void);

void keyboard_post_init_keymap(void);
void suspend_wakeup_init_keymap(void);

bool process_record_keymap(uint16_t keycode, keyrecord_t *record);
void rgb_matrix_indicators_keymap(void);
void rgb_matrix_indicators_advanced_keymap(uint8_t led_min, uint8_t led_max);
