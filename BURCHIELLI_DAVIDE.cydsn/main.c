/* ========================================
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
 * ========================================
*/
#include "project.h"
#include "stdio.h"

#include "Interrupt_Routines.h"
#include "I2C_Interface.h"
#include "REG_DRIVER.h"


#define LIS3DH_DEVICE_ADDRESS 0x18  // 7-bit I2C address of the slave device.

#define EEPROM_STARTUP_REGISTER 0x0000 // Define the Startup register address


    /******************************************/
    /*            REGISTERS DEFINES           */
    /******************************************/
    
// CONTROL REGISTER 1:

    #define LIS3DH_CTRL_REG1 0x20  //  Address of the Control register 1

    #define LIS3DH_CTRL_REG1_VALUE 0x07 // Hex value of the Control register 1

    /*
        ODR3 | ODR2 | ODR1 | ODR0 | LPen | Zen | Yen | Xen
    
          0     0      0      1      0      1     1     1
    
        ODR[3:0] = Data rate selection; 
                   ODR[3:1] set to 0 and ODR0 set to 0 to have a data rate of 1 Hz at the Start.
                   NOTE: their values are changed at the Start and everytime Custom_BUTTON_ISR is called.
        LPen  = Low-power mode enable set to 0 for HIGH RESOLUTION MODE
        Zen, Yen, Xen = Z, Y and X axes enable; all set to 1.

    */

// CONTROL REGISTER 4

    #define LIS3DH_CTRL_REG4 0x23  //  Address of the Control register 4

    #define LIS3DH_CTRL_REG4_VALUE 0x88 // Hex value of the Control register 4

    /*
        BDU | BLE | FS1 | FS0 | HR | ST1 | ST0 | SIM
    
         1     0     0     0     1    0     0     0

        BDU = Block data update; set to 1, thus output registers not updated until MSB and LSB reading;
        BLE = Big/little endian data selection; set to 0: Data LSB @ lower address;
        FS[1:0] = Full-scale selection ; set to 00 : ±2 g;
        HR = High-resolution output mode; set to 1 : high-resolution enabled;
        ST[1:0] = Self-test enable; set to 00 to disable it;
        SIM = SPI serial interface mode selection; set t default value 0; 

    */

// AXES OUTPUT REGISTERS

    #define OUT_X_L 0x28 // Address of the X-axis LSB Output Register  
    
// STATUS REGISTER

    #define STATUS_REG 0x27 // Address of the Status Register
   
    #define STATUS_REG_ZYXDA_MASK 0x08  // Mask to check if the ZYXDA bit is = 1   

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
       
    I2C_Peripheral_Start();
    UART_Debug_Start();
    EEPROM_Start();
   
    
    uint8_t AccelerationValues [6];
    char message[20] = {'\0'};
    extern uint8_t DataRateArray [DataRateArray_LENGTH];
    count = 0;
    reg_value=0;
    eeprom_value = 0;
  
    DataBuffer[0] = 0xA0;  // Write the HEADER byte as the first array element
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0; // Write the TAIL byte as the last array element
    

    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
// CONTROL REGISTER 1:
    
    UART_Debug_PutString("\r\nWriting CONTROL REGISTER 1:..\r\n");
    error = SetRegister(LIS3DH_CTRL_REG1, LIS3DH_CTRL_REG1_VALUE);

    if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", LIS3DH_CTRL_REG1_VALUE);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    
    
// CONTROL REGISTER 4:
    
    UART_Debug_PutString("\r\nWriting CONTROL REGISTER 4:..\r\n");
    error = SetRegister(LIS3DH_CTRL_REG4, LIS3DH_CTRL_REG4_VALUE);

    if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", LIS3DH_CTRL_REG4_VALUE);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 4\r\n");   
        }
    

// READ the internal EEPROM StartUp register 
        
    eeprom_value = EEPROM_ReadByte(EEPROM_STARTUP_REGISTER);
    //CyDelay(10);
   // UpdateCTRL_REG1(eeprom_value);
    
    sprintf(message, "\n EEPROM START UP : %x ", eeprom_value);
	UART_Debug_PutString(message);    
    
    /*
    EEPROM_UpdateTemperature();
    if (EEPROM_WriteByte (DataRateArray[count] , EEPROM_STARTUP_REGISTER) == 0 )
       UART_Debug_PutString("SIISISISISISISISISISI");
    CyDelay(10);
    eeprom_value = EEPROM_ReadByte (EEPROM_STARTUP_REGISTER);
    CyDelay(10);
    sprintf(message, "\n EEPROM DOPO : %x ", eeprom_value);
	UART_Debug_PutString(message);  
    */
    SearchCount (eeprom_value);
    UpdateCTRL_REG1(eeprom_value);
    isr_BUTTON_StartEx(Custom_BUTTON_ISR); //Start the ISR of the button
    
    for(;;)
    {
        if (ButtonFlag)
        {
                    ButtonFlag = 0;

                    UpdateCTRL_REG1(DataRateArray[count]);
                    
                    UpdateEEPROM();
 
            if (error == NO_ERROR)
                {
                    UART_Debug_PutString("aggiornamento completo");
                    /*
                    
                    EEPROM_UpdateTemperature();
                    EEPROM_WriteByte(DataRateArray[count], EEPROM_STARTUP_REGISTER);
                    CyDelay(10);

                    
                    uint8_t check = 3;
                    
                    sprintf(message, "\nCHECK : %u ", check);
		            UART_Debug_PutString(message);           
                    
                    if (check == CYRET_SUCCESS)
                        UART_Debug_PutString("write");
                        else UART_Debug_PutString("NOT write");
                   
                    eeprom_value=EEPROM_ReadByte(EEPROM_STARTUP_REGISTER);
                    sprintf(message, "\n --EEPROM DOPO : %x ", EEPROM_ReadByte(EEPROM_STARTUP_REGISTER));
		            UART_Debug_PutString(message);
                   */
                }
                else
                {
                    UART_Debug_PutString("Error occurred during I2C comm to update DataRate\r\n");   
                }   

            
        }
        
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            STATUS_REG,
                                            &reg_value);
        if (error == NO_ERROR)
        {
            if ((reg_value & STATUS_REG_ZYXDA_MASK) != 0)
            {
            
               error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                         OUT_X_L,
                                                         6,
                                                         AccelerationValues);                
            
                if (error == NO_ERROR)
                {
                 ConvertAcc(AccelerationValues);
                UART_Debug_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);
                }
              else
                {
                    UART_Debug_PutString("Error occurred during I2C comm to read OUTPUT REGISTERS\r\n");   
                }                
                
            }
        }
        else
        {
           UART_Debug_PutString("Error occurred during I2C comm to read Status Register\r\n");   
        }
        
    }
}

/* [] END OF FILE */
