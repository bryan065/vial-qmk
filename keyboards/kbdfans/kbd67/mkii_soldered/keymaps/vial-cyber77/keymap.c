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
#include "cyber77.h"

/* Structure for RGB_MATRIX layer indicator
 *   Note: Layer 0 aka base layer will never show any RGB layer indication
 *
 *   {FLAG, HUE, SATURATION, BRIGHTNESS}
 *
 *   FLAG:
 *     TRUE  = Custom indicator color
 *     FALSE = Transparent, will show user RGB mode/animation
 *     Note: FLAG will have to be set to TRUE for any of the effects below to work
 *
 *   HUE:
 *     Color of the layer indicator
 *
 *   SATURATION:
 *     Saturation of the layer indicator
 *
 *   BRIGHTNESS:
 *     0        = Don't show layer indicator (RGB's off)
 *     0 to 255 = No effect
 *
 */
struct RGB_MATRIX_INDICATOR indicator_matrix[8] = {
    {NULL, 0, 0, 0 },                                   // Layer 0, base layer, no effect

    {false, 0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 1
    { true, 0, 255, 0                            },     // Layer 2
    { true, 0, 255, 0                            },     // Layer 3
    {false, 0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 4
    {false, 0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 5
    {false, 0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 6
    {false, 0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS}      // Layer 7
};

/* Structure for RGB underglow layer indicator
 *   Note: Layer 0 aka base layer will never show any RGB layer indication
 *
 *   {FLAG, HUE, SATURATION, BRIGHTNESS}
 *
 *   FLAG:
 *     TRUE  = Custom indicator color
 *     FALSE = Transparent, will show user RGB mode/animation
 *     Note: FLAG will have to be set to TRUE for any of the effects below to work
 *
 *   HUE:
 *     Color of the layer indicator
 *
 *   SATURATION:
 *     Saturation of the layer indicator
 *
 *   BRIGHTNESS:
 *     0        = Don't show layer indicator (RGB's off)
 *     0 to 255 = No effect
 *
 */
struct RGB_MATRIX_INDICATOR indicator_underglow[8] = {
    {NULL, 0, 0, 0 },                                   // Layer 0, base layer, no effect

    {false,  0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 2
    { true, 45, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 3
    { true,  0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 4
    {false,  0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 5
    {false,  0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 6
    {false,  0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS},     // Layer 7
    {false,  0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS}      // Layer 8
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_all_ec( /* Base */
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  KC_MUTE, KC_VOLD, KC_VOLU,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          KC_BSLS, KC_DEL,
		KC_CAPS,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,        KC_CALC,
		KC_LSFT, KC_BSLS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   LALT(KC_PSCR),
		KC_LCTL, KC_LGUI, KC_LALT,          KC_SPC,       MO(1),           KC_SPC,          MO(2), MO(1),          KC_LEFT, KC_DOWN, KC_RGHT
  	),

	[1] = LAYOUT_all_ec( /* FN */
		KC_TILD,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, LCTL(KC_BSPC), _______, KC_SCRL, _______, _______,
		 KC_GRV, _______, _______, _______, _______, _______, _______,   KC_P7,   KC_P8,   KC_P9, _______, _______, _______,    LGUI(KC_PAUS), _______,
		_______, _______, _______, _______, _______, _______, _______,   KC_P4,   KC_P5,   KC_P6, _______, _______,          KC_PENT,          KC_MYCM,
		_______, _______, _______, _______, _______, _______, _______, _______,   KC_P1,   KC_P2,   KC_P3, _______, KC_NLCK,          KC_PGUP, KC_PSCR,
		_______, _______, _______,          _______,          _______,            KC_P0,          KC_PDOT, _______,          KC_HOME, KC_PGDN, KC_END
  	),

	[2] = LAYOUT_all_ec(
		_______, _______, _______,   _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, _______, _______, RGB_TOG, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_MOD,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          RGB_SPI,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_VAI, RGB_HUI,
		_______, _______, _______,          _______,          _______,          _______,          _______, _______,          RGB_SAD, RGB_VAD, RGB_SAI
  	),

	[3] = LAYOUT_all_ec(
		_______, _______, _______,   _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, _______, _______, KC_SLEP, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             VRSN, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MAKE,          _______,
		_______, _______, _______, _______, _______, EEP_RST_R, RESET, NK_TOGG, _______, _______, _______, _______, _______,          _______, _______,
		_______, GUI_TOG, _______,          _______,          MON_OFF,          _______,          _______, _______,          _______, _______, _______
  	)
};
