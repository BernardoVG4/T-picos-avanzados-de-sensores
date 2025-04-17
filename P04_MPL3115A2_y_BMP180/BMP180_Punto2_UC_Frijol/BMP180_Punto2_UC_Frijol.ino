//Librerias
#include <Wire.h>                    // Comunicación I2C
#include <Adafruit_Sensor.h>         // Librería base de sensores Adafruit
#include <Adafruit_BMP085_U.h>       // Librería específica para BMP180

// Crear objeto BMP180
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

//Variables a usar
float temperature;
float UC;
float Tmax;
float Tmin;
float TB = 10.0; // Para frijol

const int buzzerPin = 6; // Pin del buzzer
unsigned long lastUpdate = 0;
const unsigned long interval = 86400000; // 24h en milisegundos (1 día)

void setup() {
  Serial.begin(9600);             // Inicia la comunicación serial
  pinMode(buzzerPin, OUTPUT);     // Configura el pin del buzzer como salida

  // Inicializa el sensor BMP180
  if (!bmp.begin()) {
    Serial.print("¡No se detecta el BMP180!");
    while (1);
  }

  // Lee temperatura inicial y asigna a Tmin y Tmax
  sensors_event_t event;
  bmp.getEvent(&event);
  if (event.pressure) {
    bmp.getTemperature(&temperature);
    Tmax = temperature;
    Tmin = temperature;
  }
}

void loop() {
  sensors_event_t event;
  bmp.getEvent(&event);
  
  // Lee temperatura actual
  if (event.pressure) {
    bmp.getTemperature(&temperature);     
    Serial.print("Temperatura actual: ");
    Serial.print(temperature);
    Serial.println(" °C");
  }

  // Actualiza temperaturas máximas y mínimas del día
  if (temperature < Tmin) {
    Tmin = temperature;
  }
  if (temperature > Tmax) {
    Tmax = temperature;
  }

  // Muestra los valores máximo y mínimo actuales
  Serial.print("Temperatura max: ");
  Serial.print(Tmax);
  Serial.print(" °C");
  Serial.print("\t");
  Serial.print("Temperatura min: ");
  Serial.print(Tmin);
  Serial.println(" °C");

  //Actualizacion diaria
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    // Cálculo de unidades calor acumuladas (UC)
    UC = ((Tmax + Tmin) / 2) - TB;
    Serial.print("UC: ");
    Serial.println(UC);

    // Alarma sonora: el buzzer suena 500 ms
    tone(buzzerPin, 2000);
    delay(500);
    noTone(buzzerPin);

    // Evaluación de la etapa fenológica según UC
    if (UC <= 12.89 && UC > 0) {
      Serial.println("Emergencia");
    } else if (UC <= 64.41 && UC > 12.89) {
      Serial.println("Formación de guías");
    } else if (UC <= 91.33 && UC > 64.41) {
      Serial.println("Floración");
    } else if (UC < 99.31 && UC > 91.33) {
      Serial.println("Formación de vaina");
    } else if (UC < 112.77 && UC > 99.31) {
      Serial.println("Llenado de vainas");
    } else if (UC <= 153.24 && UC > 112.77) {
      Serial.println("Maduración");
    } else {
      Serial.println("UC fuera de rango");
    }
  }
  delay(500); 
}
