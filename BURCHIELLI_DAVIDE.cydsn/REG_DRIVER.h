/* ========================================

 * ========================================
*/
#ifndef _REG_DRIVER
    #define _REG_DRIVER
    
    #include "project.h"
    #include "ErrorCodes.h"
    
    #define DataRateArray_LENGTH 6
    #define BYTE_TO_SEND 6   // Macro that identifies the number of bytes to send
    #define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1  // Macro that identifies the number of bytes to send including the HEADER and TAIL byte.
                                                   // It identifies the dimention of the array DataBuffer.

    
    uint8_t DataRateArray [DataRateArray_LENGTH] = {
                                                        0x10, // 0 0 0 1  0 0 0 0 --> 1 Hz
                                                        0x20, // 0 0 1 0  0 0 0 0 --> 10 Hz                                  
                                                        0x30, // 0 0 1 1  0 0 0 0 --> 25 Hz            
                                                        0x40, // 0 1 0 0  0 0 0 0 --> 50 Hz    
                                                        0x50, // 0 1 0 1  0 0 0 0 --> 100 Hz
                                                        0x60, // 0 1 1 0  0 0 0 0 --> 200 Hz
                                                    };
    
    void UpdateCTRL_REG1(uint8_t DataRateToUpdate);
    void SearchCount (uint8_t eeprom_value);
    ErrorCode WriteRegister (uint8_t RegisterAddress, uint8_t RegisterValue);
    int16 Convert (uint8_t AccValue_L, uint8_t AccValue_H);
    
    uint8_t count = 0; 
    uint8_t reg_value;
    int16 OutAcc;
    int16 MultiplierFactor = 1000;
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];  // DataBuffer array contains the bytes to send.
    
#endif




/* [] END OF FILE */
