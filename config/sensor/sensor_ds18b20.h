#ifndef SENSOR_CONFIG_DS18B20_H
#define SENSOR_CONFIG_DS18B20_H

// Nombre del sensor
#define SENSOR_DS18B20_NAME "DS18B20"

// ¿Qué mide?
#define SENSOR_DS18B20_HAS_TEMPERATURE true

// Configuración hardware
#define DS18B20_DATA_PIN 15  // Pin para OneWire (ajustar según hardware)

// Control de alimentación
#define DS18B20_USE_POWER_CONTROL false  // true: usa MOSFET, false: alimentación permanente
#ifndef DS18B20_POWER_PIN
#define DS18B20_POWER_PIN 13  // Pin para controlar alimentación de sensores (si USE_POWER_CONTROL = true)
#endif
#define DS18B20_POWER_ON_DELAY_MS 30000  // 30 segundos para estabilización de sensores

// Configuración del sensor
#define DS18B20_RESOLUTION 12  // 9-12 bits de resolución
#define DS18B20_CONVERSION_DELAY_MS 750  // Tiempo de conversión para 12 bits

// Rangos válidos
#define DS18B20_TEMPERATURE_MIN -55.0f
#define DS18B20_TEMPERATURE_MAX 125.0f

// Configuración de lecturas
#define DS18B20_READ_ATTEMPTS 3
#define DS18B20_READ_DELAY_MS 100

#endif // SENSOR_CONFIG_DS18B20_H
