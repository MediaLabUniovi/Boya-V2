/**
 * @file      ttn_decoder_generator.cpp
 * @brief     Generador dinámico de decoders TTN según configuración de sensores
 *
 * Este archivo genera automáticamente el código JavaScript del decoder
 * para The Things Network (TTN) basado en los sensores configurados.
 *
 * @author    Proyecto IoT de Bajo Consumo
 * @version   1.0
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
 * @brief Imprime el código para decodificar temperatura
 */
static void print_temperature_decoder() {
    Serial.println(F("  // Temperatura (°C * 100)"));
    Serial.println(F("  data.temperatura = ((bytes[offset++] << 8) | bytes[offset++]) / 100.0;"));
}

/**
 * @brief Imprime el código para decodificar humedad
 */
static void print_humidity_decoder() {
    Serial.println(F("  // Humedad (% * 100)"));
    Serial.println(F("  data.humedad = ((bytes[offset++] << 8) | bytes[offset++]) / 100.0;"));
}

/**
 * @brief Imprime el código para decodificar presión
 */
static void print_pressure_decoder() {
    Serial.println(F("  // Presión atmosférica (hPa * 10)"));
    Serial.println(F("  data.presion = ((bytes[offset++] << 8) | bytes[offset++]) / 10.0;"));
}

/**
 * @brief Imprime el código para decodificar batería
 */
static void print_battery_decoder() {
    Serial.println(F(""));
#ifdef BATTERY_AS_PERCENTAGE
    Serial.println(F("  // Batería en porcentaje (0-100%) - Último byte"));
    Serial.println(F("  data.bateria = bytes[bytes.length - 1];"));
#else
    Serial.println(F("  // Batería en voltaje (V * 100) - Últimos 2 bytes"));
    Serial.println(F("  var batteryIndex = bytes.length - 2;"));
    Serial.println(F("  data.bateria = ((bytes[batteryIndex] << 8) | bytes[batteryIndex + 1]) / 100.0;"));
#endif
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
    Serial.println(F("=== CONFIGURACIÓN ACTUAL DE SENSORES ==="));

    Serial.print(F("Sensor activo: "));

#ifdef ENABLE_SENSOR_BME280
    Serial.print(F("BME280"));
#else
    Serial.print(F("NINGUNO (solo batería)"));
#endif

    Serial.println(F(""));

    // Calcular tamaño del payload
    uint8_t payload_size = PAYLOAD_SIZE_BYTES;
    Serial.printf("Tamaño del payload: %d bytes\r\n", payload_size);

    // Información sobre qué campos están incluidos
    Serial.println(F("Campos incluidos en el payload:"));
    if (SYSTEM_HAS_TEMPERATURE) Serial.println(F("  ✓ Temperatura (2 bytes)"));
    if (SYSTEM_HAS_HUMIDITY) Serial.println(F("  ✓ Humedad (2 bytes)"));
    if (SYSTEM_HAS_PRESSURE) Serial.println(F("  ✓ Presión atmosférica (2 bytes)"));
#ifdef BATTERY_AS_PERCENTAGE
    Serial.println(F("  ✓ Batería (1 byte - porcentaje 0-100%)"));
#else
    Serial.println(F("  ✓ Batería (2 bytes - voltaje)"));
#endif

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

    // Generar el código de decodificación según los sensores activos
    if (SYSTEM_HAS_TEMPERATURE) {
        print_temperature_decoder();
    }

    if (SYSTEM_HAS_HUMIDITY) {
        print_humidity_decoder();
    }

    if (SYSTEM_HAS_PRESSURE) {
        print_pressure_decoder();
    }

    print_battery_decoder();
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

    // Header básico
    offset += snprintf(buffer + offset, max_size - offset,
        "function decodeUplink(input) {\n"
        "  var data = {};\n"
        "  var bytes = input.bytes;\n"
        "  var offset = 0;\n\n");

    // Campos de sensores
    if (SYSTEM_HAS_TEMPERATURE) {
        offset += snprintf(buffer + offset, max_size - offset,
            "  data.temperatura = ((bytes[offset++] << 8) | bytes[offset++]) / 100.0;\n");
    }

    if (SYSTEM_HAS_HUMIDITY) {
        offset += snprintf(buffer + offset, max_size - offset,
            "  data.humedad = ((bytes[offset++] << 8) | bytes[offset++]) / 100.0;\n");
    }

    if (SYSTEM_HAS_PRESSURE) {
        offset += snprintf(buffer + offset, max_size - offset,
            "  data.presion = ((bytes[offset++] << 8) | bytes[offset++]) / 10.0;\n");
    }

    // Batería
    offset += snprintf(buffer + offset, max_size - offset,
        "\n");
#ifdef BATTERY_AS_PERCENTAGE
    offset += snprintf(buffer + offset, max_size - offset,
        "  data.bateria = bytes[bytes.length - 1];\n");
#else
    offset += snprintf(buffer + offset, max_size - offset,
        "  var batteryIndex = bytes.length - 2;\n"
        "  data.bateria = ((bytes[batteryIndex] << 8) | bytes[batteryIndex + 1]) / 100.0;\n");
#endif
    offset += snprintf(buffer + offset, max_size - offset,
        "\n"
        "  return { data: data };\n"
        "}\n");

    return offset;
}