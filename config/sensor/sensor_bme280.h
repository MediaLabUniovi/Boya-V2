#ifndef SENSOR_CONFIG_BME280_H
#define SENSOR_CONFIG_BME280_H

// Nombre del sensor
#define SENSOR_NAME "BME280"

// ¿Qué mide?
#define SENSOR_HAS_TEMPERATURE true
#define SENSOR_HAS_HUMIDITY true
#define SENSOR_HAS_PRESSURE true
#define SENSOR_HAS_DISTANCE false

// Configuración hardware
#define BME280_I2C_ADDR_PRIMARY 0x76
#define BME280_I2C_ADDR_SECONDARY 0x77
#define BME280_POWER_PIN 12
#define BME280_POWER_ON_DELAY_MS 100

// Configuración del sensor
#define BME280_SEA_LEVEL_PRESSURE 1013.25f
#define BME280_MODE 3
#define BME280_STANDBY 5
#define BME280_FILTER 4
#define BME280_SPI3W_EN 0

// Rangos válidos
#define TEMPERATURE_MIN -40.0f
#define TEMPERATURE_MAX 85.0f
#define HUMIDITY_MIN 0.0f
#define HUMIDITY_MAX 100.0f
#define PRESSURE_MIN 300.0f
#define PRESSURE_MAX 1100.0f

// Configuración de lecturas
#define BME280_READ_ATTEMPTS 3
#define BME280_READ_DELAY_MS 100

#endif // SENSOR_CONFIG_BME280_H