// **********************************************
// File name: motores.h
// File description:
// Authors name:
// Creation date:
// Revision date:
// **********************************************

#ifndef MOTORES_H
#define MOTORES_H

#include "tim.h"

// Function prototypes
void vMotorInit(TIM_HandleTypeDef *htim);
void vSetRodasDC(float fRodaEsquerdaDC, float fRodaDireitaDC);

#endif // MOTORES_H
