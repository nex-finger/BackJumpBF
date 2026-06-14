/* @file task.c */

#include "convert.h"

/* 文字列のパース
 * 印字可能文字のオフセットを複数見つけ、印字不可能文字をヌル文字に変換する
 * 改行コード (\r) を発見し次第リターンする
 * i/o: *ioS 入力文字列
 * out: *outOffset 切り出したトークンの先頭オフセット
 * RET: aRet 切り出したトークン数
 */
int strparse(unsigned char *ioS, int *outOffset)
{
    int aRet = 0;
    int i = 0;

    /* ヌル文字が来るまで調べる */
    while (ioS[i] != '\0')
    {
        /* 印字可能文字が来るまで進む */
        while (isprint(ioS[i]) == 0)
        {
            if (ioS[i] == '\0')
            {
                return aRet;
            }
            ioS[i] = '\0';
            i++;
        }

        /* トークンの先頭オフセットを記録 */
        outOffset[aRet] = i;
        aRet++;

        /* 印字不可能文字が来るまで進む */
        while (isprint(ioS[i]) == 1)
        {
            i++;
        }
    }

    return aRet;
}

/* 基本ライブラリ */

/* 文字列のコピー */
void strcpy(unsigned char *inS, unsigned char *outS)
{
    int i = 0;

    while (1)
    {
        outS[i] = inS[i];
        if (inS[i] == '\0')
        {
            break;
        }
        i++;
    }

    return;
}

/* 印字不可能： 0
 * 印字可能： 1 */
int isprint(unsigned char in)
{
    int aRet = 0;

    if (in >= '!' && in <= '~')
    {
        aRet = 1;
    }

    return aRet;
}
