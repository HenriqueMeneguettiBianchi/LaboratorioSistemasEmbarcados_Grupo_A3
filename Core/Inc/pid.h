/* ***************************************************************** */
/* File name:        pid.h                                           */
/* File description: Header file containing the functions/methods    */
/*                   interfaces for handling the PID                 */
/* Author name:      julioalvesMS, IagoAF, rBacurau                  */
/* Creation date:    21jun2018                                       */
/* Revision date:    21mai2023                                       */
/* ***************************************************************** */

#ifndef SOURCES_CONTROLLER_PID_H_
#define SOURCES_CONTROLLER_PID_H_

#define UPDATE_RATE_MS      100
#define UPDATE_RATE         (UPDATE_RATE_MS/1000.0)
#define INTEGRATOR_MAX_SIZE 100

typedef struct pid_data_type {
	float fKp, fKi, fKd;         		// PID gains
	float fError_previous;       		// used in the derivative
	float fError_sum;            		// integrator cumulative error
	unsigned short usIntegratorSize; 	//integrator window size
	float fOutputSaturation;            // output saturation
} pid_data_type;

extern float EsforcoE,EsforcoD;
/* ************************************************ */
/* Method name:        vPidInit                     */
/* Method description: Initialize the PID controller*/
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void vPidInit(float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation);


/* ************************************************** */
/* Method name:        vPidSetKp                      */
/* Method description: Set a new value for the PID    */
/*                     proportional constant          */
/* Input params:       fKp: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKp(float fKp);


/* ************************************************** */
/* Method name:        fPidGetKp                      */
/* Method description: Get the value from the PID     */
/*                     proportional constant          */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKp(void);


/* ************************************************** */
/* Method name:        vPidSetKi                      */
/* Method description: Set a new value for the PID    */
/*                     integrative constant           */
/* Input params:       fKi: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKi(float fKi);


/* ************************************************** */
/* Method name:        fPidGetKi                      */
/* Method description: Get the value from the PID     */
/*                     integrative constant           */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKi(void);


/* ************************************************** */
/* Method name:        vPidSetKd                      */
/* Method description: Set a new value for the PID    */
/*                     derivative constant            */
/* Input params:       fKd: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKd(float fKd);


/* ************************************************** */
/* Method name:        fPidGetKd                      */
/* Method description: Get the value from the PID     */
/*                     derivative constant            */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKd(void);


/* ************************************************** */
/* Method name:        vPidSetIntegratorWindow        */
/* Method description: Set a new value for the        */
/*                     integrator window (in ms)      */
/* Input params:       usIntSizeMs: New value (in ms) */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetIntegratorWindow (unsigned short usIntSizeMs);


/* ************************************************** */
/* Method name:        usPidGetIntegratorWindow       */
/* Method description: Get the value from the         */
/*                     integrator window (in ms)      */
/* Input params:       n/a                            */
/* Output params:      usIntSizeMs: Value (in ms)     */
/* ************************************************** */
unsigned short usPidGetIntegratorWindow (void);


/* ************************************************** */
/* Method name:        fPidUpdateData                 */
/* Method description: Update the control output      */
/*                     using the reference and sensor */
/*                     value                          */
/* Input params:       fSensorValue: Value read from  */
/*                     the sensor                     */
/*                     fReferenceValue: Value used as */
/*                     control reference              */
/* Output params:      float: New Control effort      */
/* ************************************************** */
float fPidUpdateData(float fvelocidadeAtual, float fVelocidadeDesejada);
void vPidInit2(float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation);
void vPidSetKp2(float fKp);
float fPidGetKp2(void);
void vPidSetKi2(float fKi);
float fPidGetKi2(void);
void vPidSetKd2(float fKd);
float fPidGetKd2(void);
void vPidSetIntegratorWindow2(unsigned short usIntSizeMs);
unsigned short usPidGetIntegratorWindow2(void);
float fPidUpdateData2(float fvelocidadeAtual, float fVelocidadeDesejada);

void ajustaVelocidadeRodas();

#endif /* SOURCES_CONTROLLER_PID_H_ */
