#include "motor.h"

// モーター初期設定
static MotorConfig motorConfig = {
  .pwmPin = 2,
  .dirPin1 = 3,
  .dirPin2 = 4,
  .maxDuty = 255,
  .minDuty = 0,
  .invertDirection = false
};

// モーター状態初期設定
static MotorState motorState = {
  .direction = MOTOR_STOP,
  .dutyCycle = 0,
  .isRunning = false,
  .lastUpdateTime = 0
};

// 指定モーター状態変数
MotorDirection currentDirection = MOTOR_STOP;

// ハードウェア設定関数プロトタイプ宣言
static void motorForward(MotorConfig &config, MotorState &state,int dutyCycle);
static void motorBackward(MotorConfig &config, MotorState &state,int dutyCycle);
static void motorStop(MotorConfig &config, MotorState &state,int dutyCycle);
static void motorBrake(MotorConfig &config, MotorState &state,int dutyCycle);

// モーター関数テーブル
MotorFunction motorFunction[MOTR_FUNC_NUM] = {
    motorStop,      // インデックス 0 = MOTOR_STOP
    motorForward,   // インデックス 1 = MOTOR_FORWARD
    motorBackward,  // インデックス 2 = MOTOR_REVERSE
    motorBrake,     // インデックス 3 = MOTOR_BRAKE
};

// 初期化
void initMotor() {
  initMotorProc(motorConfig);
}

// 設定初期化
void initMotorProc(MotorConfig &config){
  // 設定
  pinMode(config.pwmPin, OUTPUT);
  pinMode(config.dirPin1, OUTPUT);
  pinMode(config.dirPin2, OUTPUT);

  // 初期設定
  digitalWrite(config.dirPin1, LOW);
  digitalWrite(config.dirPin2, LOW);
  analogWrite(config.pwmPin, 0);
}

// モーター動作
bool MotorControl(MotorConfig &config, MotorState &state, MotorDirection direction, int dutyCycle) {

  // Duty設定
  dutyCycle = constrain(dutyCycle, config.minDuty, config.maxDuty);

  // 方向反転
  MotorDirection actualDirection = direction;
  if (config.invertDirection) {
    if (direction == MOTOR_FORWARD) actualDirection = MOTOR_REVERSE;
    else if (direction == MOTOR_REVERSE) actualDirection = MOTOR_FORWARD;
  }

  // モーター動作実行
  motorFunction[actualDirection](config,state,dutyCycle);

  // 設定更新
  state.direction = direction;
  state.dutyCycle = dutyCycle;
  state.lastUpdateTime = millis();
  return true;
}

// 前進
static void motorForward(MotorConfig &config, MotorState &state,int dutyCycle){
    digitalWrite(config.dirPin1, HIGH);
    digitalWrite(config.dirPin2, LOW);
    analogWrite(config.pwmPin, dutyCycle);
    state.isRunning = (dutyCycle > 0);
}
// 後進
static void motorBackward(MotorConfig &config, MotorState &state,int dutyCycle){
    digitalWrite(config.dirPin1, LOW);
    digitalWrite(config.dirPin2, HIGH);
    analogWrite(config.pwmPin, dutyCycle);
    state.isRunning = (dutyCycle > 0);
}
// 停止
static void motorStop(MotorConfig &config, MotorState &state,int dutyCycle){
    digitalWrite(config.dirPin1, LOW);
    digitalWrite(config.dirPin2, LOW);
    analogWrite(config.pwmPin, 0);
    state.isRunning = false;
}
// ブレーキ
static void motorBrake(MotorConfig &config, MotorState &state,int dutyCycle){
    digitalWrite(config.dirPin1, HIGH);
    digitalWrite(config.dirPin2, HIGH);
    analogWrite(config.pwmPin, 255);
    state.isRunning = false;
}

// ユーザーインターフェース関数
// モーター動作
bool driveMotor(MotorDirection direction, int dutyCycle) {
  return MotorControl(motorConfig, motorState, direction, dutyCycle);
}
// モーター停止
void stopMotor() {
  MotorControl(motorConfig, motorState, MOTOR_STOP, 0);
}
// モーターブレーキ
void brakeMotor() {
  MotorControl(motorConfig, motorState, MOTOR_BRAKE, 255);
}
