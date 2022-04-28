/*
  Copyright (c) 2022 Bryan065

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef __AVR__
#include <avr/wdt.h>
#endif

#include "quantum.h"
#include "version.h"

#if VENDOR_ID == 0xA103
  #if PRODUCT == Cyber77
    #define CYBER77
  #endif
#endif

#ifndef TD_SYS_LAYER
  #define TD_SYS_LAYER 3
#elif TD_SYS_LAYER < 1
  #error "TD_SYS_LAYER must be on layer 1 or higher"
#endif

uint16_t     spam_timer;

#ifndef SPAM_INTERVAL
  #define SPAM_INTERVAL 1000
#endif

// Spam keys for Lost Ark
uint8_t SPAM;

// 0 - Off
// 1 - Q
// 2 - W
// 3 - E
// 4 - R
// 5 - A
// 6 - S
// 7 - D
// 8 - F
// 9 - G

enum userspace_custom_keycodes {
    RGB_DEFAULT = SAFE_RANGE,   // Reset to default RGB mode
    VRSN,                       // Print QMK Version
    KC_MAKE,                    // Build QMK for keyboard
    KC_LOCK_W,                  // Locking "W" key for autowalk, to be used with tap dance functions. Otherwise, you can just use KC_LOCK feature
    MON_OFF,                    // Custom keycode to turn off monitors and timeout the backlight. To be used with nircmd on the host computer.
    EEP_RST_R,                  // Wipe eeprom and reset keyboard
    KC_CAPSOFF,                 // Caps off keycode
    KC_Q_SPAM,                  // Key spams for Lost Ark
    KC_W_SPAM,
    KC_E_SPAM,
    KC_R_SPAM,
    KC_A_SPAM,
    KC_S_SPAM,
    KC_D_SPAM,
    KC_F_SPAM,
    KC_G_SPAM,
    NEW_SAFE_RANGE
};

void keyboard_pre_init_keymap(void);
void keyboard_post_init_keymap(void);
void keyboard_post_init_rgb(void);
void suspend_wakeup_init_rgb(void);
void matrix_scan_keymap(void);

bool process_record_keymap(uint16_t keycode, keyrecord_t *record);
bool process_record_rgb(uint16_t keycode, keyrecord_t *record);

void rgb_matrix_indicator_rgb(void);
void rgb_matrix_indicator_advanced_rgb(uint8_t led_min, uint8_t led_max);