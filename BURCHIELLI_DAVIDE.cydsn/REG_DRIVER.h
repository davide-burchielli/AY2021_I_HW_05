/* ========================================

 * ========================================
*/
#ifndef _REG_DRIVER_H
    #define _REG_DRIVER_H
    
    #include "project.h"
    #include "ErrorCodes.h"
    
    #define DataRateArray_LENGTH 6
    #define BYTE_TO_SEND 6   // Macro that identifies the number of bytes to send
    #define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1  // Macro that identifies the number of bytes to send including the HEADER and TAIL byte.
                                                   // It identifies the dimention of the array DataBuffer.

    
   
    void SearchDRindex (uint8_t eeprom_value);    
    void UpdateCTRL_REG1(uint8_t DataRateToUpdate);

    ErrorCode SetRegister (uint8_t RegisterAddress, uint8_t RegisterValue);
    void ConvertAcc (uint8_t*);
    void UpdateEEPROM();
    
    uint8_t DRindex; 
    uint8_t reg_value;
    float OutAccconv;
    uint8_t eeprom_value;
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];  // DataBuffer array contains the bytes to send.
    volatile _Bool ButtonFlag;
    
#endif




/* [] END OF FILE */
