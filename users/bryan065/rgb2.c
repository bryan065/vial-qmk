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

#include "quantum.h"
#include "rgb2.h"

//===============Custom Functions=========================//
// Splash animation math
HSV SPLASH_math2(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;
    hsv.h += effect;
    hsv.v = qadd8(hsv.v, 255 - effect);
    return hsv;
} // End of splash math functions

// Boot animation, run only if RGB_MATRIX_ENABLED is defined and if the matrix is enabled.
void rgb_matrix_boot_anim(uint8_t boot_anim) {
    #if defined (RGB_MATRIX_ENABLE)
        if (rgb_matrix_config.enable) {
            rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, RGB_MATRIX_MAXIMUM_BRIGHTNESS);   // Set brightness to maximum allowed before playing animation
            rgb_matrix_set_speed_noeeprom(64);                                                                              // Set animation speed to default before playing animation
            boot_timer = timer_read();
            boot_status = boot_anim;
        }
    #endif
} // End of boot animation

//===============Custom Functions End=====================//

//==============Layer indicator code==============//
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 0:
            if (RGB_MOD_FLAG != false) { RGB_MOD_FLAG = false; }    // Set RGB_MOD_FLAG to false on the default layer
            break;                                                  // This is to disable per-key indicator on the default layer
    }
    return state;
}
//==============Layer indicator code end==========//

/* Boot_status
*   1 = Run splash boot animation
*   0 = Don't run anim / anim complete
*/
//==========Per Layer RGB Matrix indicators========//
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // RGB matrix runner function
    void rgb_matrix_boot_anim_runner(uint8_t originx, uint8_t originy) {
        for (uint8_t i = led_min; i < led_max; i++) {
                    HSV hsv = rgb_matrix_config.hsv;
                    hsv.v   = 0;
                    for (uint8_t j = 0; j < 1; j++) {
                        int16_t  dx   = g_led_config.point[i].x - originx;     // X origin of splash animation
                        int16_t  dy   = g_led_config.point[i].y - originy;     // y origin of splash animation
                        uint8_t  dist = sqrt16(dx * dx + dy * dy);
                        uint16_t tick = scale16by8(timer_elapsed(boot_timer), qadd8(rgb_matrix_config.speed, 1));
                        hsv           = SPLASH_math2(hsv, dx, dy, dist, tick);
                    }
                    hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
    }

    // Boot animation Code
    switch(boot_status) {
    case 1:
        if (timer_elapsed(boot_timer) >= STARTUP_ANIM_TIME) {                                                   // If timer is > boot animation time, load the saved RGB mode and fade in
            rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);  // Reset HSV.v to original value
            rgb_matrix_set_speed_noeeprom(rgb_matrix_config.speed);                                             // Reset speed to original value
            eeprom_read_block(&rgb_matrix_config, EECONFIG_RGB_MATRIX, sizeof(rgb_matrix_config));
            rgb_matrix_mode_noeeprom(rgb_matrix_config.mode);                                                   // Load original mode
            boot_status = 0;
        } else {                                                                                                // Otherwise, run boot animation
            rgb_matrix_boot_anim_runner(112,0);
        }
        return;
    }   // End boot animation code

    // Layer indicator code
    if (get_highest_layer(layer_state) > 0 && !RGB_MOD_FLAG) {
        uint8_t layer = get_highest_layer(layer_state);

        // Per configured key indicator
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                // Comparing LED index to matrix for configured keys or override keys
                if (index >= led_min && index <= led_max && index != NO_LED) {

                    uint16_t keycheck = keymap_key_to_keycode(layer, (keypos_t){col,row});
                    HSV hsv = rgb_matrix_config.hsv;
                    hsv.s   = 255;  // Ensure RGB colors are full saturation regardless of user's setting

                    // If key is configured, light it up
                    if (keycheck > KC_TRNS) {

                        // Per key overrides
                        switch (keycheck) {
                            case RESET:
                                hsv.h = 0;  // RED
                                break;
                            default:
                                // Per key override with toggle indicator
                                if (keycheck == NK_TOGG && keymap_config.nkro == 1) {
                                    hsv.h = 85; // GREEN if nkro is enabled
                                } else if (keycheck == GUI_TOG && keymap_config.no_gui == 1) {
                                    hsv.h = 0;  // RED if GUI is disabled
                                }
                                else {
                                    hsv.s = 0;  // Set per key lights to white and respect the user's hsv.v value
                                }
                                break;
                        }

                        // Make the key lights a bit brighter
                        if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }
                        else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }

                        RGB rgb = hsv_to_rgb(hsv);
                        RGB_MATRIX_INDICATOR_SET_COLOR(index, rgb.r, rgb.g, rgb.b);
                    }
                    else {  // If key is not configured, light it up differently or not at all.
                        RGB_MATRIX_INDICATOR_SET_COLOR(index, 0, 0, 0); //If this line is commented out, the original animation will show through. However, freezes my keyboard after 2s.
                    }
                } // End of comparison code
            }
        } // End of per configured key indicator

    } // End of layer indicator code
}
//==========Per Layer RGB Matrix indicators========//

//==========Keyboard init/suspend functions========//
// code will run on keyboard wakeup
void suspend_wakeup_init_user(void) {
    // Fade in RGB when first plugging in kb or on resume from sleep
    #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
        rgb_matrix_boot_anim(1);
    #endif
}
//=======Keyboard init/suspend functions End=======//
