/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32h7xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization
  *                      and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void
HAL_MspInit(void) {
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* System interrupt init*/
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
* @brief CRC MSP Initialization
* This function configures the hardware resources used in this example
* @param hcrc: CRC handle pointer
* @retval None
*/
void
HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc) {
    if (hcrc->Instance == CRC) {
        /* USER CODE BEGIN CRC_MspInit 0 */

        /* USER CODE END CRC_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_CRC_CLK_ENABLE();
        /* USER CODE BEGIN CRC_MspInit 1 */

        /* USER CODE END CRC_MspInit 1 */
    }

}

/**
* @brief CRC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcrc: CRC handle pointer
* @retval None
*/
void
HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc) {
    if (hcrc->Instance == CRC) {
        /* USER CODE BEGIN CRC_MspDeInit 0 */

        /* USER CODE END CRC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CRC_CLK_DISABLE();
        /* USER CODE BEGIN CRC_MspDeInit 1 */

        /* USER CODE END CRC_MspDeInit 1 */
    }

}

/**
* @brief DMA2D MSP Initialization
* This function configures the hardware resources used in this example
* @param hdma2d: DMA2D handle pointer
* @retval None
*/
void
HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* hdma2d) {
    if (hdma2d->Instance == DMA2D) {
        /* USER CODE BEGIN DMA2D_MspInit 0 */

        /* USER CODE END DMA2D_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_DMA2D_CLK_ENABLE();
        /* DMA2D interrupt Init */
        HAL_NVIC_SetPriority(DMA2D_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(DMA2D_IRQn);
        /* USER CODE BEGIN DMA2D_MspInit 1 */

        /* USER CODE END DMA2D_MspInit 1 */
    }

}

/**
* @brief DMA2D MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdma2d: DMA2D handle pointer
* @retval None
*/
void
HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef* hdma2d) {
    if (hdma2d->Instance == DMA2D) {
        /* USER CODE BEGIN DMA2D_MspDeInit 0 */

        /* USER CODE END DMA2D_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DMA2D_CLK_DISABLE();

        /* DMA2D interrupt DeInit */
        HAL_NVIC_DisableIRQ(DMA2D_IRQn);
        /* USER CODE BEGIN DMA2D_MspDeInit 1 */

        /* USER CODE END DMA2D_MspDeInit 1 */
    }

}

/**
* @brief LTDC MSP Initialization
* This function configures the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void
HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (hltdc->Instance == LTDC) {
        /* USER CODE BEGIN LTDC_MspInit 0 */

        /* USER CODE END LTDC_MspInit 0 */
        /** Initializes the peripherals clock
        */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
        PeriphClkInitStruct.PLL3.PLL3M = 25;
        PeriphClkInitStruct.PLL3.PLL3N = 288;
        PeriphClkInitStruct.PLL3.PLL3P = 2;
        PeriphClkInitStruct.PLL3.PLL3Q = 2;
        PeriphClkInitStruct.PLL3.PLL3R = 30;
        PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
        PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
        PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_LTDC_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOH_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**LTDC GPIO Configuration
        PB8     ------> LTDC_B6
        PD3     ------> LTDC_G7
        PB9     ------> LTDC_B7
        PE0     ------> LTDC_R0
        PA8     ------> LTDC_B3
        PE1     ------> LTDC_R6
        PG14     ------> LTDC_B0
        PD0     ------> LTDC_B1
        PD6     ------> LTDC_B2
        PH15     ------> LTDC_G4
        PC7     ------> LTDC_G6
        PC6     ------> LTDC_HSYNC
        PG7     ------> LTDC_CLK
        PB1     ------> LTDC_G0
        PH11     ------> LTDC_R5
        PH9     ------> LTDC_R3
        PA3     ------> LTDC_B5
        PH8     ------> LTDC_R2
        PH10     ------> LTDC_R4
        PH4     ------> LTDC_G5
        PA4     ------> LTDC_VSYNC
        PE12     ------> LTDC_B4
        PE13     ------> LTDC_DE
        PE15     ------> LTDC_R7
        PH3     ------> LTDC_R1
        PA6     ------> LTDC_G2
        PB0     ------> LTDC_G1
        PE11     ------> LTDC_G3
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_1 | GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12 | GPIO_PIN_13
                              | GPIO_PIN_15 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_8;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF13_LTDC;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_11 | GPIO_PIN_9 | GPIO_PIN_8
                              | GPIO_PIN_10 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        /* LTDC interrupt Init */
        HAL_NVIC_SetPriority(LTDC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(LTDC_IRQn);
        /* USER CODE BEGIN LTDC_MspInit 1 */

        /* USER CODE END LTDC_MspInit 1 */
    }

}

/**
* @brief LTDC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void
HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc) {
    if (hltdc->Instance == LTDC) {
        /* USER CODE BEGIN LTDC_MspDeInit 0 */

        /* USER CODE END LTDC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_LTDC_CLK_DISABLE();

        /**LTDC GPIO Configuration
        PB8     ------> LTDC_B6
        PD3     ------> LTDC_G7
        PB9     ------> LTDC_B7
        PE0     ------> LTDC_R0
        PA8     ------> LTDC_B3
        PE1     ------> LTDC_R6
        PG14     ------> LTDC_B0
        PD0     ------> LTDC_B1
        PD6     ------> LTDC_B2
        PH15     ------> LTDC_G4
        PC7     ------> LTDC_G6
        PC6     ------> LTDC_HSYNC
        PG7     ------> LTDC_CLK
        PB1     ------> LTDC_G0
        PH11     ------> LTDC_R5
        PH9     ------> LTDC_R3
        PA3     ------> LTDC_B5
        PH8     ------> LTDC_R2
        PH10     ------> LTDC_R4
        PH4     ------> LTDC_G5
        PA4     ------> LTDC_VSYNC
        PE12     ------> LTDC_B4
        PE13     ------> LTDC_DE
        PE15     ------> LTDC_R7
        PH3     ------> LTDC_R1
        PA6     ------> LTDC_G2
        PB0     ------> LTDC_G1
        PE11     ------> LTDC_G3
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_1 | GPIO_PIN_0);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_6);

        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12 | GPIO_PIN_13
                        | GPIO_PIN_15 | GPIO_PIN_11);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6);

        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_14 | GPIO_PIN_7);

        HAL_GPIO_DeInit(GPIOH, GPIO_PIN_15 | GPIO_PIN_11 | GPIO_PIN_9 | GPIO_PIN_8
                        | GPIO_PIN_10 | GPIO_PIN_4 | GPIO_PIN_3);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7 | GPIO_PIN_6);

        /* LTDC interrupt DeInit */
        HAL_NVIC_DisableIRQ(LTDC_IRQn);
        /* USER CODE BEGIN LTDC_MspDeInit 1 */

        /* USER CODE END LTDC_MspDeInit 1 */
    }

}

static uint32_t HAL_RCC_OCTOSPIM_CLK_ENABLED = 0;

/**
* @brief OSPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hospi: OSPI handle pointer
* @retval None
*/
void
HAL_OSPI_MspInit(OSPI_HandleTypeDef* hospi) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hospi->Instance == OCTOSPI1) {
        /* USER CODE BEGIN OCTOSPI1_MspInit 0 */

        /* USER CODE END OCTOSPI1_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_OCTOSPIM_CLK_ENABLED++;
        if (HAL_RCC_OCTOSPIM_CLK_ENABLED == 1) {
            __HAL_RCC_OCTOSPIM_CLK_ENABLE();
        }
        __HAL_RCC_OSPI1_CLK_ENABLE();

        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**OCTOSPI1 GPIO Configuration
        PG9     ------> OCTOSPIM_P1_IO6
        PD7     ------> OCTOSPIM_P1_IO7
        PD5     ------> OCTOSPIM_P1_IO5
        PE2     ------> OCTOSPIM_P1_IO2
        PD4     ------> OCTOSPIM_P1_IO4
        PG6     ------> OCTOSPIM_P1_NCS
        PD13     ------> OCTOSPIM_P1_IO3
        PD11     ------> OCTOSPIM_P1_IO0
        PD12     ------> OCTOSPIM_P1_IO1
        PF10     ------> OCTOSPIM_P1_CLK
        PB2     ------> OCTOSPIM_P1_DQS
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN OCTOSPI1_MspInit 1 */

        /* USER CODE END OCTOSPI1_MspInit 1 */
    } else if (hospi->Instance == OCTOSPI2) {
        /* USER CODE BEGIN OCTOSPI2_MspInit 0 */

        /* USER CODE END OCTOSPI2_MspInit 0 */
        /* Peripheral clock enable */
        HAL_RCC_OCTOSPIM_CLK_ENABLED++;
        if (HAL_RCC_OCTOSPIM_CLK_ENABLED == 1) {
            __HAL_RCC_OCTOSPIM_CLK_ENABLE();
        }
        __HAL_RCC_OSPI2_CLK_ENABLE();

        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**OCTOSPI2 GPIO Configuration
        PG11     ------> OCTOSPIM_P2_IO7
        PG10     ------> OCTOSPIM_P2_IO6
        PG12     ------> OCTOSPIM_P2_NCS
        PF1     ------> OCTOSPIM_P2_IO1
        PF0     ------> OCTOSPIM_P2_IO0
        PF2     ------> OCTOSPIM_P2_IO2
        PF4     ------> OCTOSPIM_P2_CLK
        PF3     ------> OCTOSPIM_P2_IO3
        PG1     ------> OCTOSPIM_P2_IO5
        PG0     ------> OCTOSPIM_P2_IO4
        PF12     ------> OCTOSPIM_P2_DQS
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_1 | GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPIM_P2;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4
                              | GPIO_PIN_3 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        /* USER CODE BEGIN OCTOSPI2_MspInit 1 */

        /* USER CODE END OCTOSPI2_MspInit 1 */
    }

}

/**
* @brief OSPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hospi: OSPI handle pointer
* @retval None
*/
void
HAL_OSPI_MspDeInit(OSPI_HandleTypeDef* hospi) {
    if (hospi->Instance == OCTOSPI1) {
        /* USER CODE BEGIN OCTOSPI1_MspDeInit 0 */

        /* USER CODE END OCTOSPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        HAL_RCC_OCTOSPIM_CLK_ENABLED--;
        if (HAL_RCC_OCTOSPIM_CLK_ENABLED == 0) {
            __HAL_RCC_OCTOSPIM_CLK_DISABLE();
        }
        __HAL_RCC_OSPI1_CLK_DISABLE();

        /**OCTOSPI1 GPIO Configuration
        PG9     ------> OCTOSPIM_P1_IO6
        PD7     ------> OCTOSPIM_P1_IO7
        PD5     ------> OCTOSPIM_P1_IO5
        PE2     ------> OCTOSPIM_P1_IO2
        PD4     ------> OCTOSPIM_P1_IO4
        PG6     ------> OCTOSPIM_P1_NCS
        PD13     ------> OCTOSPIM_P1_IO3
        PD11     ------> OCTOSPIM_P1_IO0
        PD12     ------> OCTOSPIM_P1_IO1
        PF10     ------> OCTOSPIM_P1_CLK
        PB2     ------> OCTOSPIM_P1_DQS
        */
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_9 | GPIO_PIN_6);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_13
                        | GPIO_PIN_11 | GPIO_PIN_12);

        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2);

        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);

        /* USER CODE BEGIN OCTOSPI1_MspDeInit 1 */

        /* USER CODE END OCTOSPI1_MspDeInit 1 */
    } else if (hospi->Instance == OCTOSPI2) {
        /* USER CODE BEGIN OCTOSPI2_MspDeInit 0 */

        /* USER CODE END OCTOSPI2_MspDeInit 0 */
        /* Peripheral clock disable */
        HAL_RCC_OCTOSPIM_CLK_ENABLED--;
        if (HAL_RCC_OCTOSPIM_CLK_ENABLED == 0) {
            __HAL_RCC_OCTOSPIM_CLK_DISABLE();
        }
        __HAL_RCC_OSPI2_CLK_DISABLE();

        /**OCTOSPI2 GPIO Configuration
        PG11     ------> OCTOSPIM_P2_IO7
        PG10     ------> OCTOSPIM_P2_IO6
        PG12     ------> OCTOSPIM_P2_NCS
        PF1     ------> OCTOSPIM_P2_IO1
        PF0     ------> OCTOSPIM_P2_IO0
        PF2     ------> OCTOSPIM_P2_IO2
        PF4     ------> OCTOSPIM_P2_CLK
        PF3     ------> OCTOSPIM_P2_IO3
        PG1     ------> OCTOSPIM_P2_IO5
        PG0     ------> OCTOSPIM_P2_IO4
        PF12     ------> OCTOSPIM_P2_DQS
        */
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_1
                        | GPIO_PIN_0);

        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4
                        | GPIO_PIN_3 | GPIO_PIN_12);

        /* USER CODE BEGIN OCTOSPI2_MspDeInit 1 */

        /* USER CODE END OCTOSPI2_MspDeInit 1 */
    }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
