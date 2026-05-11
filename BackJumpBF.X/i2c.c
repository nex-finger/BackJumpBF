/* @file i2c.c
 *
 * SCL: RD3
 * SDA: RD2
 */

#include "mcc_generated_files/system/system.h"
#include "i2c.h"

/* i2c通信線の初期化
 * SDAを入力モードにする(外部でプルアップ抵抗)
 * かならずSCL, SDAはLowになる */
void I2C_Init(void)
{
    I2C_SCLhigh();
    I2C_SDAhigh();

    I2C_StartCondition();
    I2C_StopCondition();

    /* テストプログラム ----> */
    I2C_setValue(0x07, 0x00, 0x00);
    I2C_setValue(0x07, 0x01, 0x00);
    /* <---- テストプログラム */
}

/* SCLをHiにする
 * プルアップの影響で入力モードにするとHiになる */
void I2C_SCLhigh(void)
{
    TRISD = TRISD | 0b00001000;
}

/* SCLをLowにする
 * 出力モードでLowを出力する */
void I2C_SCLlow(void)
{
    IO_RD3_SetLow();
    TRISD = TRISD & 0b11110111;
}

/* SDAをHiにする
 * プルアップの影響で入力モードにするとHiになる */
void I2C_SDAhigh(void)
{
    TRISD = TRISD | 0b00000100;
}

/* SDAをLowにする
 * 出力モードでLowを出力する */
void I2C_SDAlow(void)
{
    IO_RD2_SetLow();
    TRISD = TRISD & 0b11111011;
}

/* STARTコンディションを送信する
 * SCLのHi中にSDAを立ち下げる
 * SCLはHi前提 → 必ずLowになる
 * SDAはHi前提 → 必ずLowになる
 * SCL ─────┐
 *          └──
 * SDA ──┐
 *       └───── */
void I2C_StartCondition(void)
{
    __delay_us(I2C_TICK);
    I2C_SDAlow();
    __delay_us(I2C_TICK);
    I2C_SCLlow();
}

/* REPEATED STARTコンディションを送信する
 * STOPコンディションにならないよう再度STARTコンディションを送信する
 * SCLはLow前提 → 必ずLowになる
 * SDAは不定 → 必ずLowになる
 * SCL      ┌─────┐
 *     ─────┘     └──
 * SDA ──┬─────┐
 *     ──┘     └───── */
void I2C_RepeatedStartCondition(void)
{
    __delay_us(I2C_TICK);
    I2C_SDAhigh();
    __delay_us(I2C_TICK);
    I2C_SCLhigh();
    __delay_us(I2C_TICK);
    I2C_SDAlow();
    __delay_us(I2C_TICK);
    I2C_SCLlow();
}

/* STOPコンディションを送信する
 * SCLのhI中にSDAを立ち上げる
 * SCLはLow前提 → 必ずHiになる
 * SDAは不定 → 必ずHiになる
 * SCL      ┌─────
 *     ─────┘
 * SDA ──┐     ┌──
 *     ──┴─────┘   */
void I2C_StopCondition(void)
{
    __delay_us(I2C_TICK);
    I2C_SDAlow();
    __delay_us(I2C_TICK);
    I2C_SCLhigh();
    __delay_us(I2C_TICK);
    I2C_SDAhigh();
}

/* ビット書き込み
 * SCLはLow前提 → 必ずLowになる
 * SDAは不定 → 不定になる
 * SCL      ┌──┐
 *     ─────┘  └──
 * SDA ──┬────────
 *     ──┴──────── */
void I2C_bitWrite(unsigned char in)
{
    __delay_us(I2C_TICK);

    /* 入力に応じSDAを操作する */
    if (in == 0)
    {
        I2C_SDAlow();
    }
    else
    {
        I2C_SDAhigh();
    }

    __delay_us(I2C_TICK);
    I2C_SCLhigh();
    __delay_us(I2C_TICK * 2);
    I2C_SCLlow();
}

/* ビット読み取り
 * SCLはLow前提 → 必ずLowになる
 * SDAは不定 → 不定になる
 * SCL      ┌──┐
 *     ─────┘  └──
 * SDA ──┬────────
 *     ──┘         */
unsigned char I2C_bitRead(void)
{
    unsigned char aRet;

    __delay_us(I2C_TICK);
    I2C_SDAhigh(); /* 入力モードにしてクライアントからの入力を待つ */
    __delay_us(I2C_TICK);
    I2C_SCLhigh();
    __delay_us(I2C_TICK);
    aRet = IO_RD2_GetValue(); /* 入力ポートから取得 */
    __delay_us(I2C_TICK);
    I2C_SCLlow();

    return aRet;
}

/* ホストからクライアントへの1バイト送信
 * SCLはLow前提 → 必ずLowになる
 * SDAは不定 → 不定になる
 * aRet: Ack or Nack */
unsigned char I2C_byteWrite(unsigned char inData)
{
    /* 戻り値はクライアントからのAck */
    unsigned char aRet;
    unsigned char aBit;
    int i;

    /* 8ビットのデータを送信 */
    for (i = 0; i < 8; i++)
    {
        if ((unsigned char)(inData & 0x80) == 0x80)
        {
            aBit = 1;
        }
        else
        {
            aBit = 0;
        }
        I2C_bitWrite(aBit);
        inData *= 2;
    }

    /* 1ビットのAckを受信 */
    aRet = I2C_bitRead();

    return aRet;
}

/* ホストからクライアントへの1バイト送信
 * SCLはLow前提 → 必ずLowになる
 * SDAは不定 → 不定になる(Ackの値)
 * aRet: Ack or Nack */
unsigned char I2C_byteRead(unsigned char inAck)
{
    /* 戻り値はクライアントからのデータ */
    unsigned char aRet = 0;
    int i;

    /* 8ビットのデータを送信 */
    for (i = 0; i < 8; i++)
    {
        aRet *= 2;
        aRet += I2C_bitRead();
    }

    /* 1ビットのAckを受信 */
    I2C_bitWrite(inAck);

    return aRet;
}

/* アドレス → i2cに流すオペコードへの変換
 * in  inDevice: 物理デバイス番号
 *     inRW: R/Wビット
 * ret aRet: 通信電文 */
unsigned char I2C_DtoO(unsigned char inDevice, unsigned char inRW)
{
    /* bit7  6  5  4  3  2  1  0 */
    /*    0  1  0  0  A2 A1 A0 0*/
    unsigned char aRet = 0b01000000 | (inDevice * 2) | inRW;
    return aRet;
}

/* 1バイト書き込み
 * 指定のデバイス、アドレスに任意のデータを更新する
 * | host1| --- Start --> |client|
 * |     2| -- Opecode -> |      |
 * |     3| --- Wbit ---> |      |
 * |     4| <--- Ack ---- |      |
 * |     5| -- RegAddr -> |      |
 * |     6| <--- Ack ---- |      |
 * |     7| --- Wdata --> |      |
 * |     8| <--- Nack --- |      |
 * |     9| --- Stop ---> |      |
 * in  inDevice: 書き込み先の物理デバイス番号
 *     inAddr: 書き込み先のアドレス
 *     inData: 書き込むデータ
 * ret aAck: 書き込み先のAck, Nack */
unsigned char I2C_setValue(unsigned char inDevice, unsigned char inAddr, unsigned char inData)
{
    /* クライアントからのACKが1回でもかけていればNACK */
    unsigned char aAck = I2C_ACK;
    /* アドレス → オペコードの変換 */
    unsigned char aOpecode = I2C_DtoO(inDevice, I2C_WBIT);

    I2C_StartCondition();
    aAck |= I2C_byteWrite(aOpecode);
    aAck |= I2C_byteWrite(inAddr);
    aAck |= I2C_byteWrite(inData);
    I2C_StopCondition();

    return aAck;
}

/* 1バイト読み取り
 * 指定のデバイス、アドレスのデータを取得する
 * | host1| --- Start --> |client|
 * |     2| -- Opecode -> |      |
 * |     3| --- Wbit ---> |      |
 * |     4| <--- Ack ---- |      |
 * |     5| -- Restart -> |      |
 * |     6| -- Opecode -> |      |
 * |     7| --- Rbit ---> |      |
 * |     8| <--- Ack ---- |      |
 * |     9| <-- Rdata --- |      |
 * |    10| --- Nack ---> |      |
 * |    11| --- Stop ---> |      |
 * in  inDevice: 読み取り先の物理デバイス番号
 *     inAddr: 読み取り先のアドレス
 *     inAck: 読み取り先に応答するAck, Nack
 * ret aData: 読み取り先から取得したデータ */
unsigned char I2C_getValue(unsigned char inDevice, unsigned char inAddr, unsigned char inAck)
{
    unsigned char aData;
    return aData;
}
