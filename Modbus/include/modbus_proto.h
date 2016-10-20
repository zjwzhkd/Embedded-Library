/*******************************************************************************
* 文 件 名: modbus_proto.h
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-16
* 文件说明: Modbus函数库原型定义
*******************************************************************************/

#ifndef __MODBUS_PROTO_H
#define __MODBUS_PROTO_H

/* 头文件 --------------------------------------------------------------------*/
#include "library_common.h"

/* 类型定义 ------------------------------------------------------------------*/
/* Modbus错误枚举 */
typedef enum
{
    MB_OK = 0,              /* 无错误         */
    MB_ERR_INVAL,           /* 无效参数       */
    MB_ERR_NOREG,           /* 无效寄存器地址 */
    MB_ERR_IO,              /* I/O错误        */
    MB_ERR_TIMEOUT          /* 超时错误发生   */
} eMBError;

/* Modbus模式枚举 */
typedef enum
{
    MB_MODE_RTU = 0,        /* RTU传输模式    */
    MB_MODE_ASCII,          /* ASCII传输模式  */
    MB_MODE_TCP             /* TCP传输模式    */
} eMBMode;

/* 寄存器操作模式 */
typedef enum
{
    MB_REG_READ = 0,        /* 读取寄存器数值 */
    MB_REG_WRITE            /* 更新寄存器数值 */
} eMBRegisterMode;

/* Modbus异常响应 */
typedef enum
{
    MB_EX_NONE                                      = 0x00,
    MB_EX_ILLEGAL_FUNCTION                          = 0x01,
    MB_EX_ILLEGAL_DATA_ADDRESS                      = 0x02,
    MB_EX_ILLEGAL_DATA_VALUE                        = 0x03,
    MB_EX_SERVER_DEVICE_FAILURE                     = 0x04,
    MB_EX_ACKNOWLEDGE                               = 0x05,
    MB_EX_SERVER_DEVICE_BUSY                        = 0x06,
    MB_EX_MEMORY_PARITY_ERROR                       = 0x08,
    MB_EX_GATEWAY_PATH_UNAVAILABLE                  = 0x0A,
    MB_EX_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND   = 0x0B
} eMBException;

/* Modbus从机功能码处理函数 */
typedef eMBException (*MBSlaveFuncHandler) (uint8_t *pFrame, uint16_t *pLength);

/* 常量定义 ------------------------------------------------------------------*/
/* Modbus设备地址 */
#define MB_ADDRESS_BROADCAST                    ( 0 )
#define MB_ADDRESS_MIN                          ( 1 )
#define MB_ADDRRSS_MAX                          ( 247 )

/* Modbus功能码 */
#define MB_FUNC_READ_COILS                      ( 0x01 )
#define MB_FUNC_READ_DISCRETE_INPUTS            ( 0x02 )
#define MB_FUNC_READ_HOLDING_REGISTERS          ( 0x03 )
#define MB_FUNC_READ_INPUT_REGISTERS            ( 0x04 )
#define MB_FUNC_WRITE_SINGLE_COIL               ( 0x05 )
#define MB_FUNC_WRITE_SINGLE_REGISTER           ( 0x06 )
#define MB_FUNC_WRITE_MULTIPLE_COILS            ( 0x0F )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS        ( 0x10 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS    ( 0x17 )
#define MB_FUNC_ERROR                           ( 0x80 )

#endif  /* __MODBUS_PROTO_H */
