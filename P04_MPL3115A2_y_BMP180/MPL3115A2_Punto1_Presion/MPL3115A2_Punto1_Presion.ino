// Librería para manejar el sensor MPL3115A2
#include <Adafruit_MPL3115A2.h>

// Se crea un objeto llamado 'baro' para interactuar con el sensor
Adafruit_MPL3115A2 baro;

void setup() {
  Serial.begin(115200);  // Inicializa la comunicación serial a 115200 baudios
  while(!Serial);        // Espera a que el monitor serial esté listo
  Serial.println("Adafruit_MPL3115A2 test!");

  // Intenta inicializar el sensor
  if (!baro.begin()) {
    Serial.println("Could not find sensor. Check wiring.");
    while(1);
  }

  // Configura la presión a nivel del mar en hectopascales (hPa)
  // Esto es importante para calcular con precisión la altitud
  // El valor estándar es 1013.26 hPa
  baro.setSeaPressure(1013.26);

}

void loop() {
  // Se obtienen las lecturas del sensor
  float pressure = baro.getPressure();
  float altitude = baro.getAltitude();
  float temperature = baro.getTemperature();
  
  // Imprime los datos por el monitor serial
  //Serial.println("-----------------");
  Serial.print("pressure = "); Serial.print(pressure); Serial.println(" hPa");
  //Serial.print("altitude = "); Serial.print(altitude); Serial.println(" m");
  //Serial.print(" temperature = "); Serial.print(temperature); Serial.println(" C");

  delay(50);

}
