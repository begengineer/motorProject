#pragma once
#include <Arduino.h>

#define MOTR_FUNC_NUM 4

// モーター駆動方向の列挙体
enum MotorDirection {
    MOTOR_STOP = 0,
    MOTOR_FORWARD = 1,
    MOTOR_REVERSE = 2,
    MOTOR_BRAKE = 3
};

// モーターの初期設定構造体
struct MotorConfig {
    int pwmPin;
    int dirPin1;
    int dirPin2;
    int maxDuty;
    int minDuty;
    bool invertDirection;
};

// モーターの状態構造体
struct MotorState {
    MotorDirection direction;
    int dutyCycle;
    bool isRunning;
    unsigned long lastUpdateTime;
};

// モーター動作状態関数ポインタ
typedef void (*MotorFunction)(MotorConfig &config, MotorState &state,int dutyCycle);

// プロトタイプ宣言
void initMotor();
void initMotorProc(MotorConfig &config);
bool MotorControl(MotorConfig &config, MotorState &state, MotorDirection direction, int dutyCycle);
bool driveMotor(MotorDirection direction, int dutyCycle);
void stopMotor();
void brakeMotor();
void debugPrintMotor(); // モーター状態のデバッグ出力
