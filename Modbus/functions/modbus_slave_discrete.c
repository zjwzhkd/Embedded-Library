/*******************************************************************************
* 文 件 名: modbus_slave_discrete.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-27
* 文件说明: Modbus从机离散量输入事务处理
*******************************************************************************/

#include "modbus_func.h"
/*******************************************************************************

                                    常量定义

*******************************************************************************/
/* 读离散量输入 --------------------------------------------------------------*/
/* PDU请求帧中读离散量输入地址的偏移 */
#define MB_PDU_FUNC_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中读离散量输入数量的偏移 */
#define MB_PDU_FUNC_READ_DISCCNT_OFF            ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧中数据域长度 */
#define MB_PDU_FUNC_READ_SIZE                   ( 4 )
/* 连续读离散量输入的最大数量 */
#define MB_PDU_FUNC_READ_DISCCNT_MAX            ( 0x07D0 )

/*******************************************************************************

                                    函数接口

*******************************************************************************/

#if MB_SLAVE_FUNC_READ_DISCRETE_INPUTS_EN
/**
 * 从机读离散量输入的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncReadDiscreteInputs(uint8_t *pFrame, uint16_t *pLength)
{
eMBException ex_code = MB_EX_NONE;
eMBError reg_status;
uint16_t disc_addr;
uint16_t disc_count;
uint8_t  byte_count;
uint8_t *resp_frame;

    if ( *pLength == (MB_PDU_DATA_OFF + MB_PDU_FUNC_READ_SIZE) )
    {
        disc_addr   = (uint16_t)(pFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
        disc_addr  |= (uint16_t)(pFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
        disc_count  = (uint16_t)(pFrame[MB_PDU_FUNC_READ_DISCCNT_OFF] << 8);
        disc_count |= (uint16_t)(pFrame[MB_PDU_FUNC_READ_DISCCNT_OFF + 1]);

        /*检查读离散量输入个数是否有效*/
        if ((disc_count >= 1) && (disc_count <= MB_PDU_FUNC_READ_DISCCNT_MAX))
        {
            /*设置响应帧*/
            resp_frame = &pFrame[MB_PDU_FUNC_OFF];
            *pLength = 0;
            /*设置功能码*/
            *resp_frame++ = MB_FUNC_READ_DISCRETE_INPUTS;
            *pLength += 1;
            /*设置响应的状态值字节数*/
            if ((disc_count & 0x0007) != 0)
            {
                byte_count = (uint8_t)(disc_count/8 + 1);
            }
            else
            {
                byte_count = (uint8_t)(disc_count/8);
            }
            *resp_frame++ = byte_count;
            *pLength += 1;
            /*读离散量输入*/
            reg_status = modbus_SlaveRegDiscreteCB(resp_frame, disc_addr, disc_count);
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
