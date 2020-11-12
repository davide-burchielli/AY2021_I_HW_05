/* =========================================================================
    author: Davide Burchielli

    This file declares the functions and the variables used in the files.
 * =========================================================================
*/
#ifndef _REG_DRIVER_H
    #define _REG_DRIVER_H
    
    #include "project.h"
    #include "ErrorCodes.h"
    
    #define DataRateArray_LENGTH 6  // Macro that identifies the length of DataRateArray array
    #define BYTE_TO_SEND 6          // Macro that identifies the number of bytes to send
    #define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1  // Macro that identifies the number of bytes to send including the HEADER and TAIL byte.
                                                   // It identifies the dimention of the array DataBuffer.

    
    // Declares the functions:
    void SearchDRindex (uint8_t eeprom_value);    
    ErrorCode SetRegister (uint8_t RegisterAddress, uint8_t RegisterValue);
    void UpdateCTRL_REG1(uint8_t DataRateToUpdate);
    void ConvertAcc (uint8_t*);
    void UpdateEEPROM();
    
    // Declares the variable:
    uint8_t DRindex;        // DRindex variable is used to keep track of the current DataRateArray index
    uint8_t reg_value;      // reg_value varaible is used to memorize a register value
    float OutAccconv;       // OutAccconv is used during acceleration values convertions
    uint8_t eeprom_value;   // eeprom_value varaible is used to memorize eeprom_value value
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];  // DataBuffer array contains the bytes to send.
    volatile _Bool ButtonFlag;  //
    
#endif




/* [] END OF FILE */
