#define MQ_PIN A0                // Pin analógico del sensor
#define RL 1.0                   // Resistencia de carga en kΩ
#define CALIBRATION_SAMPLES 500  // Número de muestras para calibrar
#define CALIBRATION_DELAY 50     // Intervalo entre lecturas (ms)

// Función para calcular R0 (resistencia del sensor en aire limpio)
float calculateR0() {
  float rsSum = 0;          // Suma acumulada de las resistencias RS
  
  for(int i = 0; i < CALIBRATION_SAMPLES; i++) {
    int raw = analogRead(MQ_PIN); 
    float vrl = raw * (5.0 / 1023.0);       // Conversión del valor leído a voltaje              
    float rs = ((5.0 - vrl) / vrl) * RL;    // Cálculo de la resistencia del sensor (RS)
    rsSum += rs;                            // Acumulación de RS para el promedio
    Serial.println(i);
    delay(CALIBRATION_DELAY);
  }
  
  float rsAvg = rsSum / CALIBRATION_SAMPLES;          // Promedio de RS
  float r0 = rsAvg / 9.7;                             // Ajustar este ratio según datasheet
  return r0;                                          // Devuelve el valor de R0
}

void setup() {
  Serial.begin(115200);                                               // Inicializa la comunicación serial
  Serial.println("Calibrando R0 (coloque el sensor en aire limpio)");
  
  // Calentamiento inicial obligatorio
  delay(600);
  
  float r0 = calculateR0();      // Calcula el valor de R0
  
  Serial.print("R0 calibrado: "); // Muestra el valor de R0 obtenido
  Serial.println(r0);
}

void loop() {
  
}