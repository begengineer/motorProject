#pragma once
#include <Arduino.h>

#define EXAM_MODE_NUM 4

// 入力ピン定義
extern const int SWITCH_PINS[8];

// プルアップ抵抗を使用可否定義
extern const bool USE_PULLUP;

// センサ割り込みピン
extern const int interruptPin_1;
extern const int interruptPin_2;