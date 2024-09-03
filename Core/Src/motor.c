// *********************************************************************//
// File name: 		    motor.c                                         //
// File description: 	This file has the implementations               //
// 						of needed to use PWM controll on motor          //
// Author name:			Gabriel Erick Matheus Imamura                   //
//						Guilherme Augusto Vieira Palma                  //
//                      Henrique Meneguetti Bianchi                     //
//                      João Vitor Gomes Do N. De Siqueira              //
//                      Paulo Roberto De Araújo Junior                  //
// Creation date:		27aug2024                                       //
// Revision date:		27aug2024                                       //
// *********************************************************************//

#include "motor.h"

TIM_HandleTypeDef *htimRodaEsquerda;
TIM_HandleTypeDef *htimRodaDireita;

// *************************************************************//
// Method name:         vMotorInit                              //
// Method description: 	Inicia TIM motor como PWM               //
// Input parameters:    *htimRE -> TIM handler da roda esquerda //
//                      *htimRD -> TIM handler da roda direita  //
// Output parameters:   none                                    //
// *************************************************************//
void vMotorInit(TIM_HandleTypeDef *htimRE, TIM_HandleTypeDef *htimRD)
{
  htimRodaEsquerda = htimRE;
  htimRodaDireita = htimRD;
  HAL_TIM_PWM_Start(htimRE, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(htimRE, TIM_CHANNEL_2);
}

// *************************************************************//
// Method name:         vSetRodaEsquerdaDC                      //
// Method description: 	Seta o duty cycle da roda esquerda      //
// Input parameters:	  fRodaEsquerdaDC -> duty cylce desejado  //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaEsquerdaDC(float fRodaEsquerdaDC)
{
  __HAL_TIM_SET_COMPARE(htimRodaEsquerda, TIM_CHANNEL_1, fRodaEsquerdaDC * 1000);
}

// *************************************************************//
// Method name:         vSetRodaDireitaDC                       //
// Method description: 	Seta o duty cycle da roda direita       //
// Input parameters:	  fRodaDireitaDC -> duty cylce desejado   //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaDireitaDC(float fRodaDireitaDC)
{
  __HAL_TIM_SET_COMPARE(htimRodaDireita, TIM_CHANNEL_2, fRodaDireitaDC * 1000);
}

// *************************************************************//
// Method name:         vRodaEsquerdaHandler                    //
// Method description: 	Retorna o endereço handler da roda      //
//                      esquerda                                //
// Input parameters:	  none                                    //
// Output parameters:	  none                                    //
// *************************************************************//
TIM_HandleTypeDef *vRodaEsquerdaHandler(void)
{
  return htimRodaEsquerda;
}

// *************************************************************//
// Method name:         vRodaDireitaHandler                     //
// Method description: 	Retorna o endereço handler da roda      //
//                      direita                                 //
// Input parameters:	  none                                    //
// Output parameters:	  none                                    //
// *************************************************************//
TIM_HandleTypeDef *vRodaDireitaHandler(void)
{
  return htimRodaDireita;
}
