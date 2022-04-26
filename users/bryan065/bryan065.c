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

#include QMK_KEYBOARD_H
#include "bryan065.h"
#include "rgb.h"
#include "version.h"

#ifdef __AVR__
#include <avr/wdt.h>
#endif

// Add reconfigurable functions here, for keymap customization
// This allows for a global, userspace functions, and continued
// customization of the keymap.  Use _keymap instead of _user
// functions in the keymaps

// Userspace and keymap suspend init wakeup
__attribute__((weak)) void suspend_wakeup_init_keymap(void) {}
__attribute__((weak)) void suspend_wakeup_init_rgb(void) { suspend_wakeup_init_keymap(); }
void                       suspend_wakeup_init_user(void) {
    suspend_wakeup_init_rgb();

    // Clear any stuck keys on init wakeup
    tap_code(KC_ESC);
}

// Userspace and keymap pre init
__attribute__((weak)) void keyboard_pre_init_keymap(void) {}
void                       keyboard_pre_init_user(void) {
    keyboard_pre_init_keymap();
}

// Userspace and keymap post init
__attribute__((weak)) void keyboard_post_init_keymap(void) {}
__attribute__((weak)) void keyboard_post_init_rgb(void) { keyboard_post_init_keymap(); }
void                       keyboard_post_init_user(void) {
    keyboard_post_init_rgb();

    #if defined(PREDEFINED_TAP_DANCE) && defined(VIAL_ENABLE)
    // Sequence of on tap, on hold, double tap, tap+hold
    vial_tap_dance_entry_t td = { KC_CAPSOFF, KC_NO, KC_CAPS, MO(TD_SYS_LAYER), TAPPING_TERM };
    dynamic_keymap_set_tap_dance(0, &td);
    #endif

    // Search for specific keys and replace
    for (uint8_t layer = 0; layer <= 3; ++layer) {
      for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
          uint16_t keycheck = keymap_key_to_keycode(layer, (keypos_t){col,row});

          switch(keycheck) {
              #if defined(PREDEFINED_TAP_DANCE) && defined(VIAL_ENABLE)
              case KC_CAPS:
                // Set Capslock key to predefined TD(0)
                dynamic_keymap_set_keycode(layer, row, col, TD(0));
                break;
              #endif

              #ifdef LOST_ARK
              case KC_Q:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_Q_SPAM);
                break;
              case KC_W:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_W_SPAM);
                break;
              case KC_E:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_E_SPAM);
                break;
              case KC_R:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_R_SPAM);
                break;
              case KC_A:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_A_SPAM);
                break;
              case KC_S:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_S_SPAM);
                break;
              case KC_D:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_D_SPAM);
                break;
              case KC_F:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_F_SPAM);
                break;
              case KC_G:
                dynamic_keymap_set_keycode(TD_SYS_LAYER, row, col, KC_G_SPAM);
                break;
              #endif
          }
        }
      }
    }
}

// Userspace and keymap RGB matrix indicator user
__attribute__((weak)) void rgb_matrix_indicators_keymap(void) {}
__attribute__((weak)) void rgb_matrix_indicators_rgb(void) { rgb_matrix_indicators_keymap(); }
void rgb_matrix_indicators_user (void) {
    rgb_matrix_indicators_rgb();
}

// Userspace and keymap RGB matrix indicator advanced user
__attribute__((weak)) void rgb_matrix_indicators_advanced_keymap(uint8_t led_min, uint8_t led_max) {}
__attribute__((weak)) void rgb_matrix_indicators_advanced_rgb(uint8_t led_min, uint8_t led_max) { rgb_matrix_indicators_advanced_keymap(led_min, led_max); }
void                       rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    rgb_matrix_indicators_advanced_rgb(led_min, led_max);

}


// Userspace and keymap custom keycodes
__attribute__ ((weak)) bool process_record_keymap(uint16_t keycode, keyrecord_t *record) { return true; }
__attribute__ ((weak)) bool process_record_rgb(uint16_t keycode, keyrecord_t *record) { return process_record_keymap(keycode, record); }
bool                        process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Custom keycode, on key release
    if (!record->event.pressed) {
      switch (keycode) {
      case KC_CAPSOFF:
          if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) {
              tap_code(KC_CAPS); // Disable caps if it's enabled
          }
        return false;
      case KC_MAKE:   // Compiles the firmware, and adds the flash command based on keyboard bootloader
        clear_mods();
        if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) tap_code(KC_CAPS); // Disable caps if it's enabled
        SEND_STRING("make " QMK_KEYBOARD ":" QMK_KEYMAP);
        tap_code(KC_ENT);
        return false;
      case VRSN:      // Prints QMK firmware version information
        send_string_with_delay_P(PSTR(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION ", Built on: " QMK_BUILDDATE), TAP_CODE_DELAY);
        return false;
      case EEP_RST_R:
        eeconfig_init();
        shutdown_user();

        #ifdef __AVR__
            wdt_enable(WDTO_250MS);
        #else
            NVIC_SystemReset();
        #endif
        return false;
      case RGB_DEFAULT:
        #ifdef RGB_MATRIX_STARTUP_MODE
          rgb_matrix_mode(RGB_MATRIX_STARTUP_MODE);
        #elif defined(ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT)
          rgb_matrix_mode(RGB_MATRIX_CYCLE_LEFT_RIGHT);
        #else
          rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
        #endif
        return false;

      #ifdef LOST_ARK
      case KC_Q_SPAM: // Spam keys for idle farming in lost ark (Skill "Q" and "G" farm)
          if (SPAM != 1) { SPAM = 1;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_W_SPAM:
          if (SPAM != 2) { SPAM = 2;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_E_SPAM:
          if (SPAM != 3) { SPAM = 3;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_R_SPAM:
          if (SPAM != 4) { SPAM = 4;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_A_SPAM:
          if (SPAM != 5) { SPAM = 5;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_S_SPAM:
          if (SPAM != 6) { SPAM = 6;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_D_SPAM:
          if (SPAM != 7) { SPAM = 7;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_F_SPAM:
          if (SPAM != 8) { SPAM = 8;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      case KC_G_SPAM:
          if (SPAM != 9) { SPAM = 9;}
          else { SPAM = 0; }
          spam_timer = timer_read();
          return false;
      // Easily cancel key spam by pressing the normal key again
      case KC_Q:
      case KC_W:
      case KC_E:
      case KC_R:
      case KC_A:
      case KC_S:
      case KC_D:
      case KC_F:
      case KC_G:
          SPAM = 0;
          return true;
      #endif
      }
    }
    // Custom keycode, on key press
    else {
      switch (keycode) {
      case KC_LOCK_W:   // Locking "W" key
        register_code(KC_W);
        return false;
      }
    }
    return process_record_rgb(keycode, record);
}

// Userspace and keymap matrix scan
__attribute__((weak)) void matrix_scan_keymap(void) {}
void                       matrix_scan_user(void) {
  #ifdef LOST_ARK
    if (timer_elapsed(spam_timer) >= SPAM_INTERVAL) {
        switch(SPAM){
            case 1:
                tap_code(KC_Q);
                break;
            case 2:
                tap_code(KC_W);
                break;
            case 3:
                tap_code(KC_E);
                break;
            case 4:
                tap_code(KC_R);
                break;
            case 5:
                tap_code(KC_A);
                break;
            case 6:
                tap_code(KC_S);
                break;
            case 7:
                tap_code(KC_D);
                break;
            case 8:
                tap_code(KC_F);
                break;
            case 9:
                tap_code(KC_G);
                break;
        }
        spam_timer = timer_read();
    }
  #endif
  matrix_scan_keymap();
}

// Encoder functions
#ifdef ENCODER_ENABLE
    bool encoder_update_user(uint8_t index, bool clockwise) {
        if (index == 0) {
            if (clockwise) {
                #ifdef CYBER77  // Cyber77 specific encoder setup: row 4, col 8 > unused matrix positions
                  tap_code(dynamic_keymap_get_keycode(biton32(layer_state),4,8));
                #else
                  tap_code(KC_VOLU);
                #endif
            } else {
                #ifdef CYBER77  // Cyber77 specific encoder setup
                  tap_code(dynamic_keymap_get_keycode(biton32(layer_state),4,6));
                #else
                  tap_code(KC_VOLD);
                #endif
            }
        }
        return false;
    }
#endif
