/* ========================================

 * ========================================
*/
#include "CTR_REG1_DRIVER.h"


void UpdateCTRL_REG1(uint8_t DataRateToUpdate, uint8_t ctrl_reg1)
    {
        ctrl_reg1 &= DataRateToUpdate;
    // Update the LIS3DH Accelerometer sampling frequency
    
    UART_Debug_PutString("\r\nUpdating the  CONTROL REGISTER 1:..\r\n");
    
    if (ctrl_reg1 != LIS3DH_CTRL_REG1_VALUE)
    {
        ctrl_reg1 = LIS3DH_CTRL_REG1_VALUE;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }            


    }
/* [] END OF FILE */
