/* @file task.c
 * タスク関連 */

#include "mcc_generated_files/system/system.h"
#include "task.h"

#define null 0

/* タスクテーブル
 * ID, 関数ポインタ, リクエスト状況, 優先度 */
static struct stTaskTable sTask[] = {
    /* ↑優先度高 */
    {TASK_OUTPUT_UPDATE, task_output_update, 0},
    {TASK_INPUT_UPDATE, task_input_update, 0},
    {TASK_INPUT_QUEUE_RESET, task_input_queue_reset, 0},
    {TASK_SERIAL_INPUT, task_serial_input, 0},
    {TASK_LED_UPDATE, task_LED_update, 0},
    {TASK_10MS, task_10ms, 0},
    {TASK_SERIAL_CALLBACK, task_serial_callback, 0},
    {TASK_SERIAL_STD_OUTPUT, task_serial_std_output, 0},
    {TASK_SERIAL_ERR_OUTPUT, task_serial_err_output, 0},
    {TASK_COMMAND_PARSE, task_command_parse, 0},
    {TASK_COMMAND_EXCUTE, task_command_excute, 0},
    {TASK_IDLE, task_idle, 0}
    /* 優先度低 */
};

/* シリアルデータ入力タスク
 * シリアル通信で受信したデータをバッファに格納する
 * source:      シリアルデータ受信割り込み
 * distination: シリアルデータコールバックタスク(無条件)
 *              構文解析(改行コード確認時)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_serial_input(void)
{
    int aRet;
    return aRet;
}

/* シリアルデータコールバックタスク
 * 受信したシリアルデータをそのまま送信する
 * source:      シリアルデータ入力タスク
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_serial_callback(void)
{
    int aRet;
    return aRet;
}

/* シリアルデータ標準出力タスク
 * システム内のデータを外部へ送信する
 * source:      コマンド実行タスク
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_serial_std_output(void)
{
    int aRet;
    return aRet;
}

/* シリアルデータエラー出力タスク
 * シリアル内の異常を外部へ送信する
 * source:      コマンド実行タスク
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_serial_err_output(void)
{
    int aRet;
    return aRet;
}

/* 構文解析タスク
 * 入力データを構文解析しコマンドを確定する
 * source:      シリアルデータ入力タスク
 * distination: コマンド実行タスク(正常コマンド検知時)
 *              シリアル標準出力タスク(正常コマンド非検知時)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_command_parse(void)
{
    int aRet;
    return aRet;
}

/* コマンド実行タスク
 * 各種コマンドを実行する
 * source:      構文解析タスク
 * distination: シリアルデータ標準出力タスク(正常終了時)
 *              シリアルデータエラー出力タスク(エラー発生時)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_command_excute(void)
{
    int aRet;
    return aRet;
}

/* 入力キュー状態更新タスク
 * インプットカウンタを初期状態へリセットする
 * source:      リセット信号立ち上がり割り込み
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_input_queue_reset(void)
{
    int aRet;
    return aRet;
}

/* 出力状態更新タスク
 * 入力状態から導出される出力状態を反映する
 * source:      クロック信号立ち上がり割り込み
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_output_update(void)
{
    int aRet;
    return aRet;
}

/* 入力状態更新タスク
 * ハードウェアから提供された入力データを保持する
 * source:      クロック信号立ち下がり割り込み
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_input_update(void)
{
    int aRet;
    return aRet;
}

/* LED表示更新タスク
 * システムの状態を7セグに反映する
 * source:      出力状態更新タスク
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_LED_update(void)
{
    int aRet;
    return aRet;
}

/* 10ms定周期実行タスク
 * source:      10ms定周期割り込み
 * distination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_10ms(void)
{
    int aRet;
    return aRet;
}

/* アイドルタスク
 * source:      (なし、他にタスクがない際に実行する)
 * sidtination: (なし)
 * ret:         0   正常終了
 *              -1  エラー発生
 */
int task_idle(void)
{
    int aRet;
    return aRet;
}

/* リクエスト状況取得
 *
 */
unsigned char TASK_GET(int inID)
{
    return sTask[inID].mReqCnt;
}

/* リクエスト追加 */
void TASK_REGISTER(int inID)
{
}

/* リクエスト取り消し */
void TASK_COMPLETE(int inID)
{
}

void TASK_Scheduler(void)
{
    int i;

    for (i = 0; i < TASK_NUM; i++)
    {
    }
}
