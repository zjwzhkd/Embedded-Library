/*******************************************************************************
* 文 件 名: modbus_rtu.h
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-20
* 文件说明: Modbus RTU数据帧处理
*******************************************************************************/

#ifndef __MODBUS_RTU_H
#define __MODBUS_RTU_H

/* 头文件 --------------------------------------------------------------------*/
#include "modbus_proto.h"

/* 格式定义 ------------------------------------------------------------------*/
#define MB_RTU_ADU_SIZE_MIN             ( 4 )
#define MB_RTU_ADU_SIZE_MAX             ( 256 )
#define MB_RTU_ADU_SIZE_CRC             ( 2 )
#define MB_RTU_ADU_ADDR_OFF             ( 0 )
#define MB_RTU_ADU_PDU_OFF              ( 1 )

/* 处理函数 ------------------------------------------------------------------*/
bool MBRTUReceive(uint8_t *pRcvAddr, uint8_t **pFrame, uint16_t *pLength);
void MBRTUTransmit(uint8_t slaveAddr, uint8_t const *pFrame, uint16_t length);

#endif
