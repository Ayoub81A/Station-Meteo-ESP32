#pragma once

#include <Arduino.h>

/**
 * @brief Aggregated sensor telemetry data
 */
struct SensorData {
    float temperature;    // Degrees Celsius (°C)
    float humidity;       // Relative Humidity (%)
    float pressure;       // Atmospheric Pressure (hPa)
    float luminosity;     // Ambient Light (lux)
    bool bmeAvailable;    // Sensor online and healthy
    bool bh1750Available; // Sensor online and healthy
};

/**
 * @brief Initializes the I2C bus and connected environmental sensors.
 * 
 * Configures the I2C bus pins and probes the BME280 (with automatic
 * address fallback between 0x76 and 0x77) and the BH1750 light sensor.
 * 
 * @return true if all sensors initialized successfully, false if any failed.
 */
bool initSensors();

/**
 * @brief Reads data from all available sensors.
 * 
 * @return SensorData struct containing readings and validity flags.
 */
SensorData readSensors();
