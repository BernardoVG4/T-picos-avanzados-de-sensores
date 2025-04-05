//Librerias necesarias
#include <Wire.h>
#include <MPU6050_tockn.h>

// Crear objeto mpu usando la clase MPU6050, comunicándose por I2C
MPU6050 mpu(Wire);

// Variables para almacenar los valores de inclinación (ángulos) y tiempo
int pitchG;
int pitchD;
int rollG;
int rollD;
unsigned long past_time=0;

//Variables red neuronal, pesos y bias
const int node=2;
const int input=2;
const int outputs=2;
float P[input][1];
float W1[node][input]={{5.6622,-6.4035},
                      {-7.6346,2.3104}};

float b1[node][1]={{0.69567},
                  {3.4055}};

float a1[node][1];

float W2[outputs][node]={{4.6349,4.9014},
                      {-6.2297,2.879}};

float b2[outputs][1]={{-3.8285},
                  {1.1661}};

float a2[outputs][1];

float aux=0.0;     // Variable auxiliar para cálculos

// Rango de entrada para normalizar los datos
int maxValue=1023;
int minValue=0;

void setup() {
    Serial.begin(9600);         // Inicia el monitor serial
    Wire.begin();               // Inicia el bus I2C
    mpu.begin();                // Inicializa el MPU6050
    mpu.calcGyroOffsets(true);  // Calibra el sensor al inicio
}

void loop() {
  mpu.update();  // Actualiza los valores del sensor
    
  // Obtener ángulos de inclinación
  pitchG = mpu.getAngleX();
  rollG = mpu.getAngleY();

  // Convertir ángulos de -180 a 180 en un rango de 0 a 1023
  pitchD = constrain(map(pitchG, -180, 180, 0, 1023), 0, 1023);
  rollD = constrain(map(rollG, -180, 180, 0, 1023), 0, 1023);

  //Primera Capa Entrada
  P[0][0]=dataNormalized((float) pitchD,minValue,maxValue);
  P[1][0]=dataNormalized((float) rollD,minValue,maxValue);
    
  //Segunda Capa//
  for(int i=0;i<node;i++){
    aux=0.0;
    for(int j=0;j<input;j++){
      aux=aux+W1[i][j]*P[j][0];
    }
    a1[i][0]=tansig(aux+b1[i][0]);
  }

  //Tercera Capa//
    for(int i=0;i<outputs;i++){
      aux=0.0;
      for(int j=0;j<node;j++){
        aux=aux+W2[i][j]*a1[j][0];
      }
      a2[i][0]=tansig(aux+b2[i][0]);
    }
    
    if(a2[0][0]<=0 && a2[1][0]<=0){
      Serial.println("Izquierda");
    }
    else if(a2[0][0]<=0 && a2[1][0]>0){
      Serial.println("Derecha");
    }
    else if(a2[0][0]>0 && a2[1][0]<=0){
      Serial.println("Adelante");
    }
    else{
      Serial.println("Quieto");    
    }
    delay(50);

}

// Función de activación tansig (tangente hiperbólica)
float tansig(float n)
{
  float a=exp(n);
  float b=exp(-n);
  return (a-b)/(a+b);
}

// Función para normalizar datos al rango
float dataNormalized(int inputData, int minData, int maxData)
{
  float valueNorm;
  valueNorm=2.0*(inputData-minData)/(maxData-minData)-1.0;
  return valueNorm;
}
