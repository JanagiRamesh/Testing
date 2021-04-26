/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Custom_Libs/Inc/CAN_Driver.h"
#include "../Custom_Libs/Inc/Debug.h"
#include "../Custom_Libs/Inc/CAN_Application.h"
#include "../Custom_Libs/Inc/Enumeration.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_TIM2_Init();
  MX_UART10_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  CANSetup();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t data[8]={10,20,30,40,50,60,70,80};
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

/*  *** Light Dependent Resistor ***    */
	  uint16_t ldr_value = 0, ldr_buff[16];

	  HAL_ADC_PollForConversion(&hadc1, 10);
	  ldr_value = HAL_ADC_GetValue(&hadc1);
	  sprintf(ldr_buff,"%lu",ldr_value);
	  __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, ldr_value);
	  DebugPrint(ldr_buff);

/*  *** Negative Temprature Sensor *** */
	  uint32_t adc_value=0;

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1,100);
	  adc_value = HAL_ADC_GetValue(&hadc1);
	  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,adc_value);

/***pot****/
	  uint32_t buff[16];

	  HAL_ADC_PollForConversion(&hadc1, 10);
	  adc_value = HAL_ADC_GetValue(&hadc1);
	  sprintf(buff,"%lu",adc_value);
	  __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, adc_value);
	  DebugPrint(buff);


/* ***   Interfacing with DC MOTOR ***     */
/*  ***  if (IN-1 = 0) and (IN-2 = 0) Motor Should in IDLE State  ***  */
	  if((HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_RESET) && (HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_RESET))
	  {
		  HAL_GPIO_WritePin(Motor_Driver_1_GPIO_Port, Motor_Driver_1_Pin,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(Motor_Driver_2_GPIO_Port, Motor_Driver_2_Pin,GPIO_PIN_RESET);
	  }

/*  ***  if (IN-1 = 0) and (IN-2 = 1) Motor Should Anti-Clockwise State  ***  */
	  if((HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_RESET) && (HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_SET))
	  {
		  HAL_GPIO_WritePin(Motor_Driver_1_GPIO_Port, Motor_Driver_1_Pin,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(Motor_Driver_2_GPIO_Port, Motor_Driver_2_Pin,GPIO_PIN_SET);
	  }

/*  ***  if (IN-1 = 1) and (IN-2 = 0) Motor Should Clockwise State  ***  */
	  if((HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_SET) && (HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_RESET))
	  {
		  HAL_GPIO_WritePin(Motor_Driver_1_GPIO_Port, Motor_Driver_1_Pin,GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Motor_Driver_2_GPIO_Port, Motor_Driver_2_Pin,GPIO_PIN_RESET);
	  }

/*  ***  if (IN-1 = 1) and (IN-2 = 1) Motor Should in IDLE State  ***  */
	  if((HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_SET) && (HAL_GPIO_ReadPin(DC_Motor_IN1_GPIO_Port,DC_Motor_IN1_Pin) == GPIO_PIN_SET))
	  {
		  HAL_GPIO_WritePin(Motor_Driver_1_GPIO_Port, Motor_Driver_1_Pin,GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Motor_Driver_2_GPIO_Port, Motor_Driver_2_Pin,GPIO_PIN_SET);
	  }



/*   *** CAN Transmission  ***   */
	  Transmit_CANID(ID1,S,data,8);
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 72;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 13;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 3;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLI2SQ;
  PeriphClkInitStruct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
/*  *** Bulb Interfacing ***      */
	if(GPIO_Pin == Lamp_Input_Pin)
	{
		HAL_GPIO_WritePin(Lamp_Output_GPIO_Port, Lamp_Output_Pin,GPIO_PIN_SET);
	}

/*  *** CPU FAN-Interfacing ***      */
	if(GPIO_Pin == CPU_FAN_Input_Pin)
	{
		HAL_GPIO_WritePin(CPU_FAN_Output_GPIO_Port, CPU_FAN_Output_Pin,GPIO_PIN_SET);
	}

/*  *** Relay Interfacing ***  */
	if(GPIO_Pin == Relay_Input_Pin)
	{
		HAL_GPIO_WritePin(Relay_Output_GPIO_Port, Relay_Output_Pin,GPIO_PIN_SET);
	}

/*  *** SubMersible Water Pump *** */
	if(GPIO_Pin == SubmerciblePumpIn_Pin)
	{
		HAL_GPIO_WritePin(SubmerciblePumpOut_GPIO_Port,SubmerciblePumpOut_Pin,GPIO_PIN_SET);
	}

/*  *** SubMersible Water Pump *** */
	if(GPIO_Pin == ON_OFF_Input_GPIO_Port)
	{
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
