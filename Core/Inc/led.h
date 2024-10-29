// ********************************************** //
// File name: led.h                               //
// File description: Arquivo que lista as         //
//                   funcoes do arquivo led.c     //
//                   referente a atividade pratica//
//                   da Semana 03 + funcao led no //
//                   teclado para Semana 04       //
// Authors name: Maria Eduarda Vieira Rangel      //
//               Pedro Henrique dos Santos        //
// Creation date: 02/04/2024                      //
// Revision date: 28/08/2024                      //
// ********************************************** //

#ifndef LED_H_
#define LED_H_

typedef enum {verde = 1, vermelho, azul} leds;

/* *******************************************************************  */
/* Method name: 	   vLedWrite	         		                    */
/* Method description: Controle de ON/OFF de cada um dos Leds           */
/* Input params:	   iLedNumber (1-5) e iState (0-1)                  */
/* Output params:	   None						 	                    */
/* ******************************************************************** */
void vLedWrite(int iLedNumber, int iState);

/* ************************************************   */
/* Method name: 	   vLedToggleLed        		  */
/* Method description: Inverte nivel logico de um led */
/* Input params:	   xColorLed (enum [1..5])        */
/* Output params:	   None							  */
/* ************************************************   */
void vLedToggleLed(leds xColorLed);

#endif /* INC_LED_H_ */
