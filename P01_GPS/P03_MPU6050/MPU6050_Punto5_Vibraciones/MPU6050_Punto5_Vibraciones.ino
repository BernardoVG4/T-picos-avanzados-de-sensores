// Librerías necesarias para el sensor MPU6050
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Declaración del objeto del sensor y variables globales
Adafruit_MPU6050 mpu;
float umbralx=0, umbraly=0, umbralz=0;
float Ax=0, Ay=0, Az=0;
float tol=4.5; // Ajusta este valor según la sensibilidad deseada

// Configuración inicial
void setup() {
    Serial.begin(115200);
    while (!Serial);

    // Inicializa el MPU6050
    if (!mpu.begin()) {
        Serial.println("No se encontró el MPU6050. Verifica la conexión.");
        while (1);
    }
    Serial.println("MPU6050 listo");

    // Configuración del sensor
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    // Tomar varias mediciones para establecer umbrales promedio
    float sumaX = 0, sumaY = 0, sumaZ = 0;
    int muestras = 100;

    for (int i = 0; i < muestras; i++) {
        sensors_event_t a, g, t;
        mpu.getEvent(&a, &g, &t);
        sumaX += a.acceleration.x;
        sumaY += a.acceleration.y;
        sumaZ += a.acceleration.z;
        delay(10); // Pequeño retraso para evitar lecturas erráticas
    }

    // Promedio de valores en reposo
    umbralx = sumaX / muestras;
    umbraly = sumaY / muestras;
    umbralz = sumaZ / muestras;
    
    // Imprime los umbrales por puerto serie
    Serial.println("Umbrales establecidos:");
    Serial.print("X: "); Serial.println(umbralx);
    Serial.print("Y: "); Serial.println(umbraly);
    Serial.print("Z: "); Serial.println(umbralz);
}

void loop() {
    // Obtiene los datos actuales del sensor
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    
    Ax = a.acceleration.x;
    Ay = a.acceleration.y;
    Az = a.acceleration.z;

    bool vibracion = false;

    // Comprueba si hay una diferencia mayor al umbral + tolerancia
    if (Ax >= umbralx + tol || Ax <= umbralx - tol) {
        vibracion = true;
    }
    if (Ay >= umbraly + tol || Ay <= umbraly - tol) {
        vibracion = true;
    }
    if (Az >= umbralz + tol || Az <= umbralz - tol) {
        vibracion = true;
    }

    // Si se detecta vibración, muestra mensaje
    if (vibracion) {
        Serial.println("Vibración detectada");
        delay(1000); 
    } else {
        Serial.println("Vibración NO detectada");
    }

    delay(10); 
}
