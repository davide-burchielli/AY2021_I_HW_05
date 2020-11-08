/* ========================================

 * ========================================
*/
#include "CTRL_REG1_DRIVER.h"
#include "I2C_Interface.h"
#include "stdio.h"

#define LIS3DH_CTRL_REG1 0x20  //  Address of the Control register 1
#define LIS3DH_DEVICE_ADDRESS 0x18  // 7-bit I2C address of the slave device.


void SearchCount (uint8_t eeprom_value)
{
    for (uint8_t i=0; i< DataRateArray_LENGTH; i++)
        if (DataRateArray[i] == eeprom_value)
            count = i;
}

void UpdateCTRL_REG1(uint8_t DataRateToUpdate)
    {
        uint8_t ctrl_reg1_value;

        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG1,
                                            &ctrl_reg1_value);  
       
        ctrl_reg1_value = ( ( ctrl_reg1_value & 0x0F ) | DataRateToUpdate );
        
        // Update the LIS3DH Accelerometer sampling frequency
        error = WriteRegister(LIS3DH_CTRL_REG1, ctrl_reg1_value);            
        if (error == NO_ERROR)
            {
                sprintf(message, "DATARATE UPDATING DONE; CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1_value);
                UART_Debug_PutString(message); 
            }
            else
            {
                UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        } 

    }
/* [] END OF FILE */
