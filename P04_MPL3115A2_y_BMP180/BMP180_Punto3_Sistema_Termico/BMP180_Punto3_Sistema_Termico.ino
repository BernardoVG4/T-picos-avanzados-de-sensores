//Librerias
#include <Wire.h>                    // Comunicación I2C
#include <Adafruit_Sensor.h>         // Librería base de sensores Adafruit
#include <Adafruit_BMP085_U.h>       // Librería específica para BMP180

// Crear objeto BMP180
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

//Pin para LM35
#define LM35 A0

//Variables a usar
float temperature;
float temperature2=0;
float value;

void setup() {
  Serial.begin(9600);   // Inicia la comunicación serial

  // Inicializa el sensor BMP180
  if (!bmp.begin()) {
    Serial.print("¡No se detecta el BMP180!");
    while (1);
  }
}

void loop() {
  value=analogRead(LM35);              // Lee el valor analógico del LM35
  temperature2=((value*5.0)/1023)*100; // Conversión de voltaje a °C

  sensors_event_t event;
  bmp.getEvent(&event);
  if (event.pressure) {
    bmp.getTemperature(&temperature);  // Lee temperatura desde el BMP180
  }

  //Imprimir datos en formato que Excel los lea
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(temperature2);
  Serial.println();
  delay(2000);

}
