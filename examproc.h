#pragma once
#include <Arduino.h>
#include "common.h"

#define SAMPLE_SIZE 10

// 試験状態列挙体
enum ExamMode {
    EXAM_READY = 0,
    EXAM_RUNNING = 1,
    EXAM_PAUSED = 2,
    EXAM_COMPLETED = 3,
};

// 試験中状態列挙体
enum MotorMode {
    ACTION_MODE = 0,
    RESTING_MODE = 1,
};

// センサ状態構造体
typedef struct{
    bool sensor1;
    bool sensor2;
}SensorState;

// 試験状態遷移関数ポインタ
typedef void (*ExamStateFunc)(uint8_t switcheState);

// プロトタイプ宣言
void initialize(); // 初期化
void examproc(); // 試験本処理

// 以下が状態テーブル関数宣言
void examReady(uint8_t switcheState); // 試験開始待機
void examRunning(uint8_t switcheState); // 試験実行中
void examPaused(uint8_t switcheState); // 一時停止中
void examCompleted(uint8_t switcheState); // 試験完了

