/*******************************************************************************
* 文 件 名: modbus.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-21
* 文件说明: Modbus函数库API接口
*******************************************************************************/

#include "modbus.h"
#include "modbus_rtu.h"
#include "modbus_func.h"
/*******************************************************************************

                                    全局变量

*******************************************************************************/
static uint8_t slave_addr = 1;

static eMBError prvSlavePDUHandle(uint8_t *pPDUFrame, uint16_t *pPDULength);
/*******************************************************************************

                                    API接口

*******************************************************************************/
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
eMBError modbus_SlaveInit(eMBMode mode, uint8_t slaveAddr)
{
eMBError status = MB_OK;

    if ((MB_ADDRESS_BROADCAST == slaveAddr) || (slaveAddr < MB_ADDRESS_MIN) ||
        (slaveAddr > MB_ADDRRSS_MAX))
    {
        status = MB_ERR_INVAL;
    }
    else
    {
        slave_addr = slaveAddr;
        status = MB_OK;
    }

    return (status);
}

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
eMBError modbus_SlaveDeal(uint8_t *buffer, uint16_t *length)
{
eMBError status = MB_ERR_IGNORED;
uint8_t *pdu_frame;
uint16_t pdu_length;
uint8_t rcv_addr;

    MBRTUInit(buffer, length);
    if (false != MBRTUReceive(&rcv_addr, &pdu_frame, &pdu_length))
    {
        if (rcv_addr == slave_addr)
        {
            status = prvSlavePDUHandle(pdu_frame, &pdu_length);
        }
        else
    #if MB_SLAVE_BROADCAST
        if (rcv_addr == MB_ADDRESS_BROADCAST)
        {
            prvSlavePDUHandle(pdu_frame, &pdu_length);
            status = MB_ERR_IGNORED;
        }
        else
    #endif
        {
            status = MB_ERR_IGNORED;
        }
    }
    else
    {
        status = MB_ERR_IGNORED;
    }

    if (MB_OK == status)
    {
        MBRTUTransmit(slave_addr, pdu_frame, pdu_length);
    }
    else
    {
        *length = 0;
    }

    return (status);
}

/*******************************************************************************

                                    内部函数

*******************************************************************************/
/**
 * 从机处理PDU数据帧, 并生成响应
 *
 * @param pPDUFrame: PDU数据帧的起始地址
 *
 * @param pPDULength: 保存PDU数据帧长度的变量的指针
 *
 * @return: MB_OK - 已生成PDU响应帧
 *          MB_ERR_IGNORED - 没有生成响应帧
 */
static eMBError prvSlavePDUHandle(uint8_t *pPDUFrame, uint16_t *pPDULength)
{
eMBError status = MB_ERR_IGNORED;
eMBException ex_code;
uint8_t func_code;

    func_code = pPDUFrame[MB_PDU_FUNC_OFF];
    ex_code = MBSlaveFuncHandle(pPDUFrame, pPDULength);
    /*生成异常响应*/
    if (MB_EX_NONE != ex_code)
#if MB_SLAVE_FUNC_ERROR_EN
    {
        pPDUFrame[MB_PDU_FUNC_OFF] = func_code | MB_FUNC_ERROR;
        pPDUFrame[MB_PDU_DATA_OFF] = ex_code;
        *pPDULength = 2;
        status = MB_OK;
    }
#else   /* MB_SLAVE_FUNC_ERROR_EN */
    {
        ((void)ex_code);
        ((void)func_code);
        status = MB_ERR_IGNORED;
    }
#endif  /* MB_SLAVE_FUNC_ERROR_EN */
    else
    {
        status = MB_OK;
    }

    return (status);
}
