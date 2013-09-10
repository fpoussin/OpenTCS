#include "threads.h"

#define SPEED_TIMER TIM2
#define SPEED_TIMER_IRQn TIM2_IRQn
#define SPEED_TIMER_IRQHandler TIM2_IRQHandler
#define SPEED_TIMER_PSC (STM32_PCLK/100000) // 100KHz clock, takes 0.65s to wrap

#define RPM_TIMER TIM1
#define RPM_TIMER_IRQn TIM1_CC_IRQn
#define RPM_TIMER_IRQHandler TIM1_CC_IRQHandler
#define RPM_TIMER_PSC (STM32_PCLK/100000) // 100KHz clock, takes 0.65s to wrap

#define POT_I2C I2C1
#define POT_I2C_ADDR 0x2E /* MCP45X1 ‘0101 11’b + A0 */

#define POT_CMD_SET_WIPER 0x40

/*  MCP454X Digital pot
 *
 * ‘1000 00d’b Write Next Byte (Third Byte) to Volatile
    Wiper 0 Register
    ‘1001 00d’b Write Next Byte (Third Byte) to Volatile
    Wiper 1 Register
    ‘1100 00d’b Write Next Byte (Third Byte) to TCON
    Register
    ‘1000 010’b
    or
    ‘1000 011’b
    Increment Wiper 0 Register
    ‘1001 010’b
    or
    ‘1001 011’b
    Increment Wiper 1 Register
    ‘1000 100’b
    or
    ‘1000 101’b
    Decrement Wiper 0 Register
    ‘1001 100’b
    or
    ‘1001 101’b
    Decrement Wiper 1 Register
 *
 */


sensors_t sensors = {0, 0, 0, 0, 0};
static uint8_t TIM1CC4CaptureNumber, TIM2CC3CaptureNumber, TIM2CC4CaptureNumber;
static uint16_t TIM1CC4ReadValue1, TIM1CC4ReadValue2;
static uint16_t TIM2CC3ReadValue1, TIM2CC3ReadValue2;
static uint16_t TIM2CC4ReadValue1, TIM2CC4ReadValue2;
static int32_t accel1 = 0, accel2 = 0;
static int32_t spd1arr[2] = {0,0}, spd2arr[2] = {0,0};
static int8_t spd1arr_pos = 0, spd2arr_pos = 0;

/*
 * Function prototypes.
 */

void getCapture(void);
void getStrainGauge(void);
uint8_t setPotGain(uint8_t gain);
void getSpeedSensors(void);

/*
 * Actual functions.
 */

void startSensors(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = SPEED_TIMER_PSC - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(SPEED_TIMER, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x10;
    TIM_ICInit(SPEED_TIMER, &TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(SPEED_TIMER, &TIM_ICInitStructure);

    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = SPEED_TIMER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM enable counter */
    TIM_Cmd(SPEED_TIMER, ENABLE);

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = RPM_TIMER_PSC - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(RPM_TIMER, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x10;
    TIM_ICInit(RPM_TIMER, &TIM_ICInitStructure);

    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RPM_TIMER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM enable counter */
    TIM_Cmd(RPM_TIMER, ENABLE);

    i2cInit(POT_I2C);
    setPotGain(settings.data.sensor_gain);

    serDbg("startSensors Complete\r\n");

    char tmpstr[6];
    while (true)
    {
        /*
         * Almost everything happens within IRQ Handlers.
         */
        getStrainGauge();
        getSpeedSensors();
        getCapture();
        nilThdSleepMilliseconds(100);

//        serDbg("Accel 1/2: ");
//        itoa(accel1, tmpstr);
//        serDbg(tmpstr);
//        serDbg("/");
//        itoa(accel2, tmpstr);
//        serDbg(tmpstr);
//        serDbg("\r\n");

        serDbg("RPM: ");
        itoa(sensors.rpm, tmpstr);
        serDbg(tmpstr);
        serDbg("\r\n");
    }
}

void getCapture(void)
{
    /* Enable the CC4 Interrupt Request */
    RPM_TIMER->DIER |= TIM_DIER_CC4IE;

    /* Enable the CC3-4 Interrupt Request */
    SPEED_TIMER->DIER |= TIM_DIER_CC3IE;
    SPEED_TIMER->DIER |= TIM_DIER_CC4IE;
}

uint8_t setPotGain(uint8_t gain)
{
    uint8_t txdata[2] = {POT_CMD_SET_WIPER | ((gain & 0x80) >> 7), gain & 0x7F};
//    uint8_t rxdata[2];
//    uint8_t pot_val;

    if (i2cSendS(POT_I2C, POT_I2C_ADDR, txdata, sizeof(txdata)) != 0)
    {
        /* Handle error */
        return 1;
    }

//    if (i2cReceiveS(POT_I2C, POT_I2C_ADDR, rxdata, sizeof(rxdata)) != 0)
//    {
//        /* Handle error */
//        return 1;
//    }

//    pot_val = (rxdata[0] & 0x80) | (rxdata[1] & 0x7F);

//    return pot_val;
    return 0;
}

void getStrainGauge(void)
{
    uint32_t strain_gauge = 0;
    uint8_t nb_samples;

    nb_samples = sizeof(adc_samples)/ADC_CHANNELS;

    while (nb_samples)
    {
        strain_gauge += adc_samples[ADC_CHANNELS*--nb_samples];
    }

    strain_gauge /= sizeof(adc_samples)/ADC_CHANNELS;

    /* Returns true is strain gauge voltage exceeds threshold. */
    sensors.shifting = (strain_gauge >= settings.data.sensor_threshold);
}

void getSpeedSensors(void)
{
    // Fastest wheel speed in Hertz
    if (spd1arr[0] >= spd2arr[0]) sensors.spd = spd1arr[0];
    else sensors.spd = spd2arr[0];
}


void RPM_TIMER_IRQHandler(void)
{
    uint32_t Capture;

    if (RPM_TIMER->SR & TIM_IT_CC4)
    {  /* capture timer */

        //clear pending bit
        RPM_TIMER->SR &= ~TIM_FLAG_CC4;

        if(TIM1CC4CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            TIM1CC4ReadValue1 = RPM_TIMER->CCR4;
            TIM1CC4CaptureNumber = 1;
        }
        else if(TIM1CC4CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            TIM1CC4ReadValue2 = RPM_TIMER->CCR4;

            /* Capture computation */
            if (TIM1CC4ReadValue2 > TIM1CC4ReadValue1)
            {
                Capture = ((uint32_t)TIM1CC4ReadValue2 - (uint32_t)TIM1CC4ReadValue1);
            }
            else
            {
                Capture = (((uint32_t)TIM1CC4ReadValue2 + 0x10000) - (uint32_t)TIM1CC4ReadValue1);
            }

            /* Frequency computation */
            sensors.rpm = ((STM32_PCLK / RPM_TIMER_PSC) / Capture) * 60;

            TIM1CC4ReadValue1 = TIM1CC4ReadValue2;
            TIM1CC4CaptureNumber = 0;

            /* Disable CC4 interrupt */
            RPM_TIMER->DIER &= ~TIM_DIER_CC4IE;
        }
    }
}

void SPEED_TIMER_IRQHandler(void)
{
    uint32_t Capture = 0;

    if (SPEED_TIMER->SR & TIM_IT_CC3)
    {  /* capture timer */

        //clear pending bit
        SPEED_TIMER->SR &= ~TIM_FLAG_CC3;

        if(TIM2CC3CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            TIM2CC3ReadValue1 = SPEED_TIMER->CCR3;
            TIM2CC3CaptureNumber = 1;
        }
        else if(TIM2CC3CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            TIM2CC3ReadValue2 = SPEED_TIMER->CCR3;

            /* Capture computation */
            if (TIM2CC3ReadValue2 > TIM2CC3ReadValue1)
            {
                Capture = ((uint32_t)TIM2CC3ReadValue2 - (uint32_t)TIM2CC3ReadValue1);
            }
            else
            {
                Capture = (((uint32_t)TIM2CC3ReadValue2 + 0x10000) - (uint32_t)TIM2CC3ReadValue1);
            }

            /* Frequency computation */
            spd1arr[spd1arr_pos++] = (STM32_PCLK / SPEED_TIMER_PSC) / Capture;

            if (spd1arr_pos > 1) spd1arr_pos = 0;

            TIM2CC3ReadValue1 = TIM2CC3ReadValue2;
            TIM2CC3CaptureNumber = 0;

            /* Disable CC3 interrupt */
            SPEED_TIMER->DIER &= ~TIM_DIER_CC3IE;
        }
    }

    else if (SPEED_TIMER->SR & TIM_IT_CC4)
    {  /* capture timer */

        //clear pending bit
        SPEED_TIMER->SR &= ~TIM_FLAG_CC4;

        if(TIM2CC4CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            TIM2CC4ReadValue1 = SPEED_TIMER->CCR4;
            TIM2CC4CaptureNumber = 1;
        }
        else if(TIM2CC4CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            TIM2CC4ReadValue2 = SPEED_TIMER->CCR4;

            /* Capture computation */
            if (TIM2CC4ReadValue2 > TIM2CC4ReadValue1)
            {
                Capture = ((uint32_t)TIM2CC4ReadValue2 - (uint32_t)TIM2CC4ReadValue1);
            }
            else
            {
                Capture = (((uint32_t)TIM2CC4ReadValue2 + 0x10000) - (uint32_t)TIM2CC4ReadValue1);
            }

            /* Frequency computation */
            spd2arr[spd2arr_pos++] = (STM32_PCLK / SPEED_TIMER_PSC) / Capture;

            if (spd2arr_pos > 1) spd2arr_pos = 0;

            TIM2CC4ReadValue1 = TIM2CC4ReadValue2;
            TIM2CC4CaptureNumber = 0;

            /* Disable CC4 interrupt */
            SPEED_TIMER->DIER &= ~TIM_DIER_CC4IE;
        }
    }

    if (spd1arr_pos == 0)
    {
        accel1 = (spd1arr[1] - spd1arr[0]);
    }
    else
    {
        accel1 = (spd1arr[0] - spd1arr[1]);
    }

    if (spd2arr_pos == 0)
    {
        accel2 = (spd2arr[1] - spd2arr[0]);
    }
    else
    {
        accel2 = (spd2arr[0] - spd2arr[1]);
    }


    // Fastest wheel speed in Hertz
    if (spd1arr[0] >= spd2arr[0]) sensors.spd = spd1arr[0];
    else  sensors.spd = spd2arr[0];

    // Fastest wheel accel
    if (accel1 >= accel2) sensors.accel = accel1;
    else  sensors.accel = accel2;

    if (sensors.spd <= settings.data.min_speed || sensors.rpm <= settings.data.min_rpm)
    {
        sensors.slipping_pct = 0;
        return;
    }

    sensors.slipping_pct = ((accel2 - accel1) * 100) / accel1;
}
