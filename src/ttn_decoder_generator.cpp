/**
 * @file      ttn_decoder_generator.cpp
 * @brief     Generador dinámico de decoders TTN según configuración de sensores
 *
 * Este archivo genera automáticamente el código JavaScript del decoder
 * para The Things Network (TTN) basado en los sensores configurados.
 *
 * @author    Boya Marítima V2 - Medialab Uniovi
 * @version   2.0
 * @date      2025
 */

#include "../config/config.h"
#include <Arduino.h>

// =============================================================================
// CONFIGURACIÓN DEL GENERADOR DE DECODERS TTN
// =============================================================================

// Define esta constante en config.h para habilitar la impresión del decoder
#ifndef SHOW_TTN_DECODER
#define SHOW_TTN_DECODER 0  // Cambia a 1 para mostrar el decoder por Serial
#endif

// =============================================================================
// FUNCIONES PARA GENERAR EL DECODER TTN
// =============================================================================

/**
 * @brief Imprime el header del decoder TTN
 */
static void print_decoder_header() {
    Serial.println(F(""));
    Serial.println(F("==================== DECODIFICADOR PAYLOAD TTN ===================="));
    Serial.println(F(""));
    Serial.println(F("// COPIA Y PEGA ESTE CÓDIGO EN TTN CONSOLE -> APPLICATIONS -> PAYLOAD FORMATTERS"));
    Serial.println(F("// 1. Ve a tu aplicación en TTN Console"));
    Serial.println(F("// 2. Ve a Payload formatters -> Uplink"));
    Serial.println(F("// 3. Selecciona 'Custom Javascript formatter'"));
    Serial.println(F("// 4. Pega el código siguiente en el campo 'Formatter code'"));
    Serial.println(F("// 5. Haz clic en 'Save changes'"));
    Serial.println(F(""));
    Serial.println(F("function decodeUplink(input) {"));
    Serial.println(F("  var data = {};"));
    Serial.println(F("  var bytes = input.bytes;"));
    Serial.println(F("  var offset = 0;"));
    Serial.println(F(""));
}

/**
 * @brief Imprime el código para decodificar batería (primer byte)
 */
static void print_battery_percent_decoder() {
    Serial.println(F("  // Byte 0: Batería en porcentaje (0-100%)"));
    Serial.println(F("  data.bateria = bytes[offset++];"));
    Serial.println(F(""));
}

/**
 * @brief Imprime el código para decodificar pH
 */
static void print_ph_decoder() {
    Serial.println(F("  // Byte 1-2: pH (x100) - Little-endian"));
    Serial.println(F("  var ph_raw = bytes[offset++] | (bytes[offset++] << 8);"));
    Serial.println(F("  data.ph = ph_raw / 100.0;"));
    Serial.println(F(""));
}

/**
 * @brief Imprime el código para decodificar temperatura exterior
 */
static void print_temperature_ext_decoder() {
    Serial.println(F("  // Byte 3-4: Temperatura exterior BME280 (°C * 100) - Little-endian"));
    Serial.println(F("  var temp_ext_raw = bytes[offset++] | (bytes[offset++] << 8);"));
    Serial.println(F("  data.temp_ambiente = temp_ext_raw / 100.0;"));
    Serial.println(F(""));
}

/**
 * @brief Imprime el código para decodificar temperatura del agua
 */
static void print_temperature_water_decoder() {
    Serial.println(F("  // Byte 5-6: Temperatura agua 1m DS18B20 (°C * 100) - Little-endian"));
    Serial.println(F("  var temp_water_raw = bytes[offset++] | (bytes[offset++] << 8);"));
    Serial.println(F("  data.temp_1m = temp_water_raw / 100.0;"));
    Serial.println(F(""));
}

/**
 * @brief Imprime el código para decodificar humedad
 */
static void print_humidity_decoder() {
    Serial.println(F("  // Byte 7-8: Humedad BME280 (% * 100) - Little-endian"));
    Serial.println(F("  var humidity_raw = bytes[offset++] | (bytes[offset++] << 8);"));
    Serial.println(F("  data.humidity = humidity_raw / 100.0;"));
    Serial.println(F(""));
}

/**
 * @brief Imprime el código para decodificar presión
 */
static void print_pressure_decoder() {
    Serial.println(F("  // Byte 9-10: Presión atmosférica BME280 (hPa * 10) - Little-endian"));
    Serial.println(F("  var pressure_raw = bytes[offset++] | (bytes[offset++] << 8);"));
    Serial.println(F("  data.presion_hPa = pressure_raw / 10.0;"));
    Serial.println(F(""));
}

/**
 * @brief Imprime validación del tamaño del payload
 */
static void print_payload_validation() {
    Serial.println(F("  // Validar tamaño del payload (11 bytes esperados)"));
    Serial.println(F("  if (bytes.length !== 11) {"));
    Serial.println(F("    return {"));
    Serial.println(F("      data: data,"));
    Serial.println(F("      warnings: ['Payload size should be 11 bytes, got ' + bytes.length],"));
    Serial.println(F("      errors: []"));
    Serial.println(F("    };"));
    Serial.println(F("  }"));
    Serial.println(F(""));
}

/**
 * @brief Imprime el footer del decoder TTN
 */
static void print_decoder_footer() {
    Serial.println(F("  return { data: data };"));
    Serial.println(F("}"));
    Serial.println(F(""));
    Serial.println(F("==================== FIN DEL DECODIFICADOR ===================="));
    Serial.println(F(""));
}

/**
 * @brief Imprime información sobre la configuración actual
 */
static void print_configuration_info() {
    Serial.println(F(""));
    Serial.println(F("=== CONFIGURACIÓN BOYA MARÍTIMA V2 ==="));

    Serial.println(F("Sensores activos:"));
#ifdef ENABLE_SENSOR_BME280
    Serial.println(F("  ✓ BME280 (Temperatura exterior, Humedad, Presión)"));
#endif
#ifdef ENABLE_SENSOR_DS18B20
    Serial.println(F("  ✓ DS18B20 (Temperatura agua 1m)"));
#endif
#ifdef ENABLE_SENSOR_PH
    Serial.println(F("  ✓ DFRobot pH (pH del agua)"));
#endif

    Serial.println(F(""));

    // Calcular tamaño del payload
    uint8_t payload_size = PAYLOAD_SIZE_BYTES;
    Serial.printf("Tamaño del payload: %d bytes\r\n", payload_size);

    // Información sobre estructura del payload
    Serial.println(F("Estructura del payload (11 bytes):"));
    Serial.println(F("  Byte 0:      Batería (%)"));
    Serial.println(F("  Byte 1-2:    pH (x100) - Little-endian"));
    Serial.println(F("  Byte 3-4:    Temperatura exterior (x100) - Little-endian"));
    Serial.println(F("  Byte 5-6:    Temperatura agua 1m (x100) - Little-endian"));
    Serial.println(F("  Byte 7-8:    Humedad (x100) - Little-endian"));
    Serial.println(F("  Byte 9-10:   Presión (x10) - Little-endian"));

    Serial.println(F(""));
}

/**
 * @brief Genera e imprime el decoder TTN completo por Serial
 *
 * Esta función analiza la configuración actual de sensores y genera
 * automáticamente el código JavaScript necesario para decodificar
 * el payload en The Things Network Console.
 */
void generate_and_print_ttn_decoder() {
    if (!SHOW_TTN_DECODER) {
        return; // No mostrar si no está habilitado
    }

    print_configuration_info();
    print_decoder_header();
    print_payload_validation();

    // Generar el código de decodificación para Boya Marítima V2
    // Payload: Battery(1) + pH(2) + TempExt(2) + Temp1m(2) + Humidity(2) + Pressure(2) = 12 bytes
    
    print_battery_percent_decoder();
    
#ifdef ENABLE_SENSOR_PH
    print_ph_decoder();
#endif

#ifdef ENABLE_SENSOR_BME280
    print_temperature_ext_decoder();
#endif

#ifdef ENABLE_SENSOR_DS18B20
    print_temperature_water_decoder();
#endif

#ifdef ENABLE_SENSOR_BME280
    print_humidity_decoder();
    print_pressure_decoder();
#endif

    print_decoder_footer();
}

/**
 * @brief Genera el decoder TTN y lo devuelve como string (para uso futuro)
 *
 * @param buffer Buffer donde almacenar el código generado
 * @param max_size Tamaño máximo del buffer
 * @return Número de caracteres escritos
 */
uint16_t generate_ttn_decoder_string(char* buffer, uint16_t max_size) {
    if (!buffer || max_size < 100) return 0;

    uint16_t offset = 0;

    // Header
    offset += snprintf(buffer + offset, max_size - offset,
        "function decodeUplink(input) {\n"
        "  var data = {};\n"
        "  var bytes = input.bytes;\n"
        "  var offset = 0;\n\n"
        "  if (bytes.length !== 11) {\n"
        "    return { data: data, warnings: ['Payload size should be 11 bytes'], errors: [] };\n"
        "  }\n\n");

    // Batería
    offset += snprintf(buffer + offset, max_size - offset,
        "  data.bateria = bytes[offset++];\n");

    // pH
#ifdef ENABLE_SENSOR_PH
    offset += snprintf(buffer + offset, max_size - offset,
        "  var ph_raw = bytes[offset++] | (bytes[offset++] << 8);\n"
        "  data.ph = ph_raw / 100.0;\n");
#endif

    // Temperatura exterior BME280
#ifdef ENABLE_SENSOR_BME280
    offset += snprintf(buffer + offset, max_size - offset,
        "  var temp_ext_raw = bytes[offset++] | (bytes[offset++] << 8);\n"
        "  data.temp_ambiente = temp_ext_raw / 100.0;\n");
#endif

    // Temperatura agua DS18B20
#ifdef ENABLE_SENSOR_DS18B20
    offset += snprintf(buffer + offset, max_size - offset,
        "  var temp_water_raw = bytes[offset++] | (bytes[offset++] << 8);\n"
        "  data.temp_1m = temp_water_raw / 100.0;\n");
#endif

    // Humedad y presión BME280
#ifdef ENABLE_SENSOR_BME280
    offset += snprintf(buffer + offset, max_size - offset,
        "  var humidity_raw = bytes[offset++] | (bytes[offset++] << 8);\n"
        "  data.humidity = humidity_raw / 100.0;\n"
        "  var pressure_raw = bytes[offset++] | (bytes[offset++] << 8);\n"
        "  data.presion_hPa = pressure_raw / 10.0;\n");
#endif

    // Footer
    offset += snprintf(buffer + offset, max_size - offset,
        "\n  return { data: data, warnings: [], errors: [] };\n"
        "}\n");

    return offset;
}