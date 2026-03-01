#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char uchar;

/* ------------------------------------------------------------------------- *
 * 定数宣言
 * ------------------------------------------------------------------------- */

#define DEF_CODE_SIZE (256)   /* ソースコードのデータ長 */
#define DEF_MEMORY_SIZE (256) /* メモリのデータ長 */
#define DEF_INPUT_SIZE (16)   /* 入力のデータ長 */
#define DEF_STACK_SIZE (4)    /* スタックのデータ長 */

/* ------------------------------------------------------------------------- *
 * 構造体宣言
 * ------------------------------------------------------------------------- */

/* 命令デコーダの出力構造体 */
struct STRdecode
{
    bool isInc;
    bool isDec;
    bool isFwd;
    bool isBak;
    bool isIn;
    bool isout;
    bool isPsh;
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

/* 入力領域 */
static uchar sInput[DEF_INPUT_SIZE];

/* スタック領域 */
static uchar sStack[DEF_STACK_SIZE];

/* プログラムカウンタ */
static uchar sPCreg;

/* アクティブアドレス */
static uchar sARreg;

/* 入力オフセット */
static uchar sInputOffset;

/* ------------------------------------------------------------------------- *
 * プロトタイプ宣言
 * ------------------------------------------------------------------------- */

void reset(void);

uchar get_code(uchar inPC);
bool get_zchk(uchar inMem);
struct STRdecode get_dec(uchar inCode, bool inZchk);
uchar get_PCU(uchar inST, uchar inInc, bool inJmp);
uchar get_ARU(uchar inAR, bool inFwd, bool inBak);
uchar get_ALUmul(uchar inInput, uchar inMem, bool inIn);
uchar get_arg(bool inInc, bool inDec);

void update_stack(uchar inPC, bool inPsh, bool inPop);

uchar inc(uchar inVal);
uchar add(uchar inVal1, uchar inVal2);

/* ------------------------------------------------------------------------- *
 * 初期化処理
 * メモリの初期化、ソースコード、入力データの読み取り
 * in   : (なし)
 * out  : (なし)
 * ------------------------------------------------------------------------- */
void reset(void)
{
    int i;
    int aInputSize;

    /* メモリのクリア */
    memset(sCode, (unsigned char)0x00, DEF_CODE_SIZE);
    memset(sMemory, (unsigned char)0x00, DEF_MEMORY_SIZE);
    memset(sInput, (unsigned char)0x00, DEF_INPUT_SIZE);

    /* レジスタのクリア */
    sPCreg = (unsigned char)0x00;
    sARreg = (unsigned char)0x00;
    memset(sStack, (unsigned char)0x00, DEF_STACK_SIZE);
    sInputOffset = (unsigned char)0x00;

    /* コードのロード */
    printf("input code: ");
    for (i = 0; i < DEF_CODE_SIZE; i++)
    {
        scanf("%c", &sCode[i]);
        if (sCode[i] == '\n')
        {
            sCode[i] = 0;
            break;
        }
    }

    /* 入力のロード */
    printf("input data size: ");
    scanf("%d", &aInputSize);

    printf("input data(hex): ");
    for (i = 0; i < aInputSize; i++)
    {
        scanf("%02x", &sInput[i]);
    }

    /* debug */
    /* printf("%s\n", sCode); */
    /* printf("%s\n", sInput); */
}

void wait_check(void)
{
    return;
}

void displayStatus(uchar inCode, struct STRdecode inDec, uchar inPCUinc,
                   uchar inPCUmul, uchar inARUmul, uchar inMul, uchar inZchk, uchar inALUadd)
{
    int i;

    system("cls");

    /* コードの表示 */
    printf("code:\n");
    printf("    ");
    for (i = 0; i < 16; i++)
    {
        if (sPCreg == i)
        {
            printf("v%01x ", i);
        }
        else
        {
            printf(" %01x ", i);
        }
    }
    putchar('\n');

    printf("00: ");
    for (i = 0; i < 16; i++)
    {
        printf(" %c ", sCode[i]);
    }
    putchar('\n');

    printf("10: ");
    for (i = 16; i < 32; i++)
    {
        printf(" %c ", sCode[i]);
    }
    putchar('\n');
    putchar('\n');

    /* 入力の表示 */
    printf("input:\n");
    printf("    ");
    for (i = 0; i < 16; i++)
    {
        if (sInputOffset == i)
        {
            printf("v%01x ", i);
        }
        else
        {
            printf(" %01x ", i);
        }
    }
    putchar('\n');

    printf("00: ");
    for (i = 0; i < 16; i++)
    {
        printf("%02x ", sInput[i]);
    }
    putchar('\n');
    putchar('\n');

    /*uchar inCode, struct STRdecode inDec, uchar inPCUinc,
    uchar inPCUmul, uchar inARUmul, uchar inMul, uchar inZchk, uchar inALUadd*/
}

/*
 * コードモジュールの出力を取得
 * in  : PCreg, ZCHK
 * out : CODE
 */
uchar get_code(uchar inPC)
{
    uchar aRet = sCode[inPC];
    return aRet;
}

/*
 * ZCHKの出力を取得
 * in  : MEM
 * out : ZCHK
 */
bool get_zchk(uchar inMem)
{
    bool aRet = (inMem == 0x00) ? true : false;
    return aRet;
}

/*
 * 各モジュールの制御信号を取得
 * in  : CODE
 * out : DEC
 */
struct STRdecode get_dec(uchar inCode, bool inZchk)
{
    struct STRdecode aRet;
    int i;
    uchar aCodeTbl[8] = {'+', '-', '>', '<', '?', '!', '[', ']'};
    struct STRdecode aDecTbl[8] =
        {
            {true, false, false, false, false, false, false, false, false},
            {false, true, false, false, false, false, false, false, false},
            {false, false, true, false, false, false, false, false, false},
            {false, false, false, true, false, false, false, false, false},
            {false, false, false, false, true, false, false, false, false},
            {false, false, false, false, false, true, false, false, false},
            {false, false, false, false, false, false, true, false, false},
            {false, false, false, false, false, false, false, true, true} /* Zflg=ON時 */
        };

    for (i = 0; i < 8; i++)
    {
        if (inCode == aCodeTbl[i])
        {
            memcpy(&aRet, &aDecTbl[i], sizeof(aRet));
            break;
        }
    }
    if (i == 7)
    {
        /* ']'の時の条件付きジャンプについて考慮 */
        aRet.isJmp = (inZchk == true) ? false : true;
    }

    return aRet;
}

/*
 * PCUのレジスタに直結している入力を取得する
 * in  : PCU_INC, S1reg
 * ctl : jmp
 * out : PCU_MUL
 */
uchar get_PCU(uchar inST, uchar inInc, bool inJmp)
{
    uchar aRet = (inJmp == true) ? inST : inInc;
    return aRet;
}

/*
 * ARUのレジスタに直結している入力を取得する
 * in  : ARreg
 * ctl : fwd, bak
 * out : ARU_MUL
 */
uchar get_ARU(uchar inAR, bool inFwd, bool inBak)
{
    uchar aRet;
    uchar aSel;

    if (inFwd == true)
    {
        aSel = 0x01;
    }
    else if (inBak == true)
    {
        aSel = 0xff;
    }
    else
    {
        aSel = 0x00;
    }

    aRet = add(inAR, aSel);

    return aRet;
}

/*
 * ALUのMUL出力を取得する
 * in  : input, MEM
 * ctl : in
 * out : ALU_MUL
 */
uchar get_ALUmul(uchar inInput, uchar inMem, bool inIn)
{
    uchar aRet = (inIn == true) ? inInput : inMem;
    return aRet;
}

/*
 * ALUの被加算数をフラグから算出する
 * in  : (なし)
 * ctl : inc, dec
 * out : ALU_ARG
 */
uchar get_arg(bool inInc, bool inDec)
{
    uchar aRet;

    if (inInc == true)
    {
        aRet = 0x01;
    }
    else if (inDec == true)
    {
        aRet = 0xff;
    }
    else
    {
        aRet = 0x00;
    }

    return aRet;
}

/*
 * スタックを更新する
 * in  : stack
 * ctl : psh, pop
 * out : stack
 */
void update_stack(uchar inPC, bool inPsh, bool inPop)
{
    if (inPsh == true)
    {
        sStack[3] = sStack[2];
        sStack[2] = sStack[1];
        sStack[1] = sStack[0];
        sStack[0] = inPC;
    }
    else if (inPop == true)
    {
        sStack[0] = sStack[1];
        sStack[1] = sStack[2];
        sStack[2] = sStack[3];
        sStack[3] = 0x00;
    }
    else
    {
        /* Do Nothing */
    }
}

/*
 * 8ビットの数値をインクリメントする(0xff → 0x00)
 */
uchar inc(uchar inVal)
{
    uchar aRet = (uchar)((unsigned short)((inVal + 1) & 0x00ff));
    return aRet;
}

/*
 * 8ビットの数値を加算する
 */
uchar add(uchar inVal1, uchar inVal2)
{
    uchar aRet = (uchar)((unsigned short)((inVal1 + inVal2) & 0x00ff));
    return aRet;
}

int main(void)
{
    uchar aPCout;
    uchar aARout;
    uchar aSTout;
    uchar aMemout;
    uchar aInout;

    struct STRdecode aDec;
    uchar aPCUmul;
    uchar aARUmul;
    uchar aALUadd;

    reset();

    while (1)
    {
        /* レジスタの出力を取得 */
        aPCout = sPCreg;
        aARout = sARreg;
        aSTout = sStack[0];
        aMemout = sMemory[sPCreg];
        aInout = sInput[sInputOffset];

        /* 組み合わせ回路の更新 */
        aDec = get_dec(get_code(aPCout), get_zchk(aMemout));                                    /* 前提情報を取得 */
        aPCUmul = get_PCU(aSTout, inc(aPCout), aDec.isJmp);                                     /* PCUの更新 */
        aARUmul = get_ARU(aARout, aDec.isFwd, aDec.isBak);                                      /* ARUの更新 */
        aALUadd = add(get_arg(aDec.isInc, aDec.isDec), get_ALUmul(aInout, aMemout, aDec.isIn)); /* ALUの更新 */

        /* 表示 */
        displayStatus(
            get_code(aPCout), /* aCode*/
            aDec,
            inc(aPCout), /* aPCUinc*/
            aPCUmul,
            aARUmul,
            get_ALUmul(aInout, aMemout, aDec.isIn), /* aMul */
            get_zchk(aMemout),                      /* aZchk*/
            aALUadd);

        /* モードにより待機 */
        (void)getchar();

        /* フリップフロップ更新 */
        sPCreg = aPCUmul;
        sARreg = aARUmul;
        sMemory[aPCout] = aALUadd;
        update_stack(aPCout, aDec.isPsh, aDec.isPop);
    }

    return 0;
}