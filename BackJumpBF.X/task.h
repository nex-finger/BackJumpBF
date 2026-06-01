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

struct sTaskTable
{
    int mId;
    int (*mpFunc)(void);
    unsigned char mReq;
    unsigned char mPri;
}

#endif /* TASK_H */
