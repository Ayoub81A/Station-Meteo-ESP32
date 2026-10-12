#include <Arduino.h>
#include "config.h"
#include "sensors.h"

static void printTelemetry(const SensorData &data) {
    uint32_t uptimeSec = millis() / 1000;

    Serial.println(F("\n=================================================="));
    Serial.printf(" [Weather Station Telemetry] Uptime: %lu s\n", uptimeSec);
    Serial.println(F("=================================================="));

    // Temperature
    Serial.print(F("  Temperature : "));
    if (data.bmeAvailable) {
        Serial.printf("%.2f °C\n", data.temperature);
    } else {
        Serial.println(F("[ERROR - Sensor Offline]"));
    }

    // Humidity
    Serial.print(F("  Humidity    : "));
    if (data.bmeAvailable) {
        Serial.printf("%.2f %%\n", data.humidity);
    } else {
        Serial.println(F("[ERROR - Sensor Offline]"));
    }

    // Atmospheric Pressure
    Serial.print(F("  Pressure    : "));
    if (data.bmeAvailable) {
        Serial.printf("%.2f hPa\n", data.pressure);
    } else {
        Serial.println(F("[ERROR - Sensor Offline]"));
    }

    // Luminosity
    Serial.print(F("  Luminosity  : "));
    if (data.bh1750Available) {
        Serial.printf("%.2f lx\n", data.luminosity);
    } else {
        Serial.println(F("[ERROR - Sensor Offline]"));
    }

    Serial.println(F("=================================================="));
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    
    // Allow hardware Serial to stabilize
    delay(1000);

    Serial.println();
    Serial.println(F("**************************************************"));
    Serial.println(F("*       ESP32 Autonomous Weather Station         *"));
    Serial.println(F("*       Phase 1: Serial Sensor Reading           *"));
    Serial.println(F("**************************************************"));
    Serial.printf("[SYSTEM] CPU Frequency: %u MHz\n", getCpuFrequencyMhz());
    Serial.printf("[SYSTEM] Sampling interval: %u ms\n\n", SENSOR_READ_INTERVAL_MS);

    if (!initSensors()) {
        Serial.println(F("\n[WARNING] One or more sensors failed to initialize."));
        Serial.println(F("[WARNING] Check I2C wiring (SDA=GPIO21, SCL=GPIO22) and power supplies.\n"));
    } else {
        Serial.println(F("\n[SYSTEM] All sensors initialized successfully. Starting telemetry stream...\n"));
    }
}

void loop() {
    static uint32_t lastReadTimestamp = 0;
    uint32_t currentMillis = millis();

    if (currentMillis - lastReadTimestamp >= SENSOR_READ_INTERVAL_MS) {
        lastReadTimestamp = currentMillis;

        SensorData data = readSensors();
        printTelemetry(data);
    }

    // Yield to the underlying RTOS scheduler
    yield();
}
