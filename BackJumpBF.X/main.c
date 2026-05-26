/*
 * MAIN Generated Driver File
 *
 * @file main.c
 *
 * @defgroup main MAIN
 *
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
 */

/*
? [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip
    software and any derivatives exclusively with Microchip products.
    You are responsible for complying with 3rd party license terms
    applicable to your use of 3rd party software (including open source
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.?
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "command.h"  /* コマンド関連 */
#include "i2c.h"      /* i2c通信関連 */
#include "mcp23017.h" /* mcp23017と通信するためにi2cを組み合わせたパッケージ */

/* 改行コード確認フラグ */
extern unsigned char gEnterFlag;

/*
    Main application
*/

void init_7seg(void);

int main(void)
{
    unsigned char i;
    unsigned char rx;

    unsigned long _;

    SYSTEM_Initialize();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

    I2C_Init();

    com_init();
    init_7seg();

    while (1)
    {
        i = 0;
        _ = 0;
        while (1)
        {
            /*
            if (EUSART_IsRxReady())
            {
                rx = EUSART_Read();

                while (!EUSART_IsTxReady())
                {
                    ;
                }

                EUSART_Write(rx);
            } */
            // printf("%lu ", _);
            //(void)I2C_setValue(0x07, 0x13, 0x40);
            //__delay_ms(50);
            //(void)I2C_setValue(0x07, 0x13, 0x00);
            //__delay_ms(50);

            if (gEnterFlag)
            {
                com_response();
            }

            mcp23017_set(0x07, i);
            __delay_ms(100);

            i++;
            _++;
        }

        //(void)I2C_setValue(0x07, 0x12, 0xff);
        //__delay_ms(1000);
        // IO_RC0_Toggle();
    }
}

/* 起動時の7セグ表示 */
void init_7seg()
{
    int i;
    int j;

    for (i = 0; i < 256; i++)
    {
        for (j = 3; j <= 7; j++)
        {
            mcp23017_set((unsigned char)j, (unsigned char)i);
        }
        __delay_ms(20);
    }
}
