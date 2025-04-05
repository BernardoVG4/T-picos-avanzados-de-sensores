//Inclusión de librerías
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

const int boton = 2;  // Pin del botón
const int Led = 13;   // Pin del LED

Adafruit_MPU6050 mpu;                     // Objeto para el sensor MPU6050
float m = 0.04;                           // Masa del objeto
float Fx = 0, Fy = 0, Fz = 0;             // Variables de fuerza actual 
float Fx_max = 0, Fy_max = 0, Fz_max = 0; // Variables de fuerza máxima

// Flags de control
bool midiendo = false;
bool botonEstadoAnterior = HIGH; 
bool medicion_realizada = false; 

void setup() {
    pinMode(boton, INPUT_PULLUP);  // Configura el botón con resistencia pull-up
    pinMode(Led, OUTPUT);          // LED como salida
    Serial.begin(115200);           // Inicia comunicación serial

    while (!Serial);                // Espera a que el monitor serial esté listo

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

    // Espera a que se presione el botón antes de comenzar
    while (digitalRead(boton) == HIGH); 
    delay(200);
    digitalWrite(Led, HIGH);
    Serial.println("Listo");
}

void loop() {
    // Detección de cambio de estado del botón
    bool botonEstadoActual = digitalRead(boton);
    if (botonEstadoAnterior == HIGH && botonEstadoActual == LOW) { 
        midiendo = !midiendo;  // Cambia el estado de medición
        delay(50);  // Pequeño delay para evitar rebotes
    }
    botonEstadoAnterior = botonEstadoActual; // Guarda el estado actual del botón
    
    //Medición de fuerzas
    if (midiendo) {
        if (!medicion_realizada) {
            Serial.println("Inicio de medición");
            digitalWrite(Led, HIGH);
            Fx_max = Fy_max = Fz_max = 0; // Reinicia máximos
        }
        
        sensors_event_t a, g, t;
        mpu.getEvent(&a, &g, &t);
        
        Fx = a.acceleration.x * m;
        Fy = a.acceleration.y * m;
        Fz = a.acceleration.z * m;
        
        if (abs(Fx) > abs(Fx_max)){
            Fx_max = Fx;
        } 
        if (abs(Fy) > abs(Fy_max)){
            Fy_max = Fy;
        } 
        if (abs(Fz) > abs(Fz_max)){
            Fz_max = Fz;
        } 
        
        Serial.print("Fx: "); Serial.print(Fx);
        Serial.print("\tFy: "); Serial.print(Fy);
        Serial.print("\tFz: "); Serial.println(Fz);

        medicion_realizada = true;  // Se ha hecho al menos una medición
    } 
    //Finalización de medición
    else if (medicion_realizada) {
        Serial.println("Fin de medición");
        digitalWrite(Led, LOW);
        Serial.println("Fuerzas máximas registradas:");
        Serial.print("Fx_max: "); Serial.println(Fx_max);
        Serial.print("Fy_max: "); Serial.println(Fy_max);
        Serial.print("Fz_max: "); Serial.println(Fz_max);
        
        medicion_realizada = false;  // Reinicia la bandera para permitir una nueva medición
        midiendo= false;
    }
}