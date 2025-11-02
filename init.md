# SwitchProc プロジェクト概要

## プロジェクトの目的
8つのスイッチ入力とモーター制御を使った試験装置の制御プログラム。
状態遷移による試験フローの自動化を実現。

---

## ファイル構成

### メインファイル
- **main.ino**: Arduino メインプログラム
  - `setup()`: シリアル通信の初期化と`initialize()`の呼び出し
  - `loop()`: `examproc()`を繰り返し実行

### 共通定義
- **common.h / common.cpp**: プロジェクト全体で使用する定数定義
  - `SWITCH_PINS[8]`: スイッチ入力ピン配列 (22, 24, 26, 28, 30, 32, 34, 36)
  - `USE_PULLUP`: プルアップ抵抗の使用設定 (true)
  - `EXAM_MODE_NUM`: 試験状態の数 (4)

### スイッチ入力モジュール
- **switch.h / switch.cpp**: 8つのスイッチ入力の読み取り
  - `initSwitches()`: スイッチピンの初期化（プルアップ設定）
  - `readSwitches()`: 8つのスイッチ状態を8ビット値で返す
  - **チャタリング防止機能**: 50ms のデバウンス処理実装済み
  - **返り値**: `uint8_t` (ビット0〜7が各スイッチに対応、LOW=1, HIGH=0)

### モーター制御モジュール
- **motor.h / motor.cpp**: DCモーターの駆動制御
  - `initMotor(MotorConfig &config)`: モーターピンの初期化
  - `driveMotor(...)`: モーター駆動（方向とデューティ比を指定）
  - `stopMotor(...)`: モーター停止
  - `brakeMotor(...)`: モーターブレーキ

  **モーター駆動方向**:
  - `MOTOR_STOP`: 停止
  - `MOTOR_FORWARD`: 正転
  - `MOTOR_REVERSE`: 逆転
  - `MOTOR_BRAKE`: ブレーキ

  **現在のピン設定**:
  - PWMピン: 2
  - 方向制御ピン1: 3 (修正済み)
  - 方向制御ピン2: 4 (修正済み)

### 試験処理モジュール
- **examproc.h / examproc.cpp**: 試験フローの状態管理と処理
  - `initialize()`: スイッチとモーターの初期化
  - `examproc()`: 試験処理のメインループ（状態に応じた処理を実行）

  **状態遷移の設計（関数ポインタ配列方式）**:
  ```
  EXAM_READY → EXAM_RUNNING → EXAM_PAUSED
       ↑            ↓              ↓
       └──── EXAM_COMPLETED ←──────┘
  ```

---

## 重要な設計決定

### 1. 状態遷移に関数ポインタ配列を使用
**理由**: 状態と処理関数を列挙体のインデックスで自動的に対応させる

```cpp
// 関数ポインタテーブル
ExamStateFunc examStateFuncs[EXAM_MODE_NUM] = {
    examReady,     // EXAM_READY = 0
    examRunning,   // EXAM_RUNNING = 1
    examPaused,    // EXAM_PAUSED = 2
    examCompleted  // EXAM_COMPLETED = 3
};

// 状態遷移は列挙体の値を変えるだけ
examMode = EXAM_RUNNING;

// 実行時は配列アクセス
examStateFuncs[examMode]();
```

**メリット**:
- 状態遷移時に関数ポインタを毎回代入する必要がない
- コードが簡潔で保守しやすい
- 列挙体と関数の対応が明確

### 2. 参照渡しの使用
モーター制御関数では参照渡し（`&`）を使用：
```cpp
void initMotor(MotorConfig &config);  // 参照渡し
```

**呼び出し時**:
```cpp
initMotor(motorConfig);  // & を付けない
```

**理由**:
- 構造体のコピーを避けて効率的
- C++らしい安全な書き方
- 呼び出しがシンプル

---

## 各試験状態の役割

### EXAM_READY (試験開始待機)
- 試験条件の設定
- 開始ボタンの待機
- 開始ボタンが押されたら `EXAM_RUNNING` へ遷移

### EXAM_RUNNING (試験実行中)
- モーターの駆動制御
- 末端センサの確認
- 往復間の休止時間処理
- モーター回転方向の反転
- 休止ボタンが押されたら `EXAM_PAUSED` へ遷移
- 試験終了条件を満たしたら `EXAM_COMPLETED` へ遷移

### EXAM_PAUSED (一時停止中)
- モーターを停止
- 再開ボタンの待機
- 再開ボタンが押されたら `EXAM_RUNNING` へ遷移

### EXAM_COMPLETED (試験完了)
- 試験終了通知用LEDを点灯
- 次の試験のために `EXAM_READY` へ遷移

---

## モーター制御の補助状態

**MotorMode列挙体**: EXAM_RUNNING状態内でのモーター動作を管理
- `ACTION_MODE`: モーター駆動中
- `RESTING_MODE`: 往復間の休止中

---

## 重要な注意事項

### ⚠️ ピンの重複に注意
**過去の問題**: スイッチピン（22, 24）とモーター制御ピンが重複していた
**現在**: モーター制御ピンを 3, 4 に変更して解決済み

**使用ピン一覧**:
- スイッチ入力: 22, 24, 26, 28, 30, 32, 34, 36
- モーターPWM: 2
- モーター方向1: 3
- モーター方向2: 4

### ⚠️ チャタリング処理
スイッチの読み取りには50msのデバウンス処理が実装されています。
高速なスイッチ入力が必要な場合は `DEBOUNCE_DELAY` を調整してください。

---

## コーディング規約

### 命名規則
1. **列挙体**: `UPPER_SNAKE_CASE` (例: `EXAM_READY`, `MOTOR_FORWARD`)
2. **関数**: `camelCase` (例: `examReady()`, `readSwitches()`)
3. **変数**: `camelCase` (例: `examMode`, `motorConfig`)
4. **型定義**: `PascalCase` (例: `ExamStateFunc`, `MotorConfig`)
5. **定数**: `UPPER_SNAKE_CASE` (例: `DEBOUNCE_DELAY`, `EXAM_MODE_NUM`)

### 関数の形式
状態処理関数は形容詞形を使用:
- ❌ `examinit`, `examduring` (動詞、分かりにくい)
- ✅ `examReady`, `examRunning` (形容詞、状態が明確)

---

## 今後の実装予定

### examReady() の実装
- [ ] 試験条件の設定UI
- [ ] 開始ボタンの検出
- [ ] EXAM_RUNNING への遷移処理

### examRunning() の実装
- [ ] モーター駆動ロジック
- [ ] 末端センサの読み取り
- [ ] 往復回数のカウント
- [ ] 休止時間の管理
- [ ] モーター方向反転の実装
- [ ] 休止/終了ボタンの検出
- [ ] 状態遷移処理

### examPaused() の実装
- [ ] モーター停止処理
- [ ] 再開ボタンの検出
- [ ] EXAM_RUNNING への復帰処理

### examCompleted() の実装
- [ ] LED点灯処理
- [ ] 試験結果の表示/保存
- [ ] EXAM_READY への遷移処理

---

## トラブルシューティング

### コンパイルエラー
1. **関数が見つからない**: ヘッダーファイルのインクルードを確認
2. **型の不一致**: 参照渡し（`&`）の使い方を確認
3. **配列サイズ**: `EXAM_MODE_NUM` と実際の配列要素数が一致しているか確認

### 動作不良
1. **スイッチが反応しない**: ピン番号とプルアップ設定を確認
2. **モーターが動かない**: ピンの重複がないか確認
3. **状態遷移しない**: `examMode` の値が正しく変更されているか確認

---

## 開発履歴

### 設計の変遷
1. 単一の関数ポインタ → **関数ポインタ配列** に変更
2. `examinit/during/pause/finish` → **`examReady/Running/Paused/Completed`** に命名改善
3. ピン重複問題を発見・修正

### 主要な修正
- motor.cpp のピン設定バグ修正（`config.dirPin` → `config.dirPin1`）
- switch.h の `DEBOUNCE_DELAY` 初期化漏れ修正
- モーター制御ピンとスイッチピンの重複解消

---

## 参考情報

### 状態遷移の実装例
```cpp
// examReady() 内で状態遷移する場合
void examReady() {
    uint8_t switches = readSwitches();
    if (switches & 0x01) {  // ビット0のスイッチが押された
        examMode = EXAM_RUNNING;  // 状態遷移
    }
}
```

### モーター制御の実装例
```cpp
// examRunning() 内でモーターを制御する場合
void examRunning() {
    if (motorMode == ACTION_MODE) {
        driveMotor(MOTOR_FORWARD, 200);  // デューティ比200で正転
    } else {
        driveMotor(MOTOR_STOP, 0);  // 停止
    }
}
```

---

**最終更新**: 2025-11-02
**プロジェクト状態**: 基本フレームワーク完成、各状態の具体的処理は未実装
