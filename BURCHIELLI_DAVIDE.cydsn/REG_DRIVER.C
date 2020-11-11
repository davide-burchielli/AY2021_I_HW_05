/* ========================================

 * ========================================
*/
#include "REG_DRIVER.h"
#include "I2C_Interface.h"
#include "stdio.h"

#define LIS3DH_CTRL_REG1 0x20  //  Address of the Control register 1
#define LIS3DH_DEVICE_ADDRESS 0x18  // 7-bit I2C address of the slave device.

#define SENSITIVITY_G 1  // Sensitivity in mg/digit 
#define SENSITIVITY_MS2 (SENSITIVITY_G * 9.806)/1024 // Sensitivity in m/s^2

#define EEPROM_STARTUP_REGISTER 0x0000 // Define the Startup register address

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
    uint8_t k = 0;
    uint8_t j = 0;
    int16 AccValuesConverted[3];
    char messaggio[50] = {'\0'};
    

void SearchCount (uint8_t eeprom_value)
    {
    for (k=0; k< DataRateArray_LENGTH; k++)
        if (DataRateArray[k] == eeprom_value)
            count = k;
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
                sprintf(messag, "\n DATARATE UPDATING DONE; CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", reg_value);
                UART_Debug_PutString(messag); 
            }
            else
            {
                UART_Debug_PutString("\nError occurred during I2C comm to set control register 1\r\n");   
        } 

    }
        
void ConvertAcc (uint8_t* AccelerationValues)
{
    k=0;
    j=0;
    while (k<6)
    {
        AccValuesConverted[j] = (int16) (AccelerationValues[k] | (AccelerationValues[k+1] << 8)) >> 4;
        OutAccconv = (AccValuesConverted[j] * SENSITIVITY_MS2);
        AccValuesConverted[j] = (int16) (OutAccconv * MultiplierFactor);
        
        DataBuffer[k+1] = (uint8_t)(AccValuesConverted[j]& 0xFF);
        DataBuffer[k+2] = (uint8_t)(AccValuesConverted[j] >> 8); 
        k+=2;
        j++;
    }
}


void UpdateEEPROM()
{                      
  EEPROM_UpdateTemperature();    
  EEPROM_WriteByte (DataRateArray[count] , EEPROM_STARTUP_REGISTER);
}



/* [] END OF FILE */
