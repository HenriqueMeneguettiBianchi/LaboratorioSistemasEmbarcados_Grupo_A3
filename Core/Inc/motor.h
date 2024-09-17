// *********************************************************************//
// File name: 			motor.h                                         //
// File description: 	This file has the implementations				//
// 						of needed to use PWM controll                   //
// Author name:			Gabriel Erick Matheus Imamura                   //
//						Guilherme Augusto Vieira Palma                  //
//                      Henrique Meneguetti Bianchi                     //
//                      João Vitor Gomes Do N. De Siqueira              //
//                      Paulo Roberto De Araújo Junior                  //
// Creation date:		27aug2024                                       //
// Revision date:		16sep2024                                       //
// *********************************************************************//

#ifndef HEATERANDCOOLER_H
#define HEATERANDCOOLER_H

#include "stm32g4xx_hal.h"
#include "tim.h"

// *************************************************************//
// Method name:         vMotorInit                              //
// Method description: 	Inicia o motor                          //
// Input parameters:    *htimMotor, *htimFreq, *htimEncA,       //
//                      *htimEncB                               //
// Output parameters:   none                                    //
// *************************************************************//
void vMotorInit(TIM_HandleTypeDef *htimMotor,
				TIM_HandleTypeDef *htimFreq,
				TIM_HandleTypeDef *htimEncA,
				TIM_HandleTypeDef *htimEncB);

// *************************************************************//
// Method name:         vSetRodaEsquerdaDC                      //
// Method description: 	Seta o duty cycle da roda esquerda      //
// Input parameters:	fRodaEsquerdaDC -> duty cylce desejado  //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaEsquerdaDC(float fRodaEsquerdaDC);

// *************************************************************//
// Method name:         vSetRodaDireitaDC                       //
// Method description: 	Seta o duty cycle da roda direita       //
// Input parameters:	fRodaDireitaDC -> duty cylce desejado   //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaDireitaDC(float fRodaDireitaDC);

// *************************************************************//
// Method name:         HAL_TIM_IC_CaptureCallback              //
// Method description: 	Callback do timer Input Capture         //
// Input parameters:	*htim                                   //
// Output parameters:   none                                    //
// *************************************************************//
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

// *************************************************************//
// Method name:         fMotorGetSpeed                          //
// Method description: 	Função que retorna a velocidade         //
// Input parameters:	none                                    //
// Output parameters:   speed                                   //
// *************************************************************//
float fMotorGetSpeed(void);

#endif  // HEATERANDCOOLER_H
