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

// モーター状態のデバッグ出力
void debugPrintMotor() {
  Serial.println("=== Motor Status ===");

  // 方向表示
  Serial.print("Direction: ");
  switch (motorState.direction) {
    case MOTOR_STOP:
      Serial.println("STOP");
      break;
    case MOTOR_FORWARD:
      Serial.println("FORWARD");
      break;
    case MOTOR_REVERSE:
      Serial.println("REVERSE");
      break;
    case MOTOR_BRAKE:
      Serial.println("BRAKE");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  // Dutyサイクル
  Serial.print("Duty Cycle: ");
  Serial.print(motorState.dutyCycle);
  Serial.print(" / ");
  Serial.println(motorConfig.maxDuty);

  // 動作状態
  Serial.print("Is Running: ");
  Serial.println(motorState.isRunning ? "YES" : "NO");

  // 最終更新時刻
  Serial.print("Last Update: ");
  Serial.print(motorState.lastUpdateTime);
  Serial.println(" ms");

  // ピン設定状態
  Serial.println("Pin Configuration:");
  Serial.print("  PWM Pin: ");
  Serial.print(motorConfig.pwmPin);
  Serial.print(" (Value: ");
  Serial.print(motorState.dutyCycle);
  Serial.println(")");

  Serial.print("  Dir Pin1: ");
  Serial.print(motorConfig.dirPin1);
  Serial.print(" = ");
  Serial.println(digitalRead(motorConfig.dirPin1) ? "HIGH" : "LOW");

  Serial.print("  Dir Pin2: ");
  Serial.print(motorConfig.dirPin2);
  Serial.print(" = ");
  Serial.println(digitalRead(motorConfig.dirPin2) ? "HIGH" : "LOW");

  // 反転設定
  Serial.print("Invert Direction: ");
  Serial.println(motorConfig.invertDirection ? "YES" : "NO");

  Serial.println("====================");
}
