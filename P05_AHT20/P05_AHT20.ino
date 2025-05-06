#include <Adafruit_AHTX0.h>  // Librería para usar el sensor AHTX0 (temperatura y humedad)
Adafruit_AHTX0 aht;          // Se crea un objeto para manejar el sensor AHTX0


//Pines para los LM35
#define LM35_1 A0
#define LM35_2 A1
#define LM35_3 A2

// Variables para almacenar los valores de los sensores
float temperature1=0;
float value1;
float temperature2=0;
float value2;
float temperature3=0;
float value3;

void setup() {
  Serial.begin(9600);                  // Inicia la comunicación serial a 9600 baudios
  while(!Serial) delay(10);            // Espera a que se abra el monitor serial

  Serial.println("Iniciando sensor");

  // Inicializa el sensor AHTX0 y verifica si está conectado correctamente
  if(!aht.begin()){
    Serial.println("No se encontro el sensor");
    while(1) delay(10);
  }
  Serial.println("Sensor encontrado");  // Confirmación de que el sensor fue detectado

}

void loop() {
  sensors_event_t humidity, temp;   // Estructuras para almacenar los datos del sensor AHTX0
  aht.getEvent(&humidity, &temp);   // Obtiene los valores de temperatura y humedad del AHTX0

  // Lectura y conversión de los sensores LM35
  value1=analogRead(LM35_1);              
  temperature1=((value1*5.0)/1023)*100; 
  value2=analogRead(LM35_2);             
  temperature2=((value2*5.0)/1023)*100; 
  value3=analogRead(LM35_3);              
  temperature3=((value3*5.0)/1023)*100; 

  //Imprimir datos en formato que Excel los lea
  Serial.print(temperature1);
  Serial.print(",");
  Serial.print(temperature2);
  Serial.print(",");
  Serial.print(temperature3);
  Serial.print(",");
  Serial.print(temp.temperature);
  Serial.print(",");
  Serial.println(humidity.relative_humidity);
  
  delay(1000);
}

