/*******************************************************************************
* 文 件 名: modbus_slave_holding.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-22
* 文件说明: Modbus从机保持寄存器事务处理
*******************************************************************************/

#include "modbus_func.h"
/*******************************************************************************

                                    常量定义

*******************************************************************************/
/* 读保持寄存器 --------------------------------------------------------------*/
/* PDU请求帧中寄存器地址的偏移 */
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中寄存器数量的偏移 */
#define MB_PDU_FUNC_READ_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧中数据域长度 */
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
/* 连续读寄存器的最大数量 */
#define MB_PDU_FUNC_READ_REGCNT_MAX         ( 0x007D )

/* 写单个保持寄存器 ----------------------------------------------------------*/
/* PDU请求帧中寄存器地址的偏移 */
#define MB_PDU_FUNC_WRITE_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中寄存器写入值的偏移 */
#define MB_PDU_FUNC_WRITE_VALUE_OFF         ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧数据域长度 */
#define MB_PDU_FUNC_WRITE_SIZE              ( 4 )

/* 写多个保持寄存器 ----------------------------------------------------------*/
/* PDU请求帧中寄存器地址的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中寄存器数量的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF    ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧中字节数量的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF   ( MB_PDU_DATA_OFF + 4 )
/* PDU请求帧中寄存器写入值的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF    ( MB_PDU_DATA_OFF + 5 )
/* PDU请求帧数据域(不含写入值)长度 */
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN      ( 5 )
/* 连续写寄存器的最大数量 */
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX    ( 0x007B )

/*******************************************************************************

                                    函数接口

*******************************************************************************/
#if MB_SLAVE_FUNC_READ_HOLDING_REGISTERS_EN
/**
 * 从机读保持寄存器的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncReadHoldingRegisters(uint8_t *pFrame, uint16_t *pLength)
{
eMBException ex_code = MB_EX_NONE;
eMBError reg_status;
uint16_t reg_addr;
uint16_t reg_count;
uint8_t *resp_frame;

    if ( *pLength == (MB_PDU_DATA_OFF + MB_PDU_FUNC_READ_SIZE) )
    {
        reg_addr   = (uint16_t)(pFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
        reg_addr  |= (uint16_t)(pFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
        reg_count  = (uint16_t)(pFrame[MB_PDU_FUNC_READ_REGCNT_OFF] << 8);
        reg_count |= (uint16_t)(pFrame[MB_PDU_FUNC_READ_REGCNT_OFF + 1]);

        /*检查读寄存器的数量是否有效*/
        if ((reg_count >= 1) && (reg_count <= MB_PDU_FUNC_READ_REGCNT_MAX))
        {
            /*设置响应帧至起始位置*/
            resp_frame = &pFrame[MB_PDU_FUNC_OFF];
            *pLength = 0;
            /*设置功能码*/
            *resp_frame++ = MB_FUNC_READ_HOLDING_REGISTERS;
            *pLength += 1;
            /*设置读字节数*/
            *resp_frame++ = (uint8_t)(reg_count<<1);
            *pLength += 1;
            /*读保持寄存器*/
            reg_status = modbus_SlaveRegHoldingCB(resp_frame, reg_addr, reg_count, MB_REG_READ);
            if (MB_OK != reg_status)
            {
                ex_code = MBError2MBException(reg_status);
            }
            else
            {
                *pLength += reg_count<<1;
                ex_code = MB_EX_NONE;
            }
        }
        else
        {
            ex_code = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        ex_code = MB_EX_ILLEGAL_DATA_VALUE;
    }

    return (ex_code);
}
#endif

#if MB_SLAVE_FUNC_WRITE_SINGLE_REGISTER_EN
/**
 * 从机写单个保持寄存器的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncWriteSingleRegister(uint8_t *pFrame, uint16_t *pLength)
{
eMBException ex_code = MB_EX_NONE;
eMBError reg_status;
uint16_t reg_addr;

    if ( *pLength == (MB_PDU_DATA_OFF + MB_PDU_FUNC_WRITE_SIZE) )
    {
        reg_addr  = (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8);
        reg_addr |= (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1]);
        /*写保持寄存器*/
        reg_status = modbus_SlaveRegHoldingCB(&pFrame[MB_PDU_FUNC_WRITE_VALUE_OFF],
                                               reg_addr, 1, MB_REG_WRITE);
        if (MB_OK != reg_status)
        {
            ex_code = MBError2MBException(reg_status);
        }
        else
        {
            /*响应帧与请求帧一致*/
            ex_code = MB_EX_NONE;
        }
    }
    else
    {
        ex_code = MB_EX_ILLEGAL_DATA_VALUE;
    }

    return (ex_code);
}
#endif

#if MB_SLAVE_FUNC_WRITE_MULTIPLE_REGISTERS_EN
/**
 * 从机写多个保持寄存器的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncWriteMultipleRegisters(uint8_t *pFrame, uint16_t *pLength)
{
eMBException ex_code = MB_EX_NONE;
eMBError reg_status;
uint16_t reg_addr;
uint16_t reg_count;
uint8_t  byte_count;

    if ( *pLength >= (MB_PDU_DATA_OFF + MB_PDU_FUNC_WRITE_MUL_SIZE_MIN) )
    {
        reg_addr   = (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8);
        reg_addr  |= (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1]);
        reg_count  = (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8);
        reg_count |= (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1]);
        byte_count = (uint8_t)pFrame[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

        if ( (reg_count >= 1) &&
             (reg_count <= MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX) &&
             (byte_count == (uint8_t)(2*reg_count)) &&
             (*pLength == (MB_PDU_DATA_OFF + MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + 2*reg_count)) )
        {
            /*写保持寄存器*/
            reg_status = modbus_SlaveRegHoldingCB(&pFrame[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF],
                                                   reg_addr, reg_count, MB_REG_WRITE);
            if (MB_OK != reg_status)
            {
                ex_code = MBError2MBException(reg_status);
            }
            else
            {
                /*从写入字节数量域处, 截断请求帧, 生成响应帧*/
                *pLength = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
                ex_code = MB_EX_NONE;
            }
        }
        else
        {
            ex_code = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        ex_code = MB_EX_ILLEGAL_DATA_VALUE;
    }

    return (ex_code);
}
#endif

#if MB_SLAVE_FUNC_READWRITE_MULTIPLE_REGISTERS_EN
eMBException MBSlaveFuncReadWriteMultipleRegisters(uint8_t *pFrame, uint16_t *pLength);
#endif
