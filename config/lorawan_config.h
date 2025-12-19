/**
 * @file      lorawan_config.h
 * @brief     Configuración de claves LoRaWAN para 5 sensores
 *
 * Este archivo contiene las credenciales OTAA para 5 sensores diferentes.
 * Selecciona el sensor activo mediante el define SENSOR_ID.
 *
 * @author    Proyecto IoT de Bajo Consumo
 * @version   1.0
 * @date      2025
 */

#ifndef LORAWAN_CONFIG_H
#define LORAWAN_CONFIG_H

#include <lmic.h>
#include <Arduino.h> // For PROGMEM and uint8_t/u1_t

// ==================== SELECCIÓN DE SENSOR ====================
// Descomenta UNA de las siguientes líneas para seleccionar el sensor
#define SENSOR_ID 1  // Por defecto, Sensor 1
// #define SENSOR_ID 2
// #define SENSOR_ID 3
// #define SENSOR_ID 4
// #define SENSOR_ID 5

// ==================== CLAVES LoRaWAN OTAA ====================
// Las claves se seleccionan automáticamente según SENSOR_ID

#if SENSOR_ID == 1
    // ========== SENSOR 1 ==========
    // This EUI must be in little-endian format, so least-significant-byte
    // first. When copying an EUI from ttnctl output, this means to reverse
    // the bytes. For TTN issued EUIs the last bytes should be 0x00, 0x00, 0x00.
    static const u1_t PROGMEM APPEUI[8] = {0x00, 0x78, 0x56, 0x34, 0x12, 0xEF, 0xCD, 0xAB};  // INTRODUCIR EN LSB
    
    // This should also be in little endian format, see above. (least-significant-byte first)
    static const u1_t PROGMEM DEVEUI[8] = {0x82, 0x71, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};  // INTRODUCIR EN LSB
    
    // This key should be in big endian format (or, since it is not really a
    // number but a block of memory, endianness does not really apply). In
    // practice, a key taken from ttnctl can be copied as-is.
    // The key shown here is the semtech default key.
    static const u1_t PROGMEM APPKEY[16] = {0xD6, 0x50, 0xD1, 0x71, 0x94, 0x06, 0xBF, 0x86, 0x02, 0x78, 0xEF, 0x8D, 0x40, 0x3F, 0x68, 0x7D};  // INTRODUCIR EN MSB

#elif SENSOR_ID == 2
    // ========== SENSOR 2 ==========
    // Application EUI (AppEUI) - LSB format
    static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Device EUI (DevEUI) - LSB format
    static const u1_t PROGMEM DEVEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Application Key (AppKey) - MSB format
    static const u1_t PROGMEM APPKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR

#elif SENSOR_ID == 3
    // ========== SENSOR 3 ==========
    // Application EUI (AppEUI) - LSB format
    static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Device EUI (DevEUI) - LSB format
    static const u1_t PROGMEM DEVEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Application Key (AppKey) - MSB format
    static const u1_t PROGMEM APPKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR

#elif SENSOR_ID == 4
    // ========== SENSOR 4 ==========
    // Application EUI (AppEUI) - LSB format
    static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Device EUI (DevEUI) - LSB format
    static const u1_t PROGMEM DEVEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Application Key (AppKey) - MSB format
    static const u1_t PROGMEM APPKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR

#elif SENSOR_ID == 5
    // ========== SENSOR 5 ==========
    // Application EUI (AppEUI) - LSB format
    static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Device EUI (DevEUI) - LSB format
    static const u1_t PROGMEM DEVEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR
    
    // Application Key (AppKey) - MSB format
    static const u1_t PROGMEM APPKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // VACÍO - COMPLETAR

#else
    #error "SENSOR_ID debe estar definido como 1, 2, 3, 4 o 5"
#endif

#endif // LORAWAN_CONFIG_H
