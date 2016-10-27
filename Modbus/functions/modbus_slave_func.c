/*******************************************************************************
* 文 件 名: modbus_slave_func.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-20
* 文件说明: Modbus从机功能码事务处理
*******************************************************************************/

#include "modbus_func.h"
/*******************************************************************************

                                    函数接口

*******************************************************************************/
/**
 * Modbus Slave事务处理函数
 *
 * @param pFrame: 请求PDU数据帧的起始地址, 同样也保存正常响应PDU数据帧
 *
 * @param pLength: 保存PDU数据帧长度的变量的指针
 *
 * @return: 异常响应代码(正常响应返回MB_EX_NONE)
 */
eMBException MBSlaveFuncHandle(uint8_t *pFrame, uint16_t *pLength)
{
uint8_t func_code;
eMBException ex_code;

    func_code = pFrame[MB_PDU_FUNC_OFF];
    /*检索功能码事务处理函数*/
    switch (func_code)
    {
        /*离散量输入*/
    #if MB_SLAVE_FUNC_READ_DISCRETE_INPUTS_EN
        case MB_FUNC_READ_DISCRETE_INPUTS:
            ex_code = MBSlaveFuncReadDiscreteInputs(pFrame, pLength);
            break;
    #endif

        /*线圈*/
    #if MB_SLAVE_FUNC_READ_COILS_EN
        case MB_FUNC_READ_COILS:
            ex_code = MBSlaveFuncReadCoils(pFrame, pLength);
            break;
    #endif

    #if MB_SLAVE_FUNC_WRITE_SINGLE_COIL_EN
        case MB_FUNC_WRITE_SINGLE_COIL:
            ex_code = MBSlaveFuncWriteSingleCoil(pFrame, pLength);
            break;
    #endif

    #if MB_SLAVE_FUNC_WRITE_MULTIPLE_COILS_EN
        case MB_FUNC_WRITE_MULTIPLE_COILS:
            ex_code = MBSlaveFuncWriteMultipleCoils(pFrame, pLength);
            break;
    #endif

        /*输入寄存器*/
    #if MB_SLAVE_FUNC_READ_INPUT_REGISTERS_EN
        case MB_FUNC_READ_INPUT_REGISTERS:
            ex_code = MBSlaveFuncReadInputRegisters(pFrame, pLength);
            break;
    #endif

        /*保持寄存器*/
    #if MB_SLAVE_FUNC_READ_HOLDING_REGISTERS_EN
        case MB_FUNC_READ_HOLDING_REGISTERS:
            ex_code = MBSlaveFuncReadHoldingRegisters(pFrame, pLength);
            break;
    #endif

    #if MB_SLAVE_FUNC_WRITE_SINGLE_REGISTER_EN
        case MB_FUNC_WRITE_SINGLE_REGISTER:
            ex_code = MBSlaveFuncWriteSingleRegister(pFrame, pLength);
            break;
    #endif

    #if MB_SLAVE_FUNC_WRITE_MULTIPLE_REGISTERS_EN
        case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
            ex_code = MBSlaveFuncWriteMultipleRegisters(pFrame, pLength);
            break;
    #endif

    #if MB_SLAVE_FUNC_READWRITE_MULTIPLE_REGISTERS_EN
        case MB_FUNC_READWRITE_MULTIPLE_REGISTERS:
            ex_code = MBSlaveFuncReadWriteMultipleRegisters(pFrame, pLength);
            break;
    #endif

        /*不支持的功能码*/
        default:
            ex_code = MB_EX_ILLEGAL_FUNCTION;
            break;
    }

    return (ex_code);
}

/**
 * Modbus回调函数错误码转换函数
 *
 * @param err: Modbus回调函数错误码
 *
 * @return: 将回调函数错误码转换为异常响应代码
 */
eMBException MBError2MBException(eMBError err)
{
eMBException ex_code;

    switch (err)
    {
        case MB_OK:
            ex_code = MB_EX_NONE;
            break;

        case MB_ERR_NOREG:
            ex_code = MB_EX_ILLEGAL_DATA_ADDRESS;
            break;

        case MB_ERR_TIMEOUT:
            ex_code = MB_EX_SERVER_DEVICE_BUSY;
            break;

        default:
            ex_code = MB_EX_SERVER_DEVICE_FAILURE;
            break;
    }

    return (ex_code);
}
