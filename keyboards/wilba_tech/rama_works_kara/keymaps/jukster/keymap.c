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


// Interrupt and times for Mod/Spotlight
bool modspotlight = false;
uint16_t modspotlight_timer = 0;

bool modlayer = false;
uint16_t modlayer_timer = 0;

void add_to_prev(uint16_t kc);
void unreg_prev(void);
void timer_timeout(void);

enum userspace_custom_keycodes {
  CU_SCLN=SAFE_RANGE, // placeholder keycode to allow for KC_SCLN be used for accented C
  CU_LGUI, // keycode so that the LGUI key can also invoke spotlight
  CU_QUOT, // keycode so that the ' key can be quote, shifted to double code
  CU_COMM, // keycode so that the comma can be shifted to '
  CU_DOT, // keycode so that the dot can be shifted to "
  CU_LBRC, // keycode so that the dot can be shifted to "
  CU_BSLS, // keycode so that the dot can be shifted to "
  CU_SCLN2, // keycode so that the dot can be shifted to "
};



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	// Default layer
	[0] = LAYOUT_60_hhkb(
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_SLSH, KC_EQL,  LALT(KC_BSLS), KC_DEL,
		KC_TAB,  KC_Q,    KC_W,    KC_F,   KC_P,   KC_G,   KC_J,   KC_L,   KC_U,    KC_Y,    CU_SCLN, LALT(KC_LBRC), LALT(KC_RBRC), KC_BSPC,
		MT(MOD_LCTL, KC_BSPC), KC_A,    KC_R,   KC_S,   KC_T,   KC_D,   KC_H,   KC_N,   KC_E,    KC_I,    KC_O,    CU_QUOT,  KC_ENT,
		KC_LSFT, KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_K,   KC_M,   CU_COMM, CU_DOT,  KC_MINS, KC_RSFT,           KC_RGUI,
				KC_LALT, CU_LGUI,                            KC_SPC,                             MO(3), KC_RALT),

    // Slovene characters
	[1] = LAYOUT_60_hhkb(
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______, CU_LBRC,CU_BSLS, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, CU_SCLN2, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______,_______,_______, _______, _______,
				_______, _______,                            _______,                    _______, _______),

	// Mod Lock Layer - not done. Lokacija kurzorja - spremeniti
	[2] = LAYOUT_60_hhkb(
		  TG(2), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPLY, KC_VOLD, KC_VOLU,
		_______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, _______, _______, _______,   KC_UP, _______, _______, _______,
		KC_CAPS, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,
		_______,    KC_Z,    KC_X,    KC_C,    KC_V, _______, _______, _______,  KC_GRV,S(KC_GRV),_______, _______, _______,
				LM(4, MOD_LALT), LM(4, MOD_LGUI),                            _______,                    _______ , LM(4, MOD_RALT)),
	// Mod Layer
	[3] = LAYOUT_60_hhkb(
		  TG(2), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPLY, KC_VOLD, KC_VOLU,
		_______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, _______, _______, _______,   KC_UP, _______, _______, _______,
		KC_CAPS, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,
		_______,    KC_Z,    KC_X,    KC_C,    KC_V, _______, _______, _______,  KC_GRV,S(KC_GRV),_______, _______, _______,
				_______, _______,                            KC_ENT,                            _______, MO(5)),

    // Mod lock, lgui override to keep keyboard shortcuts, preveri detalje
	[4] = LAYOUT_60_hhkb(
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_SLSH, KC_EQL,  LALT(KC_BSLS), KC_DEL,
		KC_TAB,  KC_Q,    KC_W,    KC_F,   KC_P,   KC_G,   KC_J,   KC_L,   KC_U,    KC_Y,    CU_SCLN, LALT(KC_LBRC), LALT(KC_RBRC), KC_BSPC,
		MT(MOD_LCTL, KC_BSPC), KC_A,    KC_R,   KC_S,   KC_T,   KC_D,   KC_H,   KC_N,   KC_E,    KC_I,    KC_O,    CU_QUOT,  KC_ENT,
		KC_LSFT, KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_K,   KC_M,   CU_COMM, CU_DOT,  KC_MINS, KC_RSFT,           KC_RGUI,
				KC_LALT, CU_LGUI,                            KC_SPC,                             _______, KC_RALT),


	// Mod + Alt
	[5] = LAYOUT_60_hhkb(
		_______, EF_DEC,  EF_INC,  H1_DEC,  H1_INC,  H2_DEC,  H2_INC,  _______, _______, _______, _______, BR_DEC,  BR_INC,  _______, _______,
		_______, _______, _______, S1_DEC,  S1_INC,  S2_DEC,  S2_INC,  _______, _______, _______, _______, ES_DEC,  ES_INC,  _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_QUOT, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
				_______, _______,                            _______,                            _______, _______)


};


void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

// state of physical shift
bool lshift = false;
bool rshift = false;

// Interrupt and times for space cadet shift
bool lshiftp = false;
bool rshiftp = false;
uint16_t lshift_timer = 0;
uint16_t rshift_timer = 0;

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

// Interrupts all timers
void timer_timeout(void){
  lshiftp = false;
  rshiftp = false;
  modspotlight = false;
  modlayer = false;
}

/*
Templates for Keys, with custom shifted and non shifted Characters
*/

// Normal shift status. Used when both the ansi keycode is non shifted, and the non shifted result is, but the shifted result is obtained by shifting a different ansi keycode
#define SHIFT_NORM(kc1, kc2) \
if (record->event.pressed) { \
  timer_timeout(); \
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
  timer_timeout(); \
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
  timer_timeout(); \
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
  timer_timeout(); \
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
  timer_timeout(); \
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
  timer_timeout(); \
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

// process record user

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case KC_LGUI:
  case KC_RGUI:
    if (record->event.pressed)
      timer_timeout();
    return true;

  case KC_LSFT:
    if(record->event.pressed) {
      lshiftp = true;
      lshift_timer = timer_read();
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      lshift = true;
    } else {
      if (timer_elapsed(lshift_timer) < TAPPING_TERM && lshiftp) {
        register_code(KC_LSFT);
        register_code(KC_8);
        unregister_code(KC_8);
        unregister_code(KC_LSFT);
      }
      unreg_prev();
      if (!rshift)
        unregister_code(KC_LSFT);
      lshift = false;
    }
    return false;
  case KC_RSFT:
    if(record->event.pressed) {
      rshiftp = true;
      rshift_timer = timer_read();
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      rshift = true;
    } else {
      if (timer_elapsed(rshift_timer) < TAPPING_TERM && rshiftp) {
        register_code(KC_LSFT);
        register_code(KC_9);
        unregister_code(KC_9);
        unregister_code(KC_LSFT);
      }
      unreg_prev();
      if (!lshift)
        unregister_code(KC_LSFT);
      rshift = false;
    }
    return false;
  case KC_2:
    if(record->event.pressed){
      timer_timeout();
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
      timer_timeout();
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
  case CU_LGUI:
    if(record->event.pressed) {
      modspotlight = true;
      modspotlight_timer = timer_read();
      layer_on(1);
	  register_code(KC_LGUI);
	  } else {
      if (timer_elapsed(modspotlight_timer) < TAPPING_TERM && modspotlight) {
          unregister_code(KC_SPC);
          register_code(KC_SPC);
          unregister_code(KC_LGUI);
          layer_off(1);
  	  } else {
	  unregister_code(KC_LGUI);
      layer_off(1);
  	  }
    }
    return false;

  case CU_SCLN2:
    if(record->event.pressed) {
      unregister_code(KC_LGUI);
      layer_off(1);
	  register_code(KC_SCLN);
	  } else {
	  register_code(KC_LGUI);
      unregister_code(KC_SCLN);
      layer_on(1);
  	  }
    return false;

  case CU_BSLS:
    if(record->event.pressed) {
      unregister_code(KC_LGUI);
      layer_off(1);
	  register_code(KC_BSLS);
	  } else {
	  register_code(KC_LGUI);
      unregister_code(KC_BSLS);
      layer_on(1);
  	  }
    return false;

  case CU_LBRC:
    if(record->event.pressed) {
      unregister_code(KC_LGUI);
      layer_off(1);
	  register_code(KC_LBRC);
	  } else {
	  register_code(KC_LGUI);
      unregister_code(KC_LBRC);
      layer_on(1);
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
  return true;

  default:
    if(record->event.pressed) {
      timer_timeout();

      if (lshift || rshift)
        register_code(KC_LSFT);
      else
        unregister_code(KC_LSFT);
    }
	return true;
  }
}
