#include "sensor.h"
#include "common.h"

// センサフラグ
volatile bool ISR1_flag = false;
volatile bool ISR2_flag = false;

// センサーの初期化関数
void SensorInitialize(){
    pinMode(interruptPin_1, INPUT_PULLUP);
    pinMode(interruptPin_2, INPUT_PULLUP);

    // 割り込み設定
    attachInterrupt(digitalPinToInterrupt(interruptPin_1), SensorISR1, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPin_2), SensorISR2, RISING);
}

 // フラグを立てるだけ
void SensorISR1(){
    ISR1_flag = true;
}

 // フラグを立てるだけ
void SensorISR2(){
    ISR2_flag = true;
}

// 状態を返す
bool Sensor1State(){
    noInterrupts(); // 割り込み禁止
    bool result = ISR1_flag;
    ISR1_flag = false;  // 読み取り後クリア
    interrupts(); // 割り込み許可
    return result;
}

// 状態を返す
bool Sensor2State(){
    noInterrupts(); // 割り込み禁止
    bool result = ISR2_flag;
    ISR2_flag = false;  // 読み取り後クリア
    interrupts(); // 割り込み許可
    return result;
}