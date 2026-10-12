#include "sensors.h"
#include "config.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>

static Adafruit_BME280 bme;
static BH1750 bh1750;

static bool bmeReady = false;
static bool bh1750Ready = false;

bool initSensors() {
    Serial.println(F("[I2C] Initializing Wire bus..."));
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    // 1. Initialize BME280 with fallback address probing
    Serial.print(F("[BME280] Probing sensor on 0x"));
    Serial.println(BME280_I2C_ADDR_PRIMARY, HEX);
    if (bme.begin(BME280_I2C_ADDR_PRIMARY, &Wire)) {
        bmeReady = true;
        Serial.println(F("[BME280] OK (Detected on primary address 0x76)"));
    } else {
        Serial.print(F("[BME280] Primary not found. Probing secondary on 0x"));
        Serial.println(BME280_I2C_ADDR_SECONDARY, HEX);
        if (bme.begin(BME280_I2C_ADDR_SECONDARY, &Wire)) {
            bmeReady = true;
            Serial.println(F("[BME280] OK (Detected on secondary address 0x77)"));
        } else {
            bmeReady = false;
            Serial.println(F("[BME280] ERROR: Sensor not found! Check wiring and pull-ups."));
        }
    }

    if (bmeReady) {
        // Optimal settings for indoor/outdoor weather monitoring
        bme.setSampling(
            Adafruit_BME280::MODE_NORMAL,
            Adafruit_BME280::SAMPLING_X2,  // Temperature oversampling
            Adafruit_BME280::SAMPLING_X16, // Pressure oversampling
            Adafruit_BME280::SAMPLING_X1,  // Humidity oversampling
            Adafruit_BME280::FILTER_X16,   // IIR filter
            Adafruit_BME280::STANDBY_MS_500
        );
    }

    // 2. Initialize BH1750 ambient light sensor
    Serial.print(F("[BH1750] Probing sensor on 0x"));
    Serial.println(BH1750_I2C_ADDR, HEX);
    if (bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, BH1750_I2C_ADDR, &Wire)) {
        bh1750Ready = true;
        Serial.println(F("[BH1750] OK (Detected on address 0x23)"));
    } else {
        // Fallback to secondary address 0x5C (when ADDR pin is pulled high)
        if (bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire)) {
            bh1750Ready = true;
            Serial.println(F("[BH1750] OK (Detected on secondary address 0x5C)"));
        } else {
            bh1750Ready = false;
            Serial.println(F("[BH1750] ERROR: Sensor not found! Check wiring."));
        }
    }

    return (bmeReady && bh1750Ready);
}

SensorData readSensors() {
    SensorData data;
    data.temperature = NAN;
    data.humidity = NAN;
    data.pressure = NAN;
    data.luminosity = NAN;
    data.bmeAvailable = false;
    data.bh1750Available = false;

    if (bmeReady) {
        float temp = bme.readTemperature();
        float hum  = bme.readHumidity();
        float pres = bme.readPressure() / 100.0F; // Convert Pa to hPa

        if (!isnan(temp) && !isnan(hum) && !isnan(pres)) {
            data.temperature = temp;
            data.humidity = hum;
            data.pressure = pres;
            data.bmeAvailable = true;
        }
    }

    if (bh1750Ready) {
        float lux = bh1750.readLightLevel();
        if (lux >= 0.0F) {
            data.luminosity = lux;
            data.bh1750Available = true;
        }
    }

    return data;
}
