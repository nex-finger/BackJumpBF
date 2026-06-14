/* @file task.c */

/* 文字列のパース
 * 印字可能文字のオフセットを複数見つけ、印字不可能文字をヌル文字に変換する
 * 改行コード (\r) を発見し次第リターンする
 * in:  *inS 入力文字列
 * out: *outOffset 切り出したトークンの先頭オフセット
 * RET: aRet 切り出したトークン数
 */
int strparse(unsigned char *inS, int *outOffset)
{
    int aRet;

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