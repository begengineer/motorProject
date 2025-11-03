#include "switch.h"
#include "motor.h"
#include "examproc.h"
#include "sensor.h"

// 試験状態関数テーブル
ExamStateFunc examStateFuncs[EXAM_MODE_NUM] = {
    examReady,     // 試験初期化
    examRunning,   // 試験中
    examPaused,    // 試験停止中
    examCompleted  // 試験完了
};

// 試験状態変数
ExamMode examMode = EXAM_READY; 
// モーター状態変数
MotorMode motorMode = RESTING_MODE;
// 試験回数設定
unsigned int examNumber = 0;
// 休止時間
unsigned int  restTime = 0;
// モーター駆動時間格納用変数
double oneWayTimes[SAMPLE_SIZE] = {0};
// 時間用関数
unsigned long prevbebugTime = 0;

// 現在のセンサ状態(おそらく2つ)
SensorState currentState = {false, false};

// 内部使用関数
bool checkZero(double *oneWayTimes); // 配列に0が含まれていないかの確認
double calcMean(double *oneWayTimes); //　試験時間平均算出用

// 初期化宣言
void initialize(){
  // スイッチピンの初期化
  initSwitches();
  // モーター設定初期化
  initMotor();
  // センサ初期化
  SensorInitialize();
}

// 試験処理
void examproc(){
    // スイッチ状態読み取り
    uint8_t switches = readSwitches();

    // デバッグ
    if(millis() - prevbebugTime >= 1000){
        debugPrintSwitches();
        debugPrintMotor();
        //　時間の更新
        prevbebugTime = millis();
    }

    // 以下にに試験の条件と具体的に処理をいれて実行関数を制御する

    // 現在の状態の実行
    examStateFuncs[examMode](switches);
}

// 試験処理
void examReady(uint8_t switcheState){
    // 試験条件の設定

    // スイッチ1が押されたらexamNumberを1増やす。
    // スイッチ2が押されたらexamNumberを1減らす

    // スイッチ3が押されたらrestTimeを1増やす
    // スイッチ4が押されたらrestTimeを1減らす

    // スイッチ5が押されたらexamModeをEXAM_RUNNINGにする
}


// 試験中の挙動
void examRunning(uint8_t switcheState){
    //   static MotorDirection currentDirection = MOTOR_FORWARD;
    //   static int Countidx = 0;
    //   static double mean = 0;
    //   static unsigned long startTime = 0;

    //   // センサ状態読み込み
    //   currentState.sensor1 = Sensor1State();
    //   currentState.sensor2 = Sensor2State();
    // 　割り込み時の反転処理
    // 　currentDirection = (currentDirection == MOTOR_FORWARD) ? MOTOR_REVERSE : MOTOR_FORWARD;

    //   // 現在の処理時間の異常の判定
    //       if (checkZero(oneWayTimes)) {
    //           mean = calcMean(oneWayTimes);
    //           unsigned long currentElapsed = millis() - startTime;
    //           if (currentElapsed > mean * 1.5) {
    //               // 異常検出時
    //               examMode = EXAM_PAUSED;
    //               return;
    //           }
    //       }
    //   // 割り込み発生時
    //   if(currentState.sensor1 || currentState.sensor2){
    //       // 経過時間計算
    //       unsigned long now = millis();
    //       unsigned long elapsedTime = now - startTime;
    //       
    //       // 正常なので記録
    //       oneWayTimes[Countidx] = elapsedTime;
    //       Countidx += 1;
    //       if(Countidx >= SAMPLE_SIZE){
    //           Countidx = 0;
    //       }

    //       // 次回のために開始時刻を更新
    //       startTime = now;
    //   }

      // モーターを動作させる

      // 停止ボタンが押されたらexamPausedへ遷移

      // 試験が終了したら試験完了処理に遷移する
}

// 試験一時停止中
void examPaused(uint8_t switcheState){
    // モーターを停止
    // 再度ボタンを押されたらEXAM_RUNNINGへ遷移
}

// 試験完了処理
void examCompleted(uint8_t switcheState){
    // 試験終了通知用LEDを点灯
    // EXAM_READYへ遷移する
}

// 0があるかを確認する関数
bool checkZero(double *oneWayTimes){
    for (int i = 0;i < SAMPLE_SIZE;i++){
        if(oneWayTimes[i] == 0.0){
            return false;
        }
    }
    return true;
}

// 平均算出関数
double calcMean(double *oneWayTimes){
    double mean = 0;
    for (int i = 0;i < SAMPLE_SIZE;i++){
        mean += oneWayTimes[i];
    }
    return (double)mean / SAMPLE_SIZE;
}