#include "examproc.h" 

void setup() {
  Serial.begin(9600);
  initialize(); // 試験処理初期化
}

void loop() {
  // 試験処理
  examproc();
}