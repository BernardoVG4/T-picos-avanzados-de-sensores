//Librerias
#include <Wire.h>                    // Comunicación I2C
#include <Adafruit_Sensor.h>         // Librería base de sensores Adafruit
#include <Adafruit_BMP085_U.h>       // Librería específica para BMP180

// Crear objeto BMP180
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial

  // Inicializa el sensor BMP180
  if (!bmp.begin()) {
    Serial.print("¡No se detecta el BMP180!");
    while (1);
  }
}


void loop() {
  sensors_event_t event;
  bmp.getEvent(&event);
  
  //Calcular altitud
  if (event.pressure) {
    float temperature;
    bmp.getTemperature(&temperature);

    float seaLevelPressure = 1013.25; // Presión a nivel del mar (en hPa)
    float altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature);
    
    //Mostrar datos 
    Serial.print("Altitud: ");
    Serial.print(altitude);
    Serial.println(" m");
  }

  delay(500);
}
