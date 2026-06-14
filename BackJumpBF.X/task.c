/* @file task.c
 * タスク関連 */

#include "mcc_generated_files/system/system.h"
#include "task.h"
#include "command.h"
#include "my_library/convert.h"

#define null 0

static unsigned char sInputSerialData[INPUT_SERIAL_LEN];       /* 入力バッファ */
static unsigned char sOutputSerialRingBuff[OUTPUT_SERIAL_LEN]; /* 出力リングバッファ */
static unsigned char sErrorSerialData[ERROR_SERIAL_LEN];       /* エラー出力バッファ */
static int sInputSerialOffsetIn;                               /* 入力データ格納オフセット */
static int sInputSerialOffsetOut;                              /* 入力データ送信オフセット */
static int sOutputSerialOffsetSet;                             /* 出力データ格納オフセット */
static int sOutputSerialOffsetPut;                             /* 出力データ送信オフセット */

/* タスクテーブル
 * ID, 関数ポインタ, リクエスト状況, 優先度 */
static struct stTaskTable sTask[] = {
    /* ↑優先度高 */
    {TASK_OUTPUT_UPDATE, task_output_update, 0},
    {TASK_INPUT_UPDATE, task_input_update, 0},
    {TASK_INPUT_QUEUE_RESET, task_input_queue_reset, 0},
    {TASK_SERIAL_CALLBACK, task_serial_callback, 0},
    {TASK_SERIAL_INPUT, task_serial_input, 0},
    {TASK_LED_UPDATE, task_LED_update, 0},
    {TASK_10MS, task_10ms, 0},
    {TASK_SERIAL_ERR_OUTPUT, task_serial_err_output, 0},
    {TASK_SERIAL_STD_OUTPUT, task_serial_std_output, 0},
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
    int aRet = 0;
    unsigned char aRx;

    /* シリアル入力状態を確認 */
    if (EUSART_IsRxReady())
    {
        /* 1文字蓄積 */
        sInputSerialData[sInputSerialOffsetIn] = EUSART_Read();

        /* コールバックタスクをリクエスト */
        TASK_REGISTER(TASK_SERIAL_CALLBACK);

        /* 改行コードなら構文解析タスクをリクエスト */
        if (sInputSerialData[sInputSerialOffsetIn] == '\r')
        {
            TASK_REGISTER(TASK_COMMAND_PARSE);
        }

        sInputSerialOffsetIn++;
    }
    else
    {
        /* タスクのリクエストはあるのに入力データがない */
        aRet = -1;
    }

    /* バッファがあふれる場合、エラーを通知しバッファをクリアする */
    if (sInputSerialOffsetIn >= INPUT_SERIAL_LEN)
    {
        strcpy("Input buff has overflowed.\r\nThe operation will be ignored.\r\n", sErrorSerialData);
        TASK_REGISTER(TASK_SERIAL_ERR_OUTPUT);
    }

    /* タスク完了を通知 */
    TASK_COMPLETE(TASK_SERIAL_INPUT);

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
    int aRet = 0;

    /* 1文字出力 */
    EUSART_Write(sInputSerialData[sInputSerialOffsetOut]);
    sInputSerialOffsetOut++;

    __delay_ms(2);

    /* タスク完了を通知 */
    TASK_COMPLETE(TASK_SERIAL_CALLBACK);

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
    int aRet = 0;

    /* 1文字出力 */
    EUSART_Write(sOutputSerialRingBuff[sOutputSerialOffsetPut]);
    output_offset_put_inc();

    __delay_ms(2);

    /* タスク完了を通知 */
    TASK_COMPLETE(TASK_SERIAL_STD_OUTPUT);

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
    int aRet = 0;
    int i;

    const unsigned char cInitialStr[] = "\x1b[31m\r\n";
    const unsigned char cFinalStr[] = "\x1b[0m";

    /* 文字を赤色に変える */
    i = 0;
    while (cInitialStr[i] != '\0')
    {
        EUSART_Write(cInitialStr[i]);
        __delay_ms(2);
        i++;
    }

    /* エラーメッセージを出力 */
    i = 0;
    while (sErrorSerialData[i] != '\0')
    {
        EUSART_Write(sErrorSerialData[i]);
        __delay_ms(2);
        i++;
    }

    /* 文字をデフォルトに戻す */
    i = 0;
    while (cFinalStr[i] != '\0')
    {
        EUSART_Write(cFinalStr[i]);
        __delay_ms(2);
        i++;
    }

    /* エラーのためバッファを初期化 */
    sInputSerialOffsetIn = 0;
    sInputSerialOffsetOut = 0;

    /* タスク完了を通知 */
    TASK_COMPLETE(TASK_SERIAL_ERR_OUTPUT);

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
    int aRet = 0;

    /* テスト ----> */
    TASK_REGISTER(TASK_COMMAND_EXCUTE);
    /* <---- テスト */

    /* タスク完了を通知 */
    TASK_COMPLETE(TASK_COMMAND_PARSE);

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
    int aRet = 0;

    /* テスト ----> */
    unsigned char aTestStr[] = "std task is running!\r\n";
    com_puts(aTestStr);
    /* <---- テスト */

    /* 入力バッファの内容を初期化 */
    sInputSerialOffsetIn = 0;
    sInputSerialOffsetOut = 0;

    /* タスク完了を通知 */
    TASK_COMPLETE(TASK_COMMAND_EXCUTE);

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
    int aRet = 0;

    /* 1msなにもしない、タスクは完了させない */
    __delay_ms(1);

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
    INTCONbits.GIE = aFlag;

    return;
}

/* タスク関連初期化処理
 * in:  (なし)
 * out: (なし)
 */
void task_init(void)
{
    int i;

    /* シリアルデータ入力バッファ初期化 */
    for (i = 0; i < INPUT_SERIAL_LEN; i++)
    {
        sInputSerialData[i] = '\0';
    }
    sInputSerialOffsetIn = 0;
    sInputSerialOffsetOut = 0;

    /* シリアルデータ出力リングバッファ初期化 */
    for (i = 0; i < OUTPUT_SERIAL_LEN; i++)
    {
        sOutputSerialRingBuff[i] = '\0';
    }
    sOutputSerialOffsetPut = 0;
    sOutputSerialOffsetSet = 0;
}

/* タスクスケジューラ
 * タスク優先度の高いタスクから実行する
 * 決してreturnせず、無限ループする
 * in:  (なし)
 * out: (なし) */
void TASK_Scheduler(void)
{
    unsigned char i;
    int aTaskReservationFlg;
    int aTaskRet;

    while (1)
    {
        aTaskRet = 0;
        aTaskReservationFlg = 0;

        /* リクエストのあるタスクを検索する */
        /* 優先度の高い順に並んでいるので、発見し次第抜ける */
        for (i = 0; i < TASK_NUM; i++)
        {
            if (sTask[i].mReqCnt != 0)
            {
                aTaskReservationFlg = 1;
                break;
            }
        }

        /* もし行うタスクがなければアイドルタスクを実行 */
        if (aTaskReservationFlg == 0)
        {
            /* タスクがない */
            task_idle();
        }
        else
        {
            /* タスクがある */
            aTaskRet = sTask[i].mpFunc();
            // TASK_COMPLETE(sTask[i].mId);
        }

        if (aTaskRet != 0)
        {
            /* タスクが正常に完了しなかった場合エラー出力 */
            com_puterr(aTaskRet);
        }
    }
}

void output_offset_set_inc(void)
{
    sOutputSerialOffsetSet++;
    if (sOutputSerialOffsetSet >= OUTPUT_SERIAL_LEN)
    {
        sOutputSerialOffsetSet = 0;
    }
}

void output_offset_put_inc(void)
{
    sOutputSerialOffsetPut++;
    if (sOutputSerialOffsetPut >= OUTPUT_SERIAL_LEN)
    {
        sOutputSerialOffsetPut = 0;
    }
}

void output_register(unsigned char in)
{
    sOutputSerialRingBuff[sOutputSerialOffsetSet] = in;
    output_offset_set_inc();
}