#ifndef SENSOR_H
#define SENSOR_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

void AHT20_Init(I2C_HandleTypeDef *hi2c);
void AHT20_Start_Measure(I2C_HandleTypeDef *hi2c);
void AHT20_Read_Data(I2C_HandleTypeDef *hi2c, uint8_t* data);
void AHT20_Reset(I2C_HandleTypeDef *hi2c);
void AHT20_GetData(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity);

void BH1750_Init(I2C_HandleTypeDef *hi2c);
void BH1750_Start(I2C_HandleTypeDef *hi2c);
uint16_t BH1750_Read(I2C_HandleTypeDef *hi2c);

#endif /* SENSOR_H */
