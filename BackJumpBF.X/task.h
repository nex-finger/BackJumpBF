/* @file task.h
 * タスク関連 */

#ifndef TASK_H
#define TASK_H

/* タスク一覧 */
#define TASK_SERIAL_INPUT 0
#define TASK_SERIAL_CALLBACK 1
#define TASK_SERIAL_STD_OUTPUT 2
#define TASK_SERIAL_ERR_OUTPUT 3
#define TASK_COMMAND_PARSE 4
#define TASK_COMMAND_EXCUTE 5
#define TASK_INPUT_QUEUE_RESET 6
#define TASK_OUTPUT_UPDATE 7
#define TASK_INPUT_UPDATE 8
#define TASK_LED_UPDATE 9
#define TASK_10MS 10
#define TASK_IDLE 11

#define TASK_NUM 12

/* シリアルデータ入力バッファ */
#define INPUT_SERIAL_LEN 300

struct stTaskTable
{
    int mId;               /* タスクID */
    int (*mpFunc)(void);   /* タスク関数 */
    unsigned char mReqCnt; /* リクエスト数 */
};

/* 各タスク処理 */
int task_serial_input(void);
int task_serial_callback(void);
int task_serial_std_output(void);
int task_serial_err_output(void);
int task_command_parse(void);
int task_command_excute(void);
int task_input_queue_reset(void);
int task_output_update(void);
int task_input_update(void);
int task_LED_update(void);
int task_10ms(void);
int task_idle(void);

/* タスク登録、登録解除 */
void TASK_REGISTER(int inID);
void TASK_COMPLETE(int inID);

/* 初期化 */
void task_init(void);

/* タスクスケジューラ */
void TASK_Scheduler(void);

#endif /* TASK_H */
