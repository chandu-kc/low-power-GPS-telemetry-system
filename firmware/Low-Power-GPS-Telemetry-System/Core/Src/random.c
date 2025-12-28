/*
 * random.c
 *
 *  Created on: Jul 23, 2025
 *      Author: TSP-L16
 */

#include "main.h"
#include "stm32l1xx_it.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


void USB_ACTIVE()
{
	     HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbCtrl\",1\r\n", strlen("AT$QCPCFG=\"usbCtrl\",1\r\n"), 200);
	    HAL_Delay(200);

	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSwTrace\",1\r\n", strlen("AT$QCPCFG=\"usbSwTrace\",1\r\n"), 200);
	    HAL_Delay(200);

	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSlpMask\",0\r\n", strlen("AT$QCPCFG=\"usbSlpMask\",0\r\n"), 200);
	    HAL_Delay(200);

	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbNet\",1\r\n", strlen("AT$QCPCFG=\"usbNet\",1\r\n"), 200);
	    HAL_Delay(200);

	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"logPortSel\",0\r\n", strlen("AT$QCPCFG=\"logPortSel\",0\r\n"), 200);
	    HAL_Delay(200);

	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+USBSETUP=1\r\n", strlen("AT+USBSETUP=1\r\n"), 200);
	    HAL_Delay(200);

	    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CFUN=1,1\r\n", strlen("AT+CFUN=1,1\r\n"), 200);
	    HAL_Delay(200);

}

void USB_SLEEP_MODE()
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbCtrl\",1\r\n", strlen("AT$QCPCFG=\"usbCtrl\",1\r\n"), 200);
    HAL_Delay(200);

    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSwTrace\",0\r\n", strlen("AT$QCPCFG=\"usbSwTrace\",0\r\n"), 200);
    HAL_Delay(200);

    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSlpMask\",1\r\n", strlen("AT$QCPCFG=\"usbSlpMask\",1\r\n"), 200);
    HAL_Delay(200);

    HAL_UART_Transmit(&huart1, (uint8_t *)"AT$QCPCFG=\"usbSlpThd\",0\r\n", strlen("AT$QCPCFG=\"usbSlpThd\",0\r\n"), 200);
    HAL_Delay(200);
}

