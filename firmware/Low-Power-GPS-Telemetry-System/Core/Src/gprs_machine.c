#include "main.h"
#include "stm32l1xx_it.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern const char *GPS_ACTIVE;
extern const char *GPS_GET;
extern const char *GPS_ACTIVE_AGNSS;
extern const char *GPS_GET_STOP;

int gps_data_get(void)
{
    HAL_StatusTypeDef status;

    switch (gps_state)
    {
        case GPS_STATE_ACTIVATE:
            status = HAL_UART_Transmit(&huart1, (uint8_t *)GPS_ACTIVE, strlen(GPS_ACTIVE), 100);
                      if (status != HAL_OK)
                      {
                    	  gps_state = GPS_STATE_ACTIVATE; // Retry from beginning
                    	   HAL_Delay(100);
                      }


            break;


        case GPS_STATE_ACTIVATE_AGNSS_HOTSTART:

//        	status = HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGPSHOT\r\n", strlen("AT+CGPSHOT\r\n"), 100);
        	 //status = HAL_UART_Transmit(&huart1, (uint8_t *)GPS_ACTIVE_AGNSS, strlen(GPS_ACTIVE_AGNSS), 100);
        	status = HAL_UART_Transmit(&huart1, (uint8_t *)"AT\r\n", strlen("AT\r\n"), 100);
        	          // HAL_Delay(300);
        	          if (status != HAL_OK)
        	            {
        	               gps_state = GPS_STATE_ACTIVATE_AGNSS_HOTSTART; // Retry from beginning
        	               HAL_Delay(100);
        	             }


        	break;

        case GPS_STATE_GET:

        	if(gps_data_found_flag == 0)   // to avoid un nessasary loop
        	{
            status = HAL_UART_Transmit(&huart1, (uint8_t *)GPS_GET, strlen(GPS_GET), 100);
                      if (status != HAL_OK)
                       {
                           gps_state = GPS_STATE_GET ; // Retry from beginning
                            HAL_Delay(100);
                       }
        	}


            break;

        case GPS_STATE_DONE:



        	 break;


        default:

            break;
    }

    GPS_RESPONSE();
}

int GPS_RESPONSE()
{

      	if(strstr(rx_buffer , "OK") != NULL)
      	{
      		if( gps_state == GPS_STATE_ACTIVATE )
      		{
      			gps_state = GPS_STATE_ACTIVATE_AGNSS_HOTSTART;
      		}
      		else if (gps_state == GPS_STATE_ACTIVATE_AGNSS_HOTSTART )
      		{
      			HAL_UART_Transmit(&huart2, (uint8_t *)"AGNSS ACTIVATED\r\n", 18, 100);
      			gps_state = GPS_STATE_GET;
      		}
      		else if (gps_state ==  GPS_STATE_GET)
      		{

      			///////////////
      			                 char *gps_start = strstr((char *)rx_buffer, "$GNRMC");

      			      				if (gps_start != NULL) {
      			      				    strncpy(GPS_BUFFER, gps_start, sizeof(GPS_BUFFER) - 1);
      			      				    GPS_BUFFER[sizeof(GPS_BUFFER) - 1] = '\0';  // ensure null-termination



      			      				}



      			////////////////

      			if (is_valid_gps_data(GPS_BUFFER) != 0)
      			{
      			    //strcpy(GPS_BUFFER, rx_buffer);
      			    gps_state = GPS_STATE_DONE;

      			    // Print full NMEA data to UART2

      			    HAL_UART_Transmit(&huart2, (uint8_t *)"***GPS DATA FOUND***: ", 21, 100);
      			    HAL_UART_Transmit(&huart2, (uint8_t *)GPS_BUFFER, strlen(GPS_BUFFER), 100);
      			    HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 100);
      			    main_state_flag = 1;
      			    gps_data_found_flag = 1;
      			    gps_state = GPS_STATE_DONE;
      			    return;


      			}
      			else
      			{
      				HAL_UART_Transmit(&huart2, (uint8_t *)"RETRY TO GPS DATA\r\n", 20, 100);
       				HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 100);
       				memset(GPS_BUFFER, 0, sizeof(GPS_BUFFER));
       				memset(rx_buffer, 0, sizeof(rx_buffer));

      				gps_state =  GPS_STATE_GET;

      			}

      		}



      	}
      	 HAL_Delay(1000);

}

int is_valid_gps_data(const char *nmea_sentence)
{
    if (nmea_sentence == NULL) return 0;

    // Ensure sentence starts with $GNRMC or $GPRMC
    if (strncmp(nmea_sentence, "$GNRMC", 6) != 0)
        return 0;

    // Copy to temp buffer
    char buffer[256];
    strncpy(buffer, nmea_sentence, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // Tokenize to find status field (3rd field)
    char *token;
    int field = 0;

    token = strtok(buffer, ",");
    while (token != NULL)
    {
        field++;
        if (field == 3) // Field 3 = Status: A or V
        {

            return (token[0] == 'A') ? 1 : 0;
        }
        token = strtok(NULL, ",");
    }

    return 0; // Default to invalid if not found
}


//int is_valid_gps_data(const char *nmea_sentence)
//{
//    if (nmea_sentence == NULL) return 0;
//
//    // Ensure sentence starts with $GNGGA or $GPGGA
//    if (strncmp(nmea_sentence, "$GNMRC", 6) != 0)
//        return 0;
//
//    // Copy to a temp buffer for safe tokenizing
//    char buffer[256];
//    strncpy(buffer, nmea_sentence, sizeof(buffer) - 1);
//    buffer[sizeof(buffer) - 1] = '\0';
//
//    // Tokenize by commas
//    char *token;
//    int field = 0;
//    int fix_status = 0;
//
//    token = strtok(buffer, ",");
//    while (token != NULL)
//    {
//        field++;
//        if (field == 7) // 7th field (index 6) = Fix Status
//        {
//            fix_status = atoi(token);
//            break;
//        }
//        token = strtok(NULL, ",");
//    }
//
//    // Return true if fix is 1 (GPS fix) or 2 (DGPS fix)
//    return (fix_status == 1 || fix_status == 2);
//}


