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
#include "Acceleration_Struct.h"


#define LIS3DH_DEVICE_ADDRESS 0x18  // 7-bit I2C address of the slave device.

#define EEPROM_STARTUP_REGISTER 0x00 // Define the Startup register address


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
    #define OUT_X_H 0x29 // Address of the X-axis HSB Output Register
   
    #define OUT_Y_L 0x2A // Address of the Y-axis LSB Output Register
    #define OUT_Y_H 0x2B // Address of the Y-axis HSB Output Register 

    #define OUT_Z_L 0x2C // Address of the Z-axis LSB Output Register
    #define OUT_Z_H 0x2D // Address of the Z-axis HSB Output Register     
    
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
    AccelerationStruct AccDataConverted;
    char message[20] = {'\0'};
    
    DataBuffer[0] = 0xA0;  // Write the HEADER byte as the first array element
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0; // Write the TAIL byte as the last array element
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."

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
    uint8_t eeprom_value = EEPROM_ReadByte (EEPROM_STARTUP_REGISTER);
    
    UpdateCTRL_REG1(eeprom_value);
   
    
    
    SearchCount (eeprom_value);

    isr_BUTTON_StartEx(Custom_BUTTON_ISR); //Start the ISR of the button
    
    for(;;)
    {
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
                    AccDataConverted.x = Convert (AccelerationValues[0], AccelerationValues[1]);
                    DataBuffer[1] = (uint8_t)(AccDataConverted.x & 0xFF);
                    DataBuffer[2] = (uint8_t)(AccDataConverted.x >> 8);                    
                    
                    AccDataConverted.y = Convert (AccelerationValues[2], AccelerationValues[3]);
                    DataBuffer[3] = (uint8_t)(AccDataConverted.y & 0xFF);
                    DataBuffer[4] = (uint8_t)(AccDataConverted.y >> 8);     
                    
                    AccDataConverted.z = Convert (AccelerationValues[4], AccelerationValues[5]);
                    DataBuffer[5] = (uint8_t)(AccDataConverted.z & 0xFF);
                    DataBuffer[6] = (uint8_t)(AccDataConverted.z >> 8);  
                    
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
