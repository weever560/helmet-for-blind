/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SYN6288.h"
#include <string.h>
uint8_t rx[10];
//人的索引是14
char *Data_Obj[20]={"飞机","自行车","小鸟","船只","水瓶","公交车","轿车","小猫","椅子","牛","餐桌","小狗","马","摩托","人","植物","绵羊","沙发","火车","遥控器"};

typedef struct {
	
	uint8_t obj[20];
	uint8_t count;
}Data_pool;
Data_pool Data_pools;
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	
//	SYN_FrameInfo("[m5][v5][t5]前方有");//成功发送
	
	Data_pools.count=0;
	memset(Data_pools.obj, -1, sizeof(20));
	HAL_UART_Receive_IT(&huart1,rx,3);

	char temp[20]={'\0'};
	uint8_t flag=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		if(Data_pools.count==0)
		{
			HAL_UART_Transmit(&huart1, "nothing\r\n", sizeof( "nothing\r\n"),10000);
		}
		else
		{
			sprintf(temp,"[m5][v1][t5]%s\r\n",Data_Obj[Data_pools.obj[Data_pools.count-1]]);
			HAL_UART_Transmit(&huart1, temp, sizeof(temp),10000);
			SYN_FrameInfo(temp);
		}
			
		if(flag<10)
		{
			flag++;
		}
		else
		{
			flag=0;//清空
				Data_pools.count=0;
				memset(Data_pools.obj, -1, sizeof(20));
		}
			
		HAL_Delay(5000);
		
    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t tmp;
	if(huart->Instance == USART1)
	{
		if(rx[0]==0x0A)//0A
		{
			HAL_UART_Transmit(&huart1, "双2\r\n", sizeof("双2\r\n"),10000);
			HAL_UART_Receive_IT(&huart1, rx, 3);
			
		}	
		else if(rx[1]==0x0D)	//31 0D 0A
		{
			tmp = rx[0]-0x30;//Data_Obj[tmp]是读取到的数据
			
			
			//查找是否有相同数字的
			char temp[20]={'\0'};
			for(int i=0 ; i<=(Data_pools.count) ;i++ )//检查有没有重复的
			{
					if(tmp==Data_pools.obj[i])//如果有重复就不加
					{
						HAL_UART_Transmit(&huart1, "cf\r\n", sizeof("cf\r\n"),10000);
						break;
					}
					else if( i==(Data_pools.count) )//读到最后都没有重复就加
					{
						Data_pools.obj[Data_pools.count]=tmp;
						sprintf(temp,"%d %d %s\r\n",Data_pools.count,Data_pools.obj[Data_pools.count],Data_Obj[Data_pools.obj[Data_pools.count]]);
						HAL_UART_Transmit(&huart1, temp, sizeof(temp),10000);
						Data_pools.count++;
						break;
					}
			}
			
			
//			sprintf(temp,"%d%s\r\n",Data_pools.count,Data_Obj[Data_pools.obj[Data_pools.count]]);
//			HAL_UART_Transmit(&huart1, temp, sizeof(temp),10000);
			
	//		SYN_FrameInfo(Data_Obj[tmp]);
			HAL_UART_Transmit(&huart1, "\r\n", sizeof("\r\n"),10000);
			HAL_UART_Transmit(&huart1, "单\r\n", sizeof("单\r\n"),10000);
			HAL_UART_Receive_IT(&huart1, rx, 3);
			
		}
		else if(rx[2]==0x0D)	//31 34 0D 0A
		{
			//提取出来
			tmp = rx[1]-0x30;//Data_Obj[tmp]是读取到的数据
			tmp += 10;//变成两位数0~19
			
			
			//查找是否有相同数字的
			char temp[20]={'\0'};
			for(int i=0 ; i<=(Data_pools.count) ;i++ )//检查有没有重复的
			{
					if(tmp==Data_pools.obj[i])//如果有重复就不加
					{
						HAL_UART_Transmit(&huart1, "cf\r\n", sizeof("cf\r\n"),10000);
						break;
					}
					else if( i==(Data_pools.count) )//读到最后都没有重复就加
					{
						Data_pools.obj[Data_pools.count]=tmp;
						sprintf(temp,"%d %d %s\r\n",Data_pools.count,Data_pools.obj[Data_pools.count],Data_Obj[Data_pools.obj[Data_pools.count]]);
						HAL_UART_Transmit(&huart1, temp, sizeof(temp),10000);
						Data_pools.count++;
						break;
					}
			}
			
			HAL_UART_Transmit(&huart1, "双1\r\n", sizeof("双1\r\n"),10000);
			HAL_UART_Receive_IT(&huart1, rx, 1);
		}
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
