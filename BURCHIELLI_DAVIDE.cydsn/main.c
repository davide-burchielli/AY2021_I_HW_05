/* ========================================
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
 * ========================================
*/
#include "project.h"
#include "Interrupt_Routines.h"
#include "I2C_Interface.h"

// DEFINES:

#define 


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    I2C_Peripheral_Start();
    UART_Debug_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    

    isr_BUTTON_StartEx(Custom_BUTTON_ISR); //Start the ISR of the button
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
