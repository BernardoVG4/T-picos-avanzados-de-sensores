//Librerias
#include <Wire.h>                    // Comunicación I2C
#include <Adafruit_Sensor.h>         // Librería base de sensores Adafruit
#include <Adafruit_BMP085_U.h>       // Librería específica para BMP180

// Crear objeto BMP180
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

//Variables a usar
float Presion_ambiente;
float Presion_Persona;
float Presion_Bicicleta;
float Presion_Moto;
float Presion_Carro;
float Presion_Trailer;
float Presion_Limite;

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial

  // Inicializa el sensor BMP180
  if (!bmp.begin()) {
    Serial.print("¡No se detecta el BMP180!");
    while (1);
  }

  sensors_event_t event;
  bmp.getEvent(&event);
  
  //Establecer el valor de la presion ambiente
  if (event.pressure) {
    Serial.print("Presión: ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    Presion_ambiente=event.pressure+1;
    delay(1000);
    Serial.println("Listo para medir");
  }

  //Definir presiones para los vehiculos
  Presion_Persona=Presion_ambiente+35.0;
  Presion_Bicicleta=Presion_ambiente+55.5;
  Presion_Moto=Presion_ambiente+75.5;
  Presion_Carro=Presion_ambiente+95.5;
  Presion_Trailer=Presion_ambiente+115.5;
  Presion_Limite=Presion_ambiente+135.5;

}

void loop() {
  sensors_event_t event;
  bmp.getEvent(&event);

  //Leer presion
  if (event.pressure) {
    Serial.println(event.pressure);
  }

  //Determinar tipo de vehiculo y mostrar en el monitor serial
  if(event.pressure<Presion_Persona){
    Serial.println("Nada pasando");
  }
  if(event.pressure>=Presion_Persona && event.pressure<Presion_Bicicleta){
    Serial.println("Persona pasando");
  }
  else if(event.pressure>=Presion_Bicicleta && event.pressure<Presion_Moto){
    Serial.println("Bicicleta pasando");
  }
  else if(event.pressure>=Presion_Moto && event.pressure<Presion_Carro){
    Serial.println("Motocicleta pasando");
  }
  else if(event.pressure>=Presion_Carro && event.pressure<Presion_Trailer){
    Serial.println("Carro pasando");
  }
  else if(event.pressure>=Presion_Trailer && event.pressure<Presion_Limite){
    Serial.println("Trailer pasando");
  }
  else if(event.pressure>=Presion_Limite){
    Serial.println("Vehiculo muy pesado pasando");
  }
}