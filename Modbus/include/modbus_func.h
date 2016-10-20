/*******************************************************************************
* 文 件 名: modbus_func.h
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-20
* 文件说明: Modbus功能码事务处理
*******************************************************************************/

#ifndef __MODBUS_FUNC_H
#define __MODBUS_FUNC_H

/* 头文件 --------------------------------------------------------------------*/
#include "modbus_proto.h"

/* 从机处理函数 --------------------------------------------------------------*/
/* Modbus Slave处理函数 */
eMBException MBSlaveFuncHandle(uint8_t *pFrame, uint16_t *pLength);
eMBException MBError2MBException(eMBError err);
/* 离散量输入 */
eMBException MBSlaveFuncReadDiscreteInputs(uint8_t *pFrame, uint16_t *pLength);
/* 线圈 */
eMBException MBSlaveFuncReadCoils(uint8_t *pFrame, uint16_t *pLength);
eMBException MBSlaveFuncWriteSingleCoil(uint8_t *pFrame, uint16_t *pLength);
eMBException MBSlaveFuncWriteMultipleCoils(uint8_t *pFrame, uint16_t *pLength);
/* 输入寄存器 */
eMBException MBSlaveFuncReadInputRegisters(uint8_t *pFrame, uint16_t *pLength);
/* 保持寄存器 */
eMBException MBSlaveFuncReadHoldingRegisters(uint8_t *pFrame, uint16_t *pLength);
eMBException MBSlaveFuncWriteSingleRegister(uint8_t *pFrame, uint16_t *pLength);
eMBException MBSlaveFuncWriteMultipleRegisters(uint8_t *pFrame, uint16_t *pLength);
eMBException MBSlaveFuncReadWriteMultipleRegisters(uint8_t *pFrame, uint16_t *pLength);

#endif  /* __MODBUS_FUNC_H */
