/* @file command.c
 * コマンド関連 */

#include "mcc_generated_files/system/system.h"
#include "command.h"
#include "task.h"

/* 1文字出力
 * バッファが溢れないように1ミリ秒待機する */
void com_putchar(unsigned char in)
{
    output_register(in);
    TASK_REGISTER(TASK_SERIAL_STD_OUTPUT);
}

/* 文字列出力 */
void com_puts(unsigned char *in)
{
    int i = 0;
    while (in[i] != '\0')
    {
        com_putchar(in[i]);
        i++;
    }
}

/* 1行入力 */
void com_response(void)
{
    unsigned char aInput[32];
    int i = 0;

    /* エンターキーフラグをたたむ
    gEnterFlag = 0;

    while (1)
    {
        c = EUSART_Read();
        EUSART_Write();
        if (c == '\r')
        {
            break;
        }
    }
    EUSART_Write('\r');
    EUSART_Write('\n');
    */
}

/* 起動時に文字列を表示する */
void com_init(void)
{
    const unsigned char cStr[] = "\r\n\r\nBackJumpBF v0.0.1\r\nCopyright (c) 2026 nex-finger\r\nReleased under the 0-clause BSD License.\r\nsource: https://github.com/nex-finger/BackJumpBF\r\n";
    int i = 0;

    /* 改行コード確認フラグの初期化 */
    // gEnterFlag = 0;

    while (cStr[i] != '\0')
    {
        EUSART_Write(cStr[i]);
        __delay_ms(2);
        i++;
    }
}

/* エラー出力を表示する
 * in:  inCode エラーコード
 * out: (なし) */
void com_puterr(int inCode)
{
    if (inCode == 0)
    {
        /* ここに正常時の文字列出力 */
    }
    else if (inCode == -1)
    {
        /* ここに異常時の文字列出力 */
    }
    else
    {
        /* Do Nothing */
    }
}
