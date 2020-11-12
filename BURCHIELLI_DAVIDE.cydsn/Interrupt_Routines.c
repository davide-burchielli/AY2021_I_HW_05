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
// Increment the DRindex in order to keep track of the current Data Rate save at the DRIndex-th index of DataRateArray.
    DRindex = (DRindex + 1) % DataRateArray_LENGTH; // NOTE: reset DRindex to 0 when ( DRindex +1 )= 6;
    
    ButtonFlag = 1;   // flag variable set to 1 to warn main file that the Data Rate must be changed
}
/* [] END OF FILE */