# � Boya V2 - Sistema de Monitoreo Marítimo LoRaWAN

> ### 🎓 Proyecto Académico - Sin Ánimo de Lucro
> Este proyecto ha sido desarrollado para apoyar a los investigadores y prototipos del Medialab de la Universidad de Oviedo.  

> Todo el código, documentación y diseños están disponibles gratuitamente para fines educativos y de investigación.

---

> ### ⚠️ Responsabilidad y Normativa
> Antes de desplegar el sistema, consulta el documento:  
> [docs/2_responsabilidad.md](docs/2_responsabilidad.md)  
> Uso responsable y ético.  
> **Importante:** Infórmate sobre la normativa vigente del espectro radioeléctrico en tu país para operar dispositivos LoRaWAN legalmente.

---

**Boya marítima autónoma para monitoreo ambiental con ESP32 LilyGo T3 v1.6.1 y LoRaWAN**

[![PlatformIO](https://img.shields.io/badge/PlatformIO-3776AB?logo=platformio)](https://platformio.org/)
[![ESP32](https://img.shields.io/badge/ESP32-000000?logo=espressif)](https://www.espressif.com/)
[![LoRaWAN](https://img.shields.io/badge/LoRaWAN-EU868-FF6B35)](https://lora-alliance.org/)
[![TTN](https://img.shields.io/badge/The%20Things%20Network-000000)](https://www.thethingsnetwork.org/)

---

## 🎯 ¿Qué es este proyecto?

Una **boya marítima autónoma** diseñada para monitoreo ambiental en entornos acuáticos que combina:
- **ESP32 LilyGo T3 v1.6.1** con gestión inteligente de energía
- **LoRaWAN** para comunicación de largo alcance desde el mar
- **Sensores ambientales marítimos** para monitoreo de calidad del agua
- **Gestión de energía solar** para operación continua
- **Integración completa** con The Things Network (TTN)

### ✨ Características principales
- 🌊 **Monitoreo marítimo**: pH, temperatura del agua y condiciones atmosféricas
- 🔧 **Arquitectura modular**: Sistema flexible para diferentes configuraciones de sensores
- 📡 **LoRaWAN OTAA**: Comunicación segura de largo alcance con autenticación
- ☀️ **Energía solar**: Operación autónoma con batería y panel solar
- 📊 **Payload optimizado**: 12 bytes de datos comprimidos eficientemente
- 🖥️ **Display OLED**: Interfaz visual con información del sistema

---

## 🌊 Sensores Integrados

### 📊 Sistema de Monitoreo Completo

#### 🌡️ **BME280 - Sensor Ambiental (I2C)**
- **Temperatura exterior**: Monitoreo atmosférico (-40°C a +85°C)
- **Humedad relativa**: Medición precisa (0-100%)
- **Presión atmosférica**: Registro barométrico (300-1100 hPa)
- **Precisión**: ±1°C, ±3% HR, ±1 hPa
- **Función adicional**: Compensación de temperatura para calibración de pH

#### 🌊 **DS18B20 - Temperatura del Agua (OneWire)**
- **Temperatura a 1m de profundidad**: Monitoreo térmico del agua
- **Rango**: -55°C a +125°C
- **Precisión**: ±0.5°C
- **Interfaz**: OneWire digital
- **Encapsulado**: Sensor sumergible waterproof

#### 🧪 **DFRobot pH - Sensor de pH del Agua (Analógico)**
- **Medición de pH**: Acidez/alcalinidad del agua (0-14 pH)
- **Compensación automática**: Utiliza temperatura del BME280
- **Calibración**: Sistema de 2 puntos (pH 4.0 y pH 7.0)
- **Interfaz**: ADC analógico GPIO25
- **Almacenamiento**: Calibración guardada en EEPROM

### 📦 Estructura del Payload (12 bytes)

```cpp
Byte 0-1:  Batería (%)        - Estado de carga del sistema
Byte 2-3:  pH (x100)           - Acidez del agua
Byte 4-5:  Temp exterior (x100) - Temperatura atmosférica BME280
Byte 6-7:  Temp 1m agua (x100) - Temperatura del agua DS18B20
Byte 8-9:  Humedad (x100)      - Humedad relativa BME280
Byte 10-11: Presión (x10)      - Presión atmosférica BME280
```

---

## 🚀 Inicio Rápido (5 minutos)

### 1. **Instala el entorno**
```bash
# Clona el proyecto
git clone https://github.com/MediaLabUniovi/Boya-V2.git
cd "boya v2"

# Abre en VS Code con PlatformIO
code .
```

### 2. **Configuración de sensores** (`config/config.h`)
Los tres sensores están habilitados por defecto:
```cpp
#define ENABLE_SENSOR_BME280    // Temp, humedad, presión
#define ENABLE_SENSOR_DS18B20   // Temperatura agua 1m
#define ENABLE_SENSOR_PH        // pH del agua
```

### 3. **Configura credenciales LoRaWAN** (`config/lorawan_config.h`)
```cpp
// Selecciona tu ID de sensor (1-5)
#define SENSOR_ID 1

// Las credenciales se cargan automáticamente según SENSOR_ID
// Edita los arrays APPEUI, DEVEUI, APPKEY correspondientes
```

### 4. **Compila y sube**
```bash
pio run --target upload
```

### 5. **¡Listo!** Ve tus datos en TTN Console

---

## 📚 Documentación

| Documento | Contenido | Tiempo |
|-----------|-----------|--------|
| [**📖 Guía de Uso**](docs/6_uso.md) | Configuración completa paso a paso | 20 min |
| [**🏛️ Arquitectura**](docs/4_arquitectura.md) | Diseño técnico del sistema | 15 min |
| [**🔧 Troubleshooting**](docs/9_troubleshooting.md) | Solución de problemas comunes | 15 min |
| [**⚙️ Desarrollo**](docs/5_desarrollo.md) | Modificar y extender el código | 30 min |
| [**📡 Configuración TTN**](docs/7_configuracion_ttn.md) | Setup completo en TTN | 10 min |

---

### � Configuración de Sensores

#### 🌊 **Configuración de Boya Marítima (Actual)**
```cpp
#define ENABLE_SENSOR_BME280     // Temperatura exterior, humedad, presión
#define ENABLE_SENSOR_DS18B20    // Temperatura agua a 1m
#define ENABLE_SENSOR_PH         // pH del agua
```
**Payload**: 12 bytes | **Campos**: Batería, pH, Temp_Ext, Temp_1m, Humedad, Presión

#### ⚡ Gestión de Energía
- **Control de sensores por MOSFET**: GPIO13 alimenta sensores DS18B20 y pH
- **Tiempo de estabilización**: 30 segundos antes de lectura de pH
- **Compensación de temperatura**: pH calibrado con temperatura del BME280
- **Panel solar**: Carga automática de batería LiPo 3.7V

---

## 📡 Decoder TTN para Boya V2

Decoder JavaScript optimizado para los 12 bytes del payload de la boya marítima.

### 🔧 Decodificador Boya V2

```javascript
function decodeUplink(input) {
  var bytes = input.bytes;
  var data = {};
  
  // Validar tamaño del payload (12 bytes)
  if (bytes.length !== 12) {
    return {
      data: data,
      warnings: ['Payload size should be 12 bytes, got ' + bytes.length],
      errors: []
    };
  }
  
  // Byte 0-1: Batería (%)
  data.battery_percent = bytes[0];
  
  // Byte 2-3: pH (x100) - Little-endian
  var ph_raw = bytes[2] | (bytes[3] << 8);
  data.ph = ph_raw / 100.0;
  
  // Byte 4-5: Temperatura exterior BME280 (x100) - Little-endian
  var temp_ext_raw = bytes[4] | (bytes[5] << 8);
  data.temperature_ext = temp_ext_raw / 100.0;
  
  // Byte 6-7: Temperatura agua 1m DS18B20 (x100) - Little-endian
  var temp_water_raw = bytes[6] | (bytes[7] << 8);
  data.temperature_water_1m = temp_water_raw / 100.0;
  
  // Byte 8-9: Humedad BME280 (x100) - Little-endian
  var humidity_raw = bytes[8] | (bytes[9] << 8);
  data.humidity = humidity_raw / 100.0;
  
  // Byte 10-11: Presión BME280 (x10) - Little-endian
  var pressure_raw = bytes[10] | (bytes[11] << 8);
  data.pressure = pressure_raw / 10.0;
  
  return { data: data, warnings: [], errors: [] };
}
```

### 📊 Ejemplo de Payload

| Payload (hex) | Datos Decodificados |
|---------------|---------------------|
| `55 E8 03 F4 01 10 27 D2 0C 88 13` | `{"battery_percent": 85, "ph": 7.00, "temperature_ext": 25.00, "temperature_water_1m": 100.00, "humidity": 32.10, "pressure": 1013.6}` |

### 📝 Instalación en TTN

1. Ve a TTN Console → Applications → [Tu aplicación]
2. Navega a **Payload formatters** → **Uplink**
3. Selecciona **Custom Javascript formatter**
4. Pega el código del decoder
5. Guarda los cambios

---

## 🔋 Gestión de Energía

### ⚡ Sistema de Alimentación

La boya cuenta con un sistema de energía diseñado para operación autónoma:

- **Batería LiPo 3.7V**: Almacenamiento principal de energía
- **Panel solar USB-C**: Carga automática durante el día
- **PMU AXP2101**: Gestión inteligente de carga y batería
- **Control de sensores**: MOSFET en GPIO13 para encendido/apagado selectivo

### 💡 Optimización de Consumo

```
┌─────────────────┐    ┌──────────────────────────┐    ┌──────────────┐
│   Deep Sleep    │ -> │ Estabilización Sensores  │ -> │   TX LoRa    │
│   (ESP32)       │    │   (30s para pH)          │    │   (120mA)    │
└─────────────────┘    └──────────────────────────┘    └──────────────┘
```

### 🔌 Control de Alimentación

- **Sensores DS18B20 y pH**: Alimentación controlada por GPIO13 (MOSFET)
- **Sensor BME280**: Alimentación permanente en I2C
- **Tiempo de estabilización**: 30 segundos antes de lecturas (especialmente pH)
- **Apagado automático**: Sensores se apagan después de cada lectura

### ☀️ Energía Solar

El sistema está diseñado para carga solar continua:
- **Entrada**: USB-C 5V desde panel solar
- **Regulación**: PMU AXP2101 gestiona carga óptima
- **Protección**: Sobrecarga, descarga profunda, cortocircuito

---

## 📁 Estructura del Proyecto

```
boya-v2/
├── 📁 config/                    # ⚙️ Configuración del sistema
│   ├── config.h                  # Configuración principal
│   ├── hardware_config.h         # Configuración hardware
│   ├── lorawan_config.h          # Credenciales LoRaWAN (5 sensores)
│   ├── lorawan_config_template.h # Plantilla configuración TTN
│   └── sensor/                   # Configuraciones específicas de sensores
│       ├── sensor_bme280.h       # Config BME280 (I2C)
│       ├── sensor_ds18b20.h      # Config DS18B20 (OneWire)
│       └── sensor_ph.h           # Config DFRobot pH (ADC)
├── 📁 src/                       # 📄 Código fuente principal
│   ├── main.ino                 # 🚀 Punto de entrada principal
│   ├── LoRaBoards.cpp            # 📡 Configuración hardware LoRa
│   ├── pgm_board.cpp             # 🔧 Gestión LoRaWAN y OTAA
│   ├── sensor.cpp                # 🌡️ Coordinación de sensores
│   ├── screen.cpp                # 🖥️ Gestión display OLED
│   ├── solar.cpp                 # ☀️ Gestión energía solar
│   ├── ttn_decoder_generator.cpp # 📊 Generador decoder TTN
│   └── sensor/                   # Implementaciones de sensores
│       ├── sensor_bme280.cpp     # BME280 (Temp, humedad, presión)
│       ├── sensor_ds18b20.cpp    # DS18B20 (Temp agua)
│       └── sensor_ph.cpp         # DFRobot pH (pH agua)
├── 📁 include/                   # 📋 Headers y librerías
│   ├── LoRaBoards.h              # Headers hardware LoRa
│   ├── loramac.h                 # Headers LoRaWAN
│   ├── screen.h                  # Headers pantalla
│   ├── sensor_interface.h        # Interfaz común sensores
│   ├── solar.h                   # Headers energía solar
│   ├── ttn_decoder_generator.h   # Headers generador decoder
│   └── utilities.h               # Utilidades comunes
├── 📁 lib/                       # 📚 Librerías incluidas
│   ├── Adafruit_BME280_Library/  # Librería BME280
│   ├── Adafruit_BusIO/           # Bus I2C/SPI
│   ├── Adafruit_Sensor/          # Sensor unificado
│   ├── LMIC-Arduino/             # Stack LoRaWAN
│   ├── U8g2/                     # Display OLED
│   └── XPowersLib/               # Gestión PMU AXP2101
├── 📁 docs/                      # 📚 Documentación completa
│   ├── 1_guiadeinicio.md         # 🚀 Guía de inicio rápido
│   ├── 2_responsabilidad.md      # ⚖️ Responsabilidades del proyecto
│   ├── 3_hardware.md             # 🔧 Especificaciones hardware
│   ├── 4_arquitectura.md         # 🏗️ Arquitectura del sistema
│   ├── 5_desarrollo.md           # 🔧 Guía de desarrollo
│   ├── 6_uso.md                  # 📖 Guía de uso detallada
│   ├── 7_configuracion_ttn.md    # 📡 Setup TTN
│   ├── 8_ttn_decoder.md          # 📊 Decodificadores TTN
│   ├── 9_troubleshooting.md      # 🛠️ Solución de problemas
│   ├── board.jpg                 # Imagen hardware
│   └── datasheet_T3_V1.6.1.pdf   # Datasheet del dispositivo
├── platformio.ini                # ⚙️ Configuración PlatformIO
├── README.md                     # 📄 Este archivo
└── .gitignore                    # 🚫 Archivos ignorados por Git
```

## 🛠️ Desarrollo

### 📦 Dependencias PlatformIO

```ini
[env:T3_V1_6_SX1276]
platform = espressif32 @ 6.9.0
board = esp32dev
framework = arduino
lib_deps =
    adafruit/Adafruit BME280 Library@^2.2.2
    adafruit/Adafruit BusIO@^1.16.1
    adafruit/Adafruit Unified Sensor@^1.1.14
    milesburton/DallasTemperature@^3.9.0
    paulstoffregen/OneWire@^2.3.7
    https://github.com/DFRobot/DFRobot_PH.git
    olikraus/U8g2@^2.34.17
    mcci-catena/MCCI LoRaWAN LMIC library@^4.1.1
    lewisxhe/XPowersLib@^0.2.6
```

### 🧪 Testing y Debug

```bash
# Compilar proyecto
pio run

# Subir a la placa
pio run --target upload

# Monitor serial (115200 baud)
pio device monitor

# Limpiar y reconstruir
pio run --target clean && pio run
```

### 🔧 Calibración del Sensor de pH

El sensor DFRobot pH requiere calibración inicial:

1. **Modo calibración**: Envía comandos por Serial Monitor
   ```
   ENTERPH  // Entrar en modo calibración
   CALPH    // Calibrar con solución buffer (pH 4.0 o 7.0)
   EXITPH   // Salir y guardar calibración
   ```

2. **Proceso recomendado**:
   - Calibrar primero con solución pH 7.0 (neutro)
   - Luego calibrar con solución pH 4.0 (ácido)
   - La calibración se guarda en EEPROM automáticamente

---

## 📈 Dashboard TTN

### Visualización de Datos de Boya Marítima
```
🔋 Batería: 85%
🧪 pH: 7.28
🌡️ Temperatura Exterior: 18.5°C
🌊 Temperatura Agua (1m): 16.3°C
💧 Humedad: 72.5%
🌪️ Presión: 1015.2 hPa
📡 RSSI: -95dBm, SNR: 8.5dB
```

### JSON Decodificado
```json
{
  "battery_percent": 85,
  "ph": 7.28,
  "temperature_ext": 18.5,
  "temperature_water_1m": 16.3,
  "humidity": 72.5,
  "pressure": 1015.2
}
```

---

## 🔧 Desarrollo y Extensiones

### 🤓 **Para Estudiantes**
Este proyecto es perfecto para aprender:
- **IoT desde cero**: Conceptos básicos hasta implementación
- **ESP32**: Programación embebida en C++
- **LoRaWAN**: Redes de largo alcance
- **Sensores**: Interfaces I2C, OneWire, GPIO
- **Energía**: Gestión de consumo y carga solar

### 🚀 **Agregar Nuevo Sensor**

El sistema sigue una arquitectura modular. Para añadir un nuevo sensor:

1. **Crear archivo de configuración**: `config/sensor/sensor_nuevo.h`
   ```cpp
   #ifndef SENSOR_NUEVO_H
   #define SENSOR_NUEVO_H
   
   #define NUEVO_PIN 26
   #define NUEVO_POWER_PIN 13  // MOSFET compartido
   
   #endif
   ```

2. **Implementar funciones**: `src/sensor/sensor_nuevo.cpp`
   ```cpp
   void sensor_nuevo_init() {
       // Inicialización del sensor
   }
   
   bool sensor_nuevo_read_all(sensor_data_t* data) {
       // Lectura del sensor
       data->nuevo_valor = leer_sensor();
       return true;
   }
   ```

3. **Habilitar en config.h**:
   ```cpp
   #define ENABLE_SENSOR_NUEVO
   ```

4. **Actualizar estructura de datos** en `config/config.h`:
   ```cpp
   typedef struct {
       // ... campos existentes
       float nuevo_valor;
   } sensor_data_t;
   ```

---

## 📞 Soporte

### 🆘 **¿Problemas?**
1. **Lee primero**: [Guía de troubleshooting](docs/9_troubleshooting.md)
2. **Configuración**: [Guía de uso](docs/6_uso.md)
3. **TTN Setup**: [Configuración TTN](docs/7_configuracion_ttn.md)

### 🐛 **Reportar Bugs**
```markdown
**Entorno:**
- Hardware: LilyGo T3 v1.6.1
- Sensores activos: DHT22, BMP280
- TTN Region: EU868

**Síntomas:**
- Descripción del problema
- Logs del Serial Monitor
- Estado en TTN Console
```

---

## 🛠️ Hardware Requerido

### 📋 Lista de Componentes

| Componente | Especificación | Notas |
|------------|----------------|-------|
| **Placa principal** | LilyGo T3 v1.6.1 | ESP32 + LoRa SX1276 + PMU AXP2101 |
| **Antena** | 868MHz LoRaWAN | Incluida con la placa |
| **Batería** | LiPo 3.7V | Para operación autónoma |
| **Panel solar** | 5V USB-C | Recomendado para despliegue permanente |
| **Sensor BME280** | I2C | Temperatura, humedad, presión atmosférica |
| **Sensor DS18B20** | OneWire waterproof | Temperatura del agua sumergible |
| **Sensor pH DFRobot** | Analógico | Medición de pH del agua |
| **Soluciones buffer** | pH 4.0 y pH 7.0 | Para calibración del sensor de pH |

### 🔌 Conexiones de Sensores de la Boya

| Sensor | Pines ESP32 | Alimentación | Notas |
|--------|-------------|--------------|-------|
| **BME280** | I2C: GPIO 17 (SDA), 18 (SCL) | 3.3V | Dirección I2C: 0x76 o 0x77 |
| **DS18B20** | GPIO 15 (OneWire) | 3.3V (vía GPIO13) | Sensor waterproof sumergible |
| **pH DFRobot** | GPIO 25 (ADC) | 3.3V (vía GPIO13) | Requiere calibración inicial |
| **Control de alimentación** | GPIO 13 (MOSFET) | - | Controla DS18B20 y pH |
| **OLED SSD1306** | I2C: GPIO 17 (SDA), 18 (SCL) | 3.3V | Dirección I2C: 0x3C |

### ⚡ Diagrama de Conexiones de la Boya

```
ESP32 LilyGo T3 v1.6.1 (Boya Marítima)
├── 🔌 USB-C (Programación + Carga Solar)
├── 📡 Antena LoRa 868MHz (Comunicación TTN)
├── 🔋 Batería LiPo 3.7V (Almacenamiento energía)
├── ☀️ Panel Solar 5V (Carga continua)
├── 🌡️ BME280 (I2C GPIO 17/18) → Temp exterior, humedad, presión
├── 🌊 DS18B20 (GPIO 15) → Temperatura agua 1m
├── 🧪 pH DFRobot (GPIO 25) → pH del agua
├── ⚡ MOSFET (GPIO 13) → Control alimentación sensores
└── 🖥️ OLED (I2C GPIO 17/18) → Display estado
```

### 🌊 Consideraciones para Despliegue Marítimo

- **Encapsulado impermeable**: Protección IP67 o superior para la electrónica
- **Sensor DS18B20**: Usar versión waterproof con cable resistente
- **Sensor pH**: Proteger sonda con capuchón cuando no esté en uso
- **Antena**: Posicionar verticalmente para mejor cobertura
- **Panel solar**: Orientación óptima hacia el sur (hemisferio norte)
- **Flotabilidad**: Diseño de boya debe mantener sensores sumergidos y electrónica seca

---

## 📄 Licencia

**MIT License** - Libre para uso educativo

---

**¡Bienvenido al monitoreo marítimo con IoT y LoRaWAN!** 🌊

*[Empieza aquí](docs/6_uso.md)* | *[Arquitectura técnica](docs/4_arquitectura.md)* | *[Solución de problemas](docs/9_troubleshooting.md)*

---
**📅 Actualizado: Diciembre 2025** | **🔧 LilyGo T3 v1.6.1** | **📡 LoRaWAN EU868** | **🌊 Boya Marítima V2**

# LILYGO LoRa T3 Low Power PINOUT

[Datasheet](docs/datasheet_T3_V1.6.1.pdf)

![Board](docs/board.jpg)