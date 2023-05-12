#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <stdint.h>

#define SCL_PIN GPIO_PIN_10
#define SCL_PORT GPIOB
#define SDA_PIN GPIO_PIN_11
#define SDA_PORT GPIOB

#define SCL_H HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET)
#define SCL_L HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET)
#define SDA_H HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET)
#define SDA_L HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET)
#define SDA_READ HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)

void I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = SCL_PIN|SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    SCL_H;
    SDA_H;
}

void I2C_Start(void)
{
    SDA_H;
    SCL_H;
    HAL_Delay(1);
    SDA_L;
    HAL_Delay(1);
    SCL_L;
}

void I2C_Stop(void)
{
    SDA_L;
    SCL_H;
    HAL_Delay(1);
    SDA_H;
}

void I2C_Ack(void)
{
    SCL_L;
    SDA_L;
    SCL_H;
    HAL_Delay(1);
    SCL_L;
}

void I2C_NoAck(void)
{
    SCL_L;
    SDA_H;
    SCL_H;
    HAL_Delay(1);
    SCL_L;
}

uint8_t I2C_WaitAck(void)
{
    SCL_L;
    SDA_H;
    SCL_H;
    HAL_Delay(1);
    if(SDA_READ)
    {
        SCL_L;
        return 1;
    }
    SCL_L;
    return 0;
}

void I2C_SendByte(uint8_t byte)
{
    uint8_t i;
    for(i=0; i<8; i++)
    {
        SCL_L;
        if(byte & 0x80)
            SDA_H;
        else
            SDA_L;
        byte <<= 1;
        SCL_H;
        HAL_Delay(1);
    }
    SCL_L;
}

uint8_t I2C_ReadByte(void)
{
    uint8_t i, byte=0;
    SDA_H;
    for(i=0; i<8; i++)
    {
        SCL_L;
        HAL_Delay(1);
        SCL_H;
        byte <<= 1;
        if(SDA_READ)
            byte++;
    }
    SCL_L;
    return byte;
}

