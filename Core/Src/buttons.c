// ***************************************************** //
// File name:          button_control.c                  //
// File description:   This file handles initialization  //
//                     and status checking of buttons.   //
// Creation date:      23mar2023                         //
// Last update:        06apr2023                         //
// ***************************************************** //

#include "buttons.h"
#include "main.h"

// ***************************************************** //
// Function:           initializeButtons                 //
// Description:        Sets up each button by enabling   //
//                     the clock and configuring the port//
//                     for input mode.                   //
// Input params:       None                              //
// Output params:      None                              //
// ***************************************************** //
void initializeButtons()
{
    buttonState.ucUp = RELEASED;
    buttonState.ucDown = RELEASED;
    buttonState.ucRight = RELEASED;
    buttonState.ucLeft = RELEASED;
    buttonState.ucEnter = RELEASED;
    buttonState.ucCurrentState = RELEASED;
}

// ***************************************************** //
// Function:           getButtonStatus                   //
// Description:        Reads the GPIO registers to       //
//                     determine if a button is pressed. //
// Input params:       buttonType: the button position   //
// Output params:      An int indicating if the button   //
//                     was pressed (1) or not (0).       //
// ***************************************************** //
int getButtonStatus(tButtonType buttonType)
{
    // Variable to store the button press status
    int pressStatus = 0;

    switch (buttonType)
    {
        case (up):
            pressStatus = HAL_GPIO_ReadPin(BT_Cima_GPIO_Port, BT_Cima_Pin);
            break;
        case (down):
            pressStatus = HAL_GPIO_ReadPin(BT_Baixo_GPIO_Port, BT_Baixo_Pin);
            break;
        case (left):
            pressStatus = HAL_GPIO_ReadPin(BT_Esq_GPIO_Port, BT_Esq_Pin);
            break;
        case (right):
            pressStatus = HAL_GPIO_ReadPin(BT_Dir_GPIO_Port, BT_Dir_Pin);
            break;
        case (enter):
            pressStatus = HAL_GPIO_ReadPin(BT_Enter_GPIO_Port, BT_Enter_Pin);
            break;
    }

    // Return 1 if the button is pressed, otherwise return 0
    return (pressStatus == 0x00000000) ? 0 : 1;
}
