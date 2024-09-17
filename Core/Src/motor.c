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
// Revision date:		16sep2024                                       //
// *********************************************************************//

#include "motor.h"

#define TIMCLOCK 170000000
#define PRESCALAR 170

TIM_HandleTypeDef *htimRodas, *htimEncMotorA, *htimEncMotorB, *htimFreq;
uint32_t uiIC_Val1A = 0, uiIC_Val1B = 0;
uint32_t uiIC_Val2A = 0, uiIC_Val2B = 0;
uint32_t uiDifferenceA = 0, uiDifferenceB = 0;
int iFirstCaptureA = 0, iFirstCaptureB = 0;
float fFrequencyA = 0, fFrequencyB = 0;

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
				TIM_HandleTypeDef *htimEncB){
  htimRodas = htimMotor; // handler do timer que gera PWM para a velocidade dos motores
  htimEncMotorA = htimEncA; // handler do timer no input capture do motor A
  htimEncMotorB = htimEncB; // handler do timer no input capture do motor B
  HAL_TIM_PWM_Start(htimMotor, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(htimMotor, TIM_CHANNEL_2);
  htimFreq->Instance->CCR1 = 50;
  HAL_TIM_PWM_Start(htimFreq, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(htimEncA, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(htimEncB, TIM_CHANNEL_1);
}

// *************************************************************//
// Method name:         vSetRodaEsquerdaDC                      //
// Method description: 	Seta o duty cycle da roda esquerda      //
// Input parameters:	fRodaEsquerdaDC -> duty cylce desejado  //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaEsquerdaDC(float fRodaEsquerdaDC){
  __HAL_TIM_SET_COMPARE(htimRodas, TIM_CHANNEL_1, fRodaEsquerdaDC * 1000);
}

// *************************************************************//
// Method name:         vSetRodaDireitaDC                       //
// Method description: 	Seta o duty cycle da roda direita       //
// Input parameters:	fRodaDireitaDC -> duty cylce desejado   //
// Output parameters:   none                                    //
// *************************************************************//
void vSetRodaDireitaDC(float fRodaDireitaDC){
  __HAL_TIM_SET_COMPARE(htimRodas, TIM_CHANNEL_2, fRodaDireitaDC * 1000);
}

// *************************************************************//
// Method name:         HAL_TIM_IC_CaptureCallback              //
// Method description: 	Callback do timer Input Capture         //
// Input parameters:	*htim                                   //
// Output parameters:   none                                    //
// *************************************************************//
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim == htimEncMotorA){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
			// se a primeira borda de subida não foi capturada
			if(iFirstCaptureA == 0){
				uiIC_Val1A = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // lê o primeiro valor
				iFirstCaptureA = 1; // seta a primeira captura como true
			}
			// se a primeira borda de subida foi capturada, agora será capturada a segunda borda
			else{
				uiIC_Val2A = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // lê o segundo valor

				if(uiIC_Val2A > uiIC_Val1A){
					uiDifferenceA = uiIC_Val2A - uiIC_Val1A;
				}
				else if(uiIC_Val1A > uiIC_Val2A){
					uiDifferenceA = (0xFFFF - uiIC_Val1A) + uiIC_Val2A;
				}

				float fRefClock = TIMCLOCK/(PRESCALAR);

				fFrequencyA = fRefClock/uiDifferenceA;

				__HAL_TIM_SET_COUNTER(htim, 0); // reseta o counter
				iFirstCaptureA = 0; // seta a primeira captura como false
			}
		}
	}

	if(htim == htimEncMotorB){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
			// se a primeira borda de subida não foi capturada
			if(iFirstCaptureB == 0){
				uiIC_Val1B = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // lê o primeiro valor
				iFirstCaptureB = 1; // seta a primeira captura como true
			}
			// se a primeira borda de subida foi capturada, agora será capturada a segunda borda
			else{
				uiIC_Val2B = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // lê o segundo valor

				if(uiIC_Val2B > uiIC_Val1B){
					uiDifferenceB = uiIC_Val2B - uiIC_Val1B;
				}
				else if(uiIC_Val1B > uiIC_Val2B){
					uiDifferenceB = (0xFFFF - uiIC_Val1B) + uiIC_Val2B;
				}

				float fRefClock = TIMCLOCK/(PRESCALAR);

				fFrequencyB = fRefClock/uiDifferenceB;

				__HAL_TIM_SET_COUNTER(htim, 0); // reseta o counter
				iFirstCaptureB = 0; // seta a primeira captura como false
			}
		}
	}
}

// *************************************************************//
// Method name:         fMotorGetSpeed                          //
// Method description: 	Função que retorna a velocidade         //
// Input parameters:	none                                    //
// Output parameters:   speed                                   //
// *************************************************************//
float fMotorGetSpeed(void){
	// Calcula a frequência média das rodas (divide por 2) e divide por 20 (20 pulsos/rev)
	float fAvgFrequency = (fFrequencyA + fFrequencyB)/40;

	// Calcula a velocidade em cm/s
	// v = 2*pi*frequencia*raio
	return (6.28*3.25*fAvgFrequency); // raio = 3.25 cm (medido com paquimetro)
}
