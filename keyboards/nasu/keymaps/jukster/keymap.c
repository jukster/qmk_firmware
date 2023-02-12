/* Copyright 2021 Yiancar-Designs
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
#include QMK_KEYBOARD_H

void add_to_prev(uint16_t kc);
void unreg_prev(void);

enum userspace_custom_keycodes {
  CU_CH = SAFE_RANGE, // keycode for the č expansion
  CU_QUOT, // keycode so that the ' key can be quote, shifted to double code
  CU_COMM, // keycode so that the comma can be shifted to '
  CU_DOT, // keycode so that the dot can be shifted to "
  CU_SCLN,
  CU_BSLS // keycode so that the dot can be shifted to "
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_alice_split_bs( /* Base */
    KC_GRV,  KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,             KC_7,    KC_8,    KC_9,    KC_0,    KC_SLSH, KC_EQL,  LALT(KC_BSLS),   KC_DEL,
    KC_PGUP, KC_TAB,  KC_Q,    KC_W,    KC_F,   KC_P,   KC_G,   KC_J,   KC_L,   KC_U,    KC_Y,    CU_SCLN, KC_LBRC, KC_BSLS, KC_BSPC,
    KC_PGDN, LT(1, KC_BSPC), KC_A,    KC_R,   KC_S,   KC_T,   KC_D,   KC_H,   KC_N,   KC_E,    KC_I,    KC_O,    KC_SCLN,          KC_ENT,
             KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      LALT(KC_LBRC),    KC_K,    KC_M,   CU_COMM, CU_DOT,  KC_MINS, KC_RSFT, MO(1),
             KC_LCTL,          KC_LGUI,          KC_SPC,  MO(1),                   KC_SPC,           KC_RALT,                            KC_RCTL),

[1] = LAYOUT_alice_split_bs( /* FN */
    _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,          KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_MPLY, KC_VOLD, KC_VOLU,
    _______, _______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______,                 _______, _______, KC_UP, _______, _______, _______, _______, _______,
    _______, KC_CAPS, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                 (LALT(KC_LEFT)), KC_LEFT, KC_DOWN, KC_RGHT, LALT(KC_RGHT),_______,          _______,
             _______, KC_Z,    KC_X,    KC_C,    KC_V, _______,                 LALT(KC_RBRC), _______, _______, _______, _______,  _______, _______, _______,
             _______,          _______,          _______, _______,                 _______,          _______,                            _______)
};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

// state of physical shift
bool lshift = false;
bool rshift = false;


// Number of items that are saved in prev_kcs
uint8_t prev_indx = 0;
// Used to save the last 6 actual keycodes activated by frankenkeycodes
uint16_t prev_kcs[6] = {0, 0, 0, 0, 0, 0};

/*
Used to add a keycode to a prev_kcs to remember it.
When full the last code gets discarded and replaced by
the new one.
*/
void add_to_prev(uint16_t kc){
  for (int i=0; i<prev_indx; i++){
    if (kc == prev_kcs[i])
      return;
  }
  if (prev_indx == 6){
    for (int i=5; i>0; i--){
      prev_kcs[i] = prev_kcs[i-1];
    }
    prev_kcs[0] = kc;
  } else {
    prev_kcs[prev_indx] = kc;
    prev_indx++;
  }
}

/*
Unregisters all codes saved in prev_kcs and resets prev_indx.
gets called on multiple occasions mainly when shift is released
and when frankenkeycodes are pressed. Prevents output of
wrong characters when really specific key combinations
that would never occur during normal usage are pressed.
*/
void unreg_prev(void){
  if (prev_indx == 0)
    return;
  for (int i=0; i<prev_indx; i++){
    unregister_code(prev_kcs[i]);
  }
  prev_indx = 0;
}

/*
Templates for Keys, with custom shifted and non shifted Characters
*/

// Normal shift status. Used when both the ansi keycode is non shifted, and the non shifted result is, but the shifted result is obtained by shifting a different ansi keycode
#define SHIFT_NORM(kc1, kc2) \
if (record->event.pressed) { \
  if (lshift || rshift) { \
    register_code(KC_LSFT); \
    unregister_code(kc2); \
    register_code(kc2); \
    add_to_prev(kc2); \
  } else { \
    unregister_code(KC_LSFT); \
    unregister_code(kc1); \
    register_code(kc1); \
  } \
} else { \
  unregister_code(kc1); \
  unregister_code(kc2); \
} \
return false;

// Inverted shift status. Used when you need to send a non-shifted keycode as a result of physically pressing shift
#define SHIFT_SWITCH(kc1, kc2) \
if (record->event.pressed) { \
  if (lshift || rshift) { \
    unregister_code(KC_LSFT); \
    unregister_code(kc2); \
    register_code(kc2); \
    add_to_prev(kc2); \
  } else { \
    register_code(KC_LSFT); \
    unregister_code(kc1); \
    register_code(kc1); \
    add_to_prev(kc1); \
  } \
} else { \
  unregister_code(kc1); \
  unregister_code(kc2); \
  unreg_prev(); \
  if (lshift || rshift) \
    register_code(KC_LSFT); \
  else \
    unregister_code(KC_LSFT); \
} \
return false;

// Always shifted, Used when both the keycodes are shifted, regardless of physical state
#define SHIFT_ALL(kc1, kc2) \
if (record->event.pressed) { \
  register_code(KC_LSFT); \
  if (lshift || rshift) { \
    unregister_code(kc2); \
    register_code(kc2); \
    add_to_prev(kc2); \
  } else { \
    unregister_code(kc1); \
    register_code(kc1); \
    add_to_prev(kc1); \
  } \
} else { \
  unregister_code(kc1); \
  unregister_code(kc2); \
  unreg_prev(); \
  if (lshift || rshift) \
    register_code(KC_LSFT); \
  else \
    unregister_code(KC_LSFT); \
} \
return false;


// Never shifted. Used when both the keycodes are unshifted, regardless of physical state
#define SHIFT_NO(kc1, kc2) \
if (record->event.pressed) { \
  unregister_code(KC_LSFT); \
  if (lshift || rshift) { \
    unregister_code(kc2); \
    register_code(kc2); \
    add_to_prev(kc2); \
  } else { \
    unregister_code(kc1); \
    register_code(kc1); \
  } \
} else { \
  unregister_code(kc1); \
  unregister_code(kc2); \
  unreg_prev(); \
  if (lshift || rshift) \
    register_code(KC_LSFT); \
  else \
    unregister_code(KC_LSFT); \
} \
return false;

// Always AltGr
#define SHIFT_ALGR(kc1, kc2) \
if (record->event.pressed) { \
  unregister_code(KC_LSFT); \
  register_code(KC_ALGR); \
  if (lshift || rshift) { \
    unregister_code(kc2); \
    register_code(kc2); \
    unregister_code(kc2); \
    register_code(KC_LSFT); \
  } else { \
    unregister_code(kc1); \
    register_code(kc1); \
    unregister_code(kc1); \
  } \
  unregister_code(KC_ALGR); \
} \
return false;

// Different keycode when Ctrl is pressed
#define CTRL(kc1, kc2) \
if(record->event.pressed) { \
  if (lshift || rshift) \
    register_code(KC_LSFT); \
  else \
    unregister_code(KC_LSFT); \
  if (keyboard_report->mods & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL))){ \
    register_code(kc2); \
  } else { \
    register_code(kc1); \
  } \
} else { \
    unregister_code(kc1); \
    unregister_code(kc2); \
} \
return false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_LSFT:
    if(record->event.pressed) {
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      lshift = true;
    } else {
      unreg_prev();
      if (!rshift)
        unregister_code(KC_LSFT);
      lshift = false;
    }
    return false;
  case KC_RSFT:
    if(record->event.pressed) {
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      rshift = true;
    } else {
      unreg_prev();
      if (!lshift)
        unregister_code(KC_LSFT);
      rshift = false;
    }
    return false;
        case KC_2:
    if(record->event.pressed){
      unregister_code(KC_LSFT);
      if (lshift || rshift){
		register_code(KC_LSFT);
		register_code(KC_LALT);
        unregister_code(KC_2);
        register_code(KC_2);
        unregister_code(KC_LALT);
	} else {
        register_code(KC_2);
      }
    } else {
      unregister_code(KC_2);
    }
    return false;
    case KC_6:
    if(record->event.pressed){
      unregister_code(KC_LSFT);
      if (lshift || rshift){
		register_code(KC_LALT);
        unregister_code(KC_QUOT);
        register_code(KC_QUOT);
        unregister_code(KC_LALT);
	} else {
        register_code(KC_6);
      }
    } else {
      unregister_code(KC_6);
    }
    return false;
    case KC_7:
    SHIFT_NORM(KC_7, KC_6)
  case KC_8:
    SHIFT_NORM(KC_8, KC_EQL)
  case KC_9:
    SHIFT_NORM(KC_9, KC_8)
  case KC_0:
    SHIFT_NORM(KC_0, KC_9)
  case KC_EQL:
    SHIFT_SWITCH(KC_0, KC_EQL)
  case CU_SCLN:
    SHIFT_ALL(KC_COMM, KC_DOT)
  case CU_COMM:
    SHIFT_NORM(KC_COMM, KC_7)
  case CU_DOT:
  	SHIFT_NORM(KC_DOT, KC_2)
  case CU_QUOT:
    SHIFT_ALL(KC_7, KC_2)

    }

return true;
};
