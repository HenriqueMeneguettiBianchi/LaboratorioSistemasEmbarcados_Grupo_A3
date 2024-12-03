// *****************
// File name: sensorUltrassonico.c
//
// Author name:
// Creation date:
// Revision date:
// *****************

#ifndef INC_DISTANCESENSOR_H_
#define INC_DISTANCESENSOR_H_


extern TIM_HandleTypeDef *pDistanceSensorTIM;
extern uint32_t uiDistanceSensorRightChannel;

extern float fDistanceSensorFrontDistance;
extern float fDistanceSensorRightDistance;

void vDistanceSensorInitSensors(TIM_HandleTypeDef *htimTrigger, uint32_t uiTriggerChannel, TIM_HandleTypeDef *htimECOSensors,
    uint32_t uiChannelRight);

void vDistanceSensorSetRightDistance(float fDistance);
float fDistanceSensorGetRightDistance();
void vDistanceSensorInputCaptureCallback(TIM_HandleTypeDef *htim);
float fDistanceSensorGetSensorDistance(TIM_HandleTypeDef *htim, uint32_t uiChannel,
    uint8_t *Is_First_Captured, uint32_t *IC_Val1, uint32_t *IC_Val2);




#endif /* INC_DISTANCESENSOR_H_ */
