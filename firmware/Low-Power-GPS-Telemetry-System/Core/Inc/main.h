/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"

// ==== Configuration ====
#define RX_BUFFER_SIZE 500

// ==== Global Variables ====
uint8_t rx_buffer[RX_BUFFER_SIZE];     // DMA receive buffer
uint16_t rx_len;                   // Number of received bytes
int sec_count;

// ==== Function Prototype ====
void process_uart_data(uint8_t *data, uint16_t len);
int gps_data_get();

//GPRS STATEMACHINE HELPERS

#define MAX_RETRIES_GPS 2

typedef enum {
    GPS_STATE_ACTIVATE = 0,
	GPS_STATE_ACTIVATE_AGNSS_HOTSTART,
	GPS_HOT_START,
    GPS_STATE_GET,
    GPS_STATE_DONE

} GPS_State_t;

GPS_State_t gps_state;
static int retry_count_gps = 0;

uint8_t GPS_BUFFER[500];
int gps_data_found_flag;


int GPS_RESPONSE();

int main_state_flag;
//************MQTT********
int mqtt_machine();
int mqtt_Response();

typedef enum {
    MQTT_STATE_CREATE,
    MQTT_STATE_CONNECT,
    MQTT_STATE_PUBLISH,
	MQTT_STATE_DISCONNECT,
    MQTT_STATE_DELETE,
    MQTT_STATE_DONE
} MQTT_State;

MQTT_State mqtt_state;


//sleep machine
void Sleep();
void Wake_UP();

int current_SEC_sleep;
void NETWORK_SET_UP();
//NET_WORK_CONNACTION_CHECK_TO_AGNSS

int error_restart; //cme error fixer

int flag_gps;

void USB_ACTIVE();
void USB_SLEEP_MODE();
void CloseAllNetworkAndCheckSleep();

typedef enum {
    GNSS_DISCONNECT,
	FUNCTION_MODE_OFF,
	ENABLE_SLEEP,
	VOTE_CHECK,
	IDLE_SLEEP
} SLEEP_BEFORE_State;
SLEEP_BEFORE_State sleep_state;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
