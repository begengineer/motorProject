#include "switch.h"
#include "motor.h"
#include "examproc.h"
#include "sensor.h"

// 試験状態関数テーブル
ExamStateFunc examStateFuncs[EXAM_MODE_NUM] = {
    examReady,     // EXAM_READY = 0
    examRunning,   // EXAM_RUNNING = 1
    examPaused,    // EXAM_PAUSED = 2
    examCompleted  // EXAM_COMPLETED = 3
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

// 現在のセンサ状態
SensorState currentState = {false, false};

// 内部関数
bool checkZero(double *oneWayTimes);
double calcMean(double *oneWayTimes);

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

    // ここに試験の条件と具体的に処理をいれて実行関数を制御する

    // 現在の状態の実行
    examStateFuncs[examMode](switches);
}

// 以下の具体的処理は後で実装
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
    // 動作方向設定：static MotorDirection currentDirection = MOTOR_FORWARD;
    // static int Countidx = 0;
    // static double mean = 0;

    // センサ状態読み込み
    // currentState.sensor1 =  Sensor1State();
    // currentState.sensor2 =  Sensor2State();

    // orでtrueのとき

    // モーターを動作させる

    // 割り込み待ち→割り込みが入ったらrestTime秒間モーターを停止させるのと割り込みが入るまでの時間を格納する
    // 割り込みが入ったらタイマー変数を更新して移動時間を監視する
    // 以下は割り込みが入ったときの処理
    // まず異常チェック
    // if (checkZero(oneWayTimes)) {
    //     mean = calcMean(oneWayTimes);
    //     if (currentTime > mean * 1.5) {
    //         // 異常検出 → 配列に入れずに一時停止
    //         examMode = EXAM_PAUSED;
    //         return;
    //     }
    // }
    // 以下は割り込みが入ったときに行う処理。
    // oneWayTimes[Countidx] = currentTime;
    // Countidx += 1;
    // if(Countidx >= SAMPLE_SIZE){
    //     Countidx = 0;
    //  }

    // 停止ボタンが押されたらexamPausedへ遷移
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