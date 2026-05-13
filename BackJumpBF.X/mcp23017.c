/* @file mcp23017.c
 * mcp23017制御プログラム */

#include "i2c.h"
#include "mcp23017.h"

/* 左桁の表示テーブル */
const unsigned char cUpperTable[16][2] = {
    {0b11100000, 0b00001101}, // 0
    {0b00100000, 0b00001000}, // 1
    {0b11000000, 0b00001110}, // 2
    {0b01100000, 0b00001110}, // 3
    {0b00100000, 0b00001011}, // 4
    {0b01100000, 0b00000111}, // 5
    {0b11100000, 0b00000111}, // 6
    {0b00100000, 0b00001100}, // 7
    {0b11100000, 0b00001111}, // 8
    {0b01100000, 0b00001111}, // 9
    {0b10100000, 0b00001111}, // A
    {0b11100000, 0b00000011}, // b
    {0b11000000, 0b00000010}, // c
    {0b11100000, 0b00001010}, // d
    {0b11000000, 0b00000111}, // E
    {0b10000000, 0b00000111}  // F
};

/* 右桁の表示テーブル */
const unsigned char cLowerTable[16][2] = {
    {0b00001101, 0b01110000}, // 0
    {0b00000001, 0b01000000}, // 1
    {0b00001110, 0b01100000}, // 2
    {0b00000111, 0b01100000}, // 3
    {0b00000011, 0b01010000}, // 4
    {0b00000111, 0b00110000}, // 5
    {0b00001111, 0b00110000}, // 6
    {0b00000001, 0b01100000}, // 7
    {0b00001111, 0b01110000}, // 8
    {0b00000111, 0b01110000}, // 9
    {0b00001011, 0b01110000}, // A
    {0b00001111, 0b00010000}, // b
    {0b00001110, 0b00000000}, // c
    {0b00001111, 0b01000000}, // d
    {0b00001110, 0b00110000}, // E
    {0b00001010, 0b00110000}  // F
};

/* 小数点の表示テーブル */
const unsigned char cDotTable[2][2] = {
    {0b00000000, 0b00000000}, // OFF
    {0b00010000, 0b10000000}  // ON
};

/* 8ビットデータ → 2バイトled点灯データ
 *  --- s2 ---        --- s5 ---
 * |          |      |          |
 * s          s      s          s
 * 0          3      4          6
 * |          |      |          |
 *  --- s1 ---        --- f1 ---
 * |          |      |          |
 * f          f      f          f
 * 7          5      3          0
 * |          |      |          |
 *  --- f6 ---  (f4)  --- f2 ---  (s7)
 * in  inData: 8ビットデータ
 *     inDot: 点滅状態
 * out outData: 16ビットデータ */
void mcp23017_translate(unsigned char *outData, unsigned char inData, unsigned char inDot)
{
    unsigned char aOffsetUpper = inData / 16;
    unsigned char aOffsetLower = inData % 16;

    /* テーブルから表示データを適用 */
    outData[0] = cUpperTable[aOffsetUpper][0] | cLowerTable[aOffsetLower][0] | cDotTable[inDot][0];
    outData[1] = cUpperTable[aOffsetUpper][1] | cLowerTable[aOffsetLower][1] | cDotTable[inDot][1];
}

/* 1バイトデータ表示
 * 指定のデバイスに16進2桁のデータを反映する
 * in  inDevice: 書き込み先の物理デバイス番号
 *     inData: 書き込むデータ */
void mcp23017_set(unsigned char inDevice, unsigned char inData)
{
    /* 点滅状態 */
    static unsigned char sDotStatus[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    unsigned char aData[2];

    /* 点滅状態更新 */
    sDotStatus[inDevice] = 1 - sDotStatus[inDevice];

    /* データ → 表示パターン変換 */
    mcp23017_translate(aData, inData, sDotStatus[inDevice]);

    /* 表示 */
    I2C_setValue(inDevice, 0x12, aData[0]);
    I2C_setValue(inDevice, 0x13, aData[1]);
}
