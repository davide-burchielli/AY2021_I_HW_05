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

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

/**
*   \brief Hex value to set normal mode to the accelerator
*/
#define LIS3DH_NORMAL_MODE_CTRL_REG1 0x47 

/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23

// BDU: settato =1 impedise che il valore dell'acc venga sovrascirtto prima che i due registri MSB e LSB vengano letti

#define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x80

/**
*   \brief Address of the ADC output LSB register
*/
#define LIS3DH_OUT_ADC_1L 0x08

/**
*   \brief Address of the ADC output MSB register
*/
#define LIS3DH_OUT_ADC_1H 0x09

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
