#include "main.h"
#include "adc.h"
#include "sensorLinha.h"

extern ADC_HandleTypeDef hadc1, hadc2, hadc3, hadc4, hadc5;

float fSensor1, fSensor2, fSensor3, fSensor4, fSensor5;
unsigned int uiSensor1, uiSensor2, uiSensor3, uiSensor4, uiSensor5;

void SensorInit(ADC_HandleTypeDef *hadc) {
    HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED);
    HAL_ADC_Start(hadc);
}

float fSensorLinhaLeituraSensor() {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uiSensor1 = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Start(&hadc2);
    HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
    uiSensor2 = HAL_ADC_GetValue(&hadc2);

    HAL_ADC_Start(&hadc3);
    HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
    uiSensor3 = HAL_ADC_GetValue(&hadc3);

    HAL_ADC_Start(&hadc4);
    HAL_ADC_PollForConversion(&hadc4, HAL_MAX_DELAY);
    uiSensor4 = HAL_ADC_GetValue(&hadc4);

    HAL_ADC_Start(&hadc5);
    HAL_ADC_PollForConversion(&hadc5, HAL_MAX_DELAY);
    uiSensor5 = HAL_ADC_GetValue(&hadc5);

    float total = uiSensor1 + uiSensor2 + uiSensor3 + uiSensor4 + uiSensor5;
    if (total == 0) return 0; // Para evitar divisão por zero
    float posicao = (1 * uiSensor1 + 2 * uiSensor2 + 3 * uiSensor3 + 4 * uiSensor4 + 5 * uiSensor5) / total;

    return posicao;
}
