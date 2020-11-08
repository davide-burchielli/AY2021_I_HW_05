/* ========================================

 * ========================================
*/
#ifndef _CTR_REG1_DRIVER
    #define _CTR_REG1_DRIVER
    
    #include "project.h"
    #define DataRateArray_LENGTH 6
    
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
    
    uint8_t count = 0; 
    
#endif




/* [] END OF FILE */
