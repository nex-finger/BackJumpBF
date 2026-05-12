/* @file i2c.h
 * i2c関連
 * 詳細はMCP23017のデータシートを参照すること */

#ifndef I2C_H
#define I2C_H

/* 通信速度確保のためのウェイト(us) */
#define I2C_TICK 1

#define I2C_ACK 0
#define I2C_NACK 1

#define I2C_WBIT 0
#define I2C_RBIT 1

/* デバイスID */
#define I2C_DEVICE_ID_IN_PC_AR 0
#define I2C_DEVICE_ID_IN_ALU_ARU 1
#define I2C_DEVICE_ID_OUT_PC 2
#define I2C_DEVICE_ID_OUT_AR 3
#define I2C_DEVICE_ID_OUT_MEM 4
#define I2C_DEVICE_ID_OUT_IN 5
#define I2C_DEVICE_ID_OUT_OUT 6
#define I2C_DEVICE_ID_OUT_CODE 7

/* 初期化 */
void I2C_Init(void);

/* 電圧レベル制御 */
void I2C_SCLhigh(void);
void I2C_SCLlow(void);

void I2C_SDAhigh(void);
void I2C_SDAlow(void);

/* コンディション送信 */
void I2C_StartCondition(void);
void I2C_RepeatedStartCondition(void);
void I2C_StopCondition(void);

/* ビット単位読み書き */
void I2C_bitWrite(unsigned char in);
unsigned char I2C_bitRead(void);

/* バイト単位読み書き */
unsigned char I2C_byteWrite(unsigned char inData);
unsigned char I2C_byteRead(unsigned char inAck);

/* データ読み書き */
unsigned char I2C_setValue(unsigned char inDevice, unsigned char inAddr, unsigned char inData);
unsigned char I2C_getValue(unsigned char inDevice, unsigned char inAddr, unsigned char inAck);

/* 変換関数 */
unsigned char I2C_DtoO(unsigned char inAddr, unsigned char inRW);

#endif /* I2C_H */
