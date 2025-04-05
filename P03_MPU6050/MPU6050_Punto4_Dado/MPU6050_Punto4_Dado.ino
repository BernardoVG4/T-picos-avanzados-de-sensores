// Librerías necesarias para el sensor MPU6050 y los LEDs NeoPixel
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

// Configuración de la tira LED
#define PIN        6   // Pin de datos de los LEDs
#define NUM_LEDS   16  // Total de LEDs

// Objeto para controlar los LEDs
Adafruit_NeoPixel matriz(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Objeto para el sensor MPU6050
Adafruit_MPU6050 mpu;

void setup() {
  // Inicializa los LEDs
  matriz.begin();
  matriz.show(); // Inicializa los LEDs en apagado
  matriz.setBrightness(255);  // Ajusta brillo al 100%

  // Inicializa comunicación serial
  Serial.begin(115200);
  while (!Serial);

  // Inicializa I2C y MPU6050
  if (!mpu.begin()) {
    Serial.println("No se encontró el MPU6050. Verifica la conexión.");
    while (1);
  }
  Serial.println("MPU6050 listo");

  // Configuración del sensor
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  // Obtiene los datos del sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Detecta dirección de inclinación y enciende color correspondiente
  if (a.acceleration.z>6 && a.acceleration.y<4 &&  a.acceleration.x<4){
    Color('R');
  }
  else if (a.acceleration.z<-6 && a.acceleration.y<4 &&  a.acceleration.x<4){
    Color('B');
  }
  else if ((a.acceleration.x>6 && a.acceleration.z<4 &&  a.acceleration.y<4)){
    Color('G');
  }
  else if ((a.acceleration.x<-6 && a.acceleration.z<4 &&  a.acceleration.y<4)){
    Color('Y');
  }
  else if ((a.acceleration.y>6 && a.acceleration.z<4 &&  a.acceleration.x<4)){
    Color('M');
  }
  else if ((a.acceleration.y<-6 && a.acceleration.z<4 &&  a.acceleration.x<4)){
    Color('C');
  }
}

// Función para encender la matriz de LEDs con un color 
void Color(char a){
  switch(a){
    case 'R':  //Rojo
      matriz.fill(matriz.Color(255, 0, 0)); 
      matriz.show();
      break;
    case 'G': //Verde
      matriz.fill(matriz.Color(0, 255, 0)); 
      matriz.show();
      break;
    case 'B': //Azul
      matriz.fill(matriz.Color(0, 0, 255)); 
      matriz.show();
      break;
    case 'W':  //Blanco 
      matriz.fill(matriz.Color(255, 255, 255)); 
      matriz.show();
      break;
    case 'Y':  //Amarillo 
      matriz.fill(matriz.Color(255, 255, 0)); 
      matriz.show();
      break;
    case 'C':  //Cian 
      matriz.fill(matriz.Color(0, 255, 255)); 
      matriz.show();
      break;
    case 'M':  //Morado 
      matriz.fill(matriz.Color(255, 0, 255)); 
      matriz.show();
      break;
  }
}
