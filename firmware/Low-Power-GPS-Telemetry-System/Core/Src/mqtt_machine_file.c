/*
 * mqtt_machine_file.c
 *
 *  Created on: Jul 20, 2025
 *      Author: TSP-L16
 */
#include "main.h"
#include "stm32l1xx_it.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

// ======= AT Command Constants =======
const char *AT_CHECK        = "AT\r\n";
const char *AT_MQTT_CREATE  = "AT+MQTTCREATE=\"whizblox.com\",1884,\"client3\",25,1,\"admin\",\"admin\"\r\n";
const char *AT_MQTT_CONN    = "AT+MQTTCONN=3,0,30\r\n";
const char *AT_MQTT_PUB1    = "AT+MQTTPUB=3,\"SMS1\",\"test_1\",0,0,0\r\n";
const char *AT_MQTT_DISC = "AT+MQTTDISCONN=3\r\n";
const char *AT_MQTT_DELETE  = "AT+MQTTDELETE=3\r\n";

//////////////

/////////



int mqtt_machine()
{
	 HAL_StatusTypeDef status;

	    switch (mqtt_state) {

            case MQTT_STATE_CREATE:
            	HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT CREATING\r\n", 16, 100);
            	 status= HAL_UART_Transmit(&huart1, (uint8_t *)AT_MQTT_CREATE, strlen(AT_MQTT_CREATE), 100);
            	 if (status != HAL_OK)
            	   {
            		 mqtt_state = MQTT_STATE_CREATE;
            	      HAL_Delay(100);
            	   }


	            break;

	        case MQTT_STATE_CONNECT:
	        	HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT CONNECT\r\n", 16, 100);
	        	status= HAL_UART_Transmit(&huart1, (uint8_t *)AT_MQTT_CONN, strlen(AT_MQTT_CONN), 100);
	        	 if (status != HAL_OK)
	             {
	              	 mqtt_state = MQTT_STATE_CONNECT;
 	            	  HAL_Delay(100);

	        	  }
	        	 HAL_UART_Transmit(&huart2, (uint8_t *)"GPS1 DATA BEFORE MQTTPUB : ", 25, 100);
	        	            	 HAL_UART_Transmit(&huart2, (uint8_t *)GPS_BUFFER, strlen(GPS_BUFFER), 100);


	            break;

	        case MQTT_STATE_PUBLISH:
	        	HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT PUBLISH\r\n", 16, 100);
	        	////////////

	        	 // Step 0: Ensure GPS_BUFFER is clean and has data
	        	    if (strstr((char *)GPS_BUFFER,"$GNRMC") == NULL) {
	        	        HAL_UART_Transmit(&huart2, (uint8_t *)"INVALID GPS DATA\r\n", 20, 100);
	        	        //HAL_UART_Transmit(&huart2, (uint8_t *)"GPS DATA BEFORE MQTTPUB : ", 25, 100);
	        	       // HAL_UART_Transmit(&huart2, (uint8_t *)GPS_BUFFER, strlen(GPS_BUFFER), 100);
	        	        break;
	        	    }

	        	    // Step 1: Send AT+MQTTPUBLM with message size
	        	    char pub_command[128];
	        	    int gps_len = strlen((char *)GPS_BUFFER);

	        	    snprintf(pub_command, sizeof(pub_command),
	        	             "AT+MQTTPUBLM=3,\"GPSDATA\",0,0,0,%d\r\n", gps_len);
	        	    HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT CMD:\r\n", 11, 100);
	        	    HAL_UART_Transmit(&huart2, (uint8_t *)pub_command, strlen(pub_command), 100);
	        	    HAL_UART_Transmit(&huart1, (uint8_t *)pub_command, strlen(pub_command), 100);


	        	    HAL_Delay(300); // Consider replacing with state machine + response check

	        	    // Step 2: Transmit GPS data
	        	    HAL_UART_Transmit(&huart1, GPS_BUFFER, gps_len, 100);

	        	    // Step 3: Send Ctrl+Z to complete
	        	    uint8_t ctrl_z = 0x1A;
	        	    HAL_UART_Transmit(&huart1, &ctrl_z, 1, 100);
	        	    memset(GPS_BUFFER, 0, sizeof(GPS_BUFFER));   //CLEARING


	        	/////////////////
	            break;
	        case MQTT_STATE_DISCONNECT:

	        	status = HAL_UART_Transmit(&huart1, (uint8_t *)AT_MQTT_DISC, strlen(AT_MQTT_DISC), 100);

	 	        if (status != HAL_OK)
	        	 {
	        		    mqtt_state = MQTT_STATE_DISCONNECT ;
	        		    HAL_Delay(100);

	        	 }
	        	 break;

	        case MQTT_STATE_DELETE:

	        	status =HAL_UART_Transmit(&huart1, (uint8_t *)AT_MQTT_DELETE, strlen(AT_MQTT_DELETE), 100);
	        	  if (status != HAL_OK)
	        	  {
	             	  mqtt_state = MQTT_STATE_DELETE;
	        		  HAL_Delay(100);

	        	   }
	            break;

	        case MQTT_STATE_DONE:
	            // Final state, success
	        	HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT PUBLISH\r\n", 16, 100);
	            HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT flow completed\r\n", 22, 100);
	            main_state_flag = 2;
	            memset(rx_buffer, 0, sizeof(rx_buffer));   //resetting  incomming data buffer
	            rx_len = 0;
	            break;

	        default:
	            break;
	    }
	    HAL_Delay(1000);
	    mqtt_Response();
 }


int mqtt_Response()
{
	////////////
//	 HAL_UART_Transmit(&huart2, (uint8_t *)"****RX_BUFFER**** : ", 18, 100);
//		        	        HAL_UART_Transmit(&huart2, (uint8_t *)rx_buffer, strlen(rx_buffer), 100);
//	//////////
	if(mqtt_state == MQTT_STATE_CREATE)
	{
           if((strstr(rx_buffer , "CREATED") != NULL))
           {
        	   mqtt_state = MQTT_STATE_CONNECT;
           }
           else
           {
        	   mqtt_state = MQTT_STATE_CREATE;
        	   HAL_Delay(100);
           }

	}
	else if(mqtt_state == MQTT_STATE_CONNECT)
	{
		   HAL_Delay(2000);

		   {
			   if(strstr(rx_buffer ,"CONNECTED") != NULL)
			   {
				   mqtt_state =MQTT_STATE_PUBLISH ;


			   }
			   else if(strstr(rx_buffer , "CONNECTION EXIST") != NULL)
			    {
			   		mqtt_state = MQTT_STATE_PUBLISH;
			   		 HAL_Delay(100);

			   	}
			   else if(strstr(rx_buffer , "ERROR") != NULL)
			   {
				   mqtt_state = MQTT_STATE_CONNECT;
				   HAL_Delay(100);

			   }


		   }

	}
	else if(mqtt_state == MQTT_STATE_PUBLISH)
	{
		   HAL_Delay(2000);


		   {
			   if(strstr(rx_buffer , "PUBLISH SUCCESS") != NULL)
			   {
				   mqtt_state = MQTT_STATE_DISCONNECT ;


			   }
			   else if(strstr(rx_buffer , "CONNECTION NOT EXIST") != NULL)
			  	 {
			  		 mqtt_state = MQTT_STATE_CREATE;
			  		 HAL_Delay(100);

			  	 }
			   else if(strstr(rx_buffer , "ERROR") != NULL)
			   {
				   mqtt_state = MQTT_STATE_PUBLISH;
				   HAL_Delay(100);

			   }


		   }

	}
	else if(mqtt_state == MQTT_STATE_DISCONNECT)
		{

		 if(strstr(rx_buffer , "DISCONNECTED") != NULL)
	     {
			mqtt_state = MQTT_STATE_DELETE;
			HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT disconnected\r\n", 20, 100);

         }

		}
	else if(mqtt_state == MQTT_STATE_DELETE)
	{
		       if(strstr(rx_buffer , "OK") != NULL)
			    {
				  mqtt_state = MQTT_STATE_DONE;
				  HAL_UART_Transmit(&huart2, (uint8_t *)"MQTT deleted\r\n", 15, 100);


				}
		       else if(strstr(rx_buffer , "CLIENT NOT EXIST") != NULL)
		       	{
		          mqtt_state = MQTT_STATE_DONE  ;


		       	}
		       else if(strstr(rx_buffer , "ERROR") != NULL)
		       {
		    	  mqtt_state = MQTT_STATE_DELETE ;
		       }

	}

}

