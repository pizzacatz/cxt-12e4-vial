/* Copyright 2026 pizzacatz
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_F11,  KC_NO,   KC_MSTP,
        KC_NO,   KC_NO,   KC_MRWD, KC_MFFD,
        KC_NO,   KC_MPLY, KC_MPLY, KC_MNXT,
        KC_MUTE, KC_NO,   KC_NO,   RM_TOGG
    ),
    [1] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
    [2] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
    [3] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
    [4] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
    [5] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______
    )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(RM_HUED, RM_HUEU),
        ENCODER_CCW_CW(RM_VALD, RM_VALU),
        ENCODER_CCW_CW(RM_PREV, RM_NEXT)
    },
    [1] = {
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [2] = {
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [3] = {
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [4] = {
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [5] = {
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    }
};
#endif

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    HSV color;

    switch (get_highest_layer(default_layer_state | layer_state)) {
        case 1:
            color = (HSV){HSV_RED};
            break;
        case 2:
            color = (HSV){HSV_ORANGE};
            break;
        case 3:
            color = (HSV){HSV_GREEN};
            break;
        case 4:
            color = (HSV){HSV_BLUE};
            break;
        case 5:
            color = (HSV){HSV_PURPLE};
            break;
        default:
            color = (HSV){HSV_OFF};
            break;
    }

    if (color.v != 0) {
        color.v = rgb_matrix_get_val();
    }

    RGB rgb = hsv_to_rgb(color);
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return false;
}
#endif
