# The mouse keymap for keyboard_quantizer
- Keyboard Quantizerを使ってマウスをQMK化するサンプル(VIA対応)

```
    make keyboard_quantizer/rev3:mouse_via:flash
```

## 機能

- VIAによるボタン割当変更
- 簡易ジェスチャ機能
  - LTまたはMOを押した状態でカーソルを左上、右上、左下、右下に移動してからレイヤ0に戻ったときに任意の動作を設定できます
- VIA/Remapで表示したとき、一番上の列は左から順にボタン1から8に対応し、下のブロックはそれぞれの方向に動かしたときのジェスチャ設定です
  - ジェスチャはLT, MOの移動先のレイヤに設定してください(例: レイヤ1でカーソルを動かしてもとのレイヤ0に戻ったときのジェスチャはレイヤ1に設定する)
  
### 補足
- ジェスチャ認識はLTまたはMOを押した時点から始まります。LTの場合でもTappingTermの経過を待つ必要はありません
- Remapで認識させやすいようにPIDを変更し、ProductNameもmouse_quantizerになっています

## パラメータ
- `GESTURE_MOVE_THRESHOLD`: ジェスチャ認識に必要な最小の移動量
