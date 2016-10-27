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
#include "modbus.h"
#include "modbus_proto.h"
#include "modbus_config.h"

/* 常量定义 ------------------------------------------------------------------*/
/* PDU帧长最大值 */
#define MB_PDU_SIZE_MAX                 ( 253 )
/* PDU帧长最小值 */
#define MB_PDU_SIZE_MIN                 ( 1 )
/* PDU功能码偏移 */
#define MB_PDU_FUNC_OFF                 ( 0 )
/* PDU数据域偏移 */
#define MB_PDU_DATA_OFF                 ( 1 )

/* 从机处理函数 --------------------------------------------------------------*/
/* Modbus Slave处理函数 */
eMBException MBSlaveFuncHandle(uint8_t *pFrame, uint16_t *pLength);
eMBException MBError2MBException(eMBError err);

/* 离散量输入 */
#if MB_SLAVE_FUNC_READ_DISCRETE_INPUTS_EN
eMBException MBSlaveFuncReadDiscreteInputs(uint8_t *pFrame, uint16_t *pLength);
#endif

/* 线圈 */
#if MB_SLAVE_FUNC_READ_COILS_EN
eMBException MBSlaveFuncReadCoils(uint8_t *pFrame, uint16_t *pLength);
#endif

#if MB_SLAVE_FUNC_WRITE_SINGLE_COIL_EN
eMBException MBSlaveFuncWriteSingleCoil(uint8_t *pFrame, uint16_t *pLength);
#endif

#if MB_SLAVE_FUNC_WRITE_MULTIPLE_COILS_EN
eMBException MBSlaveFuncWriteMultipleCoils(uint8_t *pFrame, uint16_t *pLength);
#endif

/* 输入寄存器 */
#if MB_SLAVE_FUNC_READ_INPUT_REGISTERS_EN
eMBException MBSlaveFuncReadInputRegisters(uint8_t *pFrame, uint16_t *pLength);
#endif

/* 保持寄存器 */
#if MB_SLAVE_FUNC_READ_HOLDING_REGISTERS_EN
eMBException MBSlaveFuncReadHoldingRegisters(uint8_t *pFrame, uint16_t *pLength);
#endif

#if MB_SLAVE_FUNC_WRITE_SINGLE_REGISTER_EN
eMBException MBSlaveFuncWriteSingleRegister(uint8_t *pFrame, uint16_t *pLength);
#endif

#if MB_SLAVE_FUNC_WRITE_MULTIPLE_REGISTERS_EN
eMBException MBSlaveFuncWriteMultipleRegisters(uint8_t *pFrame, uint16_t *pLength);
#endif

#if MB_SLAVE_FUNC_READWRITE_MULTIPLE_REGISTERS_EN
eMBException MBSlaveFuncReadWriteMultipleRegisters(uint8_t *pFrame, uint16_t *pLength);
#endif

#endif  /* __MODBUS_FUNC_H */
