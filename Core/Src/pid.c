/* ***************************************************************** */
/* File name:        pid.c                                           */
/* File description: This file has a couple of useful functions to   */
/*                   control the implemented PID controller          */
/* Author name:      julioalvesMS, IagoAF, rBacurau                  */
/* Creation date:    21jun2018                                       */
/* Revision date:    20mai2024                                       */
/* ***************************************************************** */

#include "pid.h"
#include "main.h"
#include "motor.h"
#include "sensorLinha.h"
#include "encoder.h"

// Struct used to store the PID configuration parameters
pid_data_type xPidConfig, xPidConfig1;
// Counter used to control the integration error window
unsigned short usIntegratorCount = 0;
// Buffer used to store the errors to generate the integral error
float fIntegratorBuffer[INTEGRATOR_MAX_SIZE]={0};
float EsforcoE = 0;
float EsforcoD = 0;
float fError, fDifference, fOut;

/* ************************************************ */
/* Method name:        vPidInit                     */
/* Method description: Initialize the PID controller*/
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void vPidInit(float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation)
{
	xPidConfig.fKp = fKp;
	xPidConfig.fKd = fKd;
	xPidConfig.fKi = fKi;
	xPidConfig.fError_previous = 0;
	xPidConfig.fError_sum = 0.0;

	// Saturates Integrator size (up to 10 s)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;

	xPidConfig.fOutputSaturation = fOutputSaturation;
}

/* ************************************************** */
/* Method name:        vPidSetKp                      */
/* Method description: Set a new value for the PID    */
/*                     proportional constant          */
/* Input params:       fKp: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKp(float fKp)
{
	xPidConfig.fKp = fKp;
}


/* ************************************************** */
/* Method name:        fPidGetKp                      */
/* Method description: Get the value from the PID     */
/*                     proportional constant          */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKp(void)
{
	return xPidConfig.fKp;
}


/* ************************************************** */
/* Method name:        vPidSetKi                      */
/* Method description: Set a new value for the PID    */
/*                     integrative constant           */
/* Input params:       fKi: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKi(float fKi)
{
	xPidConfig.fKi = fKi;
}


/* ************************************************** */
/* Method name:        fPidGetKi                      */
/* Method description: Get the value from the PID     */
/*                     integrative constant           */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKi(void)
{
	return xPidConfig.fKi;
}


/* ************************************************** */
/* Method name:        vPidSetKd                      */
/* Method description: Set a new value for the PID    */
/*                     derivative constant            */
/* Input params:       fKd: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKd(float fKd)
{
	xPidConfig.fKd = fKd;
}


/* ************************************************** */
/* Method name:        fPidGetKd                      */
/* Method description: Get the value from the PID     */
/*                     derivative constant            */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKd(void)
{
	return xPidConfig.fKd;
}

/* ************************************************** */
/* Method name:        vPidSetIntegratorWindow        */
/* Method description: Set a new value for the        */
/*                     integrator window (in ms)      */
/* Input params:       usIntSizeMs: New value (in ms) */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetIntegratorWindow (unsigned short usIntSizeMs)
{
	// Saturates Integrator size (10000 ms)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;
}

/* ************************************************** */
/* Method name:        usPidGetIntegratorWindow       */
/* Method description: Get the value from the         */
/*                     integrator window (in ms)      */
/* Input params:       n/a                            */
/* Output params:      usIntSizeMs: Value (in ms)     */
/* ************************************************** */
unsigned short usPidGetIntegratorWindow (void)
{
	return (xPidConfig.usIntegratorSize*UPDATE_RATE_MS);
}

/* ************************************************** */
/* Method name:        fPidUpdateData                 */
/* Method description: Update the control output      */
/*                     using the reference and sensor */
/*                     value                          */
/* Input params:       fSensorvelocity: Value read from  */
/*                     the sensor                     */
/*                     fReferenceValue: Value used as */
/*                     control reference              */
/* Output params:      float: New Control effort      */
/* ************************************************** */
float fPidUpdateData(float fvelocidadeAtual, float fVelocidadeDesejada)
{
	float fError, fDifference, fOut;

	// Proportional error
	fError = fVelocidadeDesejada - fvelocidadeAtual;

	//Ingtegral error
	xPidConfig.fError_sum = xPidConfig.fError_sum - fIntegratorBuffer[usIntegratorCount] + fError;

	fIntegratorBuffer[usIntegratorCount] = fError;

	if(++usIntegratorCount >= xPidConfig.usIntegratorSize)
		usIntegratorCount = 0;

	// Differential error
	fDifference = (fError - xPidConfig.fError_previous);

	fOut = xPidConfig.fKp * fError
		 + xPidConfig.fKi * xPidConfig.fError_sum * UPDATE_RATE
		 + xPidConfig.fKd * fDifference / UPDATE_RATE;

	xPidConfig.fError_previous = fError;

    // Output Saturation
	if(fOut > xPidConfig.fOutputSaturation)
		fOut = xPidConfig.fOutputSaturation;
	else 
		if (fOut < 0)
			fOut = 0;

	return fOut;
}
void vPidInit2(float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation)
{
	xPidConfig1.fKp = fKp;
	xPidConfig1.fKd = fKd;
	xPidConfig1.fKi = fKi;
	xPidConfig1.fError_previous = 0;
	xPidConfig1.fError_sum = 0.0;

	// Saturates Integrator size (up to 10 s)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig1.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;

	xPidConfig1.fOutputSaturation = fOutputSaturation;
}

/* ************************************************** */
/* Method name:        vPidSetKp2                      */
/* Method description: Set a new value for the PID    */
/*                     proportional constant          */
/* Input params:       fKp: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKp2(float fKp)
{
	xPidConfig1.fKp = fKp;
}


/* ************************************************** */
/* Method name:        fPidGetKp2                      */
/* Method description: Get the value from the PID     */
/*                     proportional constant          */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKp2(void)
{
	return xPidConfig1.fKp;
}


/* ************************************************** */
/* Method name:        vPidSetKi2                      */
/* Method description: Set a new value for the PID    */
/*                     integrative constant           */
/* Input params:       fKi: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKi2(float fKi)
{
	xPidConfig1.fKi = fKi;
}


/* ************************************************** */
/* Method name:        fPidGetKi2                      */
/* Method description: Get the value from the PID     */
/*                     integrative constant           */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKi2(void)
{
	return xPidConfig1.fKi;
}


/* ************************************************** */
/* Method name:        vPidSetKd2                      */
/* Method description: Set a new value for the PID    */
/*                     derivative constant            */
/* Input params:       fKd: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKd2(float fKd)
{
	xPidConfig1.fKd = fKd;
}


/* ************************************************** */
/* Method name:        fPidGetKd2                      */
/* Method description: Get the value from the PID     */
/*                     derivative constant            */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKd2(void)
{
	return xPidConfig1.fKd;
}

/* ************************************************** */
/* Method name:        vPidSetIntegratorWindow2        */
/* Method description: Set a new value for the        */
/*                     integrator window (in ms)      */
/* Input params:       usIntSizeMs: New value (in ms) */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetIntegratorWindow2 (unsigned short usIntSizeMs)
{
	// Saturates Integrator size (10000 ms)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig1.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;
}

/* ************************************************** */
/* Method name:        usPidGetIntegratorWindow2       */
/* Method description: Get the value from the         */
/*                     integrator window (in ms)      */
/* Input params:       n/a                            */
/* Output params:      usIntSizeMs: Value (in ms)     */
/* ************************************************** */
unsigned short usPidGetIntegratorWindow2 (void)
{
	return (xPidConfig1.usIntegratorSize*UPDATE_RATE_MS);
}

/* ************************************************** */
/* Method name:        fPidUpdateData2                 */
/* Method description: Update the control output      */
/*                     using the reference and sensor */
/*                     value                          */
/* Input params:       fSensorvelocity: Value read from  */
/*                     the sensor                     */
/*                     fReferenceValue: Value used as */
/*                     control reference              */
/* Output params:      float: New Control effort      */
/* ************************************************** */
float fPidUpdateData2(float fvelocidadeAtual, float fVelocidadeDesejada)
{
	float fError, fDifference, fOut;

	// Proportional error
	fError = fVelocidadeDesejada - fvelocidadeAtual;

	//Ingtegral error
	xPidConfig1.fError_sum = xPidConfig1.fError_sum - fIntegratorBuffer[usIntegratorCount] + fError;

	fIntegratorBuffer[usIntegratorCount] = fError;

	if(++usIntegratorCount >= xPidConfig1.usIntegratorSize)
		usIntegratorCount = 0;

	// Differential error
	fDifference = (fError - xPidConfig1.fError_previous);

	fOut = xPidConfig1.fKp * fError
		 + xPidConfig1.fKi * xPidConfig1.fError_sum * UPDATE_RATE
		 + xPidConfig1.fKd * fDifference / UPDATE_RATE;

	xPidConfig1.fError_previous = fError;

    // Output Saturation
	if(fOut > xPidConfig1.fOutputSaturation)
		fOut = xPidConfig1.fOutputSaturation;
	else
		if (fOut < 0)
			fOut = 0;

	return fOut;
}
void ajustaVelocidadeRodas(){
	EsforcoD = fPidUpdateData(velocidadeRodaDireita,velocidadeDireita)/100;
	EsforcoE = fPidUpdateData2(velocidadeRodaEsquerda,velocidadeEsquerda)/100;
	//EsforcoD = EsforcoD + 0.34;
	//EsforcoE = EsforcoE + 0.34;

	//if(EsforcoD < 0.4) {
	//	EsforcoD = 0.4;
	//}
	//if(EsforcoE < 0.4) {
	//		EsforcoE = 0.4;
	//	}
	vSetRodasDC(EsforcoE, EsforcoD);


}
