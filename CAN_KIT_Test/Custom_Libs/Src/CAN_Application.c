/*
 * CAN_Application.c
 *
 *  Created on: 20-Apr-2021
 *      Author: Admin
 */


#include "../Custom_Libs/Inc/CAN_Application.h"


void CANSetup()
{
	HAL_CAN_Start(CAN_1);
	HAL_CAN_ActivateNotification(CAN_1, CAN_IT_RX_FIFO0_MSG_PENDING);
//	HAL_CAN_ActivateNotification(CAN_1, CAN_IT_RX_FIFO1_MSG_PENDING);

//	CAN1_Filter_IDMask(0x1806E5F4, 0, E, FBANK1, FIFO_1, ENABLE);
	CAN_Filter_IDList(ID1, ID2, S, FBANK0, FIFO_0, ENABLE);
}
