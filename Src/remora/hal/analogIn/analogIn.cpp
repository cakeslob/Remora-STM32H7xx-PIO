#include "analogIn.h"
#include <cstdio>

AnalogIn::AnalogIn(const std::string& portAndPin) 
    : portAndPin(portAndPin)
{
    uint32_t function = (uint32_t)NC;

    if (portAndPin[0] == 'P') {
        portIndex = portAndPin[1] - 'A';
        pinNumber = std::stoi(portAndPin.substr(3));
        pin = 1 << pinNumber;
    } else {
        printf("Invalid port and pin definition\n");
        return;
    }

    pinName = static_cast<PinName>((portIndex << 4) | pinNumber);

    // Get the peripheral name from the pinName
    handle.Instance = (ADC_TypeDef *)pinmap_peripheral(pinName, PinMap_ADC);

    // Get the functions (adc channel) from the pinName
    function = pinmap_function(pinName, PinMap_ADC);

    // Configure GPIO
    analogInPin = new Pin(portAndPin, STM_PIN_FUNCTION(function));

    // Analog clock configuration
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_CLKP;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        printf("analogin_init HAL_RCCEx_PeriphCLKConfig");
    }

    channel = STM_PIN_CHANNEL(function);
    differential = STM_PIN_INVERTED(function);

    // PER_CLK is set to 64MHz HSI by SetSystemClock().  We want 32MHz for the ADC clock since
    // that's the highest we can get to that's under 50MHz.
    // On STM32H74x rev Y, the ADC does not have an internal divider so we need to divide the clock
    // by 2.  On all other STM32H7 chips, there is an internal /2 divider.
#ifdef TARGET_STM32H7_480MHZ
    if(HAL_GetREVID() <= REV_ID_Y)
    {
        handle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV2;
    }
    else
#endif
    {
        handle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
    }
    handle.Init.Resolution               = ADC_RESOLUTION_16B;
    handle.Init.ScanConvMode             = ADC_SCAN_DISABLE;
    handle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
    handle.Init.LowPowerAutoWait         = DISABLE;
    handle.Init.ContinuousConvMode       = DISABLE;
    handle.Init.NbrOfConversion          = 1;
    handle.Init.DiscontinuousConvMode    = DISABLE;
    handle.Init.NbrOfDiscConversion      = 0;
    handle.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
    handle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    handle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;
    handle.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    handle.Init.OversamplingMode         = DISABLE;

if ((ADCName)(reinterpret_cast<int>(handle.Instance)) == ADC_1) {
    __HAL_RCC_ADC12_CLK_ENABLE();
}

if ((ADCName)(reinterpret_cast<int>(handle.Instance)) == ADC_2) {
    __HAL_RCC_ADC12_CLK_ENABLE();
}

if ((ADCName)(reinterpret_cast<int>(handle.Instance)) == ADC_3) {
    __HAL_RCC_ADC3_CLK_ENABLE();
}

    if (HAL_ADC_Init(&handle) != HAL_OK) {
        printf("analogin_init HAL_ADC_Init");
    }

    if ((ADCName)(reinterpret_cast<int>(handle.Instance)) == ADC_1) {
        ADC_MultiModeTypeDef multimode = {0};
        multimode.Mode = ADC_MODE_INDEPENDENT;
        if (HAL_ADCEx_MultiModeConfigChannel(&handle, &multimode) != HAL_OK) {
            printf("HAL_ADCEx_MultiModeConfigChannel HAL_ADC_Init");
        }
    }

    // Calibration
    if (differential) {
        HAL_ADCEx_Calibration_Start(&handle, ADC_CALIB_OFFSET, ADC_DIFFERENTIAL_ENDED);
    } else {
        HAL_ADCEx_Calibration_Start(&handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    }
}

uint16_t AnalogIn::read()
{
   ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC channel
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
    sConfig.Offset       = 0;
    if (differential) {
        sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
    } else {
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
    }
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.OffsetRightShift       = DISABLE;
        sConfig.OffsetSignedSaturation = DISABLE;

    switch (channel) {
        case 0:
            sConfig.Channel = ADC_CHANNEL_0;
            break;
        case 1:
            sConfig.Channel = ADC_CHANNEL_1;
            break;
        case 2:
            sConfig.Channel = ADC_CHANNEL_2;
            break;
        case 3:
            sConfig.Channel = ADC_CHANNEL_3;
            break;
        case 4:
            sConfig.Channel = ADC_CHANNEL_4;
            break;
        case 5:
            sConfig.Channel = ADC_CHANNEL_5;
            break;
        case 6:
            sConfig.Channel = ADC_CHANNEL_6;
            break;
        case 7:
            sConfig.Channel = ADC_CHANNEL_7;
            break;
        case 8:
            sConfig.Channel = ADC_CHANNEL_8;
            break;
        case 9:
            sConfig.Channel = ADC_CHANNEL_9;
            break;
        case 10:
            sConfig.Channel = ADC_CHANNEL_10;
            break;
        case 11:
            sConfig.Channel = ADC_CHANNEL_11;
            break;
        case 12:
            sConfig.Channel = ADC_CHANNEL_12;
            break;
        case 13:
            sConfig.Channel = ADC_CHANNEL_13;
            break;
        case 14:
            sConfig.Channel = ADC_CHANNEL_14;
            break;
        case 15:
            sConfig.Channel = ADC_CHANNEL_15;
            break;
        case 16:
            sConfig.Channel = ADC_CHANNEL_16;
            break;
        case 17:
            sConfig.Channel = ADC_CHANNEL_17;
            break;
        case 18:
            sConfig.Channel = ADC_CHANNEL_18;
            break;
        case 19:
            sConfig.Channel = ADC_CHANNEL_19;
            break;
        default:
            return 0;
    }


    if (HAL_ADC_ConfigChannel(&handle, &sConfig) != HAL_OK) {
        printf("HAL_ADC_ConfigChannel issue");
    }

    if (HAL_ADC_Start(&handle) != HAL_OK) {
        printf("HAL_ADC_Start issue");
    }

    // Wait end of conversion and get value
    uint16_t adcValue = 0;
    if (HAL_ADC_PollForConversion(&handle, 10) == HAL_OK) {
        adcValue = (uint16_t)HAL_ADC_GetValue(&handle);
    }

    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE((&handle)->Instance), LL_ADC_PATH_INTERNAL_NONE);

    if (HAL_ADC_Stop(&handle) != HAL_OK) {
        printf("HAL_ADC_Stop issue");
    }

    return adcValue;    
}

