/*
 * Debug.c
 *
 *  Created on: 20-Apr-2021
 *      Author: Admin
 */


#include "../Custom_Libs/Inc/Debug.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"


void DebugPrint(const char* fmt, ...)
{
    char buff[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, sizeof(buff), fmt, args);
    HAL_UART_Transmit(&huart10, (uint8_t*)buff, strlen(buff), 5000);
    va_end(args);
}
