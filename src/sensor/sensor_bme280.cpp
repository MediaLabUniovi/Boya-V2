#include "../../config/config.h"

#ifdef ENABLE_SENSOR_BME280
// Implementación BME280 (presión, temperatura y humedad)
#include <Adafruit_BME280.h>
#include <Wire.h>
#include "sensor_interface.h"
#include "LoRaBoards.h"

// Objeto global del sensor
static Adafruit_BME280 bme;

// Estado del sensor
static bool sensor_available = false;

/**
 * @brief Inicializa el sensor BME280
 */
bool sensor_bme280_init(void) {
    Serial.println("BME280: Iniciando búsqueda del sensor...");
    
    // La librería Adafruit_BME280 ya maneja Wire internamente
    // Solo hacemos un pequeño delay para estabilización
    delay(100);
    
    // Escanear el bus I2C para ver qué dispositivos hay
    Serial.println("BME280: Escaneando bus I2C...");
    byte error, address;
    int nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.printf("  Dispositivo I2C encontrado en dirección 0x%02X\n", address);
            nDevices++;
        }
    }
    if (nDevices == 0) {
        Serial.println("  No se encontraron dispositivos I2C en el bus!");
    } else {
        Serial.printf("  Total: %d dispositivo(s) encontrado(s)\n", nDevices);
    }
    
    // Intentar primero con dirección 0x76
    Serial.print("BME280: Probando dirección 0x76... ");
    if (bme.begin(0x76, &Wire)) {
        Serial.println("¡Encontrado!");
    }
    // Si no funciona, intentar con dirección 0x77
    else {
        Serial.println("No encontrado");
        Serial.print("BME280: Probando dirección 0x77... ");
        if (bme.begin(0x77, &Wire)) {
            Serial.println("¡Encontrado!");
        }
        // Si ninguna dirección funciona
        else {
            Serial.println("No encontrado");
            Serial.println("BME280: ERROR - No encontrado en 0x76 ni 0x77");
            Serial.println("Verifica conexiones: VCC->3.3V, GND->GND, SDA->GPIO21, SCL->GPIO22");
            sensor_available = false;
            return false;
        }
    }
    
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_X2,   // Temperatura
                    Adafruit_BME280::SAMPLING_X16,  // Presión
                    Adafruit_BME280::SAMPLING_X1,   // Humedad
                    Adafruit_BME280::FILTER_X16,
                    Adafruit_BME280::STANDBY_MS_500);
    Serial.println("BME280: Sensor inicializado correctamente.");
    sensor_available = true;
    return true;
}

/**
 * @brief Verifica si el sensor está disponible
 */
bool sensor_bme280_is_available(void) {
    return sensor_available;
}

/**
 * @brief Intenta reinicializar el sensor
 */
bool sensor_bme280_retry_init(void) {
    if (sensor_available) return true;
    Serial.println("Reintentando inicialización del sensor BME280...");
    return sensor_bme280_init();
}

/**
 * @brief Lee todos los datos del sensor BME280
 */
bool sensor_bme280_read_all(sensor_data_t* data) {
    if (!sensor_available || !data) return false;

    data->temperature = bme.readTemperature();
    data->humidity = bme.readHumidity();  // BME280 sí mide humedad
    data->pressure = bme.readPressure() / 100.0F;  // Convertir a hPa
    data->battery = readBatteryVoltage();
    data->valid = true;

    if (isnan(data->temperature) || isnan(data->pressure) || isnan(data->humidity)) {
        Serial.println("BME280: Error en lectura");
        data->temperature = SENSOR_ERROR_TEMPERATURE;
        data->pressure = SENSOR_ERROR_PRESSURE;
        data->humidity = SENSOR_ERROR_HUMIDITY;
        data->valid = false;
        return false;
    }

    Serial.printf("BME280: Lectura exitosa - Temp: %.1f°C, Hum: %.1f%%, Pres: %.1f hPa\n",
                  data->temperature, data->humidity, data->pressure);
    return true;
}

/**
 * @brief Obtiene el payload empaquetado para BME280
 */
uint8_t sensor_bme280_get_payload(payload_config_t* config) {
    if (!config || config->max_size < PAYLOAD_SIZE_BYTES) return 0;

    sensor_data_t data;
    bool sensor_ok = sensor_bme280_read_all(&data);

    if (!sensor_ok) {
        data.temperature = SENSOR_ERROR_TEMPERATURE;
        data.humidity = SENSOR_ERROR_HUMIDITY;
        data.pressure = SENSOR_ERROR_PRESSURE;
        sensor_bme280_retry_init();
    }

    uint8_t offset = 0;
    int16_t temp_int = (int16_t)(data.temperature * 100);
    config->buffer[offset++] = temp_int >> 8;
    config->buffer[offset++] = temp_int & 0xFF;

    // Humedad (disponible en BME280)
    uint16_t hum_int = (uint16_t)(data.humidity * 100);
    config->buffer[offset++] = hum_int >> 8;
    config->buffer[offset++] = hum_int & 0xFF;

    // Presión
    uint16_t pres_int = (uint16_t)(data.pressure * 10);
    config->buffer[offset++] = pres_int >> 8;
    config->buffer[offset++] = pres_int & 0xFF;

    // Distancia (no disponible)
    config->buffer[offset++] = 0xFF;
    config->buffer[offset++] = 0xFF;

    uint16_t batt_int = (uint16_t)(data.battery * 100);
    config->buffer[offset++] = batt_int >> 8;
    config->buffer[offset++] = batt_int & 0xFF;

    config->written = offset;
    return offset;
}

/**
 * @brief Obtiene el nombre del sensor
 */
const char* sensor_bme280_get_name(void) {
    return "BME280";
}

/**
 * @brief Fuerza el estado del sensor para testing
 */
void sensor_bme280_set_available_for_testing(bool available) {
    sensor_available = available;
    Serial.printf("TESTING: Sensor BME280 forzado a %s\n", available ? "disponible" : "no disponible");
}

#endif // ENABLE_SENSOR_BME280
