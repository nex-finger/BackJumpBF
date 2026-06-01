/* @file task.c
 * タスク関連 */

#include "mcc_generated_files/system/system.h"
#include "task.h"

static struct sTaskTable[] = {
    {TASK_SERIAL_INPUT, t_serialinput, 0, 4},
    {TASK_SERIAL_CALLBACK, null, 0, 3},
    {TASK_SERIAL_STD_OUTPUT, null, 0, 2},
    {TASK_SERIAL_ERR_OUTPUT, null, 0, 2},
    {TASK_COMMAND_PARSE, null, 0, 2},
    {TASK_COMMAND_EXCUTE, null, 0, 2},
    {TASK_INPUT_QUEUE_RESET, null, 0, 5},
    {TASK_OUTPUT_UPDATE, null, 5},
    {TASK_INPUT_UPDATE, null, 5},
    {TASK_LED_UPDATE, null, 2},
    {TASK_10MS, null, 4},
    {TASK_IDLE, null, 1}};