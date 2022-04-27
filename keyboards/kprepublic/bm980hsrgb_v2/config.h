 /* Copyright 2022 bdtc123
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0x4B50
#define PRODUCT_ID      0x1151
#define DEVICE_VER      0x0002
#define MANUFACTURER    KP Republic
#define PRODUCT         bm980v2

/* key matrix size */
#define MATRIX_ROWS 7
#define MATRIX_COLS 14
#define MATRIX_ROW_PINS { B5, F1, F4, F5, F6, F7, B6 }
#define MATRIX_COL_PINS { F0, E6, B0, B1, B2, B3, D6, D7, B4, D4, D5, D3, D2, B7 }
#define UNUSED_PINS

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION ROW2COL
#define LED_CAPS_LOCK_PIN E2
#define LED_NUM_LOCK_PIN C6
#define LED_SCROLL_LOCK_PIN C7
#define LED_PIN_ON_STATE 0

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
//#define DEBOUNCE 5
/* disable these deprecated features by default */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

//#define RGB_DISABLE_AFTER_TIMEOUT 0 // number of ticks to wait until disabling effects
#define RGB_DISABLE_WHEN_USB_SUSPENDED true // turn off effects when suspended

#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT

#define DRIVER_ADDR_1 0b0110000
#define DRIVER_COUNT 1
#define DRIVER_1_LED_TOTAL 98
#define DRIVER_LED_TOTAL DRIVER_1_LED_TOTAL

//#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 220
