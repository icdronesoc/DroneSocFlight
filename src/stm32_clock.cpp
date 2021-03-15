#ifdef PLATFORM_STM32
#include <Arduino.h>

extern "C" void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure the main internal regulator output voltage
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Configure the clock source (HSE) and PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = CLOCK_FREQUENCY / 2; // VCO Input Clock target is 2MHz, External Clock / Divider = 2 so Divider = External Clock / 2
    RCC_OscInitStruct.PLL.PLLN = CORE_FREQUENCY; // VCO Output Clock is core frequency * 2 (due to 2MHz input)
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // Core Clock is VCO Output clock / 2
    RCC_OscInitStruct.PLL.PLLQ = CORE_FREQUENCY * 2 / 48; // USB Clock 48MHz = VCO Output / Divider so Divider = VCO Output / 48 = Core Frequency * 2 / 48 TODO 1.5x divider for F411
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    // Initialize the CPU, AHB and APB buses clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) Error_Handler();
}
#endif
