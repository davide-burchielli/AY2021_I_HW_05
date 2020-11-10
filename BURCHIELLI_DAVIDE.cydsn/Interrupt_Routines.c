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


// Define Custom_BUTTON_ISR
CY_ISR (Custom_BUTTON_ISR)
{
    count = (count + 1) % DataRateArray_LENGTH;
    
    ButtonFlag = 1;
   
}
/* [] END OF FILE */