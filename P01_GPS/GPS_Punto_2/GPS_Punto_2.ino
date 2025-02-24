#include <SoftwareSerial.h> // Librería para comunicación serie por software
#include <SD.h>             // Librería para manejo de tarjeta SD

// Definición de pines para el GPS y la tarjeta SD
#define GPS_RX 2  // Pin RX del GPS
#define GPS_TX 3  // Pin TX del GPS
#define SD_CS 4   // Pin Chip Select (CS) para la tarjeta SD

// Configuración de la comunicación serial con el GPS
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// Variables para almacenamiento de datos del GPS
char buffer[100];             // Buffer para almacenar la línea de datos NMEA
byte bufferIndex = 0;         // Índice de la posición actual en el buffer
unsigned long lastSave = 0;   // Tiempo en el que se realizó el último guardado
int fileHour = -1;            // Variable para almacenar la hora del archivo
int saveCount = 0;            // Contador de veces que se ha guardado en la SD

void setup() {
  Serial.begin(9600);       // Inicialización del puerto serie
  gpsSerial.begin(9600);    // Inicialización del GPS
  
  // Verificación de la tarjeta SD
  if (!SD.begin(SD_CS)) {
    Serial.println("Error en tarjeta SD");  //Mensaje de error si la tarjeta no se inicializa
    return;
  }
  
  // Esperar primer fix del GPS
  while(!obtenerDatosGPS());
}



void loop() {
  // Verifica si hay nuevos datos del GPS
  if(obtenerDatosGPS()) {
    unsigned long currentTime = millis();  // Obtiene el tiempo actual del sistema
    // Guarda los datos cada 10 minutos (600000 ms), hasta 30 veces (5 horas)
    if(currentTime - lastSave >= 600000 && saveCount < 30) {
      guardarEnSD();
      lastSave = currentTime;  // Actualiza el tiempo del último guardado
      saveCount++;             // Incrementa el contador de guardados
    }
    // Si se han realizado 30 guardados, detiene el programa
    if(saveCount >= 30) {
      while(true); // Bucle infinito para detener ejecución
    }
  }
}

// Función para obtener datos del GPS
bool obtenerDatosGPS() {
  static bool nuevaLinea = false; // Indica si se ha iniciado una nueva línea de datos
  
  while(gpsSerial.available()) { // Verifica si hay datos disponibles en el GPS
    char c = gpsSerial.read();   // Lee un carácter del GPS
    
    if(c == '$') {              // Detecta inicio de una nueva línea NMEA
      bufferIndex = 0;          // Reinicia el índice del buffer
      nuevaLinea = true;        // Indica que se ha iniciado una nueva línea
    }
    
    if(nuevaLinea) {                
      if(c == '\n') {                 // Si se encuentra un salto de línea, finaliza la lectura
        buffer[bufferIndex] = '\0';   // Termina la cadena
        nuevaLinea = false;           // Indica que la línea ha finalizado
        return true;                  // Indica que hay una nueva línea disponible
      }
       
      buffer[bufferIndex++] = c;           // Almacena el carácter en el buffer
      if(bufferIndex >= sizeof(buffer)) {  // Previene desbordamiento del buffer
        bufferIndex = 0;                   // Reinicia el índice del buffer
        nuevaLinea = false;                // Descarta la línea si supera el tamaño del buffer
      }
    }
  }
  return false;                             // No hay nueva línea disponible
}

// Función para guardar los datos en la tarjeta SD
void guardarEnSD() {
  char hora[3];
  int horaActual = extraerHoraGPS(); // Obtiene la hora del GPS
  
  // Si la hora ha cambiado, actualiza el archivo
  if(horaActual != fileHour) {
    fileHour = horaActual;            // Actualiza la variable con la nueva hora
    sprintf(hora, "%02d", fileHour);  // Convierte la hora en string de dos dígitos
  }
  // Genera el nombre del archivo 
  String nombreArchivo = "GPS_" + String(hora) + ".txt";
  File archivo = SD.open(nombreArchivo, FILE_WRITE); // Abre el archivo en modo escritura
  
  if(archivo) {               // Si el archivo se abrió correctamente
    archivo.println(buffer);  // Escribe la línea de datos del GPS en el archivo
    archivo.close();          // Cierra el archivo
  }
}


// Función para extraer la hora de la cadena de datos del GPS
int extraerHoraGPS() { 
  char* token = strtok(buffer, ",");  // Divide la cadena en partes separadas por ","
  for(int i = 0; i < 1; i++) {       // Hora está en segundo campo
    token = strtok(NULL, ",");      // Se avanza al siguiente campo
  }
  
  if(token && strlen(token) >= 6) {  // Verifica que el token tenga al menos 6 caracteres
    char hora[3];
    strncpy(hora, token, 2);   // Extrae los dos primeros caracteres (hora)
    hora[2] = '\0';            // Termina la cadena con un carácter nulo
    return atoi(hora);         // Convierte la hora a entero
  }
  return -1;                   
}