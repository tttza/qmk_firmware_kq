/* Copyright 2020 sekigon-gonnoc
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
#include "keycode.h"
#include "keymap_jp.h"
#include "print.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
};

const uint16_t us2jis[][2] = {
  {KC_LPRN, JP_LPRN},
  {KC_RPRN, JP_RPRN},
  {KC_AT,   JP_AT},
  {KC_LBRC, JP_LBRC},
  {KC_RBRC, JP_RBRC},
  {KC_LCBR, JP_LCBR},
  {KC_RCBR, JP_RCBR},
  {KC_MINS, JP_MINS},
  {KC_EQL,  JP_EQL},
  {KC_BSLS, JP_BSLS},
  {KC_SCLN, JP_SCLN},
  {KC_QUOT, JP_QUOT},
  {KC_GRV,  JP_GRV},
  {KC_PLUS, JP_PLUS},
  {KC_COLN, JP_COLN},
  {KC_UNDS, JP_UNDS},
  {KC_PIPE, JP_PIPE},
  {KC_DQT,  JP_DQT},
  {KC_ASTR, JP_ASTR},
  {KC_TILD, JP_TILD},
  {KC_AMPR, JP_AMPR},
  {KC_CIRC, JP_CIRC},
};


// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    CUSTOM_KEYCODE1 = SAFE_RANGE,
};


#define LOSP LT(_LOWER, KC_SPC)
#define RAEN LT(_RAISE, KC_ENT)


void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] =  LAYOUT(
    KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4,     KC_F5, KC_F6, KC_F7, KC_F8,     KC_F9, KC_F10, KC_F11, KC_F12,    KC_PSCR, KC_SCROLLLOCK, KC_PAUS,       KC_WREF, KC_WREF,
        JP_ZHTG, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, JP_MINS, JP_CIRC, JP_YEN, KC_BSPACE, KC_INSERT, KC_HOME, KC_PGUP,       KC_NUMLOCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
        KC_TAB,   KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, JP_AT, JP_LBRC,           KC_ENT,   KC_DEL, KC_END,  KC_PGDN,          KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS,
        KC_CAPS,   KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, JP_SCLN, JP_COLN, JP_RBRC,                                                  KC_KP_4, KC_KP_5, KC_KP_6,
        KC_LSFT,    KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, JP_SLSH, JP_BSLS, KC_RSFT,                 KC_UP,                     KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER,
        KC_LCTRL, KC_LGUI, KC_LALT, JP_MHEN, LOSP,  RAEN, JP_KANA, KC_RALT, KC_RGUI, KC_APP, KC_RCTRL,            KC_LEFT,  KC_DOWN, KC_RIGHT,       KC_KP_0,          KC_KP_DOT
    )
};
// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (!record->event.pressed) return true;


  uint16_t skeycode; // シフトビットを反映したキーコード
  bool lshifted = keyboard_report->mods & MOD_BIT(KC_LSFT); // シフトキーの状態
  bool rshifted = keyboard_report->mods & MOD_BIT(KC_RSFT);
  bool lctrled = keyboard_report->mods & MOD_BIT(KC_LCTRL); // コントロールキーの状態
  bool rctrled = keyboard_report->mods & MOD_BIT(KC_RCTRL);
  bool shifted = lshifted | rshifted;
  bool ctrled = lctrled | rctrled;

  if (shifted) {
    skeycode = QK_LSFT | keycode;
  } else {
    skeycode = keycode;
  }
  uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);

  // CTRL+SHIFT+F1を無変換、CTRL+SHIFT+F7を変換に割り当て
  if (ctrled & shifted) {
      if (keycode == KC_F1 || keycode == KC_F7){
        unregister_code(KC_LSFT);
        unregister_code(KC_RSFT);
        unregister_code(KC_LCTRL);
        unregister_code(KC_RCTRL);
        if (keycode == KC_F1){
          tap_code(JP_MHEN);
        } else {
          tap_code(JP_HENK);
        }
        return false;
      }
  }

#ifdef CONSOLE_ENABLE
#endif

  for (int i = 0; i < sizeof(us2jis) / sizeof(us2jis[0]); i++) {
    if (us2jis[i][0] == skeycode) {
      unregister_code(KC_LSFT);
      unregister_code(KC_RSFT);

      if ((us2jis[i][1] & QK_LSFT) == QK_LSFT || (us2jis[i][1] & QK_RSFT) == QK_RSFT) {
        register_code(KC_LSFT);
        tap_code(us2jis[i][1]);
        unregister_code(KC_LSFT);
      } else {
        tap_code(us2jis[i][1]);
      }
      if (lshifted) register_code(KC_LSFT);
      if (rshifted) register_code(KC_RSFT);
      return false;
    }
  }

    return true;
}

#ifdef OLED_DRIVER_ENABLE
#    include "rev1.h"
#    include "oled_driver.h"
#endif

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) { render_logo(); }
#endif

