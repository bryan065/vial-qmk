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
#include "rgb.h"

//===============Custom Functions=========================//
void rgb_matrix_fade_in(void) {
    fade_timer = timer_read();
    rgb_original_hsv = rgb_matrix_config.hsv;  //grab current V value to restore
    rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, 0);
    fade_status = 1;
}

void rgb_matrix_fade_out(void) {
    fade_timer = timer_read();
    rgb_original_hsv = rgb_matrix_config.hsv;  //grab current V value to restore
    fade_status = -1;
}

void rgb_matrix_fade_out_noeeprom(void) {
    fade_timer = timer_read();
    rgb_original_hsv = rgb_matrix_config.hsv;  //grab current V value to restore
    fade_status = -2;
}

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
void rgb_matrix_indicators_advanced_rgb(uint8_t led_min, uint8_t led_max) {
    rgb_matrix_indicators_advanced_keymap(led_min, led_max);
    
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
            rgb_matrix_fade_in();                                                                               // Fade in after boot animation is complete
            boot_status = 0;
        } else {                                                                                                // Otherwise, run boot animation
            rgb_matrix_boot_anim_runner(30,0);
        }
        return;
    case 2:
        if (timer_elapsed(boot_timer) >= STARTUP_ANIM_TIME) {                                                   // If timer is > boot animation time, load the saved RGB mode and fade in
            rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);  // Reset HSV.v to original value
            rgb_matrix_set_speed_noeeprom(rgb_matrix_config.speed);                                             // Reset speed to original value

            timeout = true;
            rgb_matrix_disable_noeeprom();

            boot_status = 0;
        } else {                                                                                                // Otherwise, run boot animation
            rgb_matrix_boot_anim_runner(112,64);
        }
        return;
    }   // End boot animation code

    // Layer indicator code
    if (get_highest_layer(layer_state) > 0 && !RGB_MOD_FLAG) {
        uint8_t layer = get_highest_layer(layer_state);

        // Lightbar layer indicator
        HSV hsv = rgb_matrix_config.hsv;
        hsv.s   = 255;  // Ensure RGB colors are full saturation regardless of user's setting

        // Layer color settings
        //  LIGHTBAR_FLAG
        //
        //  TRUE    = custom indicator color
        //  FALSE   = no custom indicator color
        switch (layer) {
            case 1:
                LIGHTBAR_FLAG = false;
                if (LIGHTBAR_FLAG) { hsv.h = 0;} // Red but disabled so animation shows through
                break;
            case 2:
                LIGHTBAR_FLAG = true;
                if (LIGHTBAR_FLAG) { hsv.h = 50;} // Yellow
                break;
            case 3:
                LIGHTBAR_FLAG = true;
                if (LIGHTBAR_FLAG) { hsv.h = 0;} // Red
                break;
            default:
                LIGHTBAR_FLAG = false;
                break;
        }

        //apply the colors to the layers, if configured. Otherwise, the user's RGB mode will show through
        RGB rgb = hsv_to_rgb(hsv);

        if (LIGHTBAR_FLAG) {
            for (uint8_t i = led_min; i <= led_max; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], 0x02)) { // 0x02 == LED_FLAG_UNDERGLOW
                    RGB_MATRIX_INDICATOR_SET_COLOR(i, rgb.r, rgb.g, rgb.b);
                }
            }
        }
        // End of lightbar layer indicator

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
                            case LCTL(KC_BSPC):
                                hsv.h = 0;  // RED
                                break;
                            case EEP_RST_R:
                                hsv.h = 50; // YELLOW
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
                        }

                        // Make the key lights a bit brighter
                        if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }
                        else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }

                        rgb = hsv_to_rgb(hsv);
                        RGB_MATRIX_INDICATOR_SET_COLOR(index, rgb.r, rgb.g, rgb.b);
                    }
                    else {  // If key is not configured, light it up differently or not at all.
                        RGB_MATRIX_INDICATOR_SET_COLOR(index, 0, 0, 0); //If this line is commented out, the original animation will show through. However, freezes my keyboard after 2s.
                    }
                } // End of comparison code
            }
        } // End of per configured key indicator

    } // End of layer indicator code

    #ifdef CYBER77
        // Caps lock / num lock code
        if (host_keyboard_led_state().caps_lock) {
            HSV hsv = rgb_matrix_config.hsv;
            hsv.s = 0;                                                                  // Use white LED for indicator

            if (hsv.v < 100) { hsv.v = 100; }                                           // Ensure the caps/num lock has a minimum brightness and doesn't exceed the defined max brightness
            else if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }     //   Also makes the caps/num lock a bit brighter than the rest of the LED's
            else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }


            RGB rgb = hsv_to_rgb(hsv);
            RGB_MATRIX_INDICATOR_SET_COLOR(3, rgb.r, rgb.g, rgb.b);                     // Assuming caps lock is at led #3
        }
        if (host_keyboard_led_state().num_lock) {
            HSV hsv = rgb_matrix_config.hsv;
            hsv.s = 0;

            if (hsv.v < 100) { hsv.v = 100; }
            else if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }
            else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }

            RGB rgb = hsv_to_rgb(hsv);
            RGB_MATRIX_INDICATOR_SET_COLOR(5, rgb.r, rgb.g, rgb.b);                     // Assuming num lock is at led #5
        } // End of caps/num lock code
    #endif
}

/* fade_status
*    1 = fade in
*   -1 = fade out
*   -2 = fade out, no eeprom
*/
//=============================================//
void rgb_matrix_indicators_rgb(void) {
    rgb_matrix_indicators_keymap();

    // Fade animation code
    HSV hsv = rgb_matrix_get_hsv();
    switch(fade_status) {
        case 1:     // Fade in code, slows down fade in to every FADE_TIME ms
            if ((timer_elapsed(fade_timer) > FADE_TIME) && (hsv.v <= rgb_original_hsv.v)) {
                rgb_matrix_increase_val_noeeprom();
                fade_timer = timer_read();
            } else if (hsv.v >= rgb_original_hsv.v) {   // End code, when brightness has reached max configured
                fade_status = 0;
                rgb_matrix_sethsv( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);
            }
            return;
        case -1:    // Fade out code, slows down fade in to every FADE_TIME ms
            if ((timer_elapsed(fade_timer) > FADE_TIME) && (hsv.v > 0)) {
                rgb_matrix_decrease_val_noeeprom();
                fade_timer = timer_read();
            } else if (hsv.v == 0) {    // End code, when brightness has reached 0
                fade_status = 0;
                rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);
                rgb_matrix_disable();
            }
            return;
        case -2:    // Fade out code, no EEPROM. Slows down fade in to every FADE_TIME ms
            if ((timer_elapsed(fade_timer) > FADE_TIME) && (hsv.v > 0)) {
                rgb_matrix_decrease_val_noeeprom();
                fade_timer = timer_read();
            } else if (hsv.v == 0) {    // End code, when brightness has reached max configured
                fade_status = 0;
                rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);
                rgb_matrix_disable_noeeprom();
            }
            return;
    } // End of fade animation code

    // Custom RGB timeout code that includes fades - FADE OUT
    #if RGB_DISABLE_TIMEOUT == 0
        if (rgb_matrix_config.enable) {
            #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
                if ((timer_elapsed32(rgb_anykey_timeout) > UINT32_MAX)) {
                    rgb_anykey_timeout = UINT32_MAX;
                } else if ((timer_elapsed32(rgb_anykey_timeout) > (uint32_t)RGB_CUSTOM_TIMEOUT_DELAY) && (rgb_matrix_config.enable)) {
                    rgb_matrix_fade_out_noeeprom();
                    timeout = true;
                }
            #endif
        }
    #endif // End of custom RGB timeout code - FADE OUT
}

//==========Per Layer RGB Matrix indicators========//

//=================Keycode Functions ================//
bool process_record_rgb(uint16_t keycode, keyrecord_t *record) {
    // Custom timeout code to add fades - FADE IN
    #if RGB_DISABLE_TIMEOUT == 0
        #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
            if (record->event.pressed) {
                rgb_anykey_timeout = timer_read32();    // Reset timeout timer on any key press

                if (timeout == true) {                  // Reset timer and turn on backlight to last mode if timed-out
                    timeout = false;
                    rgb_matrix_enable_noeeprom();
                    rgb_matrix_fade_in();
                }
            }
        #endif
    #endif // End of custom timeout code - FADE IN
    
    
    // Custom keycode, on key release
    if (!record->event.pressed) {
        switch(keycode) {
        case RGB_MODE_FORWARD ... RGB_SPD:  // Add RGB_MOD_FLAG = true to all RGB modification keys.
            RGB_MOD_FLAG = true;            //   This is to let the per key indicator know to stop if the RGB settings are modified so
            return true;                   //   the user can see the changes again without the layer indicator in the way
        case RGB_TOG:   // Override original RGB_TOG function and add fades
            #if RGB_DISABLE_TIMEOUT == 0
            #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
                if (rgb_matrix_is_enabled()) {
                    rgb_matrix_fade_out();
                } else if (!rgb_matrix_is_enabled()) {
                    rgb_matrix_enable();
                    rgb_matrix_fade_in();
                }
            #endif
            #endif
            return false;
        case MON_OFF:
            SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(500) "mon" SS_DELAY(500) SS_TAP(X_ENT));

            // Run a shutdown animation before kb goes to sleep
            #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
                rgb_matrix_boot_anim(2);
            #endif
            return false;
        }
    }
    return process_record_keymap(keycode, record);
}
//==============Keycode Functions End=============//

//========Keyboard & RGB matrix strip wiring / index====//
/*
*   ___________________________________
*  /   0          1  2  3  4  5  6     \_____________          <-- Light bar
* ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┬───┐
* │ 7 │ 8 │ 9 │ 10│ 11│ 12│ 13│ 14│ 15│ 16│ 17│ 18│ 19│ 20    │21 │
* ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
* │ 36  │35 │34 │33 │32 │31 │30 │29 │28 │27 │26 │25 │24 │23   │22 │
* ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
* │ 37   │38 │39 │40 │41 │42 │43 │44 │45 │46 │47 │48 │  49    │50 │
* ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
* │ 64     │63 │62 │61 │60 │59 │58 │57 │56 │55 │54 │53    │51 │51 │
* ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┼───┤
* │ 65 │ 66 │ 67 │  68      │ 69 │  70    │71 │72 │   │73 │74 │75 │
* └────┴────┴────┴──────────┴────┴────────┴───┴───┘   └───┴───┴───┘
*/

#ifdef CYBER77
    //LED TO MATRIX MAP
    led_config_t g_led_config = { {
            // Key Matrix to LED Index
            {      7,      8,      9,     10,     11,     12,     13,     14,     15, 16, 17, 18,     19,     20,     21 },
            {     36,     35,     34,     33,     32,     31,     30,     29,     28, 27, 26, 25,     24,     23,     22 },
            {     37,     38,     39,     40,     41,     42,     43,     44,     45, 46, 47, 48, NO_LED,     49,     50 },
            {     64,     63,     62,     61,     60,     59,     58,     57,     56, 55, 54, 53, NO_LED,     52,     51 },
            {     65,     66,     67,     68, NO_LED,     69, NO_LED,     70, NO_LED, 71, 72, 73, NO_LED,     74,     75 }
        }, {
            // LED Index to Physical Position **BASED ON KEY MATRIX TO LED INDEX (back and fourth snake in this case)
            {10,0},{50,0},{62,0},{74,0},{86,0},{98,0},{110,0},
            {0,12},{15,12},{30,12},{45,12},{60,12},{75,12},{90,12},{105,12},{120,12},{135,12},{150,12},{165,12},{180,12},{203,12},{224,12},
            {224,25},{203,25},{180,25},{165,25},{150,25},{135,25},{120,25},{105,25},{90,25},{75,25},{60,25},{45,25},{30,25},{15,25},{0,25},
            {0,38},{25,38},{40,38},{55,38},{70,38},{85,38},{100,38},{115,38},{130,38},{145,38},{160,38},{175,38},{201,38},{224,38},
            {224,51},{210,51},{189,51},{169,51},{154,51},{139,51},{120,51},{105,51},{90,51},{75,51},{60,51},{45,51},{25,51},{5,51},
            {2,64},{21,64},{39,64},{70,64},{90,64},{120,64},{150,64},{165,64},{190,64},{205,64},{224,64}
        }, {
            // LED Index to Flag
            2,2,2,2,2,2,2, // Lightbar
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    1, 1,
            1,    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 4, 1,
            1, 1, 1,    1,    1,    1,    1, 1, 4, 4, 4,
        }
    };
#endif
//====Keyboard & RGB matrix strip wiring / index end====//

//==========RGB init/suspend functions========//
void keyboard_post_init_rgb(void) {
    keyboard_post_init_keymap();
    
    // Start timer for custom rgb timeout
    #if (RGB_DISABLE_TIMEOUT == 0)
        #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
            rgb_anykey_timeout = timer_read32();
        #endif
    #endif
    
    // Fade in RGB when first plugging in kb or on resume from sleep
    #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
        rgb_matrix_boot_anim(1);
    #endif
}

void suspend_wakeup_init_rgb(void) {
    suspend_wakeup_init_keymap();
    
    // Fade in RGB when first plugging in kb or on resume from sleep
    #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
        rgb_matrix_boot_anim(1);
    #endif
}
//=======RGB init/suspend functions End=======//
