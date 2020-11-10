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

    
   
    void SearchCount (uint8_t eeprom_value);    
    void UpdateCTRL_REG1(uint8_t DataRateToUpdate);

    ErrorCode SetRegister (uint8_t RegisterAddress, uint8_t RegisterValue);
    int16 Convert (uint8_t AccValue_L, uint8_t AccValue_H);
    
    uint8_t count; 
    uint8_t reg_value;
    int16 OutAcc;
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];  // DataBuffer array contains the bytes to send.
    _Bool ButtonFlag;
    
#endif




/* [] END OF FILE */
