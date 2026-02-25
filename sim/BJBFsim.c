#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char uchar;

/* ------------------------------------------------------------------------- *
 * 定数宣言
 * ------------------------------------------------------------------------- */

#define DEF_CODE_SIZE (256)   /* ソースコードのデータ長 */
#define DEF_MEMORY_SIZE (256) /* メモリのデータ長 */
#define DEF_STACK_SIZE (4)    /* スタックのデータ長 */

/* ------------------------------------------------------------------------- *
 * 構造体宣言
 * ------------------------------------------------------------------------- */

/* 命令デコーダの出力構造体 */
struct STRdecode
{
    bool isInc;
    bool isDec;
    bool isForword;
    bool isBack;
    bool isIn;
    bool isout;
    bool isPush;
    bool isPop;
    bool isJmp;
};

/* ------------------------------------------------------------------------- *
 * static変数宣言
 * ------------------------------------------------------------------------- */

/* コード領域 */
static uchar sCode[DEF_CODE_SIZE];

/* メモリ領域 */
static uchar sMemory[DEF_MEMORY_SIZE];

/* スタック領域 */
static uchar sStack[DEF_STACK_SIZE];

/* プログラムカウンタ */
static uchar sPCreg;

/* アクティブアドレス */
static uchar sARreg;

/* ------------------------------------------------------------------------- *
 * プロトタイプ宣言
 * ------------------------------------------------------------------------- */

uchar get_input(void);
uchar get_code(uchar inPC);
uchar get_mem(uchar inAR);
uchar get_flg(uchar inMem);
struct STRdecode get_decoder(uchar inCode);
uchar get_ALU(uchar inMem, bool inIncF, bool inDecF);
uchar get_Stack(void);
uchar get_PCU(uchar inPC, uchar inStack, bool inJmpF);

void wait_check(void);

/* ------------------------------------------------------------------------- *
 * 初期化処理
 * メモリの初期化、ソースコード、入力データの読み取り
 * in   : (なし)
 * out  : (なし)
 * ------------------------------------------------------------------------- */
void init(void)
{
    int i;

    memset(sCode, (unsigned char)0x00, DEF_CODE_SIZE);
    memset(sMemory, (unsigned char)0x00, DEF_MEMORY_SIZE);
    memset(sStack, (unsigned char)0x00, DEF_STACK_SIZE);

    sPCreg = (unsigned char)0x00;
    sARreg = (unsigned char)0x00;
}

void wait_check(void)
{
    return;
}

int main(void)
{
    uchar aInputOut;          /* 入力モジュールから取得した値 */
    uchar aDiskOut;           /* ROMから取得した値 */
    uchar aMemOut;            /* RAMから取得した値 */
    uchar aZeroFlg;           /* ゼロフラグの値 */
    bool aZflg;               /* フラグの値(条件付きジャンプに使用) */
    struct STRdecode aDecOut; /* 命令デコーダの出力 */
    uchar aALUOut;            /* ALUの出力 */
    uchar aStackOut;          /* スタック一番手前の出力 */
    uchar aPCUOut;            /* PCU(ProgramCounterUnit)の出力 */

    init();

    while (1)
    {
        /* データロード */
        aInputOut = get_input();
        aDiskOut = get_code(sPCreg);
        aMemOut = get_mem(sARreg);
        aZflg = get_flg(aMemOut);
        aDecOut = get_decoder(aDiskOut);
        aALUOut = get_ALU(aMemOut, aDecOut.isInc, aDecOut.isDec);
        aStackOut = get_Stack();
        aPCUOut = get_PCU(sPCreg, aStackOut, aDecOut.isJmp);

        /* モードにより待機 */
        wait_check();

        /* フリップフロップ更新 */
    }

    return 0;
}