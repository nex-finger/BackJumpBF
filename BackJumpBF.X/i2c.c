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

/* 1バイト書き込み
 * | host1| --- Start --> |client|
 * |     2| -- Opecode -> |      |
 * |     3| --- Wbit ---> |      |
 * |     4| <--- Ack ---- |      |
 * |     5| -- RegAddr -> |      |
 * |     6| <--- Ack ---- |      |
 * |     7| --- Wdata --> |      |
 * |     8| <--- Nack --- |      |
 * |     9| --- Stop ---> |      | */

/* 1バイト読み込み
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
 * |    11| --- Stop ---> |      | */
