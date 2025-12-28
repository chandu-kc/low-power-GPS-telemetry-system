/*
 * Waiting_to_Wake_up.c
 *
 *  Created on: Jul 20, 2025
 *      Author: TSP-L16
 */
#include "main.h"
#include "stm32l1xx_it.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern int SLEEP_TIME_SEC;


void Wake_UP()
{

	  if((sec_count - current_SEC_sleep) > SLEEP_TIME_SEC )
	  {
		  gps_state = GPS_STATE_ACTIVATE;
		  mqtt_state = MQTT_STATE_CREATE;
		  sleep_state =  GNSS_DISCONNECT;//again reassign the sleep state
		  gps_data_found_flag = 0;
          HAL_UART_Transmit(&huart2, (uint8_t *)"***CAVLI Wakeup From SLEEP***\r\n", 33, 100);

          HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CFUN=1\r\n", strlen("AT+CFUN=1\r\n"), 100);
		   HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CFUN=1\r\n", strlen("AT+CFUN=1\r\n"), 100);   // triggering cavli to wake up
		  //if(strstr(rx_buffer , "OK"))
		 	  {
		 		 HAL_UART_Transmit(&huart2, (uint8_t *)"***CAVLI ACTIVE***\r\n", 22, 100);
		 		// HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CFUN=1\r\n", strlen("AT+CFUN=1\r\n"), 100);//TURN ON RADIO

		 		 NETWORK_SET_UP();
		 		 main_state_flag = 0;
		 		 sec_count = 0;
		 	  }

	  }
}


