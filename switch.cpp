#include "common.h"
#include "switch.h"

// チャタリング防止用の遅延時間（ミリ秒）
const unsigned long DEBOUNCE_DELAY = 50;

// 前回の読み取り時刻
unsigned long lastDebounceTime[8] = {0};

// 前回の生の状態
int lastSwitchState[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

// 現在の確定した状態
int switchState[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

// スイッチ初期化関数
void initSwitches() {
  for (int i = 0; i < 8; i++) {
    if (USE_PULLUP) {
      pinMode(SWITCH_PINS[i], INPUT_PULLUP);
    } else {
      pinMode(SWITCH_PINS[i], INPUT);
    }
  }
}

// スイッチ読み取り関数
uint8_t readSwitches() {
  // 結果格納用変数
  uint8_t result = 0;
  
  for (int i = 0; i < 8; i++) {
    int reading = digitalRead(SWITCH_PINS[i]);
    
    // 状態が変化したらデバウンスタイマーをリセット
    if (reading != lastSwitchState[i]) {
      lastDebounceTime[i] = millis();
    }
    
    // デバウンス時間が経過したら状態を更新
    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      if (reading != switchState[i]) {
        switchState[i] = reading;
      }
    }
    
    // 前回の状態を保存
    lastSwitchState[i] = reading;
    
    // ビットセット
      if (switchState[i] == LOW) {
        result |= (1 << i);
    } else {
        result &= ~(1 << i);
    }
  }
  
  return result;
}

//　スイッチ状態の取得関数
bool checkSWState(int bit){
  // 引数のビット範囲のチェック
  if(bit < 0 || bit > 7){
    return false;
  }
  // スイッチ状態の取得
  uint8_t currentSWState = readSwitches();

  // 指定ビットの状態を返す
  return (currentSWState >> bit) & 0x01;
}

// スイッチ状態のデバッグ出力
void debugPrintSwitches() {
  uint8_t currentSWState = readSwitches();

  Serial.println("=== Switch Status ===");
  Serial.println("Individual Switches:");
  for (int i = 0; i < 8; i++) {
    Serial.print("  SW");
    Serial.print(i);
    Serial.print(": ");
    if ((currentSWState >> i) & 0x01) {
      Serial.println("ON (Pressed)");
    } else {
      Serial.println("OFF (Released)");
    }
  }
  Serial.println("====================");
}