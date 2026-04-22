/* --------------------------------------------------------------------------
 * カンサスシティスタンダード音声波形作成プログラム
 * 出力ファイル仕様
 * 拡張子:              wav
 * チャンネル数:        2ch
 * サンプリング周波数:  16,000Hz
 * サンプルビット:      16bit
 * -------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
 * ヘッダファイルインポート
 * -------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* --------------------------------------------------------------------------
 * プリプロセッサ
 * -------------------------------------------------------------------------- */
#define DEF_STR_DEAULT_FILE_NAME "out"
#define DEF_STR_OUTPUT_EXTENSION "wav"

#define DEF_NUM_FILE_NAME_LEN 8
#define DEF_NUM_FILE_EXT_LEN 3
#define DEF_NUM_FILE_TOTAL_LEN (DEF_NUM_FILE_NAME_LEN + DEF_NUM_FILE_EXT_LEN + 2)

/* --------------------------------------------------------------------------
 * ファイルシステムに都合の悪い文字を変換する、8文字までで切る
 * "a1A!_6789?" → "a1A__78"
 * i/o : ioStr サニタイズする文字列
 * -------------------------------------------------------------------------- */
void sanitize(char *ioStr)
{
    int i;

    i = 0;

    /* 文字列が終わるか、8文字目までサニタイズしたら終わり */
    while ((i == DEF_NUM_FILE_NAME_LEN) || (ioStr[i] == '\0'))
    {
        if (isalnum(ioStr[i]) == 0)
        {
            /* サニタイズ先は '_' 固定 */
            ioStr[i] = '_';
        }
        i++;
    }

    /* 番兵 */
    ioStr[DEF_NUM_FILE_NAME_LEN] = '\0';
}

/* --------------------------------------------------------------------------
 * ヘッダ書き込み
 *
 * i/o : fp 書き込みファイルポインタ
 * -------------------------------------------------------------------------- */
void writeHeader(FILE *inFp)
{
    FILE *aRefFp;
    unsigned char aBuf;
    int i;

    /* ひな形のファイルから抜き出す */
    aRefFp = fopen("ref.wav", "r");
    for (i = 0; i < 0x2c; i++)
    {
        fread(&aBuf, 1, 1, aRefFp);
        fwrite(&aBuf, 1, 1, inFp);
    }
}

/* --------------------------------------------------------------------------
 * データ書き込み
 * in  : (なし)
 * out : (なし)
 * -------------------------------------------------------------------------- */
void writeData(FILE *inFp)
{
    int i;
    char aTest[147] = {0x00};
    for (i = 0; i < (120 + 257 * 8); i++)
    {
        fwrite(aTest, 147, 1, inFp);
    }
}

/* --------------------------------------------------------------------------
 * エントリポイント
 * in  : argv[1] 出力ファイル名(8文字まで)
 * out : (なし)
 * -------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    /* ファイル名8文字+拡張子3文字+null文字1文字 */
    char aWriteFileName[DEF_NUM_FILE_TOTAL_LEN];

    /* ファイルポインタ */
    FILE *aFp;

    /* 一時変数 */
    int _i;

    /* 出力ファイル指定 */
    if (argc != 2)
    {
        /* デフォルトファイル名適用 */
        sprintf(aWriteFileName, "%s.%s\0",
                DEF_STR_DEAULT_FILE_NAME,
                DEF_STR_OUTPUT_EXTENSION);
    }
    else
    {
        /* 文字列のサニタイジング */
        sanitize(argv[1]);

        /* ファイル名適用 */
        sprintf(aWriteFileName, "%s.%s\0",
                argv[1],
                DEF_STR_OUTPUT_EXTENSION);
    }

    /* debug */
    /* printf("%d %s\n", argc, aWriteFileName); */

    /* ファイルを開く */
    aFp = fopen(aWriteFileName, "w");

    /* ヘッダ書き込み */
    writeHeader(aFp);

    /* データ書き込み */
    writeData(aFp);

    /* ファイルを閉じる */
    fclose(aFp);

    return 0;
}