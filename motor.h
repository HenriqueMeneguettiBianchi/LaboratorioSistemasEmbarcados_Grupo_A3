// *********************************************************************//
// File name:           motor.h                                         //
// File description:    This file has the implementations               //
//                      of needed to use PWM controll on motor          //
// Author name:         Gabriel Erick Matheus Imamura                   //
//                      Guilherme Augusto Vieira Palma                  //
//                      Henrique Meneguetti Bianchi                     //
//                      João Vitor Gomes Do N. De Siqueira              //
//                      Paulo Roberto De Araújo Junior                  //
// Creation date:       27aug2024                                       //
// Revision date:       27aug2024                                       //
// *********************************************************************//

#ifndef HEATERANDCOOLER_H
#define HEATERANDCOOLER_H

#include "stm32g4xx_hal.h"
#include "tim.h"

// *************************************************************//
// Method name:         vMotorInit                              //
// Method description: 	Inicia TIM motor como PWM               //
// Input parameters:    *htimRE -> TIM handler da roda esquerda //
//                      *htimRD -> TIM handler da roda direita  //
// Output parameters:	  none                                    //
// *************************************************************//
void vMotorInit(TIM_HandleTypeDef *htimRE, TIM_HandleTypeDef *htimRD);

// Method name:         vSetRodaEsquerdaDC                      //
// Method description: 	Seta o duty cycle da roda esquerda      //
// Input parameters:	  fRodaEsquerdaDC -> duty cylce desejado  //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaEsquerdaDC(float fRodaEsquerdaDC);

// *************************************************************//
// Method name:         vSetRodaDireitaDC                       //
// Method description: 	Seta o duty cycle da roda direita       //
// Input parameters:	  fRodaDireitaDC -> duty cylce desejado   //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaDireitaDC(float fRodaDireitaDC);

// *************************************************************//
// Method name:         vRodaEsquerdaHandler                    //
// Method description: 	Retorna o endereço handler da roda      //
//                      esquerda                                //
// Input parameters:	none                                    //
// Output parameters:	none                                    //
// *************************************************************//
TIM_HandleTypeDef *vRodaEsquerdaHandler(void);

// *************************************************************//
// Method name:         vRodaDireitaHandler                     //
// Method description: 	Retorna o endereço handler da roda      //
//                      direita                                 //
// Input parameters:	none                                    //
// Output parameters:	none                                    //
// *************************************************************//
TIM_HandleTypeDef *vRodaDireitaHandler(void);

#endif  // HEATERANDCOOLER_H