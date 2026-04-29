/* --------------------------------------------------------------------------
 * カンサスシティスタンダード音声波形作成プログラム
 * 出力ファイル仕様
 * 拡張子:              wav
 * チャンネル数:        2ch
 * サンプリング周波数:  48,000Hz
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
 * i/o : inFp 書き込みファイルポインタ
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
 * bit0の書き込み
 * in  : inFp ファイルポインタ
 * out : (なし)
 * -------------------------------------------------------------------------- */
void writeBit(FILE *inFp, int inBit)
{
    const unsigned short aHigh = 0x7fff;
    const unsigned short aLow = 0x8001;
    int aLen;
    int aTim;
    int i;

    if (inBit == 0)
    {
        aLen = 20;
        aTim = 4;
    }
    else if (inBit == 1)
    {
        aLen = 10;
        aTim = 8;
    }
    else
    {
        aLen = 0;
        aTim = 0;
    }

    for (i = 0; i < aTim; i++)
    {
        fwrite(&aHigh, 2, aLen, inFp);
        fwrite(&aLow, 2, aLen, inFp);
    }
}

/* --------------------------------------------------------------------------
 * データエンコード
 * in  : inC エンコードする命令
 * out : outData 符号化した7ビット
 * -------------------------------------------------------------------------- */
void encode(char inC, int *outData)
{
    const char aCharTable[8] = {'+', '-', '>', '<', '?', '!', '[', ']'};
    const int aMatrixTable[8][7] = {
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 1, 0, 1, 0, 1},
        {0, 0, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 1, 1},
        {0, 1, 0, 1, 1, 0, 0},
        {0, 1, 1, 0, 1, 1, 0},
        {0, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 0}};
    int aTarget = 1;
    int i;

    for (i = 0; i < 8; i++)
    {
        if (inC == aCharTable[i])
        {
            aTarget = i + 1;
            break;
        }
    }

    memcpy(outData, aMatrixTable[aTarget], sizeof(int) * 7);
}

/* --------------------------------------------------------------------------
 * データ書き込み
 * in  : (なし)
 * out : (なし)
 * -------------------------------------------------------------------------- */
void writeData(FILE *inFp)
{
    char inC;
    int aData[7];
    int i;
    int j;

    /* 120bitのbit0 */
    for (i = 0; i < 120; i++)
    {
        writeBit(inFp, 0);
    }

    /* 本データ256バイト */
    for (i = 0; i < 256; i++)
    {
        /* 入力データ取得 */
        scanf("%c", &inC);

        /* 符号化 */
        encode(inC, aData);

        for (j = 0; j < 7; j++)
        {
            writeBit(inFp, aData[j]);
        }
        writeBit(inFp, 0);
    }

    /* 終了記号 */
    for (i = 0; i < 8; i++)
    {
        writeBit(inFp, 0);
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