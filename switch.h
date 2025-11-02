#pragma once
#include <Arduino.h>

// チャタリング防止用の遅延時間
extern const unsigned long DEBOUNCE_DELAY;

// 前回の読み取り時刻
extern unsigned long lastDebounceTime[8];

// 前回の生の状態
extern int lastSwitchState[8];

// 現在の確定した状態
extern int switchState[8];

// プロトタイプ宣言
void initSwitches() ; // スイッチ初期化
uint8_t readSwitches(); // スイッチ読み込み
