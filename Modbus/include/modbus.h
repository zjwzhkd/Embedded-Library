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
#include "modbus_utils.h"

/* API接口 -------------------------------------------------------------------*/
/**
 * 初始化Modbus Slave
 *
 * @param mode: Modbus通讯类型
 *
 * @param slaveAddr: 从机地址
 *
 * @return: MB_OK - 初始化成功
 *          MB_ERR_INVAL - 无效的参数
 */
eMBError modbus_SlaveInit(eMBMode mode, uint8_t slaveAddr);

/**
 * Modbus Slave处理函数
 *
 * @param buffer: 接收/发送缓冲地址
 *
 * @param length: 保存接收/发送帧长度的变量的指针
 *
 * @return: MB_OK - 缓冲内已经准备好响应帧
 *          MB_ERR_IGNORED - 从机不响应
 */
eMBError modbus_SlaveDeal(uint8_t *buffer, uint16_t *length);

/* 回调函数 ------------------------------------------------------------------*/
/**
 * Modbus Slave保持寄存器读写回调函数
 *
 * @param regBuf: Modbus上层协议栈提供的缓存地址
 *
 * @param startAddr: 回调函数需要处理的寄存器的起始地址, 这里指协议地址(从0开始)
 *
 * @param nRegs: 回调函数需要处理的寄存器的数量
 *
 * @param mode: MB_REG_READ  - 回调函数读取寄存器, 将结果写入缓存
 *              MB_REG_WRITE - 回调函数读取缓存, 并更新寄存器的值
 *
 * @return: MB_OK          - 没有错误发生
 *          MB_ERR_NOREG   - 协议栈提供的地址范围超出应用层的处理范围,
 *                           将导致MB_EX_ILLEGAL_DATA_ADDRESS异常响应
 *          MB_ERR_TIMEOUT - 请求的寄存器还未完成准备, 需要延后处理,
 *                           将导致MB_EX_SERVER_DEVICE_BUSY异常响应
 *          MB_ERR_IO      - 发生了一个无法恢复的错误, 将导致
 *                           MB_EX_SERVER_DEVICE_FAILURE异常响应
 */
eMBError modbus_SlaveRegHoldingCB(uint8_t *regBuf, uint16_t startAddr,
                                  uint16_t nRegs, eMBRegisterMode mode);

/**
 * Modbus Slave输入寄存器读回调函数
 *
 * @param regBuf: Modbus上层协议栈提供的缓存地址
 *
 * @param startAddr: 回调函数需要处理的寄存器的起始地址, 这里指协议地址(从0开始)
 *
 * @param nRegs: 回调函数需要处理的寄存器的数量
 *
 * @return: MB_OK          - 没有错误发生
 *          MB_ERR_NOREG   - 协议栈提供的地址范围超出应用层的处理范围,
 *                           将导致MB_EX_ILLEGAL_DATA_ADDRESS异常响应
 *          MB_ERR_TIMEOUT - 请求的寄存器还未完成准备, 需要延后处理,
 *                           将导致MB_EX_SERVER_DEVICE_BUSY异常响应
 *          MB_ERR_IO      - 发生了一个无法恢复的错误, 将导致
 *                           MB_EX_SERVER_DEVICE_FAILURE异常响应
 */
eMBError modbus_SlaveRegInputCB(uint8_t *regBuf, uint16_t startAddr,
                                uint16_t nRegs);

/**
 * Modbus Slave线圈读写回调函数
 *
 * @param regBuf: Modbus上层协议栈提供的缓存地址, 线圈状态对应着缓存的位域,
 *                startAddr对应线圈的状态存储在regBuf[0]的LSB中, 当写入缓存时,
 *                最后一个缓存字节中, 未使用的数据位需要保持为0
 *
 * @param startAddr: 回调函数需要处理的线圈的起始地址, 这里指协议地址(从0开始)
 *
 * @param nCoils: 回调函数需要处理的线圈的数量
 *
 * @param mode: MB_REG_READ  - 回调函数读取线圈, 将状态写入缓存
 *              MB_REG_WRITE - 回调函数读取缓存, 并设置线圈的状态
 *
 * @return: MB_OK          - 没有错误发生
 *          MB_ERR_NOREG   - 协议栈提供的地址范围超出应用层的处理范围,
 *                           将导致MB_EX_ILLEGAL_DATA_ADDRESS异常响应
 *          MB_ERR_TIMEOUT - 请求的寄存器还未完成准备, 需要延后处理,
 *                           将导致MB_EX_SERVER_DEVICE_BUSY异常响应
 *          MB_ERR_IO      - 发生了一个无法恢复的错误, 将导致
 *                           MB_EX_SERVER_DEVICE_FAILURE异常响应
 */
eMBError modbus_SlaveRegCoilsCB(uint8_t *regBuf, uint16_t startAddr,
                                uint16_t nCoils, eMBRegisterMode mode);

/**
 * Modbus Slave离散量输入读回调函数
 *
 * @param regBuf: Modbus上层协议栈提供的缓存地址, 离散量输入状态对应着缓存的位域,
 *                startAddr对应离散量输入的状态存储在regBuf[0]的LSB中,
 *                最后一个缓存字节中, 未使用的数据位保持为0
 *
 * @param startAddr: 回调函数需要处理的离散量输入的起始地址, 这里指协议地址(从0开始)
 *
 * @param nDiscrete: 回调函数需要处理的离散量输入的数量
 *
 * @return: MB_OK          - 没有错误发生
 *          MB_ERR_NOREG   - 协议栈提供的地址范围超出应用层的处理范围,
 *                           将导致MB_EX_ILLEGAL_DATA_ADDRESS异常响应
 *          MB_ERR_TIMEOUT - 请求的寄存器还未完成准备, 需要延后处理,
 *                           将导致MB_EX_SERVER_DEVICE_BUSY异常响应
 *          MB_ERR_IO      - 发生了一个无法恢复的错误, 将导致
 *                           MB_EX_SERVER_DEVICE_FAILURE异常响应
 */
eMBError modbus_SlaveRegDiscreteCB(uint8_t *regBuf, uint16_t startAddr,
                                   uint16_t nDiscrete);

#endif  /* __MODBUS_H */
