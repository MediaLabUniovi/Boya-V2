#include "../../config/config.h"

#ifdef ENABLE_SENSOR_PH
// Implementacion sensor de pH DFRobot
#include <DFRobot_PH.h>
#include "sensor_interface.h"
#include "LoRaBoards.h"

// Objeto global del sensor DFRobot_PH
static DFRobot_PH ph_sensor;

// Estado del sensor
static bool sensor_available = false;
static bool sensor_powered = false;

// Variables para lecturas
static float temperature = PH_DEFAULT_TEMPERATURE;  // Temperatura para compensacion

/**
 * @brief Enciende alimentacion de sensores
 */
static void sensor_ph_power_on(void) {
    if (sensor_powered) return;
    
    pinMode(PH_POWER_PIN, OUTPUT);
    digitalWrite(PH_POWER_PIN, HIGH);
    sensor_powered = true;
    
    Serial.println("pH: Alimentacion de sensores activada");
    Serial.printf("pH: Esperando %d ms para estabilizacion...\n", PH_POWER_ON_DELAY_MS);
    delay(PH_POWER_ON_DELAY_MS);
}

/**
 * @brief Apaga alimentacion de sensores
 */
static void sensor_ph_power_off(void) {
    if (!sensor_powered) return;
    
    digitalWrite(PH_POWER_PIN, LOW);
    sensor_powered = false;
    
    Serial.println("pH: Alimentacion de sensores desactivada");
    delay(1000);  // Delay para garantizar desconexion completa
}

/**
 * @brief Inicializa el sensor de pH DFRobot
 */
bool sensor_ph_init(void) {
    Serial.println("pH: Iniciando sensor de pH DFRobot...");
    
    // Configurar pin ADC
    pinMode(PH_ANALOG_PIN, INPUT);
    
    // Configurar pin de alimentacion
    pinMode(PH_POWER_PIN, OUTPUT);
    
    // Inicializar la libreria DFRobot_PH
    ph_sensor.begin();
    
    Serial.printf("pH: Pin ADC configurado en GPIO%d\n", PH_ANALOG_PIN);
    Serial.printf("pH: Pin de alimentacion configurado en GPIO%d\n", PH_POWER_PIN);
    Serial.println("pH: Libreria DFRobot_PH inicializada");
    
    sensor_available = true;
    
    return true;
}

/**
 * @brief Verifica si el sensor esta disponible
 */
bool sensor_ph_is_available(void) {
    return sensor_available;
}

/**
 * @brief Intenta reinicializar el sensor
 */
bool sensor_ph_retry_init(void) {
    if (sensor_available) return true;
    Serial.println("Reintentando inicializacion del sensor de pH...");
    return sensor_ph_init();
}

/**
 * @brief Lee el valor de pH usando la libreria DFRobot
 */
static float read_ph_value(void) {
    uint32_t sum = 0;
    
    // Tomar multiples muestras y promediar
    for (int i = 0; i < PH_READ_SAMPLES; i++) {
        sum += analogRead(PH_ANALOG_PIN);
        delay(PH_READ_DELAY_MS);
    }
    
    float avg_reading = sum / (float)PH_READ_SAMPLES;
    
    // Convertir a voltaje
    float voltage = (avg_reading / PH_ADC_RESOLUTION) * PH_REFERENCE_VOLTAGE;
    
    Serial.printf("pH: Lectura ADC promedio = %.1f, Voltaje = %.3f V\n", avg_reading, voltage);
    
    // Usar la libreria DFRobot_PH para calcular el pH con compensacion de temperatura
    float ph_value = ph_sensor.readPH(voltage, temperature);
    
    return ph_value;
}

/**
 * @brief Actualiza la temperatura para compensacion de pH
 * @param temp Temperatura en grados C
 */
void sensor_ph_set_temperature(float temp) {
    if (temp >= -50.0f && temp <= 100.0f) {
        temperature = temp;
        Serial.printf("pH: Temperatura actualizada a %.2f grados C\n", temp);
    }
}

/**
 * @brief Lee todos los datos del sensor de pH
 */
bool sensor_ph_read_all(sensor_data_t* data) {
    if (!sensor_available || !data) return false;

    // Encender alimentacion de sensores antes de leer
    sensor_ph_power_on();
    
    // Leer valor de pH
    float ph = read_ph_value();
    
    // Verificar si la lectura es valida
    if (ph < PH_MIN || ph > PH_MAX) {
        Serial.printf("pH: ADVERTENCIA - Lectura fuera de rango: %.2f\n", ph);
        // No marcar como error, solo advertencia
    }
    
    data->ph = ph;
    Serial.printf("pH: Valor de pH = %.2f\n", ph);
    
    // Apagar alimentacion despues de leer
    sensor_ph_power_off();
    
    return true;
}

/**
 * @brief Obtiene el payload del sensor de pH
 */
uint8_t sensor_ph_get_payload(payload_config_t* config) {
    if (!config || !sensor_available) return 0;

    sensor_data_t data;
    if (!sensor_ph_read_all(&data)) {
        data.ph = SENSOR_ERROR_PH;
    }

    // Codificar pH como uint16 (multiplicar por 100)
    uint16_t ph_int = (uint16_t)(data.ph * 100);
    
    config->buffer[0] = ph_int >> 8;
    config->buffer[1] = ph_int & 0xFF;
    
    return 2;  // 2 bytes enviados
}

/**
 * @brief Obtiene el nombre del sensor
 */
const char* sensor_ph_get_name(void) {
    return SENSOR_PH_NAME;
}

/**
 * @brief Fuerza el estado del sensor para testing
 */
void sensor_ph_set_available_for_testing(bool available) {
    sensor_available = available;
    Serial.printf("TESTING: Sensor pH forzado a %s\n", available ? "disponible" : "no disponible");
}

#endif // ENABLE_SENSOR_PH
