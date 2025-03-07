#include "Adafruit_VL53L0X.h"              // Librería para manejar el sensor VL53L0X
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // Creación del objeto para el sensor

const double alfa = 0.05;       // Coeficiente de filtrado EMA
double distancia,distancia_ant; // Variables para almacenar la distancia actual y la anterior
int distancia_filtrada;         // Variable para la distancia filtrada

void setup() {
  Serial.begin(115200);   // Inicialización del puerto serie
  while (! Serial) {
    delay(1);             // Espera hasta que el puerto serie esté listo
  }
  
  Serial.println("Adafruit VL53L0X test");

  if (!lox.begin()) {                            // Inicialización del sensor VL53L0X
    Serial.println(F("Failed to boot VL53L0X")); // Mensaje de error si falla la inicialización
    while(1);                                     // Bucle infinito en caso de fallo
  }
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure; // Estructura para almacenar la medición
  lox.rangingTest(&measure, false);         // Realiza la medición de distancia
  distancia = (measure.RangeMilliMeter/10); // Convierte la distancia de mm a cm

  distancia_filtrada = ((alfa * distancia) + ((1 - alfa)* distancia_ant));  //Filtro EMA
  Serial.print("Distancia filtrada: "); 
  Serial.println(distancia_filtrada);    // Imprime la distancia filtrada
  distancia_ant = distancia;             // Guarda la distancia actual para la siguiente iteración
}
