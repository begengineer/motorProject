#pragma once
#include <Arduino.h>

// プロトタイプ宣言
void SensorInitialize();
void SensorISR1();
void SensorISR2();
bool Sensor1State();
bool Sensor2State();