#define MQ_PIN A0      // Pin analógico conectado al sensor
#define RL 1.0        // Resistencia de carga en kΩ
#define R0_CLEAN_AIR 1.88 // valor del sensor en aire limpio despues de 12hrs de funcionaminto

//Funcion para calcular la concentracion de un gas en ppm (partes por millon)
float LeerConcentracion(const String Nom_Gas) { 
  int sensorValue = analogRead(MQ_PIN); // muestreo de la senal del sensor
  float VRL = sensorValue * (5.0 / 1023.0); //voltaje en el sensor
  float Rs = ((5.0 - (VRL-1)) * RL); // Calcula la resistencia del sensor
  float ratio = Rs / R0_CLEAN_AIR; // relacion de resistencias
  float concentration; // variable que almacena el resultado

  // Aplicamos las ecuaciónes correspondientes
    if (Nom_Gas == "alcohol")
       concentration = 4579.3 * pow(ratio,-2.973); // alcohol
    else if (Nom_Gas == "propano")
       concentration = 714.34 * pow(ratio,-2.034); //propano
    else if (Nom_Gas == "H2")
       concentration = 1032.3 * pow(ratio,-2.074); //hidrogeno H2
    else if (Nom_Gas == "CH4")
       concentration = 4499.9 * pow(ratio,-2.653); //metano CH4
    else if (Nom_Gas == "CO")
       concentration = 46720 * pow(ratio,-3.314); // monoxido de carbono CO
    else if (Nom_Gas == "LPG")
       concentration = 687.71 * pow(ratio,-2.03); // LPG

  return concentration;
}

void setup() {
  Serial.begin(115200); // Inicia comunicación serie
  // Calentamiento inicial del sensor 
  Serial.println("Calentando el sensor...");
  delay(300);
}

void loop() {
  //  ***Seleccionar funcion segun el gas a medir***
  //leerAlcohol();
  //leerHidrogeno();
  //leerMetano();
  //leerPropano();
  //leerMonoxido_de_Carbono();
  leerLPG();//gas licuado
    
  delay(100); // Espera entre lecturas
}


// ***Funciones para medir cada gas***
void leerAlcohol(){
  float alcohol = LeerConcentracion("alcohol");
  // convertimos ppm a porcentaje
  float porcentaje = map(alcohol,0,10000,0,100); // 10 000 ppm --> 1%
  Serial.print("Concentración de alcohol: ");
  Serial.print(alcohol);
  Serial.print(" ppm");
  
  Serial.print("  porcentaje de alcohol: ");
  Serial.print(porcentaje);
  Serial.println(" %");
}

void leerHidrogeno(){
  float Hidrogeno = LeerConcentracion("H2");
  // convertimos ppm a porcentaje
  float porcentaje = map(Hidrogeno,0,10000,0,100); // 10 000 ppm --> 1%
  Serial.print("Concentración de Hidrogeno: ");
  Serial.print(Hidrogeno);
  Serial.print(" ppm");
  
  Serial.print("  porcentaje de Hidrogeno: ");
  Serial.print(porcentaje);
  Serial.println(" %");
}

void leerMetano(){
  float Metano = LeerConcentracion("CH4");
  // convertimos ppm a porcentaje
  float porcentaje = map(Metano,0,10000,0,100); // 10 000 ppm --> 1%
  Serial.print("Concentración de Metano: ");
  Serial.print(Metano);
  Serial.print(" ppm");
  
  Serial.print("  porcentaje de Metano: ");
  Serial.print(porcentaje);
  Serial.println(" %");
}

void leerPropano(){
  float Propano = LeerConcentracion("propano");
  // convertimos ppm a porcentaje
  float porcentaje = map(Propano,0,10000,0,100); // 10 000 ppm --> 1%
  Serial.print("Concentración de Propano: ");
  Serial.print(Propano);
  Serial.print(" ppm");
  
  Serial.print("  porcentaje de Propano: ");
  Serial.print(porcentaje);
  Serial.println(" %");
}

void leerMonoxido_de_Carbono(){
  float co = LeerConcentracion("CO");
  // convertimos ppm a porcentaje
  float porcentaje = map(co,0,10000,0,100); // 10 000 ppm --> 1%
  Serial.print("Concentración de Monoxido de Carbono: ");
  Serial.print(co);
  Serial.print(" ppm");
  
  Serial.print("  porcentaje de Monoxido de Carbono: ");
  Serial.print(porcentaje);
  Serial.println(" %");
}

void leerLPG(){
  float lpg = LeerConcentracion("LPG");
  // convertimos ppm a porcentaje
  float porcentaje = map(lpg,0,10000,0,100); // 10 000 ppm --> 1%
  Serial.print("Concentración de LPG: ");
  Serial.print(lpg);
  Serial.print(" ppm");
  
  Serial.print("  porcentaje de LPG: ");
  Serial.print(porcentaje);
  Serial.println(" %");
}
