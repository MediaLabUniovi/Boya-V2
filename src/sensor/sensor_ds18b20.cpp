#include "../../config/config.h"

#ifdef ENABLE_SENSOR_DS18B20
// Implementación DS18B20 (temperatura a 1m de profundidad)
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sensor_interface.h"
#include "LoRaBoards.h"

// Objetos globales del sensor
static OneWire oneWire(DS18B20_DATA_PIN);
static DallasTemperature sensors(&oneWire);

// Estado del sensor
static bool sensor_available = false;
static bool sensor_powered = false;

/**
 * @brief Enciende alimentación de sensores
 */
static void sensor_ds18b20_power_on(void) {
#if DS18B20_USE_POWER_CONTROL
    if (sensor_powered) return;
    
    pinMode(DS18B20_POWER_PIN, OUTPUT);
    digitalWrite(DS18B20_POWER_PIN, HIGH);
    sensor_powered = true;
    
    Serial.println("DS18B20: Alimentación de sensores activada");
    Serial.printf("DS18B20: Esperando %d ms para estabilización...\n", DS18B20_POWER_ON_DELAY_MS);
    delay(DS18B20_POWER_ON_DELAY_MS);
#else
    Serial.println("DS18B20: Alimentación permanente (sin control por MOSFET)");
    sensor_powered = true;
    delay(100);  // Pequeño delay para estabilización
#endif
}

/**
 * @brief Apaga alimentación de sensores
 */
static void sensor_ds18b20_power_off(void) {
#if DS18B20_USE_POWER_CONTROL
    if (!sensor_powered) return;
    
    digitalWrite(DS18B20_POWER_PIN, LOW);
    sensor_powered = false;
    
    Serial.println("DS18B20: Alimentación de sensores desactivada");
    delay(1000);  // Delay para garantizar desconexión completa
#else
    // No hacer nada si la alimentación es permanente
    Serial.println("DS18B20: Alimentación permanente (no se apaga)");
#endif
}

/**
 * @brief Inicializa el sensor DS18B20
 */
bool sensor_ds18b20_init(void) {
    Serial.println("DS18B20: Iniciando sensor de temperatura a 1m...");
    Serial.printf("DS18B20: Pin de datos configurado en GPIO%d\n", DS18B20_DATA_PIN);
    Serial.printf("DS18B20: Pin de alimentacion configurado en GPIO%d\n", DS18B20_POWER_PIN);
    
    // Configurar pin de datos como entrada con pull-up
    pinMode(DS18B20_DATA_PIN, INPUT_PULLUP);
    Serial.println("DS18B20: Pull-up activado en pin de datos");
    
    // Encender alimentación de sensores
    sensor_ds18b20_power_on();
    
    // Inicializar librería DallasTemperature
    sensors.begin();
    Serial.println("DS18B20: Libreria DallasTemperature inicializada");
    
    // Verificar si hay dispositivos conectados
    int deviceCount = sensors.getDeviceCount();
    Serial.printf("DS18B20: %d dispositivo(s) encontrado(s) en el bus OneWire\n", deviceCount);
    
    if (deviceCount == 0) {
        Serial.println("DS18B20: ERROR - No se encontro ningun sensor");
        Serial.printf("DS18B20: Verifica conexiones: VCC->GPIO%d(MOSFET), GND->GND, DATA->GPIO%d con resistor 4.7K a VCC\n", 
                     DS18B20_POWER_PIN, DS18B20_DATA_PIN);
        sensor_available = false;
        sensor_ds18b20_power_off();
        return false;
    }
    
    // Configurar resolución
    sensors.setResolution(DS18B20_RESOLUTION);
    Serial.printf("DS18B20: Resolucion configurada a %d bits\n", DS18B20_RESOLUTION);
    
    Serial.println("DS18B20: Sensor inicializado correctamente");
    sensor_available = true;
    
    // Apagar sensores hasta que se necesiten
    sensor_ds18b20_power_off();
    
    return true;
}

/**
 * @brief Verifica si el sensor está disponible
 */
bool sensor_ds18b20_is_available(void) {
    return sensor_available;
}

/**
 * @brief Intenta reinicializar el sensor
 */
bool sensor_ds18b20_retry_init(void) {
    if (sensor_available) return true;
    Serial.println("Reintentando inicialización del sensor DS18B20...");
    return sensor_ds18b20_init();
}

/**
 * @brief Lee todos los datos del sensor DS18B20
 */
bool sensor_ds18b20_read_all(sensor_data_t* data) {
    if (!sensor_available || !data) return false;

    // Encender alimentación de sensores antes de leer
    sensor_ds18b20_power_on();
    
    // Solicitar lectura de temperaturas
    sensors.requestTemperatures();
    delay(DS18B20_CONVERSION_DELAY_MS);
    
    // Leer temperatura del primer sensor (índice 0)
    float temp = sensors.getTempCByIndex(0);
    
    // Verificar si la lectura es válida
    if (temp == DEVICE_DISCONNECTED_C || temp < DS18B20_TEMPERATURE_MIN || temp > DS18B20_TEMPERATURE_MAX) {
        Serial.println("DS18B20: ERROR - Lectura inválida");
        data->temperature_1m = SENSOR_ERROR_TEMPERATURE;
        sensor_ds18b20_power_off();
        return false;
    }
    
    data->temperature_1m = temp;
    Serial.printf("DS18B20: Temperatura a 1m = %.2f °C\n", temp);
    
    // Apagar alimentación después de leer
    sensor_ds18b20_power_off();
    
    return true;
}

/**
 * @brief Obtiene el payload del sensor DS18B20
 */
uint8_t sensor_ds18b20_get_payload(payload_config_t* config) {
    if (!config || !sensor_available) return 0;

    sensor_data_t data;
    if (!sensor_ds18b20_read_all(&data)) {
        data.temperature_1m = SENSOR_ERROR_TEMPERATURE;
    }

    // Codificar temperatura como int16 (multiplicar por 100)
    int16_t temp_int = (int16_t)(data.temperature_1m * 100);
    
    config->buffer[0] = temp_int >> 8;
    config->buffer[1] = temp_int & 0xFF;
    
    return 2;  // 2 bytes enviados
}

/**
 * @brief Obtiene el nombre del sensor
 */
const char* sensor_ds18b20_get_name(void) {
    return SENSOR_DS18B20_NAME;
}

/**
 * @brief Fuerza el estado del sensor para testing
 */
void sensor_ds18b20_set_available_for_testing(bool available) {
    sensor_available = available;
}

#endif // ENABLE_SENSOR_DS18B20
