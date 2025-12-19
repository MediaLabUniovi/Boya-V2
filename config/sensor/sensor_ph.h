#ifndef SENSOR_CONFIG_PH_H
#define SENSOR_CONFIG_PH_H

// Nombre del sensor
#define SENSOR_PH_NAME "DFRobot pH Sensor"

// ¿Qué mide?
#define SENSOR_PH_HAS_PH true

// Configuración hardware
#define PH_ANALOG_PIN 25  // Pin ADC para sensor de pH DFRobot (GPIO25)
#ifndef PH_POWER_PIN
#define PH_POWER_PIN 13   // Pin para controlar alimentación de sensores
#endif
#define PH_POWER_ON_DELAY_MS 30000  // 30 segundos para estabilización

// Configuración del sensor DFRobot
#define PH_REFERENCE_VOLTAGE 3.3f             // Voltaje de referencia ADC (3.3V para ESP32)
#define PH_ADC_RESOLUTION 4096.0f             // Resolución ADC (12 bits = 4096 valores)

// Temperatura para compensación (se puede actualizar con sensor de temperatura)
#define PH_DEFAULT_TEMPERATURE 25.0f          // Temperatura por defecto en °C

// Rangos válidos
#define PH_MIN 0.0f
#define PH_MAX 14.0f

// Configuración de lecturas
#define PH_READ_SAMPLES 10  // Número de muestras para promediar
#define PH_READ_DELAY_MS 20  // Delay entre muestras (mínimo 20ms recomendado)

#endif // SENSOR_CONFIG_PH_H
