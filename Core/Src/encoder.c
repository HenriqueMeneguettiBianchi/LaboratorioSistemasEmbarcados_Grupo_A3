#include "main.h"
#include "tim.h"

#define CONTADOR_MAX 50000
#define FATOR_MULTIPLICACAO_VELOCIDADE 20.41f // 2*3,14*3,25 --> 3,25 Raio do encoder medido em cm
#define FREQUENCIA_CLOCK   170000000
#define PRESCALAR_TIMER  1700

static uint32_t valorCaptura1 = 0;
static uint32_t valorCaptura2 = 0;
static uint32_t valorCaptura3 = 0;
static uint32_t valorCaptura4 = 0;
static uint32_t diferencaTempo = 0;
static int flagPrimeiraCaptura = 0;
static uint32_t diferencaTempo2 = 0;
static int flagPrimeiraCaptura2 = 0;
int overflow1 = 0;
int overflow2 = 0;
int contadorMaximo = 50000;


static float frequenciaRodaEsquerda = 0;
static float frequenciaRodaDireita = 0;
float velocidadeRodaEsquerda = 0;
float velocidadeRodaDireita = 0;

void inicializarEncoders(TIM_HandleTypeDef *timer16, TIM_HandleTypeDef *timer17) {
    HAL_TIM_IC_Start_IT(timer16, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(timer17, TIM_CHANNEL_1);
    HAL_TIM_Base_Start_IT(timer16);
    HAL_TIM_Base_Start_IT(timer17);
}

void processarCapturaRodaEsquerda(TIM_HandleTypeDef *htim) {
    if (flagPrimeiraCaptura == 0)
    {	overflow1 =0;
    	valorCaptura1 = htim->Instance->CCR1;
        flagPrimeiraCaptura = 1;
    } else {
        valorCaptura2 = htim->Instance -> CCR1;
        valorCaptura2 += contadorMaximo*overflow1;
        diferencaTempo = (valorCaptura2 - valorCaptura1);
        float clockReferencia = FREQUENCIA_CLOCK / PRESCALAR_TIMER;
        frequenciaRodaEsquerda = clockReferencia / diferencaTempo;
        velocidadeRodaEsquerda = (frequenciaRodaEsquerda / 20) * FATOR_MULTIPLICACAO_VELOCIDADE;

        __HAL_TIM_SET_COUNTER(htim, 0);
        flagPrimeiraCaptura = 0;
        overflow1 =0;
    }
}

void processarCapturaRodaDireita(TIM_HandleTypeDef *htim) {
    if (flagPrimeiraCaptura2 == 0)
    {	overflow2 =0;
    	valorCaptura3 = htim->Instance->CCR1;
        flagPrimeiraCaptura2 = 1;
    } else {
        valorCaptura4 = htim->Instance -> CCR1;
        valorCaptura4 += contadorMaximo*overflow2;
        diferencaTempo2 = (valorCaptura4 - valorCaptura3);
        float clockReferencia = FREQUENCIA_CLOCK / PRESCALAR_TIMER;
        frequenciaRodaDireita = clockReferencia / diferencaTempo2;
        velocidadeRodaDireita = (frequenciaRodaDireita / 20) * FATOR_MULTIPLICACAO_VELOCIDADE;

        __HAL_TIM_SET_COUNTER(htim, 0);
        flagPrimeiraCaptura2 = 0;
        overflow2 =0;
    }
}




float fGetVelocidadeRodaEsquerda(void)
{
    return velocidadeRodaEsquerda;
}

float fGetVelocidadeRodaDireita(void)
{
    return velocidadeRodaDireita;
}
__weak void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim);

