// Librería para manejar el sensor MPL3115A
#include <Adafruit_MPL3115A2.h>

// Se crea un objeto llamado 'baro' para interactuar con el sensor
Adafruit_MPL3115A2 baro;

void setup() {
  Serial.begin(115200);   // Inicializa la comunicación serial a 115200 baudios
  while(!Serial);          // Espera a que el monitor serial esté listo
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
  // Se obtienen los datos desde el sensor
  double pressure = baro.getPressure();
  double altitude = baro.getAltitude();
  double temperature = baro.getTemperature();

  // Se convierte temperatura de °C a Kelvin
  double temp_abs = temperature + 274.15;
  // Se calcula la densidad del aire usando la fórmula:
  // ρ = P / (R * T)
  double densidad = ((pressure*100) / (287.05 * temp_abs));
  
  //Mostrar datos
  Serial.println("-----------------");
  Serial.print("pressure = "); Serial.print(pressure*100); Serial.println(" Pa");
 // Serial.print("altitude = "); Serial.print(altitude); Serial.println(" m");
  Serial.print("temperature = "); Serial.print(temp_abs); Serial.println(" K");
  Serial.print("densidad = "); Serial.print(densidad); Serial.println(" ");

  delay(250);
}
