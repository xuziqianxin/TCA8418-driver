#include "TCA8418.h"
/**
  * @file    TCA8418.c
  * @brief   This file provides code for initializing and using the functions of the TCA8418.
  * @author  虚字潜心(xu_zi_qian_xin)
  * @version 0.1.0
  * @date    2026-03-06
  * 
  * @copyright Copyright (c) 2026 虚字潜心. All rights reserved.
  * @license  SPDX-License-Identifier: MIT
  */
TCA8418_REGISTER_Def TCARegister = { 0 };
ERROR_CODE Error = { 0 };
uint64_t KeyMap = 0;
int16_t KeyLine = 0;

/**
 * @file     softwareiic.c
 * @brief    ???STM32 HAL??CUBEMX?????????IIC
 * @version  V1.0.0
 * @author   Sundea
 * @date     2023/5/8
 */


/**
 * @brief IIC??
 * @param  ?
 * @return ?
 */
void IIC_Delay(uint8_t time)
{
  uint8_t n = SystemCoreClock/1000000/8*time;
	do
	{
		__NOP();
	}
	while(n--);
}

/**
 * @brief IIC???
 * @param  ?
 * @return ?
 */
void Soft_IIC_Init(void)
{
    IIC_SDA_H();
    IIC_SCL_H();
}

/**
 * @brief SDA????????
 * @param  ?
 * @return ?
 */
static void Soft_IIC_Output(void)
{
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_OUTPUT_PP;
    SOFT_IIC_GPIO_STRUCT.Pin = IIC_SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(IIC_SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

/**
 * @brief SDA????????
 * @param  ?
 * @return ?
 */
static void Soft_IIC_Input(void)
{
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_INPUT;
    SOFT_IIC_GPIO_STRUCT.Pin = IIC_SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(IIC_SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

/**
 * @brief IIC????
 * @param  ?
 * @return ?
 */
void Soft_IIC_Start(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC????
 * @param  ?
 * @return ?
 */
void Soft_IIC_Stop(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
}

/**
 * @brief IIC????
 * @param  ?
 * @return ?
 */
void Soft_IIC_ACK(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC?????
 * @param  ?
 * @return ?
 */
void Soft_IIC_NACK(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
}

/**
 * @brief IIC??????
 * @param  ?
 * @return 0???  1???
 */
uint8_t Soft_IIC_Wait_ACK(void)
{
    uint8_t wait;
    Soft_IIC_Output();
    IIC_SDA_H();
    Soft_IIC_Input();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    while (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN))
    {
        wait++;
        if (wait > 200)
        {
            Soft_IIC_Stop();
            return 0;
        }
    }
    IIC_SCL_L();
    return 1;
}

/**
 * @brief IIC???1
 * @param  ?
 * @return ?
 */
void Soft_IIC_Write_High(void)
{
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC???0
 * @param  ?
 * @return ?
 */
void Soft_IIC_Write_Low(void)
{
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC??????
 * @param  ?
 * @return ????, 0??? 1???
 */
uint8_t Soft_IIC_Write_Byte(uint8_t Byte)
{
    uint8_t i;
    Soft_IIC_Output();
    for (i = 0x80; i != 0; i >>= 1)
    {
        if (Byte & i)
        {
            Soft_IIC_Write_High();
        }
        else
        {
            Soft_IIC_Write_Low();
        }
    }
    return (Soft_IIC_Wait_ACK());
}

/**
 * @brief IIC?????
 * @param  ACK:?? NACK:???
 * @return ???????
 */
uint8_t Soft_IIC_Recv_Byte(ACK_STATUS ack_sta)
{
    uint8_t data = 0, i;
    Soft_IIC_Input();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    for (i = 0x80; i != 0; i >>= 1)
    {
        if (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN) == 1)
        {
            data |= i;
        }
        IIC_Delay(IIC_DELAY_TIME);
        IIC_SCL_L();
        IIC_Delay(IIC_DELAY_TIME);
        IIC_SCL_H();
        IIC_Delay(IIC_DELAY_TIME);
    }
    if (ack_sta == ACK)
    {
        Soft_IIC_ACK();
    }
    else
    {
        Soft_IIC_NACK();
    }
    return data;
}

/**
	* @brief: TCA8418 Write Register
	* @param[in] Address: TCA8418 address
	* @param[in] Register: TCA8418 Register address
	* @param[in] data: TCA8418 wirte data
	*/
void TCA8418_Write_Register(uint8_t Address, uint8_t Register, uint8_t data)
{
	Soft_IIC_Start();
	Error.ACT_TIME_OUT = Soft_IIC_Write_Byte(Address);
	Error.ACT_TIME_OUT = Soft_IIC_Write_Byte(Register);
	Error.ACT_TIME_OUT = Soft_IIC_Write_Byte(data);
	Soft_IIC_Stop();
}

uint8_t TCA8418_Read_Register(uint8_t Address, uint8_t Register)
{
	uint8_t data = 0;
	Soft_IIC_Start();
	Error.ACT_TIME_OUT = Soft_IIC_Write_Byte(Address);
	Error.ACT_TIME_OUT = Soft_IIC_Write_Byte(Register);
	
	Soft_IIC_Start();
	Error.ACT_TIME_OUT = Soft_IIC_Write_Byte(Address+1);
	data = Soft_IIC_Recv_Byte(NACK);
	Soft_IIC_Stop();
	return data;
}

void TCA8418_Init(void)
{
	TCARegister.CFG = 0x0;
	TCARegister.KP_GPIO1 = 0x0;
	TCARegister.KP_GPIO2 = 0x0;
	TCARegister.KP_GPIO3 = 0x0;
	TCARegister.GPIO_DIR1 = 0x3F;
	TCARegister.GPIO_DIR2 = 0x0;
	TCARegister.GPIO_DIR3 = 0x0;
	TCARegister.GPIO_DAT_OUT1 = 0x0;
	TCARegister.DEBOUNCE_DIS1 = 0xFF;
	TCARegister.DEBOUNCE_DIS2 = 0x0;
	TCARegister.DEBOUNCE_DIS3 = 0x0;
	TCARegister.GPIO_PULL1 = 0xFF;
	TCARegister.GPIO_PULL2 = 0xFF;
	TCARegister.GPIO_PULL3 = 0xFF;
	TCA8418_Write_Register(WADDR, CFG, TCARegister.CFG);
	TCA8418_Write_Register(WADDR, KP_GPIO1, TCARegister.KP_GPIO1);
	TCA8418_Write_Register(WADDR, KP_GPIO2, TCARegister.KP_GPIO2);
	TCA8418_Write_Register(WADDR, KP_GPIO3, TCARegister.KP_GPIO3);
	TCA8418_Write_Register(WADDR, GPIO_DIR1, TCARegister.GPIO_DIR1);
	TCA8418_Write_Register(WADDR, GPIO_DIR2, TCARegister.GPIO_DIR2);
	TCA8418_Write_Register(WADDR, GPIO_DIR3, TCARegister.GPIO_DIR3);
	TCA8418_Write_Register(WADDR, GPIO_DAT_OUT1, TCARegister.GPIO_DAT_OUT1);
	TCA8418_Write_Register(WADDR, DEBOUNCE_DIS1, TCARegister.DEBOUNCE_DIS1);
	TCA8418_Write_Register(WADDR, DEBOUNCE_DIS2, TCARegister.DEBOUNCE_DIS2);
	TCA8418_Write_Register(WADDR, DEBOUNCE_DIS3, TCARegister.DEBOUNCE_DIS3);
	TCA8418_Write_Register(WADDR, GPIO_PULL1, TCARegister.GPIO_PULL1);
	TCA8418_Write_Register(WADDR, GPIO_PULL2, TCARegister.GPIO_PULL2);
	TCA8418_Write_Register(WADDR, GPIO_PULL3, TCARegister.GPIO_PULL3);
}

uint64_t Scan_Keybord(void)
{
	KeyMap = 0;
	KeyLine = 0;
	TCARegister.GPIO_DAT_OUT1 = 0x10;
	for(uint8_t i = 0; i < 5; i++)
	{
		TCA8418_Write_Register(WADDR, GPIO_DAT_OUT1, (TCARegister.GPIO_DAT_OUT1 >> i));
		KeyLine = TCA8418_Read_Register(WADDR, GPIO_DAT_STAT3);
		KeyLine = KeyLine << 8;
		TCA8418_Read_Register(WADDR, GPIO_DAT_STAT3);
		KeyLine = KeyLine + TCA8418_Read_Register(WADDR, GPIO_DAT_STAT2);
		TCA8418_Read_Register(WADDR, GPIO_DAT_STAT2);
		KeyMap = KeyMap + KeyLine;
		if(i != 4)
		{
			KeyMap = KeyMap << 10;
		}
	}
	return KeyMap;
}
