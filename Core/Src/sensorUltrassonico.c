// *****************
// File name: sensorUltrassonico.c
// File description: Biblioteca para sensor ultrassonico
//
// Author name:
// Creation date:
// Revision date:
// *****************
#include "main.h"
#include "sensorUltrassonico.h"

TIM_HandleTypeDef *pDistanceSensorTIM;
TIM_HandleTypeDef *pDistanceSensorTrigger;

uint32_t uiDistanceSensorRightChannel;
uint32_t uiDistanceSensorTriggerChannel;


float Distance = 0;
float Difference = 0;
float fDistanceSensorRightDistance;

uint32_t uiICVal1_Right = 0;
uint32_t uiICVal2_Right = 0;

uint8_t uiIsFirstCapture_Right = 0;


// Esse codigo foi modificado de https://controllerstech.com/hcsr04-ultrasonic-sensor-and-stm32/
float fDistanceSensorGetSensorDistance(TIM_HandleTypeDef* htim, uint32_t uiChannel,
    uint8_t* Is_First_Captured, uint32_t* IC_Val1, uint32_t* IC_Val2)
{

  uint8_t isFirtsCaptured = *Is_First_Captured;
  uint32_t icVal1 = *IC_Val1;
  uint32_t icVal2 = *IC_Val2;

  if (isFirtsCaptured == 0) // if the first value is not captured
  {
    icVal1 = HAL_TIM_ReadCapturedValue(htim, uiChannel); // read the first value
    isFirtsCaptured = 1;  // set the first captured as true
    // Now change the polarity to falling edge
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, uiChannel,
        TIM_INPUTCHANNELPOLARITY_FALLING);
  }

  else if (isFirtsCaptured == 1)   // if the first is already captured
  {
    icVal2 = HAL_TIM_ReadCapturedValue(htim, uiChannel); // read second value
    __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

    if (icVal2 > icVal1)
    {
      Difference = icVal2 - icVal1;
    }

    else if (icVal1 > icVal2)
    {
      Difference = (10000 - icVal1) + icVal2;
    }

    Distance = Difference * .034 / 2.0;

    isFirtsCaptured = 0; // set it back to false

    // set polarity to rising edge
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, uiDistanceSensorRightChannel,
        TIM_INPUTCHANNELPOLARITY_RISING);
            //__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
  }

  *Is_First_Captured = isFirtsCaptured;
  *IC_Val1 = icVal1;
  *IC_Val2 = icVal2;

  return Distance;
}

void vDistanceSensorInitSensors(TIM_HandleTypeDef *htimTrigger, uint32_t uiTriggerChannel, TIM_HandleTypeDef *htimECOSensors,
     uint32_t uiChannelRight)
{

  pDistanceSensorTrigger = htimTrigger;
  pDistanceSensorTIM = htimECOSensors;

  uiDistanceSensorTriggerChannel = uiTriggerChannel;

  uiDistanceSensorRightChannel = uiChannelRight;


  HAL_TIM_IC_Start_IT(htimECOSensors, uiChannelRight);


  // The PWM Trigger is set for a cycle of 100ms, so we have to set the duty cycle of the PWM to 1/10000 to get 10 us

  HAL_TIM_PWM_Start(pDistanceSensorTrigger, uiTriggerChannel);
  pDistanceSensorTrigger->Instance->CCR1 = 1;
}

void vDistanceSensorSetRightDistance(float fDistance)
{
  fDistanceSensorRightDistance = fDistance;
}


float fDistanceSensorGetRightDistance()
{
  return fDistanceSensorRightDistance;
}


void vDistanceSensorInputCaptureCallback(TIM_HandleTypeDef *htim)
{
  float distancia;

  // Right Sensor
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // if the interrupt source is channel1
  {
    distancia = fDistanceSensorGetSensorDistance(htim, uiDistanceSensorRightChannel,
        &uiIsFirstCapture_Right, &uiICVal1_Right, &uiICVal2_Right);
    if (uiIsFirstCapture_Right == 0)
    {
      vDistanceSensorSetRightDistance(distancia);
    }
  }

}
