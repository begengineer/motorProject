#include "common.h"

// 入力ピン定義
const int SWITCH_PINS[8] = {22, 24, 26, 28, 30, 32, 34, 36};

// プルアップ抵抗を使用可否定義
const bool USE_PULLUP = true;

// 割り込みピン定義
const int interruptPin_1 = 9;
const int interruptPin_2 = 10;