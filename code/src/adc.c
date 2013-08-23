#include "nil.h"
#include "threads.h"
#include "stm32f0xx.h"

adc_status_t adc_status = { ADC_STOPPED };
uint16_t adc_samples[20];

void startAdc(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

    ADC_DeInit(ADC1);

    RCC_ADCCLKConfig(RCC_ADCCLK_HSI14); /* Enable ADC1 clock so that we can talk to it */
    RCC_HSI14Cmd(ENABLE);

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_samples;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = sizeof(adc_samples)*sizeof(adc_samples[0]);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA1 Channel1 Transfer Complete interrupt */
//    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Enable DMA1 channel1 IRQ Channel */
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    /* DMA1 Channel1 enable */
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE); // Enable ADC1 EOC interrupt
    /* ADC DMA request in circular mode */
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

    /* Enable ADC_DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);

    /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */
    ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_2 , ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_239_5Cycles);

    ADC_TempSensorCmd(ENABLE);

    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Wait the ADCEN falg */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

    /* ADC1 regular Software Start Conv */
    ADC_StartOfConversion(ADC1);

    adc_status.status = ADC_STARTED;
}

/* DMA1_Channel1_IRQHandler */
void Vector64(void)
{
  /* Test on DMA1 Channel1 Transfer Complete interrupt */
  if(DMA_GetITStatus(DMA1_IT_TC1))
  {
    DMA_ClearITPendingBit(DMA1_IT_GL1);
  }
}
