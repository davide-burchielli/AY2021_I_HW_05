/* =========================================================================================
    author: Davide Burchielli

    Custom_BUTTON_ISR is called every time the Switch_Button is pressed.

 * =========================================================================================
*/
#include "Interrupt_Routines.h"
#include "REG_DRIVER.h"

// Define Custom_BUTTON_ISR
CY_ISR (Custom_BUTTON_ISR)
{
    DRindex = (DRindex + 1) % DataRateArray_LENGTH;  
    ButtonFlag = 1;
}
/* [] END OF FILE */