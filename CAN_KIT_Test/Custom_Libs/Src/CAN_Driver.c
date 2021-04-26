/*
 * CAN_Driver.c
 *
 *  Created on: 20-Apr-2021
 *      Author: Admin
 */


#include "../Custom_Libs/Inc/CAN_Driver.h"
#include "../Custom_Libs/Inc/Debug.h"

CAN_TxHeaderTypeDef	 TxMsg;
uint8_t TxMsg_Data[8];


uint8_t ReceivedBuff[BuffSize] = {0};
uint16_t ReceivedFrameCounter = 0;
uint8_t CAN_RX_Flag = 0;

uint8_t ReceivedBuffX[BuffSizeX] = {0};
uint16_t ReceivedFrameCounterX = 0;
uint8_t CAN_RX_FlagX = 0;

uint8_t value = 0;


void Transmit_CANID(uint32_t U32_CANID, IDType U8_IdType, uint8_t *U8_TransArr, uint8_t TransArr_size)
{
	if(U8_IdType == S)
	{
		TxMsg.StdId = U32_CANID;
		TxMsg.IDE = CAN_ID_STD;
	}
	else if(U8_IdType == E)
	{
		TxMsg.ExtId = U32_CANID;
		TxMsg.IDE = CAN_ID_EXT;
	}
	TxMsg.RTR = CAN_RTR_DATA;
	TxMsg.DLC = TransArr_size;
	TxMsg.TransmitGlobalTime = DISABLE;

	HAL_CAN_AddTxMessage(CAN_1, &TxMsg, U8_TransArr, (uint32_t*)CAN_TX_MAILBOX0);
}

void CAN1_Filter_IDMask(uint32_t U32_CANid, uint32_t U32_CANmask, IDType U8_IdType, FilterBanks U8_CAN_Filterbank, FIFO_NUM U8_FIFO0, FunctionalState State)
{
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterActivation = State;

	sFilterConfig.FilterBank = (uint32_t)(U8_CAN_Filterbank);	//32-bit MASK Filter Bank (Reference Manual, Pg. 1099, Fig. 388)
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	if(U8_FIFO0 == FIFO_0)
	{
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	}
	else if(U8_FIFO0 == FIFO_1)
	{
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	}
	if(U8_IdType == E)
	{
		sFilterConfig.FilterIdHigh = (uint16_t) (U32_CANid >> 13);
		sFilterConfig.FilterIdLow = (uint16_t) (U32_CANid << 3);
		sFilterConfig.FilterMaskIdHigh = (uint16_t) (U32_CANmask >> 13);
		sFilterConfig.FilterMaskIdLow = (uint16_t) (U32_CANmask << 3);
	}
	else if(U8_IdType == S)
	{
		sFilterConfig.FilterIdHigh = (uint16_t)(U32_CANid << 5);
		sFilterConfig.FilterIdLow = 0;
		sFilterConfig.FilterMaskIdHigh = (uint16_t)(U32_CANmask << 5);
		sFilterConfig.FilterMaskIdLow = 0;
	}
	if(HAL_CAN_ConfigFilter(CAN_1, &sFilterConfig) != HAL_OK)
	{
		//Error Handler Code to be added later to act as redundancy.
		Error_Handler();
	}
}

void CAN_Filter_IDList(uint32_t U32_CAN_ID1, uint32_t U32_CAN_ID2, IDType U8_IdType, FilterBanks U8_CAN_Filterbank, FIFO_NUM U8_FIFO0, FunctionalState state)
{
	CAN_FilterTypeDef	sFilterConfig;

	sFilterConfig.FilterActivation				=	state;
	sFilterConfig.FilterBank					=	(uint32_t)(U8_CAN_Filterbank);
	sFilterConfig.FilterMode					=   CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale					=	CAN_FILTERSCALE_32BIT;
	if(U8_FIFO0 == FIFO_0)
	{
		sFilterConfig.FilterFIFOAssignment		=	CAN_FILTER_FIFO0;
	}
	else if(U8_FIFO0 == FIFO_1)
	{

		sFilterConfig.FilterFIFOAssignment		=	CAN_FILTER_FIFO1;
	}
	if(U8_IdType == S)
	{
		sFilterConfig.FilterIdHigh				=	ID1 << 5;
		sFilterConfig.FilterIdLow				=	0;
		sFilterConfig.FilterMaskIdHigh			=	ID2 << 5;
		sFilterConfig.FilterMaskIdLow			=	0;
	}
	else if(U8_IdType == E)
	{
		sFilterConfig.FilterIdHigh 				= (ID1 & 0x1FFFE000)>>13;
		sFilterConfig.FilterIdLow 				= ((ID1 & 0x00001FFF)<<3)|0x04;
		sFilterConfig.FilterMaskIdHigh 			= (ID2 & 0x1FFFE000)>>13;
		sFilterConfig.FilterMaskIdLow 			= ((ID2 & 0x00001FFF)<<3)|0x04;
	}
	sFilterConfig.SlaveStartFilterBank			=	0;

	HAL_CAN_ConfigFilter(CAN_1, &sFilterConfig);
}

#if 0
// This is for Parcing of CAN DATA Transmission :
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == CAN_1)
	{
		HAL_CAN_GetRxMessage(CAN_1, CAN_RX_FIFO0, &RxMsg, ReceivedBuff);

		CAN_RX_Flag = 1;
//		value = ReceivedBuff[5];
		value = (ReceivedBuff[5] >> 4) & 1;
		DebugPrint("%X",value);

	    if(value == 1 )
		{
	    	DebugPrint("%d",value);
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_RESET);
		}
		else if(value == 0 )
		{
			DebugPrint("%d",value);
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_SET);
		}

		ReceivedFrameCounter++;

		if(ReceivedFrameCounter == BuffSize)
		{
			ReceivedFrameCounter = 0;
		}
		Transmit_CANID(RxMsg[ReceivedFrameCounter].StdId, S, ReceivedBuff, 8);
	}

}
#endif



//working code
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == CAN_1)
	{
		HAL_CAN_GetRxMessage(CAN_1, CAN_RX_FIFO0, &RxMsg, ReceivedBuff);

		CAN_RX_Flag = 1;


		ReceivedFrameCounter++;

		if(ReceivedFrameCounter == BuffSize)
		{
			ReceivedFrameCounter = 0;
		}
		Transmit_CANID(RxMsg[ReceivedFrameCounter].StdId, S, ReceivedBuff, 8);
	}

}



#if 0
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == CAN_1)
	{
		HAL_CAN_GetRxMessage(CAN_1, CAN_RX_FIFO1, &RxMsgX[ReceivedFrameCounterX], ReceivedBuffX[ReceivedFrameCounterX]);

		CAN_RX_FlagX = 1;
		Transmit_CANID(RxMsgX[ReceivedFrameCounterX].ExtId, E, ReceivedBuffX[ReceivedFrameCounterX], RxMsgX[ReceivedFrameCounterX].DLC);

		ReceivedFrameCounterX++;

		if(ReceivedFrameCounterX == BuffSizeX)
		{
			ReceivedFrameCounterX = 0;
		}

	}

}
#endif

