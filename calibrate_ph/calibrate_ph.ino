/**
 * @file      calibrate_ph.ino
 * @brief     Programa de calibración del sensor de pH DFRobot
 *
 * Este programa permite calibrar el sensor de pH usando comandos por Serial.
 * Sube este sketch al ESP32, abre Serial Monitor a 115200 baud y sigue las instrucciones.
 *
 * Comandos disponibles:
 * - ENTERPH : Entrar en modo calibración
 * - CALPH   : Calibrar con solución actual (primero pH 7.0, luego pH 4.0)
 * - EXITPH  : Guardar calibración y salir
 *
 * @author    Boya Marítima V2
 * @date      2025
 */

#include <DFRobot_PH.h>
#include <EEPROM.h>

// Configuración del sensor
#define PH_ANALOG_PIN 25              // Pin ADC para sensor de pH (GPIO25)
#define PH_REFERENCE_VOLTAGE 3.3f     // Voltaje de referencia ADC
#define PH_ADC_RESOLUTION 4096.0f     // Resolución ADC (12 bits)
#define PH_READ_SAMPLES 10            // Número de muestras para promediar
#define PH_READ_DELAY_MS 20           // Delay entre muestras

// Objeto del sensor
DFRobot_PH ph_sensor;

// Variables globales
float temperature = 25.0;  // Temperatura para compensación (°C)

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n========================================");
  Serial.println("  CALIBRACIÓN SENSOR pH DFRobot");
  Serial.println("  Boya Marítima V2");
  Serial.println("========================================\n");
  
  // Configurar pin analógico
  pinMode(PH_ANALOG_PIN, INPUT);
  
  // Inicializar EEPROM
  EEPROM.begin(32);
  
  // Inicializar sensor
  ph_sensor.begin();
  
  Serial.println("✓ Sensor pH inicializado correctamente");
  Serial.println("\n--- INSTRUCCIONES DE CALIBRACIÓN ---");
  Serial.println("1. Escribe 'ENTERPH' y presiona Enter para entrar en modo calibración");
  Serial.println("2. Sumerge el sensor en solución buffer pH 7.0");
  Serial.println("3. Espera a que la lectura se estabilice");
  Serial.println("4. Escribe 'CALPH' y presiona Enter");
  Serial.println("5. Sumerge el sensor en solución buffer pH 4.0");
  Serial.println("6. Espera a que la lectura se estabilice");
  Serial.println("7. Escribe 'CALPH' y presiona Enter");
  Serial.println("8. Escribe 'EXITPH' y presiona Enter para guardar");
  Serial.println("\nOtros comandos:");
  Serial.println("- 'TEMP XX.X' : Actualizar temperatura para compensación");
  Serial.println("- 'READ'      : Leer valor actual de pH");
  Serial.println("========================================\n");
  
  Serial.printf("Temperatura actual: %.1f °C\n", temperature);
  Serial.println("\nEsperando comandos...\n");
}

void loop() {
  // Leer y mostrar pH continuamente
  static unsigned long lastRead = 0;
  if (millis() - lastRead > 1000) {  // Cada segundo
    lastRead = millis();
    
    // Leer voltaje del sensor
    float voltage = readVoltage();
    
    // Calcular pH
    float ph_value = ph_sensor.readPH(voltage, temperature);
    
    // Mostrar información
    Serial.printf("Voltaje: %.3f V | pH: %.2f | Temp: %.1f °C\n", 
                  voltage, ph_value, temperature);
  }
  
  // Procesar comandos por Serial
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    
    Serial.println("\n>> Comando recibido: " + command);
    processCommand(command);
  }
  
  delay(10);
}

/**
 * @brief Lee el voltaje del sensor de pH
 */
float readVoltage() {
  uint32_t sum = 0;
  
  // Tomar múltiples muestras y promediar
  for (int i = 0; i < PH_READ_SAMPLES; i++) {
    sum += analogRead(PH_ANALOG_PIN);
    delay(PH_READ_DELAY_MS);
  }
  
  float avg_reading = sum / (float)PH_READ_SAMPLES;
  float voltage = (avg_reading / PH_ADC_RESOLUTION) * PH_REFERENCE_VOLTAGE;
  
  return voltage;
}

/**
 * @brief Procesa comandos recibidos por Serial
 */
void processCommand(String cmd) {
  if (cmd == "ENTERPH") {
    Serial.println("✓ Entrando en modo calibración...");
    Serial.println("  Sumerge el sensor en solución buffer pH 7.0");
    Serial.println("  Cuando se estabilice, escribe 'CALPH'");
    
  } else if (cmd == "CALPH") {
    float voltage = readVoltage();
    ph_sensor.calibration(voltage, temperature);
    Serial.println("✓ Punto de calibración guardado");
    Serial.printf("  Voltaje: %.3f V, Temperatura: %.1f °C\n", voltage, temperature);
    
  } else if (cmd == "EXITPH") {
    Serial.println("✓ Guardando calibración en EEPROM...");
    EEPROM.commit();
    Serial.println("✓ Calibración completada y guardada");
    Serial.println("  Puedes cerrar este programa y subir el código principal");
    
  } else if (cmd == "READ") {
    float voltage = readVoltage();
    float ph_value = ph_sensor.readPH(voltage, temperature);
    Serial.println("\n--- LECTURA ACTUAL ---");
    Serial.printf("Voltaje ADC: %.3f V\n", voltage);
    Serial.printf("Valor de pH: %.2f\n", ph_value);
    Serial.printf("Temperatura: %.1f °C\n", temperature);
    Serial.println("---------------------\n");
    
  } else if (cmd.startsWith("TEMP ")) {
    float newTemp = cmd.substring(5).toFloat();
    if (newTemp >= 0 && newTemp <= 100) {
      temperature = newTemp;
      Serial.printf("✓ Temperatura actualizada a %.1f °C\n", temperature);
    } else {
      Serial.println("✗ Error: Temperatura fuera de rango (0-100 °C)");
    }
    
  } else {
    Serial.println("✗ Comando no reconocido");
    Serial.println("  Comandos válidos: ENTERPH, CALPH, EXITPH, READ, TEMP XX.X");
  }
  
  Serial.println();
}
