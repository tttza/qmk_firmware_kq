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
#include "keymap_jp.h"
#include "dynamic_keymap.h"

// clang-format off
// Set empty to reduce firm size
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { };
// clang-format on


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



// Override to avoid messy initial keymap show on VIA
// Patch to dynamic_keymap.c is required
void dynamic_keymap_reset(void) {
    for (int layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer++) {
        for (int row = 0; row < MATRIX_ROWS - 1; row++) {
            for (int column = 0; column < MATRIX_COLS; column++) {
                dynamic_keymap_set_keycode(layer, row, column, row * MATRIX_COLS + column);
            }
        }
        for (int column = 0; column < MATRIX_COLS; column++) {
            dynamic_keymap_set_keycode(layer, MATRIX_ROWS - 1, column, KC_LCTRL + column);
        }
    }
}


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

