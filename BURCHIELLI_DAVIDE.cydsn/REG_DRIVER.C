/* ========================================

 * ========================================
*/
#include "REG_DRIVER.h"
#include "I2C_Interface.h"
#include "stdio.h"

#define LIS3DH_CTRL_REG1 0x20  //  Address of the Control register 1
#define LIS3DH_DEVICE_ADDRESS 0x18  // 7-bit I2C address of the slave device.

#define SENSITIVITY_G 1  // Sensitivity in mg/digit 
#define SENSITIVITY_MS2 SENSITIVITY_G * 0.00981  // Sensitivity in m/s^2

    int16 MultiplierFactor = 1000;
    uint8_t DataRateArray [DataRateArray_LENGTH] = {
                                                        0x10, // 0 0 0 1  0 0 0 0 --> 1 Hz
                                                        0x20, // 0 0 1 0  0 0 0 0 --> 10 Hz                                  
                                                        0x30, // 0 0 1 1  0 0 0 0 --> 25 Hz            
                                                        0x40, // 0 1 0 0  0 0 0 0 --> 50 Hz    
                                                        0x50, // 0 1 0 1  0 0 0 0 --> 100 Hz
                                                        0x60, // 0 1 1 0  0 0 0 0 --> 200 Hz
                                                    };
    char messag[50]= {'\0'};

void SearchCount (uint8_t eeprom_value)
    {
    for (uint8_t i=0; i< DataRateArray_LENGTH; i++)
        if (DataRateArray[i] == eeprom_value)
            count = i;
    }
    
ErrorCode SetRegister (uint8_t RegisterAddress, uint8_t RegisterValue)
    {
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                      RegisterAddress,
                                                      &reg_value);
        if (reg_value != RegisterValue)
        {
            reg_value = RegisterValue;
            error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                 RegisterAddress,
                                                 reg_value);
        }
        return error ? ERROR : NO_ERROR;
    }

void UpdateCTRL_REG1(uint8_t DataRateToUpdate)
    {

        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG1,
                                            &reg_value);  
       
        reg_value = ( ( reg_value & 0x0F ) | DataRateToUpdate );
        
        // Update the LIS3DH Accelerometer sampling frequency
        error = SetRegister(LIS3DH_CTRL_REG1, reg_value);            
        if (error == NO_ERROR)
            {
                sprintf(messag, "DATARATE UPDATING DONE; CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", reg_value);
                UART_Debug_PutString(messag); 
            }
            else
            {
                UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        } 

    }
        
int16 Convert (uint8_t AccValue_L, uint8_t AccValue_H)   
    {
        OutAcc = (int16) (AccValue_L | (AccValue_H << 8)) >> 4;
        OutAcc = OutAcc * SENSITIVITY_MS2;
        
        OutAcc = (int16) (OutAcc * MultiplierFactor);
        
        return OutAcc ; 
    }
/* [] END OF FILE */
