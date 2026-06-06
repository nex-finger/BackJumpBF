/* @file task.c
 * タスク関連 */

#include "mcc_generated_files/system/system.h"
#include "task.h"
#include "command.h"

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

/* リクエスト追加
 * タスクIDのタスクをリクエストする
 * in:  inID タスクID
 * out: (なし)
 */
void TASK_REGISTER(int inID)
{
    unsigned char aFlag;
    int i;

    /* 多重割り込み禁止 */
    aFlag = INTCONbits.GIE;
    INTCONbits.GIE = 0;

    for (i = 0; i < TASK_NUM; i++)
    {
        if (sTask[i].mId == inID)
        {
            sTask[i].mReqCnt++;
            break;
        }
    }

    /* 多重割り込み許可 */
    INTCONbits.GIE = 1;

    return;
}

/* リクエスト取り消し
 * タスクIDのタスクリクエストを解除する
 * タスクが複数リクエストされている場合リクエスト状態はリセットされない
 * in:  inID
 * out: (なし)
 */
void TASK_COMPLETE(int inID)
{
    unsigned char aFlag;
    int i;

    /* 多重割り込み禁止 */
    aFlag = INTCONbits.GIE;
    INTCONbits.GIE = 0;

    for (i = 0; i < TASK_NUM; i++)
    {
        if (sTask[i].mId == inID)
        {
            if (sTask[i].mReqCnt != 0)
            {
                /* タスクリクエストがない場合はデクリメントしない */
                sTask[i].mReqCnt--;
            }
            break;
        }
    }

    /* 多重割り込み許可 */
    INTCONbits.GIE = 1;

    return;
}

/* タスクスケジューラ
 * タスク優先度の高いタスクから実行する
 * 決してreturnせず、無限ループする
 * in:  (なし)
 * out: (なし) */
void TASK_Scheduler(void)
{
    unsigned char i;
    int aTaskRet;

    /* リクエストのあるタスクを検索する */
    /* 優先度の高い順に並んでいるので、発見し次第抜ける */
    for (i = 0; i < TASK_NUM; i++)
    {
        if (sTask[i].mReqCnt != 0)
        {
            break;
        }
    }

    aTaskRet = sTask[i].mpFunc();
    TASK_COMPLETE(i);
    if (aTaskRet != 0)
    {
        /* タスクが正常に完了しなかった場合エラー出力 */
        com_puterr(aTaskRet);
    }
}
