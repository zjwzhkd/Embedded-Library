/*******************************************************************************
* 文 件 名: modbus_slave_coil.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-26
* 文件说明: Modbus从机线圈事务处理
*******************************************************************************/

#include "modbus_func.h"
/*******************************************************************************

                                    常量定义

*******************************************************************************/
/* 读线圈 --------------------------------------------------------------------*/
/* PDU请求帧中读线圈地址的偏移 */
#define MB_PDU_FUNC_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中读线圈数量的偏移 */
#define MB_PDU_FUNC_READ_COILCNT_OFF            ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧中数据域长度 */
#define MB_PDU_FUNC_READ_SIZE                   ( 4 )
/* 连续读线圈的最大数量 */
#define MB_PDU_FUNC_READ_COILCNT_MAX            ( 0x07D0 )

/* 写单个线圈 ----------------------------------------------------------------*/
/* PDU请求帧中写线圈地址的偏移 */
#define MB_PDU_FUNC_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中线圈状态值的偏移 */
#define MB_PDU_FUNC_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧数据域长度 */
#define MB_PDU_FUNC_WRITE_SIZE                  ( 4 )

/* 写多个线圈 ----------------------------------------------------------------*/
/* PDU请求帧中写线圈地址的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中写线圈数量的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF       ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧中写入字节数的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 4 )
/* PDU请求帧中线圈状态值的偏移 */
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF        ( MB_PDU_DATA_OFF + 5 )
/* PDU请求帧数据域(不含写入值)长度 */
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN          ( 5 )
/* 连续写线圈的最大数量 */
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX       ( 0x007B0 )

/*******************************************************************************

                                    函数接口

*******************************************************************************/
#if MB_SLAVE_FUNC_READ_COILS_EN
/**
 * 从机读线圈的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncReadCoils(uint8_t *pFrame, uint16_t *pLength)
{
eMBException ex_code = MB_EX_NONE;
eMBError reg_status;
uint16_t coil_addr;
uint16_t coil_count;
uint8_t  byte_count;
uint8_t *resp_frame;

    if ( *pLength == (MB_PDU_DATA_OFF + MB_PDU_FUNC_READ_SIZE) )
    {
        coil_addr   = (uint16_t)(pFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
        coil_addr  |= (uint16_t)(pFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
        coil_count  = (uint16_t)(pFrame[MB_PDU_FUNC_READ_COILCNT_OFF] << 8);
        coil_count |= (uint16_t)(pFrame[MB_PDU_FUNC_READ_COILCNT_OFF + 1]);

        /*检查读线圈个数是否有效*/
        if ((coil_count >= 1) && (coil_count <= MB_PDU_FUNC_READ_COILCNT_MAX))
        {
            /*设置响应帧*/
            resp_frame = &pFrame[MB_PDU_FUNC_OFF];
            *pLength = 0;
            /*设置功能码*/
            *resp_frame++ = MB_FUNC_READ_COILS;
            *pLength += 1;
            /*设置响应的状态值字节数*/
            if ((coil_count & 0x0007) != 0)
            {
                byte_count = (uint8_t)(coil_count/8 + 1);
            }
            else
            {
                byte_count = (uint8_t)(coil_count/8);
            }
            *resp_frame++ = byte_count;
            *pLength += 1;
            /*读线圈*/
            reg_status = libModbusSlaveRegCoilsCB(resp_frame, coil_addr, coil_count, MB_REG_READ);
            if (MB_OK != reg_status)
            {
                ex_code = MBError2MBException(reg_status);
            }
            else
            {
                ex_code = MB_EX_NONE;
                *pLength += byte_count;
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

#if MB_SLAVE_FUNC_WRITE_SINGLE_COIL_EN
/**
 * 从机写单个线圈的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncWriteSingleCoil(uint8_t *pFrame, uint16_t *pLength)
{
eMBException ex_code = MB_EX_NONE;
eMBError reg_status;
uint16_t coil_addr;
uint8_t  coil_buf[2];

    if ( *pLength == (MB_PDU_DATA_OFF + MB_PDU_FUNC_WRITE_SIZE) )
    {
        coil_addr  = (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8);
        coil_addr |= (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1]);
        /*检查线圈状态值*/
        if ( (0x00 == pFrame[MB_PDU_FUNC_WRITE_VALUE_OFF+1]) &&
            ((0x00 == pFrame[MB_PDU_FUNC_WRITE_VALUE_OFF]) ||
             (0xFF == pFrame[MB_PDU_FUNC_WRITE_VALUE_OFF])) )
        {
            coil_buf[1] = 0;
            if (0xFF == pFrame[MB_PDU_FUNC_WRITE_VALUE_OFF])
            {
                coil_buf[0] = 1;
            }
            else
            {
                coil_buf[0] = 0;
            }
            /*写线圈*/
            reg_status = libModbusSlaveRegCoilsCB(&coil_buf[0], coil_addr, 1, MB_REG_WRITE);
            if (MB_OK != reg_status)
            {
                ex_code = MBError2MBException(reg_status);
            }
            else
            {
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

#if MB_SLAVE_FUNC_WRITE_MULTIPLE_COILS_EN
/**
 * 从机写多个线圈的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncWriteMultipleCoils(uint8_t *pFrame, uint16_t *pLength)
{
eMBException ex_code = MB_EX_NONE;
eMBError reg_status;
uint16_t coil_addr;
uint16_t coil_count;
uint8_t  byte_count, byte_count_verify;

    if ( *pLength >= (MB_PDU_DATA_OFF + MB_PDU_FUNC_WRITE_MUL_SIZE_MIN) )
    {
        coil_addr   = (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8);
        coil_addr  |= (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1]);
        coil_count  = (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF] << 8);
        coil_count |= (uint16_t)(pFrame[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF + 1]);
        byte_count  = (uint8_t)pFrame[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];
        /*计算状态值字节数*/
        if ((coil_count & 0x0007) != 0)
        {
            byte_count_verify = (uint8_t)(coil_count/8 + 1);
        }
        else
        {
            byte_count_verify = (uint8_t)(coil_count/8);
        }

        if ((coil_count >= 1) && (coil_count <= MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX) &&
            (byte_count == byte_count_verify) &&
            (*pLength == (MB_PDU_DATA_OFF + MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + (uint16_t)byte_count)))
        {
            /*写线圈*/
            reg_status = libModbusSlaveRegCoilsCB(&pFrame[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF],
                                                 coil_addr, coil_count, MB_REG_WRITE);
            if (MB_OK != reg_status)
            {
                ex_code = MBError2MBException(reg_status);
            }
            else
            {
                ex_code = MB_EX_NONE;
                *pLength = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
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
