/*******************************************************************************
* 文 件 名: modbus_rtu.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-20
* 文件说明: Modbus RTU数据帧处理
*******************************************************************************/

#include "modbus_rtu.h"
#include "modbus_crc16.h"
/*******************************************************************************

                                    常量定义

*******************************************************************************/
/* 格式定义 ------------------------------------------------------------------*/
/* Modbus RTU帧长最小值 */
#define MB_RTU_ADU_SIZE_MIN             ( 4 )
/* Modbus RTU帧长最大值 */
#define MB_RTU_ADU_SIZE_MAX             ( 256 )
/* Modbus RTU校验长度 */
#define MB_RTU_ADU_SIZE_CRC             ( 2 )
/* Modbus RTU地址偏移 */
#define MB_RTU_ADU_ADDR_OFF             ( 0 )
/* Modbus RTU PDU偏移 */
#define MB_RTU_ADU_PDU_OFF              ( 1 )

/*******************************************************************************

                                    全局变量

*******************************************************************************/
/* Modbus RTU缓存设置 */
static uint8_t *rtu_adu_buf;
static uint16_t *rtu_adu_plen;

/*******************************************************************************

                                    处理函数

*******************************************************************************/
/**
 * 初始化Modbus RTU缓冲设置
 *
 * @param pBuf: 帧缓冲起始地址
 *
 * @param pBufLen: 帧长度变量指针
 */
void MBRTUInit(uint8_t *pBuf, uint16_t *pBufLen)
{
    rtu_adu_buf = pBuf;
    rtu_adu_plen = pBufLen;
}

/**
 * 接收Modbus RTU数据帧
 *
 * @param pRcvAddr: 保存接收地址的变量的指针
 *
 * @param pFrame: 保存PDU起始地址的变量的指针
 *
 * @param pLength: 保存PDU长度的变量的指针
 *
 * @return: 布尔值, 返回true表示成功的接收, 返回false表示无效的接收
 */
bool MBRTUReceive(uint8_t *pRcvAddr, uint8_t **pFrame, uint16_t *pLength)
{
bool status = false;
uint16_t adu_len = *rtu_adu_plen;
uint8_t *adu_frame = rtu_adu_buf;
uint16_t adu_crc16;

    if ((adu_len >= MB_RTU_ADU_SIZE_MIN) && (adu_len <= MB_RTU_ADU_SIZE_MAX))
    {
        adu_crc16 = (uint16_t)(adu_frame[adu_len-1]<<8) + adu_frame[adu_len-2];
        if (adu_crc16 == modbus_crc16(adu_frame, adu_len-MB_RTU_ADU_SIZE_CRC))
        {
            *pRcvAddr = adu_frame[MB_RTU_ADU_ADDR_OFF];
            *pFrame = &adu_frame[MB_RTU_ADU_PDU_OFF];
            *pLength = adu_len - MB_RTU_ADU_PDU_OFF - MB_RTU_ADU_SIZE_CRC;
            status = true;
        }
    }

    return (status);
}

/**
 * 发送Modbus RTU数据帧
 *
 * @param slaveAddr: 从机地址
 *
 * @param pFrame: PDU的起始地址
 *
 * @param length: PDU长度
 */
void MBRTUTransmit(uint8_t slaveAddr, uint8_t const *pFrame, uint16_t length)
{
uint16_t adu_crc16;

    ((void)pFrame);
    rtu_adu_buf[MB_RTU_ADU_ADDR_OFF] = slaveAddr;
    length++;
    adu_crc16 = modbus_crc16(rtu_adu_buf, length);
    rtu_adu_buf[length++] = adu_crc16 & 0xff;
    rtu_adu_buf[length++] = adu_crc16 >> 8;
    *rtu_adu_plen = length;
}
