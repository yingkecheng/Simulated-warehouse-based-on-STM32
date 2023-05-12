#include "sensor.h"
#include "cmsis_os.h"


#define AHT20_I2C_ADDRESS 0x38  // AHT20 I2C address
#define AHT20_INIT_CMD    0xE1  // AHT20 initialization command
#define AHT20_START_CMD   0xAC  // AHT20 start measurement command
#define AHT20_NORMAL_CMD  0xA8  // AHT20 normal cycle mode command
#define AHT20_SOFT_RESET  0xBA  // AHT20 soft reset command

#define AHT20_TEMPERATURE_CONST 200.0f
#define AHT20_HUMIDITY_CONST 100.0f


void AHT20_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[3] = {AHT20_INIT_CMD, 0x08, 0x00};
    HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDRESS << 1, data, sizeof(data), 100);
}

void AHT20_Start_Measure(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[3] = {AHT20_START_CMD, 0x33, 0x00};
    HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDRESS << 1, data, sizeof(data), 100);
}

void AHT20_Read_Data(I2C_HandleTypeDef *hi2c, uint8_t* data)
{
    HAL_I2C_Master_Receive(hi2c, (AHT20_I2C_ADDRESS << 1) | 0x01, data, 6, 100);
}

void AHT20_Reset(I2C_HandleTypeDef *hi2c)
{
    uint8_t data = AHT20_SOFT_RESET;
    HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDRESS << 1, &data, 1, 100);
}

void AHT20_GetData(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity)
{
    uint8_t data[6];
    uint32_t raw_humidity;
    uint32_t raw_temperature;

    AHT20_Start_Measure(hi2c);
    HAL_Delay(80);  // Wait for the measurement to complete
    AHT20_Read_Data(hi2c, data);

    raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    raw_temperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    *humidity = ((float)raw_humidity * AHT20_HUMIDITY_CONST) / 1048576.0f;
    *temperature = ((float)raw_temperature * AHT20_TEMPERATURE_CONST / 1048576.0f) - 50.0f;
}

#define BH1750_I2C_ADDRESS_HIGH 0x5C 
#define BH1750_POWER_ON         0x01 
#define BH1750_RESET            0x07 
#define BH1750_CONT_HIGH_RES_MODE  0x10 

void BH1750_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[1] = {BH1750_POWER_ON};
    HAL_I2C_Master_Transmit(hi2c, BH1750_I2C_ADDRESS_HIGH << 1, data, sizeof(data), 100);
}

void BH1750_Start(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[1] = {BH1750_CONT_HIGH_RES_MODE};
    HAL_I2C_Master_Transmit(hi2c, BH1750_I2C_ADDRESS_HIGH << 1, data, sizeof(data), 100);
}

uint16_t BH1750_Read(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[2];
    uint16_t light_intensity;

    HAL_I2C_Master_Receive(hi2c, (BH1750_I2C_ADDRESS_HIGH << 1) | 0x01, data, sizeof(data), 100);
    light_intensity = (uint16_t)(data[0] << 8 | data[1]);

    return light_intensity / 1.2; 
}
