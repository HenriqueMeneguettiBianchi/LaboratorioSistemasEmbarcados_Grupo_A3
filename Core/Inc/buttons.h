// ***************************************************** //
// File name:          button_control.h                  //
// File description:   Header file for button            //
//                     initialization and status checks. //
// Creation date:      23mar2023                         //
// Last update:        21jun2023                         //
// ***************************************************** //

#ifndef INC_BUTTON_CONTROL_H_
#define INC_BUTTON_CONTROL_H_
#endif /* INC_BUTTON_CONTROL_H_ */

// Enum to define the button positions with unique values
typedef enum
{
    up = 1,
    down = 2,
    left = 3,
    right = 4,
    enter = 5

} tButtonType;

// ***************************************************** //
// Function:           initializeButtons                 //
// Description:        Initializes buttons by enabling   //
//                     the clock and setting ports to    //
//                     input mode.                       //
// Input params:       None                              //
// Output params:      None                              //
// ***************************************************** //
void initializeButtons(void);

// ***************************************************** //
// Function:           getButtonStatus                   //
// Description:        Reads GPIO register to check if   //
//                     the corresponding button is       //
//                     pressed.                          //
// Input params:       buttonType: the button position.   //
// Output params:      Returns an integer (1 if pressed,  //
//                     0 if not).                        //
// ***************************************************** //
int getButtonStatus(tButtonType buttonType);
