// Se incluye la librería SoftwareSerial para comunicar con el GPS
#include <SoftwareSerial.h>

// Se crea un objeto de la clase SoftwareSerial con los pines 10 (RX) y 11 (TX)
SoftwareSerial GPS(10,11); 

char c=' ';               // Variable para almacenar los caracteres recibidos del GPS

void setup() {
  Serial.begin(115200);   // Se inicia la comunicación serie con el monitor serial a 115200 baudios
  GPS.begin(9600);        // Se inicia la comunicación con el módulo GPS a 9600 baudios
}

void loop() {
  if (GPS.available()){    // Se verifica si hay datos disponibles en el GPS
    c=GPS.read();          // Se lee el GPS
    Serial.print(c);       // Se imprime en el monitor serie
  }
}
