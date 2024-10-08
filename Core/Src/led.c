// **************************************************** //
// File name:         led_control.c                      //
// File description:  Source file for controlling LEDs,  //
//                    including initialization, state    //
//                    writing, and toggling.             //
// Creation date:     07/10/2024                          //
// Last update:       08/10/2024                         //
// **************************************************** //

#include "led.h"
#include "main.h"

// **************************************************** //
// Function:             initializeLed                  //
// Description:          Configures each LED by enabling //
//                       the clock and setting up the    //
//                       port mode.                      //
// Input params:         None                            //
// Output params:        None                            //
// **************************************************** //
void initializeLed()
{
    // Code for initializing LEDs can be added here
}

// **************************************************** //
// Function:             setLedState                    //
// Description:          Sets the LED state to on or off //
// Input params:         ledNumber: specifies the LED    //
//                       ledState: 1 to turn on, 0 to turn off //
// Output params:        None                            //
// **************************************************** //
void setLedState(tLedType ledNumber, int ledState)
{
    if (ledState == 1)
    {
        activateLed(ledNumber);
    }
    else if (ledState == 0)
    {
        deactivateLed(ledNumber);
    }
}

// **************************************************** //
// Function:             activateLed                    //
// Description:          Turns on the specified LED      //
// Input params:         ledNumber: specifies the LED    //
// Output params:        None                            //
// **************************************************** //
void activateLed(tLedType ledNumber)
{
    // Write 1 on the pin of the ODR register that turns the LED on
    switch (ledNumber)
    {
        case red:
        	LedRed_GPIO_Port->ODR |= LedRed_Pin;
            break;
        case green:
        	LedGreen_GPIO_Port->ODR |= LedGreen_Pin;
            break;
        case blue:
        	LedBlue_GPIO_Port->ODR |= LedBlue_Pin;
            break;
    }
}

// **************************************************** //
// Function:             deactivateLed                  //
// Description:          Turns off the specified LED     //
// Input params:         ledNumber: specifies the LED    //
// Output params:        None                            //
// **************************************************** //
void deactivateLed(tLedType ledNumber)
{
    // Write 0 on the pin of the ODR register to turn the LED off
    switch (ledNumber)
    {
        case red:
            LedRed_GPIO_Port->ODR &= ~(LedRed_Pin);
            break;
        case green:
            LedGreen_GPIO_Port->ODR &= ~(LedGreen_Pin);
            break;
        case blue:
            LedBlue_GPIO_Port->ODR &= ~(LedBlue_Pin);
            break;
    }
}

// **************************************************** //
// Function:             toggleLedState                 //
// Description:          Toggles the state of the specified LED //
// Input params:         ledNumber: specifies the LED    //
// Output params:        None                            //
// **************************************************** //
void toggleLedState(tLedType ledNumber)
{
    int currentState;
    int toggleResult;

    // Read the ODR register value to get the current state of the pins
    currentState = (ledNumber == blue || ledNumber == red) ? LedBlue_GPIO_Port->ODR : LedGreen_GPIO_Port->ODR;

    // Check the current state of the specified LED
    switch (ledNumber)
    {
        case red:
            toggleResult = currentState & LedRed_Pin;
            break;
        case green:
            toggleResult = currentState & LedGreen_Pin;
            break;
        case blue:
            toggleResult = currentState & LedBlue_Pin;
            break;
    }

    // If the LED is off, turn it on; if it's on, turn it off
    if (toggleResult == 0x00000000)
    {
        activateLed(ledNumber);
    }
    else
    {
        deactivateLed(ledNumber);
    }
}
