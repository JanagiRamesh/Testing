/*
 * CAN_Driver.h
 *
 *  Created on: 20-Apr-2021
 *      Author: Admin
 */

#ifndef INC_CAN_DRIVER_H_
#define INC_CAN_DRIVER_H_



#include "can.h"
#include "../Custom_Libs/Inc/Enumeration.h"

#define arr_size(x)		sizeof(x)/sizeof(x[0])
#define		CAN_1		&hcan1
#define BuffSize		100
#define BuffSizeX		100

#define ID1			0x110
#define ID2			0x111
#define ID3			0x112
#define ID4			0x113
#define ID5			0x114

CAN_RxHeaderTypeDef	 RxMsg[BuffSize];
CAN_RxHeaderTypeDef	 RxMsgX[BuffSizeX];

uint8_t TxMsg_Data[8];
uint8_t CAN_RX_Flag;
uint8_t ReceivedBuff[BuffSize];
uint16_t ReceivedFrameCounter;

//void CANSetup();
void CAN1_Filter_IDMask(uint32_t U32_CANid, uint32_t U32_CANmask, IDType U8_IdType, FilterBanks U8_CAN_Filterbank, FIFO_NUM U8_FIFO0, FunctionalState State);
void Transmit_CANID(uint32_t U32_CANID, IDType U8_IdType, uint8_t *U8_TransArr, uint8_t TransArr_size);
void CAN_Filter_IDList(uint32_t U32_CAN_ID1, uint32_t U32_CAN_ID2, IDType U8_IdType, FilterBanks U8_CAN_Filterbank, FIFO_NUM U8_FIFO0, FunctionalState state);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);


#endif /* INC_CAN_DRIVER_H_ */
