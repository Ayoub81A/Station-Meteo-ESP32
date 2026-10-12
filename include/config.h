#pragma once

#include <Arduino.h>

// ==========================================
// Hardware Pinout Configuration
// ==========================================
// ESP32 default hardware I2C bus pins
constexpr uint8_t I2C_SDA_PIN = 21;
constexpr uint8_t I2C_SCL_PIN = 22;

// ==========================================
// Sensor I2C Addresses
// ==========================================
// BME280 default I2C address (0x76 on most third-party modules, 0x77 on Adafruit)
constexpr uint8_t BME280_I2C_ADDR_PRIMARY   = 0x76;
constexpr uint8_t BME280_I2C_ADDR_SECONDARY = 0x77;

// BH1750 default I2C address (0x23 when ADDR pin is LOW or NC, 0x5C when ADDR is HIGH)
constexpr uint8_t BH1750_I2C_ADDR           = 0x23;

// ==========================================
// System & Telemetry Configuration
// ==========================================
constexpr uint32_t SERIAL_BAUD_RATE         = 115200;
constexpr uint32_t SENSOR_READ_INTERVAL_MS  = 2000;
