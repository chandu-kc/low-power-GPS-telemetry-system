/*
 * NETWORK_SUPPORT.c
 *
 *  Created on: Jul 22, 2025
 *      Author: TSP-L16
 */

#include "main.h"
#include "stm32l1xx_it.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void NETWORK_SET_UP()
{
    // 1. Basic AT test
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT\r\n", strlen("AT\r\n"), 200);
    HAL_Delay(300);


    // 2. Disable echo
    HAL_UART_Transmit(&huart1, (uint8_t *)"ATE0\r\n", strlen("ATE0\r\n"), 200);
    HAL_Delay(300);


    // 3. Set APN
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n", strlen("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n"), 200);
    HAL_Delay(300);


    // 4. Attach to GPRS
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGATT=1\r\n", strlen("AT+CGATT=1\r\n"), 200);
    HAL_Delay(300);


    // 5. Activate PDP context
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGACT=1,1\r\n", strlen("AT+CGACT=1,1\r\n"), 200);
    HAL_Delay(300);


    // 6. Confirm PDP with IP
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGPADDR=1\r\n", strlen("AT+CGPADDR=1\r\n"), 200);
    HAL_Delay(1000);


    // Check if "+CGPADDR: 1," exists in response (basic check)

    if(strstr(rx_buffer , "+CGPADDR: 1,") != NULL)
    {

    	  HAL_UART_Transmit(&huart2, (uint8_t *)"CONNECTED TO NETWORK\r\n", 23, 100);
    	  HAL_Delay(1000);


    } else {
    	  HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CFUN=1\r\n", strlen("AT+CFUN=1\r\n"), 100);//TURN ON RADIO
    	  HAL_Delay(1000);

    	  HAL_UART_Transmit(&huart2, (uint8_t *)"FAILED TO CONNECT NETWORK\r\n", 29, 100);
    	  NETWORK_SET_UP();

    }
}


