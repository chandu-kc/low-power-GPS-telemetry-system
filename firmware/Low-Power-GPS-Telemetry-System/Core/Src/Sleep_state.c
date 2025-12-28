/*
 * Sleep_state.c
 *
 *  Created on: Jul 20, 2025
 *      Author: TSP-L16
 */
#include "main.h"
#include "stm32l1xx_it.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

const char *GPS_DEACTIVE = "AT+CGPS=0\r\n";
const char *FUNCTION_MODE_OFF1 = "AT+CFUN=0\r\n";
const char *SLEEP_SIGN = "AT$QCPURC=\"SLEEP2\",1\r\n";
const char *SLEEP ="AT+PMUCFG=1,3,1000\r\n";
const char *check_vote = "AT$QCVOTECHK\r\n";


void Sleep()
{
	HAL_Delay(200);
	HAL_StatusTypeDef status;

	switch(sleep_state)
	{
	case GNSS_DISCONNECT:

		HAL_UART_Transmit(&huart2, (uint8_t *)"GPS_DEACTIVE:\r\n", strlen("GPS_DEACTIVE:\r\n"), 100);
		status = HAL_UART_Transmit(&huart1, (uint8_t *)GPS_DEACTIVE, strlen(GPS_DEACTIVE), 100);//DISCONNECT GNSS
		if (status != HAL_OK)
		{
		  sleep_state =  GNSS_DISCONNECT;
		  HAL_Delay(100);
		 }
		break;
	case FUNCTION_MODE_OFF:

		//CloseAllNetworkAndCheckSleep();
		HAL_UART_Transmit(&huart2, (uint8_t *)"FUN_DEACTIVE:\r\n", strlen("FUN_DEACTIVE:\r\n"), 100);
		status = HAL_UART_Transmit(&huart1, (uint8_t *)FUNCTION_MODE_OFF1, strlen(FUNCTION_MODE_OFF1), 100);//TURN OF RADIO
		if (status != HAL_OK)
		   {
		     sleep_state = FUNCTION_MODE_OFF;
		     HAL_Delay(100);
		    }

		break;
	case ENABLE_SLEEP:
		 HAL_UART_Transmit(&huart2, (uint8_t *)"SLEEP:\r\n", strlen("SLEEP\r\n"), 100);

		 HAL_UART_Transmit(&huart1, (uint8_t *)SLEEP_SIGN, strlen(SLEEP_SIGN), 100);

		 status = HAL_UART_Transmit(&huart1, (uint8_t *)SLEEP, strlen(SLEEP), 100);
		 if (status != HAL_OK)
		   {
		 	  sleep_state = ENABLE_SLEEP;
		      HAL_Delay(100);
		   }


		break;
	case VOTE_CHECK:
		 status = HAL_UART_Transmit(&huart1, (uint8_t *)check_vote, strlen(check_vote), 100);
		   if (status != HAL_OK)
		    {
		 	  sleep_state = VOTE_CHECK;
		 	  HAL_Delay(100);
		     }

		break;

	case IDLE_SLEEP:
		    //initializing sleep count
        break;




	}

///////////*********************RESPONSE STAGE****************///////////
	 if(strstr(rx_buffer , "OK") && (sleep_state != IDLE_SLEEP) )
	  {
		 //////////////////////
		 if(sleep_state ==  GNSS_DISCONNECT)
		 	{
			 sleep_state = FUNCTION_MODE_OFF;

		 	}
		 else if(sleep_state == FUNCTION_MODE_OFF)
		    {

			   sleep_state = ENABLE_SLEEP;
		 	}
		 else if(sleep_state == ENABLE_SLEEP)
		    {
			  sleep_state = VOTE_CHECK ;

		    }
		 else if(sleep_state == VOTE_CHECK)
		    {

			       // if (strstr(rx_buffer, "httpAtA") || strstr(rx_buffer, "CAVMQTT"))
			          {
			        	//CloseAllNetworkAndCheckSleep();
			        //	sleep_state = ENABLE_SLEEP;
			          }
			       // else if (strstr(rx_buffer, "Prohibit State: NULL"))
			          {
			        	sleep_state = IDLE_SLEEP ;
			          }


		 	}


	  }

	 if(strstr(rx_buffer , "+SLPMODE: 3"))
	 {
		 HAL_UART_Transmit(&huart2, (uint8_t *)"***CAVLI ENTERED SLEEP***\r\n", 29, 100);
		 main_state_flag = 3;
		 current_SEC_sleep = sec_count;
	 }

}

void CloseAllNetworkAndCheckSleep(void)
{

	 // Step 1: Deactivate PDP context
	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGACT=0,1\r\n", strlen("AT+CGACT=0,1\r\n"), 200);
	    HAL_Delay(300);

	    // Step 2: Detach from GPRS network
	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGATT=0\r\n", strlen("AT+CGATT=0\r\n"), 200);
	    HAL_Delay(300);

	    // Step 3: Clear the PDP/APN configuration (Optional)
	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGDCONT=1\r\n", strlen("AT+CGDCONT=1\r\n"), 200);
	    HAL_Delay(300);

	 // Step 1: Turn off GPS
	    const char *GPS_OFF = "AT+CGPS=0\r\n";
	    HAL_UART_Transmit(&huart1, (uint8_t *)GPS_OFF, strlen(GPS_OFF), 100);
	    HAL_Delay(200);

	    // Step 2: Disconnect MQTT
	    const char *MQTT_DISCONN = "AT+MQTTDISCONN=3\r\n"; // Use correct client ID
	    HAL_UART_Transmit(&huart1, (uint8_t *)MQTT_DISCONN, strlen(MQTT_DISCONN), 100);
	    HAL_Delay(200);

	    // Step 3: Delete MQTT Client
	    const char *MQTT_DELETE = "AT+MQTTDELETE=3\r\n";
	    HAL_UART_Transmit(&huart1, (uint8_t *)MQTT_DELETE, strlen(MQTT_DELETE), 100);
	    HAL_Delay(200);

	    // Step 4: Terminate HTTP
	    HAL_UART_Transmit(&huart1, (uint8_t *)"ATE\r\n", strlen("ATE\r\n"), 100);
	    const char *HTTP_TERM = "AT+HTTPTERM\r\n";
	    HAL_UART_Transmit(&huart1, (uint8_t *)HTTP_TERM, strlen(HTTP_TERM), 100);
	    HAL_Delay(200);

	    // Step 5: Detach LTE Network
	    const char *DETACH = "AT+CGATT=0\r\n";
	    HAL_UART_Transmit(&huart1, (uint8_t *)DETACH, strlen(DETACH), 100);
	    HAL_Delay(200);

	    // Step 6: Disable USB blockers (Optional, needed if USB connected)
	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSwTrace\",0\r\n", strlen("AT$QCPCFG=\"usbSwTrace\",0\r\n"), 100);
	    HAL_Delay(100);
	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSlpMask\",1\r\n", strlen("AT$QCPCFG=\"usbSlpMask\",1\r\n"), 100);
	    HAL_Delay(100);
	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSlpThd\",0\r\n", strlen("AT$QCPCFG=\"usbSlpThd\",0\r\n"), 100);
	    HAL_Delay(100);

}





