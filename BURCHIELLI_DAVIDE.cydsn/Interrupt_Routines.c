/* =========================================================================================
    author: Davide Burchielli

    Custom_BUTTON_ISR is called every time the Switch_Button is pressed.

 * =========================================================================================
*/
#include "Interrupt_Routines.h"
#include "REG_DRIVER.h"


#define LIS3DH_CTRL_REG1 
#define EEPROM_STARTUP_REGISTER 0x00 // Define the Startup register address
#define DataRateArray_LENGTH 6

extern uint8_t DataRateArray [DataRateArray_LENGTH];
// Define Custom_BUTTON_ISR
CY_ISR (Custom_BUTTON_ISR)
{
    count = (count + 1) % DataRateArray_LENGTH;

    UpdateCTRL_REG1( DataRateArray[count]);
        if (error == NO_ERROR)
            {
                UART_Debug_PutString("DATARATE UPDATING DONE\r\n");
                EEPROM_UpdateTemperature();
                EEPROM_WriteByte(DataRateArray[count], EEPROM_STARTUP_REGISTER);
            }
            else
            {
                UART_Debug_PutString("Error occurred during I2C comm to update DataRate\r\n");   
        }       
}
/* [] END OF FILE */