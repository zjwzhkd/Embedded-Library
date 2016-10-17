/*******************************************************************************
* 文 件 名: modbus.h
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-16
* 文件说明: Modbus函数库API接口
*******************************************************************************/

#ifndef __MODBUS_H
#define __MODBUS_H

/* 头文件 --------------------------------------------------------------------*/
#include "modbus_proto.h"

/* API接口 -------------------------------------------------------------------*/
eMBError    modbus_SlaveInit(eMBMode mode, uint8_t slaveAddr);
eMBError    modbus_SlaveDeal(uint8_t *buffer, uint16_t *length);

/* 回调函数 ------------------------------------------------------------------*/
eMBError    modbus_SlaveRegHoldingCB(uint8_t *regBuf, uint16_t startAddr,
                                     uint16_t nRegs, eMBRegisterMode mode);
eMBError    modbus_SlaveRegInputCB(uint8_t *regBuf, uint16_t startAddr,
                                   uint16_t nRegs);
eMBError    modbus_SlaveRegCoilsCB(uint8_t *regBuf, uint16_t startAddr,
                                   uint16_t nCoils, eMBRegisterMode mode);
eMBError    modbus_SlaveRegDiscreteCB(uint8_t *regBuf, uint16_t startAddr,
                                      uint16_t nDiscrete);

#endif  /* __MODBUS_H */
