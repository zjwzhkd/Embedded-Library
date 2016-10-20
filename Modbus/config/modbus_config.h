/*******************************************************************************
* 文 件 名: modbus_config.h
* 创 建 者: Keda Huang
* 版    本: V1.0
* 创建日期: 2016-10-20
* 文件说明: Modbus配置
*******************************************************************************/

#ifndef __MODBUS_CONFIG_H
#define __MODBUS_CONFIG_H

/* 从机配置 ------------------------------------------------------------------*/
/* 从机广播功能 */
#define MB_SLAVE_BROADCAST                                  ( 0 )
/* 从机读线圈功能 */
#define MB_SLAVE_FUNC_READ_COILS_EN                         ( 1 )
/* 从机读离散量输入功能 */
#define MB_SLAVE_FUNC_READ_DISCRETE_INPUTS_EN               ( 1 )
/* 从机读保持寄存器功能 */
#define MB_SLAVE_FUNC_READ_HOLDING_REGISTERS_EN             ( 1 )
/* 从机读输入寄存器功能 */
#define MB_SLAVE_FUNC_READ_INPUT_REGISTERS_EN               ( 1 )
/* 从机写单个线圈功能 */
#define MB_SLAVE_FUNC_WRITE_SINGLE_COIL_EN                  ( 1 )
/* 从机写单个寄存器功能 */
#define MB_SLAVE_FUNC_WRITE_SINGLE_REGISTER_EN              ( 1 )
/* 从机写多个线圈功能 */
#define MB_SLAVE_FUNC_WRITE_MULTIPLE_COILS_EN               ( 1 )
/* 从机写多个寄存器功能 */
#define MB_SLAVE_FUNC_WRITE_MULTIPLE_REGISTERS_EN           ( 1 )
/* 从机读/写多个寄存器功能 */
#define MB_SLAVE_FUNC_READWRITE_MULTIPLE_REGISTERS_EN       ( 1 )
/* 从机异常响应功能 */
#define MB_SLAVE_FUNC_ERROR_EN                              ( 1 )

#endif
