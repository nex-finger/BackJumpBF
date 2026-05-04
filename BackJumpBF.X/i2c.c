/* @file i2c.c
 *
 * SCL: RD3
 * SDA: RD2
 */

#include "mcc_generated_files/system/system.h"
#include "i2c.h"

/* i2c通信線の初期化
 * SDAを入力モードにする(外部でプルアップ抵抗) */
void I2C_Init(void)
{
    I2C_SDAhigh();
}

/* SDAをHiにする
 * プルアップの影響で入力モードにするとHiになる */
void I2C_SDAhigh(void)
{
    TRISC = TRISC | 0b00000010;
}
