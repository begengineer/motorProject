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
        result |= (1 << i);   // 押されている → 1をセット
    } else {
        result &= ~(1 << i);  // 離されている → 0をセット
    }
  }
  
  return result;
}