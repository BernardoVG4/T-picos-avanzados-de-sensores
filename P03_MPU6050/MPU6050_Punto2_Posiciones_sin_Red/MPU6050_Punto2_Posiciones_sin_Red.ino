// Librerías necesarias
#include <Wire.h>
#include <MPU6050_tockn.h>

// Crear objeto MPU6050 usando la comunicación I2C
MPU6050 mpu(Wire);

// Variables para almacenar los valores de inclinación (ángulos) 
float pitch;
float roll;

void setup() {
    Serial.begin(9600);         // Inicia el monitor serial
    Wire.begin();               // Inicia el bus I2C
    mpu.begin();                // Inicializa el MPU6050
    mpu.calcGyroOffsets(true);  // Calibra el sensor al inicio
}

void loop() {
  mpu.update();            // Actualiza los valores del sensor

  // Obtener ángulos de inclinación
  pitch=mpu.getAngleX();
  roll=mpu.getAccAngleY();

  // Condiciones para obtener posicion del sensor
  if (pitch >=-5 && pitch<=5 && roll>=-5 && roll<=5 ){
    Serial.println("Sensor quieto");
  }
  else if (roll<=-20 && pitch>=-5 && pitch<=5 ){
    Serial.println("Sensor inclinado hacia enfrente");
  }
  else if ( pitch<=-20 && roll>=-5 && roll<=5 ){
    Serial.println("Sensor inclinado hacia derecha");
  }
  else if ( pitch>=20 && roll>=-5 && roll<=5 ){
    Serial.println("Sensor inclinado hacia izquierda");
  }
}
