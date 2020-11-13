/* ==================================================================
    author: Davide Burchielli

    This file defines all the functions called by the main.c file. 
 * ==================================================================
*/

#include "REG_DRIVER.h"
#include "I2C_Interface.h"
#include "stdio.h"

#define LIS3DH_CTRL_REG1 0x20          //  Address of the Control register 1
#define LIS3DH_DEVICE_ADDRESS 0x18     // 7-bit I2C address of the slave device.
#define EEPROM_STARTUP_REGISTER 0x0000 // Define the Startup register address
#define MULTIPLIER_FACTOR 1000         // Factor used to send float variables to Bridge Control Panel.
                                       // Set it = to 10^x where x = number of digits you want to send.

#define SENSITIVITY_G 2  // Sensitivity in mg/digit. It is defined in the Datasheet since:
                         // - FS bits (CTRL_REG4) are set to 01, so ±4 g full scale
                         // - High-resolution mode (12 bits)
                        
#define SENSITIVITY_MS2 (SENSITIVITY_G * 9.806)/1000 // Sensitivity in m/s^2

uint8_t k = 0;
uint8_t j = 0;
int16 AccValuesConverted[3];  // Array that contains the 3 axis converted acceleration values


// DataRateArray is an array that contains the 6 possible Data Rates of the CTRL_REG1. 
// NOTE: the Data Rate is defined by the 4 most significant bits, whereas the other 4 bits are set to 0 
// since in UpdateCTRL_REG1() function CTRL_REG1 is updated with the current DataRateArray value through
// an OR bit a bit operation, so that the 4 least significant bits of the register are left invaried.
uint8_t DataRateArray [DataRateArray_LENGTH] = {
                                                    0x10, // 0 0 0 1  0 0 0 0 --> 1 Hz
                                                    0x20, // 0 0 1 0  0 0 0 0 --> 10 Hz                                  
                                                    0x30, // 0 0 1 1  0 0 0 0 --> 25 Hz            
                                                    0x40, // 0 1 0 0  0 0 0 0 --> 50 Hz    
                                                    0x50, // 0 1 0 1  0 0 0 0 --> 100 Hz
                                                    0x60, // 0 1 1 0  0 0 0 0 --> 200 Hz
                                               };
  

    // SetRegister function is used to set the value (RegisterValue) of an register (address: RegisterAddress).
    // It returns the presence (or the absence) of an error during the register reading/writing operations.
ErrorCode SetRegister (uint8_t RegisterAddress, uint8_t RegisterValue)
    {
        // Read the register
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            RegisterAddress,
                                            &reg_value);
        if (error == ERROR)  
            UART_Debug_PutString("ERROR : reading register in SetRegister function\r\n");  
        else
            {
                if (reg_value != RegisterValue)  // If the value of the register is already equal to RegisterValue avoid the writing operation
                {                                 
                    // Write RegisterValue inside the register
                    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                         RegisterAddress,
                                                         RegisterValue);
                }
            }
        return error ? ERROR : NO_ERROR;
    }
    
    
    
    // At startup this function is called to find out to which DataRateArray value the eeprom_value correspond
    // and save the corrisponding DataRateArray index in DRindex.
void SearchDRindex (uint8_t eeprom_value)  
    {                                      
        for (k=0; k< DataRateArray_LENGTH; k++)   // Cycle inside DataRateArray 
            if (DataRateArray[k] == eeprom_value)
                { 
                    DRindex = k;  // Save the index in DRindex
                    break;        
                }
    }    
    
    
    
    // UpdateCTRL_REG1 function update CTRL_REG1 value with the new Data Rate (DataRateToUpdate)
void UpdateCTRL_REG1(uint8_t DataRateToUpdate)
    {
        // Read UpdateCTRL_REG1
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG1,
                                            &reg_value);  
        if (error == ERROR)  
            UART_Debug_PutString("ERROR : reading register in UpdateCTRL_REG1 function\r\n"); 
        else
            {
                // Maintain only the 4 least significant bits of reg_value and do an or bit a bit operation
                // with DataRateToUpdate to set 4 most significant bits (Data Rate bits).
                // NOTE: DataRateToUpdate 4 least significant bits are = to 0   
                reg_value = ( ( reg_value & 0x0F ) | DataRateToUpdate );
                
                // Set CTRL_REG1 value to update the new Data Rate 
                error = SetRegister(LIS3DH_CTRL_REG1, reg_value);  
                if (error == ERROR)  
                    UART_Debug_PutString("ERROR : setting CONTROL REGISTER 1\r\n");
            }
    }
   
    
    
    // ConvertAcc function takes as input the AccelerationValues array that contains the 6 read OUTPUT REGISTERS
    // and saves the 3 axis accelerations (in m/s^2) in DataBuffer array.
void ConvertAcc (uint8_t* AccelerationValues)
{
    k=0;
    j=0;
    while (k<6)  // Repeate the cycle until the 3 axis accelerations are saved into DataBuffer array.
    {
        // Reconstruct the acceleration value which is in 12 bits 
        AccValuesConverted[j] = (int16) (AccelerationValues[k] | (AccelerationValues[k+1] << 8)) >> 4;
        // Convert the acceleration value in m/s^2 
        OutAccconv = (AccValuesConverted[j] * SENSITIVITY_MS2);
        // Multiply for the MULTIPLIER_FACTOR and cast the result in int16 to save only the digits necessary
        AccValuesConverted[j] = (int16) (OutAccconv * MULTIPLIER_FACTOR);
        
        // 
        DataBuffer[k+1] = (uint8_t)(AccValuesConverted[j]& 0xFF);  // Save the LSB of AccValuesConverted[j] as the (k+1)° element of DataBuffer array
        DataBuffer[k+2] = (uint8_t)(AccValuesConverted[j] >> 8);   // Save the MSB of AccValuesConverted[j] as the (k+2)° element of DataBuffer array
        
        k+=2;  // Update k to pass to the next axix acceleration
        j++;
    }
}



    // UpdateEEPROM function writes DataRateArray[DRindex] on the EEPROM_STARTUP_REGISTER
void UpdateEEPROM()
{                      
  EEPROM_UpdateTemperature();    
  EEPROM_WriteByte (DataRateArray[DRindex] , EEPROM_STARTUP_REGISTER);
}


/* [] END OF FILE */
