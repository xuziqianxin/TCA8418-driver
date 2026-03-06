/**
  * @file    TCA8418.h
  * @brief   This document contains all the function prototypes of the TCA8418.c file
  * @author  虚字潜心(xu_zi_qian_xin)
  * @version 0.1.0
  * @date    2026-03-06
  * 
  * @copyright Copyright (c) 2026 虚字潜心. All rights reserved.
  * @license  SPDX-License-Identifier: MIT
  */
	
#ifndef __TCA8418_H_
#define __TCA8418_H_

#include "main.h"

typedef enum{
    NACK = 0,
    ACK  = 1
}ACK_STATUS;

#define IIC_DELAY_TIME 10

#define IIC_SCL_PIN GPIO_PIN_4
#define IIC_SCL_PORT GPIOB

#define IIC_SDA_PIN GPIO_PIN_5
#define IIC_SDA_PORT GPIOB

// #define IIC_SDA_H() GPIOB->BSRR = 0x010
// #define IIC_SDA_L() GPIOB->BRR = 0x010
// #define IIC_SCL_H() GPIOB->BSRR = 0x100
// #define IIC_SCL_L() GPIOB->BRR = 0x100

#define IIC_SCL_H()     HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET)
#define IIC_SCL_L()     HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET)
#define IIC_SDA_H()     HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET)
#define IIC_SDA_L()     HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_RESET)

#define WADDR 0x68
#define RADDR 0x69

typedef enum TCA8418MAP
{
	CFG = 0x01,
	INT_STAT = 0x02,
	KEY_LCK_EC,
	KEY_EVENT_A,
	KEY_EVENT_B,
	KEY_EVENT_C,
	KEY_EVENT_D,
	KEY_EVENT_E,
	KEY_EVENT_F,
	KEY_EVENT_G,
	KEY_EVENT_H,
	KEY_EVENT_I,
	KEY_EVENT_J,
	KP_LCK_TIMER,
	UNLOCK1,
	UNLOCK2,
	GPIO_INT_STAT1,
	GPIO_INT_STAT2,
	GPIO_INT_STAT3,
	GPIO_DAT_STAT1,
	GPIO_DAT_STAT2,
	GPIO_DAT_STAT3,
	GPIO_DAT_OUT1,
	GPIO_DAT_OUT2,
	GPIO_DAT_OUT3,
	GPIO_INT_EN1,
	GPIO_INT_EN2,
	GPIO_INT_EN3,
	KP_GPIO1,
	KP_GPIO2,
	KP_GPIO3,
	GPI_EM1,
	GPI_EM2,
	GPI_EM3,
	GPIO_DIR1,
	GPIO_DIR2,
	GPIO_DIR3,
	GPIO_INT_LVL1,
	GPIO_INT_LVL2,
	GPIO_INT_LVL3,
	DEBOUNCE_DIS1,
	DEBOUNCE_DIS2,
	DEBOUNCE_DIS3,
	GPIO_PULL1,
	GPIO_PULL2,
	GPIO_PULL3
} TCA8418_REGISTER_MAP;

typedef struct TCA8418_REGISTERs
{
	uint8_t CFG;
	uint8_t INT_STAT;
	uint8_t KEY_LCK_EC;
	uint8_t KEY_EVENT_A;
	uint8_t KEY_EVENT_B;
	uint8_t KEY_EVENT_C;
	uint8_t KEY_EVENT_D;
	uint8_t KEY_EVENT_E;
	uint8_t KEY_EVENT_F;
	uint8_t KEY_EVENT_G;
	uint8_t KEY_EVENT_H;
	uint8_t KEY_EVENT_I;
	uint8_t KEY_EVENT_J;
	uint8_t KP_LCK_TIMER;
	uint8_t UNLOCK1;
	uint8_t UNLOCK2;
	uint8_t GPIO_INT_STAT1;
	uint8_t GPIO_INT_STAT2;
	uint8_t GPIO_INT_STAT3;
	uint8_t GPIO_DAT_STAT1;
	uint8_t GPIO_DAT_STAT2;
	uint8_t GPIO_DAT_STAT3;
	uint8_t GPIO_DAT_OUT1;
	uint8_t GPIO_DAT_OUT2;
	uint8_t GPIO_DAT_OUT3;
	uint8_t GPIO_INT_EN1;
	uint8_t GPIO_INT_EN2;
	uint8_t GPIO_INT_EN3;
	uint8_t KP_GPIO1;
	uint8_t KP_GPIO2;
	uint8_t KP_GPIO3;
	uint8_t GPI_EM1;
	uint8_t GPI_EM2;
	uint8_t GPI_EM3;
	uint8_t GPIO_DIR1;
	uint8_t GPIO_DIR2;
	uint8_t GPIO_DIR3;
	uint8_t GPIO_INT_LVL1;
	uint8_t GPIO_INT_LVL2;
	uint8_t GPIO_INT_LVL3;
	uint8_t DEBOUNCE_DIS1;
	uint8_t DEBOUNCE_DIS2;
	uint8_t DEBOUNCE_DIS3;
	uint8_t GPIO_PULL1;
	uint8_t GPIO_PULL2;
	uint8_t GPIO_PULL3;
} TCA8418_REGISTER_Def;

typedef union TCA8418_ERROR
{
	uint8_t ACT_TIME_OUT;
} ERROR_CODE;

void Soft_IIC_Init(void);
void Soft_IIC_Start(void);
void Soft_IIC_Stop(void);
void Soft_IIC_ACK(void);
void Soft_IIC_NACK(void);
uint8_t Soft_IIC_Wait_ACK(void);
uint8_t Soft_IIC_Write_Byte(uint8_t Byte);
uint8_t Soft_IIC_Recv_Byte(ACK_STATUS ack_sta);
void TCA8418_Init(void);
void TCA8418_Write_Register(uint8_t Address, uint8_t Register, uint8_t data);
uint8_t TCA8418_Read_Register(uint8_t Address, uint8_t Register);
uint64_t Scan_Keybord(void);



#endif /* __TCA8418_H_ */
