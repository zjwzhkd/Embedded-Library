/*******************************************************************************
* 文 件 名: modbus_utils.c
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-25
* 文件说明: Modbus实用工具
*******************************************************************************/

#include "modbus_utils.h"
/*******************************************************************************

                                    实用函数

*******************************************************************************/
/**
 * 本函数通过数组来实现位域设置操作, 一次操作最多置位/清除8位
 *
 * @param byteBuf: 待操作的字节缓存, 注意本函数没有进行长度检查,
 *                 缓存的长度必须大于bitOffset/8+1
 *
 * @param bitOffset: 待操作的起始位地址, 注意, byteBuf[0]的LSB位地址为0
 *
 * @param nBits: 待修改的位个数, 不能超过8位
 *
 * @param value: 待修改的位新值, bitOffset指向的位新值是value的LSB位值
 */
void libModbusUtilSetBits(uint8_t *byteBuf, uint16_t bitOffset, uint8_t nBits, uint8_t value)
{
uint16_t byte_offset, pre_bits;
uint16_t tmp_buf;
uint16_t tmp_value = value;
uint16_t tmp_mask = (uint16_t)( (1 << (uint16_t)nBits) - 1 );

    libAssert(nBits <= 8);

    /*计算起始位地址对应的字节偏移*/
    byte_offset = (uint16_t)(bitOffset/8);
    /*起始位地址在字节内对应的位置*/
    pre_bits = (uint16_t)(bitOffset - byte_offset*8);

    /*移位*/
    tmp_value <<= pre_bits;
    tmp_mask <<= pre_bits;

    /*读-修改-写*/
    tmp_buf  = (uint16_t)(byteBuf[byte_offset]);
    tmp_buf |= (uint16_t)(byteBuf[byte_offset+1] << 8);

    tmp_buf = (tmp_buf & (~tmp_mask)) | (tmp_value & tmp_mask);

    byteBuf[byte_offset] = (uint8_t)(tmp_buf & 0xFF);
    byteBuf[byte_offset+1] = (uint8_t)(tmp_buf >> 8);
}

/**
 * 本函数通过数组来实现位域读取操作, 一次操作最多读取8位
 *
 * @param byteBuf: 待读取的字节缓存
 *
 * @param bitOffset: 待读取的起始位地址, 注意, byteBuf[0]的LSB位地址为0
 *
 * @param nBits: 待读取的位个数, 不能超过8位
 *
 * @return: 读取的位值, 返回结果的LSB位值是bitOffset指向的位值
 */
uint8_t libModbusUtilGetBits(uint8_t *byteBuf, uint16_t bitOffset, uint8_t nBits)
{
uint16_t byte_offset, pre_bits;
uint16_t tmp_buf;
uint16_t tmp_mask = (uint16_t)( (1 << (uint16_t)nBits) - 1 );

    libAssert(nBits <= 8);

    /*计算起始位地址对应的字节偏移*/
    byte_offset = (uint16_t)(bitOffset/8);
    /*起始位地址在字节内对应的位置*/
    pre_bits = (uint16_t)(bitOffset - byte_offset*8);

    /*读数据*/
    tmp_buf  = (uint16_t)(byteBuf[byte_offset]);
    tmp_buf |= (uint16_t)(byteBuf[byte_offset+1] << 8);
    tmp_buf >>= pre_bits;
    tmp_buf &= tmp_mask;

    return ((uint8_t)tmp_buf);
}
