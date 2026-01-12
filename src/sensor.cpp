/**
 * @file      sensor.cpp
 * @brief     Implementacion del sistema de sensores
 *
 * Este archivo implementa el sistema de lectura y empaquetado de multiples sensores.
 *
 * @author    Proyecto IoT de Bajo Consumo
 * @version   1.0
 * @date      2025
 */

#include "../config/config.h"  // Configuracion unificada del proyecto
#include "sensor_interface.h"  // Interfaz generica de sensores
#include "LoRaBoards.h"  // Para readBatteryVoltage y batteryPercentFromVoltage

// Declaracion externa para funciones de carga solar
extern bool isSolarChargingBattery();

// ============================================================================
// FUNCIONES PARA GESTIONAR TODOS LOS SENSORES
// ============================================================================

/**
 * @brief Inicializa todos los sensores habilitados
 * @return true si al menos un sensor se inicializo correctamente
 */
bool sensors_init_all(void) {
    bool any_init = false;
    
#ifdef ENABLE_SENSOR_BME280
    if (sensor_bme280_init()) {
        Serial.println("BME280 inicializado");
        any_init = true;
    }
#endif

#ifdef ENABLE_SENSOR_DS18B20
    if (sensor_ds18b20_init()) {
        Serial.println("DS18B20 inicializado");
        any_init = true;
    }
#endif

#ifdef ENABLE_SENSOR_PH
    if (sensor_ph_init()) {
        Serial.println("Sensor de pH inicializado");
        any_init = true;
    }
#endif

    return any_init;
}

/**
 * @brief Verifica si al menos un sensor esta disponible
 * @return true si al menos un sensor esta operativo
 */
bool sensors_is_any_available(void) {
    bool any_available = false;
    
#ifdef ENABLE_SENSOR_BME280
    any_available |= sensor_bme280_is_available();
#endif

#ifdef ENABLE_SENSOR_DS18B20
    any_available |= sensor_ds18b20_is_available();
#endif

#ifdef ENABLE_SENSOR_PH
    any_available |= sensor_ph_is_available();
#endif

    return any_available;
}

/**
 * @brief Intenta reinicializar todos los sensores
 * @return true si al menos un sensor se reinicializo correctamente
 */
bool sensors_retry_init_all(void) {
    bool any_retry = false;
    
#ifdef ENABLE_SENSOR_BME280
    any_retry |= sensor_bme280_retry_init();
#endif

#ifdef ENABLE_SENSOR_DS18B20
    any_retry |= sensor_ds18b20_retry_init();
#endif

#ifdef ENABLE_SENSOR_PH
    any_retry |= sensor_ph_retry_init();
#endif

    return any_retry;
}

/**
 * @brief Lee datos de todos los sensores habilitados
 * @param data Puntero a estructura donde almacenar los datos
 * @return true si se pudieron leer datos de al menos un sensor
 */
bool sensors_read_all(sensor_data_t* data) {
    if (!data) return false;

    // Inicializar con valores de error
    data->temperature = SENSOR_ERROR_TEMPERATURE;
    data->humidity = SENSOR_ERROR_HUMIDITY;
    data->pressure = SENSOR_ERROR_PRESSURE;
    data->temperature_1m = SENSOR_ERROR_TEMPERATURE;
    data->ph = SENSOR_ERROR_PH;
    data->battery = readBatteryVoltage();
    data->valid = false;

    bool any_data = false;

    // Leer del sensor BME280
#ifdef ENABLE_SENSOR_BME280
    {
        sensor_data_t bme_data;
        if (sensor_bme280_read_all(&bme_data)) {
            if (bme_data.temperature != SENSOR_ERROR_TEMPERATURE) {
                data->temperature = bme_data.temperature;
                Serial.printf("DEBUG: BME280 Temperatura exterior = %.2f °C\n", data->temperature);
                any_data = true;
            }
            if (bme_data.humidity != SENSOR_ERROR_HUMIDITY) {
                data->humidity = bme_data.humidity;
                Serial.printf("DEBUG: BME280 Humedad = %.2f %%\n", data->humidity);
                any_data = true;
            }
            if (bme_data.pressure != SENSOR_ERROR_PRESSURE) {
                data->pressure = bme_data.pressure;
                Serial.printf("DEBUG: BME280 Presion = %.2f hPa\n", data->pressure);
                any_data = true;
            }
        }
    }
#endif

    // Leer del sensor DS18B20 (temperatura a 1m)
#ifdef ENABLE_SENSOR_DS18B20
    {
        sensor_data_t ds18b20_data;
        if (sensor_ds18b20_read_all(&ds18b20_data)) {
            if (ds18b20_data.temperature_1m != SENSOR_ERROR_TEMPERATURE) {
                data->temperature_1m = ds18b20_data.temperature_1m;
                Serial.printf("DEBUG: DS18B20 Temperatura agua 1m = %.2f °C\n", data->temperature_1m);
                any_data = true;
            }
        }
    }
#endif

    // Leer del sensor de pH (con compensacion de temperatura si esta disponible)
#ifdef ENABLE_SENSOR_PH
    {
        // Si tenemos temperatura del BME280, actualizar compensacion de pH
        #ifdef ENABLE_SENSOR_BME280
        if (data->temperature != SENSOR_ERROR_TEMPERATURE) {
            sensor_ph_set_temperature(data->temperature);
            Serial.printf("DEBUG: pH compensado con temperatura = %.2f °C\n", data->temperature);
        }
        #endif
        
        sensor_data_t ph_data;
        if (sensor_ph_read_all(&ph_data)) {
            if (ph_data.ph != SENSOR_ERROR_PH) {
                data->ph = ph_data.ph;
                Serial.printf("DEBUG: pH sensor = %.2f\n", data->ph);
                any_data = true;
            }
        }
    }
#endif

    // Mostrar resumen final
    Serial.println("DEBUG: ========== RESUMEN DE LECTURAS ==========");
    Serial.printf("DEBUG: Bateria = %.2f V (%.0f%%)\n", data->battery, 
                  (data->battery - 3.3) / (4.2 - 3.3) * 100.0);
    if (data->ph != SENSOR_ERROR_PH) 
        Serial.printf("DEBUG: pH = %.2f\n", data->ph);
    if (data->temperature != SENSOR_ERROR_TEMPERATURE) 
        Serial.printf("DEBUG: Temp exterior = %.2f °C\n", data->temperature);
    if (data->temperature_1m != SENSOR_ERROR_TEMPERATURE) 
        Serial.printf("DEBUG: Temp agua 1m = %.2f °C\n", data->temperature_1m);
    if (data->humidity != SENSOR_ERROR_HUMIDITY) 
        Serial.printf("DEBUG: Humedad = %.2f %%\n", data->humidity);
    if (data->pressure != SENSOR_ERROR_PRESSURE) 
        Serial.printf("DEBUG: Presion = %.2f hPa\n", data->pressure);
    Serial.println("DEBUG: ==========================================");

    data->valid = any_data;
    return any_data;
}

/**
 * @brief Construye el payload con datos de todos los sensores
 * @param config Configuracion del payload
 * @return Numero de bytes escritos
 */
uint8_t sensors_get_payload(payload_config_t* config) {
    if (!config || config->max_size < PAYLOAD_SIZE_BYTES) return 0;

    sensor_data_t data;
    bool sensor_ok = sensors_read_all(&data);

    if (!sensor_ok) {
        // Si no hay datos validos, intentar reinicializar
        sensors_retry_init_all();
        // Usar datos de error
        data.temperature = SENSOR_ERROR_TEMPERATURE;
        data.humidity = SENSOR_ERROR_HUMIDITY;
        data.pressure = SENSOR_ERROR_PRESSURE;
        data.temperature_1m = SENSOR_ERROR_TEMPERATURE;
        data.ph = SENSOR_ERROR_PH;
    }

    uint8_t offset = 0;

    // Orden del payload: Bateria, pH, Temperatura exterior, Temperatura 1m, Humedad, Presion
    
    // 1. Bateria (1 byte) - PRIMERO
#ifdef BATTERY_AS_PERCENTAGE
    // Enviar como porcentaje (1 byte)
    uint8_t batt_percent = batteryPercentFromVoltage(data.battery);
    Serial.printf("DEBUG: Battery voltage %.2f V = %u%%\n", data.battery, batt_percent);
    config->buffer[offset++] = batt_percent;              // Solo 1 byte
#else
    // Enviar como voltaje (2 bytes)
    uint16_t batt_int = (uint16_t)(data.battery * 100);
    Serial.printf("DEBUG: Battery voltage %.2f V as %u (0x%04X)\n", data.battery, batt_int, batt_int);
    config->buffer[offset++] = batt_int & 0xFF;           // Low byte primero
    config->buffer[offset++] = batt_int >> 8;             // High byte despues
#endif

    // 2. pH (2 bytes)
#ifdef ENABLE_SENSOR_PH
    if (SYSTEM_HAS_PH) {
        uint16_t ph_int = (uint16_t)(data.ph * 100);
        Serial.printf("DEBUG PAYLOAD: pH %.2f → %u (0x%04X) → bytes[%u,%u]\n", 
                     data.ph, ph_int, ph_int, ph_int & 0xFF, ph_int >> 8);
        config->buffer[offset++] = ph_int & 0xFF;        // Low byte primero
        config->buffer[offset++] = ph_int >> 8;           // High byte despues
    }
#endif

    // 3. Temperatura exterior/BME280 (2 bytes)
#ifdef ENABLE_SENSOR_BME280
    if (SYSTEM_HAS_TEMPERATURE) {
        int16_t temp_int = (int16_t)(data.temperature * 100);
        Serial.printf("DEBUG PAYLOAD: Temp_ext %.2f → %d (0x%04X) → bytes[%u,%u]\n", 
                     data.temperature, temp_int, (uint16_t)temp_int, temp_int & 0xFF, (temp_int >> 8) & 0xFF);
        config->buffer[offset++] = temp_int & 0xFF;      // Low byte primero
        config->buffer[offset++] = (temp_int >> 8) & 0xFF; // High byte despues
    }
#endif

    // 4. Temperatura a 1m (2 bytes)
#ifdef ENABLE_SENSOR_DS18B20
    if (SYSTEM_HAS_TEMP_1M) {
        int16_t temp1m_int = (int16_t)(data.temperature_1m * 100);
        Serial.printf("DEBUG PAYLOAD: Temp_1m %.2f → %d (0x%04X) → bytes[%u,%u]\n", 
                     data.temperature_1m, temp1m_int, (uint16_t)temp1m_int, temp1m_int & 0xFF, (temp1m_int >> 8) & 0xFF);
        config->buffer[offset++] = temp1m_int & 0xFF;     // Low byte primero
        config->buffer[offset++] = (temp1m_int >> 8) & 0xFF; // High byte despues
    }
#endif

    // 5. Humedad (2 bytes)
#ifdef ENABLE_SENSOR_BME280
    if (SYSTEM_HAS_HUMIDITY) {
        int16_t hum_int = (int16_t)(data.humidity * 100);
        Serial.printf("DEBUG PAYLOAD: Humedad %.2f → %d (0x%04X) → bytes[%u,%u]\n", 
                     data.humidity, hum_int, (uint16_t)hum_int, hum_int & 0xFF, (hum_int >> 8) & 0xFF);
        config->buffer[offset++] = hum_int & 0xFF;        // Low byte primero
        config->buffer[offset++] = (hum_int >> 8) & 0xFF; // High byte despues
    }
#endif

    // 6. Presion (2 bytes)
#ifdef ENABLE_SENSOR_BME280
    if (SYSTEM_HAS_PRESSURE) {
        uint16_t pres_int = (uint16_t)(data.pressure * 10);
        Serial.printf("DEBUG PAYLOAD: Presion %.2f → %u (0x%04X) → bytes[%u,%u]\n", 
                     data.pressure, pres_int, pres_int, pres_int & 0xFF, pres_int >> 8);
        config->buffer[offset++] = pres_int & 0xFF;       // Low byte primero
        config->buffer[offset++] = pres_int >> 8;         // High byte despues
    }
#endif

    config->written = offset;
    
    // Imprimir buffer enviado
    Serial.print("\nBuffer enviado [");
    for (uint8_t i = 0; i < offset; i++) {
        Serial.print(config->buffer[i]);
        Serial.print(" ");
    }
    Serial.println("]");
    
    return offset;
}

/**
 * @brief Obtiene el nombre de los sensores activos
 * @return Cadena con los nombres de los sensores
 */
const char* sensors_get_name(void) {
    static char name_buffer[100] = "";
    name_buffer[0] = '\0';
    
#ifdef ENABLE_SENSOR_BME280
    strcat(name_buffer, "BME280 ");
#endif
#ifdef ENABLE_SENSOR_DS18B20
    strcat(name_buffer, "DS18B20 ");
#endif
#ifdef ENABLE_SENSOR_PH
    strcat(name_buffer, "pH ");
#endif
    
    if (strlen(name_buffer) == 0) {
        strcpy(name_buffer, "NINGUNO");
    }
    
    return name_buffer;
}

/**
 * @brief Fuerza el estado de todos los sensores para testing
 * @param available true para simular disponible, false para simular fallo
 */
void sensors_set_available_for_testing(bool available) {
#ifdef ENABLE_SENSOR_BME280
    sensor_bme280_set_available_for_testing(available);
#endif
#ifdef ENABLE_SENSOR_DS18B20
    sensor_ds18b20_set_available_for_testing(available);
#endif
#ifdef ENABLE_SENSOR_PH
    sensor_ph_set_available_for_testing(available);
#endif
}

// ============================================================================
// FUNCIONES DE COMPATIBILIDAD HACIA ATRAS
// ============================================================================

/**
 * Funciones legacy para mantener compatibilidad con el codigo existente
 * Estas funciones llaman a la nueva interfaz multisensor
 */

bool initSensor() {
    return sensors_init_all();
}

bool isSensorAvailable() {
    return sensors_is_any_available();
}

bool retrySensorInit() {
    return sensors_retry_init_all();
}

float readTemperature() {
    sensor_data_t data;
    if (sensors_read_all(&data) && data.valid) {
        return data.temperature;
    }
    return SENSOR_ERROR_TEMPERATURE;
}

float readHumidity() {
    sensor_data_t data;
    if (sensors_read_all(&data) && data.valid) {
        return data.humidity;
    }
    return SENSOR_ERROR_HUMIDITY;
}

float readPressure() {
    sensor_data_t data;
    if (sensors_read_all(&data) && data.valid) {
        return data.pressure;
    }
    return SENSOR_ERROR_PRESSURE;
}

void setSensorAvailableForTesting(bool available) {
    sensors_set_available_for_testing(available);
}

uint8_t getSensorPayload(uint8_t* payload, uint8_t maxSize) {
    payload_config_t config = {payload, maxSize, 0};
    return sensors_get_payload(&config);
}

bool getSensorDataForDisplay(float& temp, float& hum, float& pres, float& battery) {
    sensor_data_t data;
    bool ok = sensors_read_all(&data);

    temp = data.temperature;
    hum = data.humidity;
    pres = data.pressure;
    battery = data.battery;

    return ok && data.valid;
}
