/* Copyright 2021 Jason Williams (Wilba)
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
// Default layout for RAMA WORKS KARA
#include QMK_KEYBOARD_H

enum userspace_custom_keycodes {
  CU_CH = SAFE_RANGE, // keycode for the č expansion
  CU_SH, // keycode for the š expansion
  CU_ZH // keycode for the ž expansion
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	// Default layer
	[0] = LAYOUT_60_hhkb(
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, TG(1),
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
		LT(2, KC_BSPC), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
		KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC, KC_CAPS,
				KC_LCTL, KC_LGUI,                            KC_SPC,                             KC_RALT, MO(2)),

	// Mod Lock Layer
	[1] = LAYOUT_60_hhkb(
		  TG(1), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, TG(1),
		_______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, XXXXXXX,   KC_UP, XXXXXXX, _______, _______,_______, _______,
		KC_BSPC, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, (LALT(KC_LEFT)), KC_LEFT, KC_DOWN, KC_RGHT, LALT(KC_RGHT), _______, _______,
		_______,    KC_Z,    KC_X,    KC_C,    KC_V, _______, _______, _______,  KC_GRV,S(KC_GRV),_______, _______, _______,
				LM(3, MOD_LCTL), LM(3, MOD_LGUI),                            _______,                   LM(3, MOD_LALT), _______ ),

	// Mod Layer
	[2] = LAYOUT_60_hhkb(
		KC_ESC , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPLY, KC_VOLD, KC_VOLU,
		_______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, XXXXXXX,   KC_UP, XXXXXXX, _______, CU_SH, CU_ZH, KC_DEL,
		KC_BSPC, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, (LALT(KC_LEFT)), KC_LEFT, KC_DOWN, KC_RGHT, LALT(KC_RGHT),CU_CH, _______,
		_______,    KC_Z,    KC_X,    KC_C,    KC_V, _______, _______, _______,  KC_GRV,S(KC_GRV),_______, _______, MO(4),
				_______, _______,                            KC_ENT,                            _______,_______ ),

	// Mod lock, lgui override to keep keyboard shortcuts, preveri detalje
    [3] = LAYOUT_60_hhkb(
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, XXXXXXX,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
		MT(MOD_LCTL, KC_BSPC), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, XXXXXXX,
				KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, XXXXXXX),


	// Brightness Layer
	[4] = LAYOUT_60_hhkb(
		_______, EF_DEC,  EF_INC,  H1_DEC,  H1_INC,  H2_DEC,  H2_INC,  _______, _______, _______, _______, BR_DEC,  BR_INC,  _______, _______,
		_______, _______, _______, S1_DEC,  S1_INC,  S2_DEC,  S2_INC,  _______, _______, _______, _______, ES_DEC,  ES_INC,  _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
				_______, _______,                            _______,                            _______, _______)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case CU_CH:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_SPC)SS_UP(X_LCTL)SS_DELAY(10)";"SS_DOWN(X_LCTL)SS_TAP(X_SPC)SS_UP(X_LCTL));
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case CU_SH:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_SPC)SS_UP(X_LCTL)SS_DELAY(10)"["SS_DOWN(X_LCTL)SS_TAP(X_SPC)SS_UP(X_LCTL));
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case CU_ZH:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_SPC)SS_UP(X_LCTL)SS_DELAY(10)"\\"SS_DOWN(X_LCTL)SS_TAP(X_SPC)SS_UP(X_LCTL));
        } else {
            // when keycode QMKBEST is released
        }
        break;
    }
return true;
};
