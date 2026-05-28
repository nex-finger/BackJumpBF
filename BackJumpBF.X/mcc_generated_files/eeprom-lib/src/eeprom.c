/**
 * EEPROM generated driver source file.
 *
 * @file eeprom.c
 * 
 * @ingroup eeprom
 * 
 * @brief Contains API definitions for the EEPROM driver.
 *
 * @version EEPROM Driver Version 1.0.1
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

#include "../../eeprom-lib/eeprom.h"
#include "../../eeprom-lib/eeprom_interface.h"
#include "../../nvm/nvm.h"
#include <stdlib.h>

const EEPROM_INTERFACE_t eeprom_interface = {
    .SequentialWrite = NULL,
    .SequentialRead = NULL,
    .PageWrite = NULL,
    .PageRead = NULL,
    .ByteWrite = EEPROM_ByteWrite,
    .ByteRead = EEPROM_ByteRead,
    .IsBusy = NVM_IsBusy,
};

bool EEPROM_ByteWrite(uint32_t address, uint8_t *data)
{
    uint8_t getData = *data;
    eeprom_address_t writeAddress = (eeprom_address_t) (address);
    NVM_UnlockKeySet(UNLOCK_KEY);
    EEPROM_Write(writeAddress, getData);
    NVM_UnlockKeyClear();
    while (NVM_IsBusy());    
    if (NVM_StatusGet() == NVM_OK)
    {
        return true;
    }
    else
    {
        NVM_StatusClear();
        return false;
    }
}

bool EEPROM_ByteRead(uint32_t address, uint8_t *data)
{
    eeprom_address_t readAddress = (eeprom_address_t) (address);
    *data = EEPROM_Read(readAddress);
    return true;
}
