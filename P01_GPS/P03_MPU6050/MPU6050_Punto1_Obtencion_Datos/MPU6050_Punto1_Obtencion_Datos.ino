//Librerias necesarias
#include <Wire.h>
#include <MPU6050_tockn.h>

// Crear objeto mpu usando la clase MPU6050, comunicándose por I2C
MPU6050 mpu(Wire);

// Variables para almacenar los valores de inclinación (ángulos)
int pitchG;
int pitchD;
int rollG;
int rollD;

void setup() {
    Serial.begin(9600);          // Inicia comunicación serial a 9600 baudios
    Wire.begin();                // Inicia comunicación I2C
    mpu.begin();                 // Inicializa el sensor MPU6050
    mpu.calcGyroOffsets(true);   // Calibra el sensor al inicio
}

void loop() {
    mpu.update();                 // Actualiza los valores del sensor

    // Obtener los ángulos actuales del sensor
    pitchG = mpu.getAngleX();
    rollG = mpu.getAngleY();
    
    // Mapear los ángulos (-180 a 180) al rango 0 a 1023
    pitchD = constrain(map(pitchG, -180, 180, 0, 1023), 0, 1023);
    rollD = constrain(map(rollG, -180, 180, 0, 1023), 0, 1023);
    
    // Enviar los valores por Serial en un formato delimitado por < y > y separados por coma
    Serial.print('<');
    Serial.print(pitchD / 1000);  // Imprime los miles
    Serial.print((pitchD / 100) % 10);  // Centenas
    Serial.print((pitchD / 10) % 10);  // Decenas
    Serial.print(pitchD % 10);  // Unidades

    Serial.print(',');

    Serial.print(rollD / 1000);  // Imprime los miles
    Serial.print((rollD / 100) % 10);  // Centenas
    Serial.print((rollD / 10) % 10);  // Decenas
    Serial.print(rollD % 10);  // Unidades

    Serial.println('>');

    delay(50);
} 