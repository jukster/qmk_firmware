/* Copyright 2018 'mechmerlin'
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
///

// Interrupt and times for Mod/Spotlight
bool modspotlight = false;
uint16_t modspotlight_timer = 0;

bool modlayer = false;
uint16_t modlayer_timer = 0;

void add_to_prev(uint16_t kc);
void unreg_prev(void);
void timer_timeout(void);

enum userspace_custom_keycodes {
  CU_QUOT=SAFE_RANGE, // placeholder keycode to allow for KC_QUOT be used for soft accented C
  CU_SCLN, // placeholder keycode to allow for KC_SCLN be used for accented C
  CU_LGUI, // keycode so that the LGUI key can also invoke spotlight
  CU_MOD // keycode to mod-tap to different layers
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap (Base Layer) Default Layer
   * ,----------------------------------------------------------------.
   * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =| \| Vo+| Vo-|
   * |----------------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P| [ | ] |Bspc |PgUp|
   * |----------------------------------------------------------------|
   * |Bspc(CTRL)|A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return|PgDn|
   * |----------------------------------------------------------------|
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift |Cmd| Del|
   * |----------------------------------------------------------------|
   * |Ctrl|Mod |CMD |         Space          |Mod|Alt|Lef|Dow| Up| Rig|
   * `----------------------------------------------------------------'
   */
[0] = LAYOUT_65_ansi_split_bs(
  KC_ESC,  KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_SLSH, KC_EQL,  LALT(KC_BSLS), KC_VOLD, KC_VOLU, \
  KC_TAB,  KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P, LALT(KC_LBRC), LALT(KC_RBRC), KC_BSPC, KC_PGUP,    \
  MT(MOD_LCTL, KC_BSPC), KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    CU_SCLN,    CU_QUOT,        KC_ENT,  KC_PGDN,    \
  KC_LSFT, KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_MINS, KC_RSFT,           KC_RGUI,   KC_DEL,     \
  KC_LCTL, CU_MOD, CU_LGUI,                KC_SPC,           CU_MOD, KC_RALT,        KC_LEFT,       KC_DOWN, KC_UP, KC_RGHT),

  /* Keymap Mod lock Layer
   * ,----------------------------------------------------------------.
   * |    |   |   |   |   |   |   |   |   |   |   |   |    |      |   |
   * |----------------------------------------------------------------|
   * |     |   |MK1|MKU|MK2|   |   |PUp| Up |   |    |   |    |   |   |
   * |----------------------------------------------------------------|
   * |      |   |MKL|MKD|MKR|   |a(l)|lft|down|rght|a(r)| č |   |     |    
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |PDn|   |   |   |      |   |    |
   * |----------------------------------------------------------------|
   * |    |    |    |                       |   |   |    |   |   |    |
   * `----------------------------------------------------------------'
   */
[1] = LAYOUT_65_ansi_split_bs(

_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,XXXXXXX,XXXXXXX,XXXXXXX,_______,_______, \
       _______,XXXXXXX,KC_BTN1,KC_MS_U,KC_BTN2,KC_WH_U,XXXXXXX,KC_PGUP,  KC_UP,LALT(KC_UP),XXXXXXX,XXXXXXX,XXXXXXX,_______,_______, \
         _______,XXXXXXX,KC_MS_L,KC_MS_D,KC_MS_R,KC_WH_D,LALT(KC_LEFT),KC_LEFT,KC_DOWN,KC_RGHT,LALT(KC_RGHT),XXXXXXX,_______,_______, \
             _______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,KC_PGDN,XXXXXXX,LALT(KC_DOWN),XXXXXXX,      _______,_______,_______, \
     _______,_______,_______,                    KC_ENT,                         _______,_______,_______,_______,_______,_______),

  /* Keymap ModLayer
   * ,----------------------------------------------------------------.
   * |    |   |   |   |   |   |   |   |   |   |   |RGB|Mute|Pause|    |
   * |----------------------------------------------------------------|
   * |     |   |MK1|MKU|MK2|   |   |PUp| Up |   |  š | ž | ć  |   |   |
   * |----------------------------------------------------------------|
   * |      |   |MKL|MKD|MKR|   |a(l)|lft|down|rght|a(r)| č |   |     |    
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |PDn|   |   |   |      |CAPS|   |
   * |----------------------------------------------------------------|
   * |    |    |    |        Enter          |   |   |    |   |   |    |
   * `----------------------------------------------------------------'
   */
	 

[2] = LAYOUT_65_ansi_split_bs(

_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,XXXXXXX,XXXXXXX,RGB_TOG,KC_MUTE,KC_MPLY, \
       _______,XXXXXXX,KC_BTN1,KC_MS_U,KC_BTN2,KC_WH_U,XXXXXXX,KC_PGUP,  KC_UP,XXXXXXX,KC_LBRC,KC_BSLS,KC_QUOT,_______,_______, \
         _______,XXXXXXX,KC_MS_L,KC_MS_D,KC_MS_R,KC_WH_D,LALT(KC_LEFT),KC_LEFT,KC_DOWN,KC_RGHT,LALT(KC_RGHT),KC_SCLN,_______,_______, \
             _______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,KC_PGDN,XXXXXXX,XXXXXXX,XXXXXXX,      _______,_______,_______, \
     _______,_______,_______,                    KC_ENT,                         _______,_______,_______,_______,_______,_______)	 
 };

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

void led_set_user(uint8_t usb_led) {
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
	  register_code(KC_LGUI);
	  } else {
      if (timer_elapsed(modspotlight_timer) < TAPPING_TERM && modspotlight) {
          unregister_code(KC_SPC);
          register_code(KC_SPC);
          unregister_code(KC_LGUI);
  	  } else {
	  unregister_code(KC_LGUI);		  
  	  }
    }
    return false;
  case CU_MOD:
    if(record->event.pressed) {
      modlayer = true;
      modlayer_timer = timer_read();
	  layer_on(2);
	  } else {
      if (timer_elapsed(modlayer_timer) < TAPPING_TERM && modlayer) {
          layer_off(2);
		  layer_invert(1);
  	  } else {
          layer_off(2);
  	  }
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
  case CU_QUOT:
    SHIFT_ALL(KC_7, KC_2)
  case CU_SCLN:
    SHIFT_ALL(KC_COMM, KC_DOT)
  case KC_COMM:
    SHIFT_NO(KC_COMM, KC_GRV)
  case KC_DOT:
  	SHIFT_NORM(KC_DOT, KC_GRV)
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

// layer highlighting

// Light LEDs 9 & 10 in cyan when keyboard layer 1 is active
const rgblight_segment_t PROGMEM my_base_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 16, HSV_WHITE}
	);
	
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 16, HSV_RED}
	);

const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 16, HSV_ORANGE}
	);
	
const rgblight_segment_t PROGMEM my_caps_lock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 16, HSV_PURPLE}
	);
	
// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
	my_base_layer,
	my_layer1_layer,    // Overrides layer 1 lock layer
	my_layer2_layer,    // Overrides layer 1 lock layer
	my_caps_lock_layer    // Overrides layer 1 lock layer
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Both layers will light up if both kb layers are active
    rgblight_set_layer_state(0, layer_state_cmp(state, 0));
    rgblight_set_layer_state(1, layer_state_cmp(state, 1));
    rgblight_set_layer_state(2, layer_state_cmp(state, 2));
    return state;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(3, led_state.caps_lock);
    return true;
}
