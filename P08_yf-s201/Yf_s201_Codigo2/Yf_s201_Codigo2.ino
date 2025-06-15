// Variables utilizadas en la interrupción
volatile boolean first;
volatile boolean triggered;
volatile unsigned long overflowCount;
volatile unsigned long startTime;
volatile unsigned long finishTime;

// Interrupción externa al detectar un flanco de subida (rising edge)
void isr () 
{
  unsigned int counter = TCNT1;  // Guarda rápidamente el valor actual del timer

  // Si ya se ha medido un ciclo completo, no hace nada
  if (triggered)
    return;
  // Si es el primer flanco detectado
  if (first)
    {
    startTime = (overflowCount << 16) + counter;
    first = false;
    return;  
    }
  // Segundo flanco detectado, se calcula el tiempo final  
  finishTime = (overflowCount << 16) + counter;
  triggered = true;
  detachInterrupt(0);   
}  

// Rutina de interrupción por desbordamiento del Timer1
ISR (TIMER1_OVF_vect) 
{
  overflowCount++;
} 

// Prepara el sistema para una nueva medición por interrupciones
void prepareForInterrupts ()
  {
  EIFR = _BV (INTF0);  
  first = true;
  triggered = false;  
  attachInterrupt(0, isr, RISING);     
  }  
  

void setup () {
  Serial.begin(115200);       
  Serial.println("Frequency Counter");
  
  // Reinicia Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  // Habilita la interrupción por desbordamiento del Timer1
  TIMSK1 = _BV (TOIE1);   
  TCNT1 = 0;     
  TCCR1B =  _BV (CS20);  

  // Prepara las interrupciones externas
  prepareForInterrupts ();   
  
}

void loop () {
  // Si aún no se ha completado una medición, no hace nada
  if (!triggered)
    return;
 
  // Calcula el tiempo transcurrido entre dos flancos
  unsigned long elapsedTime = finishTime - startTime;
  // Calcula la frecuencia: cada tick son 62.5 nanosegundos (1 / 16 MHz)
  float freq = 1.0 / ((float (elapsedTime) * 62.5e-9));  

  // Calcula el caudal en litros por minuto usando la fórmula del sensor
  float litros_m = freq / 7.5; 

  // Muestra los resultados por el monitor serial
  Serial.print ("litros/min: ");
  Serial.print (litros_m);
  Serial.print ("   ");

  Serial.print ("Frequency: ");
  Serial.print (freq);
  Serial.println (" Hz. ");
  
  // Prepara todo para una nueva medición
  prepareForInterrupts ();   
}   