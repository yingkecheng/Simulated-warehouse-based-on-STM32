#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#define AHT20_I2C_ADDRESS    0x38 << 1  // AHT20 I2C address
#define AHT20_INIT_CMD       0xE1  // AHT20 initialization command
#define AHT20_START_MEASURE  0xAC  // AHT20 start measurement command
#define AHT20_NORMAL_CMD     0xA8  // AHT20 normal cycle mode command
#define AHT20_SOFT_RESET     0xBA  // AHT20 soft reset command

// Initialize the AHT20 sensor
HAL_StatusTypeDef AHT20_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[3] = {AHT20_INIT_CMD, 0x08, 0x00};
    return HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDRESS, data, sizeof(data), HAL_MAX_DELAY);
}

// Start a measurement
HAL_StatusTypeDef AHT20_Start_Measure(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[3] = {AHT20_START_MEASURE, 0x33, 0x00};
    return HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDRESS, data, sizeof(data), HAL_MAX_DELAY);
}

// Read the measurement data
HAL_StatusTypeDef AHT20_Read_Data(I2C_HandleTypeDef *hi2c, uint8_t* data, uint16_t size)
{
    return HAL_I2C_Master_Receive(hi2c, AHT20_I2C_ADDRESS, data, size, HAL_MAX_DELAY);
}

// Reset the AHT20 sensor
HAL_StatusTypeDef AHT20_Reset(I2C_HandleTypeDef *hi2c)
{
    uint8_t data = AHT20_SOFT_RESET;
    return HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDRESS, &data, 1, HAL_MAX_DELAY);
}

// Get the temperature and humidity
HAL_StatusTypeDef AHT20_Get_Temp_Humidity(I2C_HandleTypeDef *hi2c, float* temperature, float* humidity)
{
    uint8_t data[6];

    // Start a measurement
    if(AHT20_Start_Measure(hi2c) != HAL_OK)
        return HAL_ERROR;
    
    // Wait for the measurement to complete
    HAL_Delay(80);
    
    // Read the measurement data
    if(AHT20_Read_Data(hi2c, data, sizeof(data)) != HAL_OK)
        return HAL_ERROR;
    
    // Calculate the humidity
    uint32_t rawHumidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    *humidity = ((float)rawHumidity * 100) / 0x100000;
    
    // Calculate the temperature
	// Calculate the temperature
	uint32_t rawTemperature = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
	*temperature = ((float)rawTemperature * 200 / 0x100000) - 50;

	return HAL_OK;
}
