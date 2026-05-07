/* i2c関連 */

#ifndef I2C_H
#define I2C_H

#define I2C_TICK 6

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

/* バイト単位読み書き */
void I2C_byteWrite(unsigned char in);
unsigned char I2C_byteRead(void);

/* 1バイト取得 */

#endif /* I2C_H */
