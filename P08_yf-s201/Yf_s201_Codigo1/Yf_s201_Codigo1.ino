#include <Wire.h>              // Librería para comunicación 
#include <Adafruit_GFX.h>      // Librería gráfica base de Adafruit
#include <Adafruit_SSD1306.h>  // Librería para manejar pantallas OLED SSD1306

//Dimensiones de pantalla
#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64

// Se crea un objeto llamado oled con las dimensiones y el bus I2C
Adafruit_SSD1306 oled(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

//Variables auxiliares
volatile int pulsos;                // Contador de pulsos del sensor
float caudal_Lmin;                  // Caudal instantáneo en litros por minuto
float litrosTotales;                // Litros acumulado
unsigned long tiempoAnterior = 0;   // Para llevar control del tiempo de muestreo

// Función de interrupción
void contadorPulsos() {
  pulsos++;
}

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);                                           // Pin 2 como entrada con resistencia pull-up interna
  attachInterrupt(digitalPinToInterrupt(2), contadorPulsos, RISING);  // Interrupción en flanco de subida    
 
  // Inicializa la pantalla OLED en la dirección I2C 0x3C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Dirección I2C usualmente 0x3C
    Serial.println(F("Fallo al iniciar OLED"));
    while (true);
  }
 
  // Mensaje inicial en la pantalla OLED
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println("Sensor de Caudal");
  oled.display();
  delay(1000);
}

void loop() {
  unsigned long tiempoActual = millis();
  
  // Se actualiza cada 100 ms (0.1 segundos)
  if (tiempoActual - tiempoAnterior >= 100) {
    detachInterrupt(digitalPinToInterrupt(2));  // Se desactiva la interrupción mientras se procesa

    float frecuencia = pulsos;
    caudal_Lmin = frecuencia / 7.5;       // Fórmula para convertir frecuencia a L/min
    float litros = caudal_Lmin / 60.0;    // Convierte de L/min a litros por cada segundo
    litrosTotales += litros;               // Acumula litros totales
    
    // Muestra los valores por el monitor serial
    Serial.print("Caudal: ");
    Serial.print(caudal_Lmin);
    Serial.print(" L/min\tTotal: ");
    Serial.print(litrosTotales);
    Serial.println(" L");

     // Muestra los valores en la pantalla OLED
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println("Sensor de Caudal");

    oled.setCursor(0, 20);
    oled.print("Caudal: ");
    oled.print(caudal_Lmin, 1);

    oled.println(" L/min");

    oled.setCursor(0, 40);
    oled.print("Total: ");
    oled.print(litrosTotales, 2);
    oled.println(" L");

    oled.display();

    pulsos = 0;                     // Reinicia el contador de pulsos
    tiempoAnterior = tiempoActual;  // Actualiza el tiempo de referencia

    attachInterrupt(digitalPinToInterrupt(2), contadorPulsos, RISING); // Reactiva la interrupción
  } 
}
