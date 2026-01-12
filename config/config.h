#ifndef CONFIG_H
#define CONFIG_H

// =============================================================================
// CONFIGURACIÓN DE HARDWARE (PLACA)
// =============================================================================

// Selección de placa LilyGo LoRa
// Descomenta solo UNA de las siguientes líneas según tu placa

// #define T3_V1_3_SX1276    // T3 V1.3 con SX1276
// #define T3_V1_3_SX1278    // T3 V1.3 con SX1278
#define T3_V1_6_SX1276    // T3 V1.6 con SX1276
// #define T3_V1_6_SX1278    // T3 V1.6 con SX1278
// #define T_BEAM_SX1276     // T-Beam con SX1276
// Otras placas disponibles en hardware_config.h

#include <Arduino.h>
#include "hardware_config.h"  // Configuración específica del hardware

// Incluir configuración LoRaWAN (con las claves)
#include <lmic.h>
#include "lorawan_config.h"

// =============================================================================
// CONFIGURACIÓN DE SENSORES
// =============================================================================

// Sensor BME280 (Presión + Temperatura + Humedad)
#define ENABLE_SENSOR_BME280

// Sensor DS18B20 (Temperatura a 1 metro de profundidad)
#define ENABLE_SENSOR_DS18B20

// Sensor de pH analógico
#define ENABLE_SENSOR_PH

// =============================================================================
// INCLUSIÓN AUTOMÁTICA DE SENSORES
// =============================================================================

#ifdef ENABLE_SENSOR_BME280
#include "sensor/sensor_bme280.h"
#endif

#ifdef ENABLE_SENSOR_DS18B20
#include "sensor/sensor_ds18b20.h"
#endif

#ifdef ENABLE_SENSOR_PH
#include "sensor/sensor_ph.h"
#endif

// =============================================================================
// CONFIGURACIÓN DE PANTALLA OLED
// =============================================================================

#define ENABLE_DISPLAY false         // true: pantalla activa, false: apagada para máximo ahorro
#define SHOW_ACTIVITY_INDICATORS false // true: mostrar indicadores permanentes, false: solo mensajes

// Duraciones de mensajes (ms)
#define MESSAGE_DURATION_SUCCESS 5000
#define MESSAGE_DURATION_ERROR 3000
#define MESSAGE_DURATION_WARNING 4000
#define MESSAGE_DURATION_INFO 3000
#define MESSAGE_DURATION_SLEEP 3000

// =============================================================================
// CONFIGURACIÓN LoRaWAN
// =============================================================================

// Región LoRaWAN
#define LORAWAN_REGION LMIC_region_t::LMIC_REGION_eu868

// Parámetros de transmisión
#define TX_POWER_DBM 17              // Potencia de transmisión (máx 17 para evitar interferencias)
#define BACKOFF_INITIAL_SECONDS 300  // Backoff inicial exponencial

// =============================================================================
// CLAVES LoRaWAN OTAA (¡MODIFICA EN lorawan_config.h!)
// =============================================================================
// Las claves LoRaWAN se definen en lorawan_config.h para mantenerlas separadas
// de la configuración general (por seguridad y organización)

// =============================================================================
// CONFIGURACIÓN DE TIMING Y ENERGÍA
// =============================================================================

#define SEND_INTERVAL_SECONDS 900    // Intervalo entre envíos (15 minutos)
#define WATCHDOG_TIMEOUT_MINUTES 5   // Timeout del watchdog en minutos

// Energía y batería
#define ENABLE_SOLAR_CHARGING true   // Habilitar carga solar
#define BATTERY_LOW_THRESHOLD 20     // Umbral de batería baja (%)
#define BATTERY_AS_PERCENTAGE        // Descomentar para enviar batería como porcentaje (1 byte)
                                     // Comentar para enviar como voltaje (2 bytes)

// =============================================================================
// CONFIGURACIÓN DE DEPURACIÓN Y LOGGING
// =============================================================================

#define ENABLE_SERIAL_LOGS true      // Habilitar logs por Serial
#define LOG_LEVEL 1                  // 0: ninguno, 1: básico, 2: detallado
#define SHOW_TTN_DECODER true  // true: mostrar decoder TTN por Serial al iniciar

// =============================================================================
// CONFIGURACIÓN DE PAYLOAD Y DATOS
// =============================================================================

// Determinación automática de capacidades del sistema
#if defined(ENABLE_SENSOR_BME280) && SENSOR_HAS_TEMPERATURE
#define SYSTEM_HAS_TEMPERATURE 1
#else
#define SYSTEM_HAS_TEMPERATURE 0
#endif

#if defined(ENABLE_SENSOR_BME280) && SENSOR_HAS_HUMIDITY
#define SYSTEM_HAS_HUMIDITY 1
#else
#define SYSTEM_HAS_HUMIDITY 0
#endif

#if defined(ENABLE_SENSOR_BME280) && SENSOR_HAS_PRESSURE
#define SYSTEM_HAS_PRESSURE 1
#else
#define SYSTEM_HAS_PRESSURE 0
#endif

#if defined(ENABLE_SENSOR_DS18B20) && SENSOR_DS18B20_HAS_TEMPERATURE
#define SYSTEM_HAS_TEMP_1M 1
#else
#define SYSTEM_HAS_TEMP_1M 0
#endif

#if defined(ENABLE_SENSOR_PH) && SENSOR_PH_HAS_PH
#define SYSTEM_HAS_PH 1
#else
#define SYSTEM_HAS_PH 0
#endif

// Cálculo del tamaño del payload usando directivas del preprocesador
#if defined(ENABLE_SENSOR_BME280)
#define PAYLOAD_SIZE_TEMPERATURE 2
#define PAYLOAD_SIZE_HUMIDITY 2
#define PAYLOAD_SIZE_PRESSURE 2
#else
#define PAYLOAD_SIZE_TEMPERATURE 0
#define PAYLOAD_SIZE_HUMIDITY 0
#define PAYLOAD_SIZE_PRESSURE 0
#endif

#if defined(ENABLE_SENSOR_DS18B20)
#define PAYLOAD_SIZE_TEMP_1M 2
#else
#define PAYLOAD_SIZE_TEMP_1M 0
#endif

#if defined(ENABLE_SENSOR_PH)
#define PAYLOAD_SIZE_PH 2
#else
#define PAYLOAD_SIZE_PH 0
#endif

// Tamaño de batería según configuración
#ifdef BATTERY_AS_PERCENTAGE
#define PAYLOAD_SIZE_BATTERY 1  // 1 byte para porcentaje (0-100%)
#else
#define PAYLOAD_SIZE_BATTERY 2  // 2 bytes para voltaje (* 100)
#endif

// Orden del payload: Batería, pH, Temperatura exterior, Temperatura 1m, Humedad, Presión
#define PAYLOAD_SIZE_BYTES ( \
    PAYLOAD_SIZE_BATTERY + \
    PAYLOAD_SIZE_PH + \
    PAYLOAD_SIZE_TEMPERATURE + \
    PAYLOAD_SIZE_TEMP_1M + \
    PAYLOAD_SIZE_HUMIDITY + \
    PAYLOAD_SIZE_PRESSURE \
)

// Valores de error para lecturas fallidas
#define SENSOR_ERROR_TEMPERATURE -999.0f
#define SENSOR_ERROR_HUMIDITY -1.0f
#define SENSOR_ERROR_PRESSURE -1.0f
#define SENSOR_ERROR_BATTERY -1.0f
#define SENSOR_ERROR_PH -1.0f

// =============================================================================
// ESTRUCTURAS DE DATOS PARA SENSORES (MODIFICABLES POR EL USUARIO)
// =============================================================================
// Estas estructuras pueden ser modificadas al añadir nuevos sensores

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Estructura que contiene todas las lecturas del sensor
 * @note MODIFICA esta estructura al añadir nuevos tipos de datos de sensores
 */
typedef struct {
    float temperature;        /**< Temperatura exterior en °C (BME280) */
    float humidity;           /**< Humedad relativa en % (BME280) */
    float pressure;           /**< Presión atmosférica en hPa (BME280) */
    float temperature_1m;     /**< Temperatura a 1m de profundidad en °C (DS18B20) */
    float ph;                 /**< Valor de pH */
    float battery;            /**< Voltaje de batería en V */
    bool valid;               /**< true si todas las lecturas son válidas */
} sensor_data_t;

/**
 * @brief Estructura para configuración del payload LoRaWAN
 * @note MODIFICA esta estructura si necesitas cambiar el formato del payload
 */
typedef struct {
    uint8_t* buffer;      /**< Buffer donde escribir el payload */
    uint8_t max_size;     /**< Tamaño máximo del buffer */
    uint8_t written;      /**< Bytes realmente escritos */
    // Añade aquí campos adicionales para configuración de payload
    // bool include_timestamp;
    // uint8_t compression_level;
} payload_config_t;

#endif // CONFIG_H
