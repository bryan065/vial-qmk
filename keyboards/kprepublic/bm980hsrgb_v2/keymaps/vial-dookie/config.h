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

// User settings
#define USB_POLLING_INTERVAL_MS 2
#define QMK_KEYS_PER_SCAN 8
#define DEBOUNCE 2

#undef PRODUCT
#define PRODUCT DookieBoard980v2
#define FORCE_NKRO
#define LAYER_STATE_8BIT

// Custom defines from /user/bryan065
#define RGB_CUSTOM_TIMEOUT_DELAY 900000     // Timout delay in ms. Set to 15 minutes
#define LOST_ARK
#define BOOT_ANIM_X 112
#define BOOT_ANIM_Y 0

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

// Vial config
#define DYNAMIC_KEYMAP_LAYER_COUNT 3
#define VIAL_TAP_DANCE_ENTRIES 1
#define PREDEFINED_TAP_DANCE
#define TD_SYS_LAYER 2

#define VIAL_KEYBOARD_UID {0xDC, 0x11, 0xA8, 0xCD, 0x29, 0xD7, 0x70, 0xBB}

/* Vial unlock keycombo: ESC + Return */
#define VIAL_UNLOCK_COMBO_ROWS { 0, 3 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 12}

// Vial firmware size tweaks
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define NO_ACTION_ONESHOT

//RGB Matrix Effects
#define RGB_MATRIX_STARTUP_SPD 64

#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL

#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH

#define RGB_MATRIX_KEYPRESSES


