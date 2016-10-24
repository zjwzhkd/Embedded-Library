/*******************************************************************************
* 文 件 名: modbus_slave_input.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-24
* 文件说明: Modbus从机输入寄存器事务处理
*******************************************************************************/

#include "modbus_func.h"
/*******************************************************************************

                                    常量定义

*******************************************************************************/
/* 读输入寄存器 --------------------------------------------------------------*/
/* PDU请求帧中读寄存器地址的偏移 */
#define MB_PDU_FUNC_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0 )
/* PDU请求帧中读寄存器数量的偏移 */
#define MB_PDU_FUNC_READ_REGCNT_OFF             ( MB_PDU_DATA_OFF + 2 )
/* PDU请求帧中数据域长度 */
#define MB_PDU_FUNC_READ_SIZE                   ( 4 )
/* 连续读寄存器的最大数量 */
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x007D )

/*******************************************************************************

                                    函数接口

*******************************************************************************/
#if MB_SLAVE_FUNC_READ_INPUT_REGISTERS_EN
/**
 * 从机读输入寄存器的事务处理
 *
 * @param pFrame: PDU帧的起始地址
 *
 * @param pLength: 保存请求/响应PDU帧长度的变量的指针
 *
 * @return: 异常响应码, MB_EX_NONE表示正常响应
 */
eMBException MBSlaveFuncReadInputRegisters(uint8_t *pFrame, uint16_t *pLength)
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
            *resp_frame++ = MB_FUNC_READ_INPUT_REGISTERS;
            *pLength += 1;
            /*设置读字节数*/
            *resp_frame++ = (uint8_t)(reg_count<<1);
            *pLength += 1;
            /*读输入寄存器*/
            reg_status = modbus_SlaveRegInputCB(resp_frame, reg_addr, reg_count);
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
