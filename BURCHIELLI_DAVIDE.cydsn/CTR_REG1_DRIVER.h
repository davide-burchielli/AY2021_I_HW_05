/* ========================================

 * ========================================
*/
#ifndef _CTR_REG1_DRIVER
    #define _CTR_REG1_DRIVER
    
    #include "project.h"
    
    uint8_t DataRateArray [6] = {
                                    0x1F, // 0 0 0 1  1 1 1 1 --> 1 Hz
                                    0x2F, // 0 0 1 0  1 1 1 1 --> 10 Hz                                  
                                    0x3F, // 0 0 1 1  1 1 1 1 --> 25 Hz            
                                    0x4F, // 0 1 0 0  1 1 1 1 --> 50 Hz    
                                    0x5F, // 0 1 0 1  1 1 1 1 --> 100 Hz
                                    0x6F, // 0 1 1 0  1 1 1 1 --> 200 Hz
                                };
    
    void UpdateCTRL_REG1(uint8_t, uint8_t);
    
#endif




/* [] END OF FILE */
