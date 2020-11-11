/* ===========================================================================================
    author: Davide Burchielli
   
    The aim of this project is to test I2C bus protocol on  the LIS3DH 3- Axis Accelerometer.

    This main.c file: 
    - starts the hardware components
    - sets the initial vaue of the LIS3DH registers
    - sets the accelerometer Data Rate value equal to the EEPROM Start-Up register value
    - manages the switch from one Data Rate to the next one
    - Sends the converted acceleration values to the Bridge Control Panel.
 * ===========================================================================================
*/

#include "project.h"
#include "stdio.h"

#include "Interrupt_Routines.h"
#include "I2C_Interface.h"
#include "REG_DRIVER.h"


#define LIS3DH_DEVICE_ADDRESS 0x18  // 7-bit I2C address of the slave device.

#define EEPROM_STARTUP_REGISTER 0x0000 // Define the EEPROM Startup register address


    /******************************************/
    /*            REGISTERS DEFINES           */
    /******************************************/
    
// CONTROL REGISTER 1:

    #define LIS3DH_CTRL_REG1 0x20  //  Address of the Control register 1

    #define LIS3DH_CTRL_REG1_VALUE 0x07 // Hex value of the Control register 1

    /*
        ODR3 | ODR2 | ODR1 | ODR0 | LPen | Zen | Yen | Xen
    
          0     0      0      0      0      1     1     1
    
        ODR[3:0] = Data rate selection bits; 
                   Set to 0 since their values are recovered at the EEPROM StartUp register at start-up and
                   changed everytime Custom_BUTTON_ISR is called.
        LPen  = Low-power mode enable; set to 0 for HIGH RESOLUTION MODE
        Zen, Yen, Xen = Z, Y and X axes enable; all set to 1.

    */

// CONTROL REGISTER 4

    #define LIS3DH_CTRL_REG4 0x23  //  Address of the Control register 4

    #define LIS3DH_CTRL_REG4_VALUE 0x98 // Hex value of the Control register 4

    /*
        BDU | BLE | FS1 | FS0 | HR | ST1 | ST0 | SIM
    
         1     0     0     1     1    0     0     0

        BDU = Block data update; set to 1, thus output registers are not updated until MSB and LSB reading;
        BLE = Big/little endian data selection; set to default value 0 : Data LSB @ lower address;
        FS[1:0] = Full-scale selection ; set to 01 : ±4 g since sperimentally, the acceleration values for fast
                  movements of the accelerometer are > 2g.
        HR = High-resolution output mode; set to 1 : high-resolution enabled;
        ST[1:0] = Self-test enable; set to 00 to disable it;
        SIM = SPI serial interface mode selection; set to default value 0; 

    */

// AXES OUTPUT REGISTERS

    // NOTE: since the 3 axes OUTPUT REGISTERS are one after the other, a MultiRead operation is executed, 
    //      thus it is needed just the first of the 6 registers : OUT_X_L

    #define OUT_X_L 0x28 // Address of the X-axis LSB Output Register
    #define NUMBER_OF_REGISTERS 6  // Number of OUTPUT REGISTERS to be read sequentially (2 for each axis)
    
// STATUS REGISTER

    #define STATUS_REG 0x27 // Address of the Status Register
   
    #define STATUS_REG_ZYXDA_MASK 0x08  // Mask to check if STATUS_REG ZYXDA bit is = 1(X, Y and Z-axis new data available)

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
       
    UART_Debug_Start();      // Starts the UART_Debag 
    EEPROM_Start();          // Starts the EEPROM 
    I2C_Peripheral_Start();  // Starts the I2C_Peripheral
    
    CyDelay(10);
    
    // Define variables:
    
    uint8_t AccelerationValues [6];  // AccelerationValues array contains the values of the 6 OUTPUT REGISTERS.
    extern uint8_t DataRateArray [DataRateArray_LENGTH];  // Global variable
    DRindex = 0;           // DRindex variable is used to keep track of the current DataRateArray index
    reg_value=0;           // reg_value varaible is used to memorize a register value
    eeprom_value = 0;      // eeprom_value varaible is used to memorize eeprom_value value
  
    DataBuffer[0] = 0xA0;  // Write the HEADER byte as the first DataBuffer array element
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0; // Write the TAIL byte as the last DataBuffer array element
    

    /******************************************/
    /*      Setting Registers values          */
    /******************************************/
    
// CONTROL REGISTER 1:
    
    error = SetRegister(LIS3DH_CTRL_REG1, LIS3DH_CTRL_REG1_VALUE);  // Set CTRL_REG1 value

    if (error == ERROR)  
        UART_Debug_PutString("ERROR : setting CONTROL REGISTER 1\r\n");   

// CONTROL REGISTER 4:
    
    error = SetRegister(LIS3DH_CTRL_REG4, LIS3DH_CTRL_REG4_VALUE);  // Set CTRL_REG4 value

    if (error == ERROR)  
        UART_Debug_PutString("ERROR : setting CONTROL REGISTER 4 \r\n");  

// READ the internal EEPROM StartUp register 
        
    eeprom_value = EEPROM_ReadByte(EEPROM_STARTUP_REGISTER);
    SearchDRindex (eeprom_value);   // Look for to which DataRateArray index eeprom_value correspond
    UpdateCTRL_REG1(eeprom_value);  // Update CTRL_REG1 bits to modify the Data Rate with eeprom_value
    
    isr_BUTTON_StartEx(Custom_BUTTON_ISR); //Start the ISR of the button
    
    for(;;)
    {
        if (ButtonFlag)  // if Custom_BUTTON_ISR has been called
        {
            ButtonFlag = 0;  // Reset the flag
            UpdateEEPROM();  // Write on the EEPROM StartUp register the new Data Rate value
            UpdateCTRL_REG1(DataRateArray[DRindex]);  // Update CTRL_REG1 bits to modify the Data Rate with DataRateArray[DRindex]
        }

        // Read STATUS_REG
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            STATUS_REG,
                                            &reg_value);
        
        if (error == ERROR)  
            UART_Debug_PutString("ERROR : reading STATUS REGISTER \r\n"); 
        else
        {
            if ((reg_value & STATUS_REG_ZYXDA_MASK) != 0)  // If the STATUS_REG ZYXDA bit is = 1, new acceleration data 
                                                           // are available, thus read the OUTPUT REGISTERS
            {
                // Read the 6 OUTPUT REGISTERS and same their values in AccelerationValues array
                error = I2C_Peripheral_ReadRegisterMulti( LIS3DH_DEVICE_ADDRESS,
                                                         OUT_X_L,
                                                         NUMBER_OF_REGISTERS,
                                                         AccelerationValues);                
                if (error == ERROR)  
                    UART_Debug_PutString("ERROR : multiple reading OUTPUT REGISTERS \r\n"); 
                else
                    {
                     ConvertAcc(AccelerationValues);  // Call ConvertAcc function to convert the read acceleration values
                     UART_Debug_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);  // Send to Bridge Control Panel the 3 axis acceleration values
                    }            
            }
        }
    }
}

/* [] END OF FILE */
