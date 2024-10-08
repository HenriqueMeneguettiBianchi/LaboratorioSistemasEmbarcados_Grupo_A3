// **************************************************** //
// File name:           led_control.h                    //
// File description:    Header for LED control functions //
//                      for initializing, writing,       //
//                      toggling, and managing LEDs.     //
// Creation date:     07/10/2024                          //
// Last update:       08/10/2024                         //
// **************************************************** //

#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_


// Define the LED types, each number represents a different LED
typedef enum
{
  red = 1, green = 2, blue = 3
} tLedType;

// **************************************************** //
// Function:             initializeLed                  //
// Description:          Sets up the LEDs by enabling    //
//                       the clock and configuring       //
//                       the port as input mode.         //
// Input params:         None                            //
// Output params:        None                            //
// **************************************************** //
void initializeLed(void);

// **************************************************** //
// Function:             setLedState                    //
// Description:          Sets the state of the LED (on/off) //
// Input params:         ledNumber: specifies which LED  //
//                       ledState: 1 to turn on, 0 to turn off //
// Output params:        None                            //
// **************************************************** //
void setLedState(tLedType ledNumber, int ledState);

// **************************************************** //
// Function:             activateLed                    //
// Description:          Turns on the specified LED      //
// Input params:         ledNumber: specifies which LED  //
// Output params:        None                            //
// **************************************************** //
void activateLed(tLedType ledNumber);

// **************************************************** //
// Function:             deactivateLed                  //
// Description:          Turns off the specified LED     //
// Input params:         ledNumber: specifies which LED  //
// Output params:        None                            //
// **************************************************** //
void deactivateLed(tLedType ledNumber);

// **************************************************** //
// Function:             toggleLedState                 //
// Description:          Toggles the state of the LED    //
// Input params:         ledNumber: specifies which LED  //
// Output params:        None                            //
// **************************************************** //
void toggleLedState(tLedType ledNumber);
#endif /* LED_CONTROL_H_ */