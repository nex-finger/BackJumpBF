/* i2c関連 */

#ifndef I2C_H
#define I2C_H

#define I2C_TICK 6

#define I2C_ACK 0
#define I2C_NACK 1

#define I2C_WBIT 0
#define I2C_RBIT 1

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
