#include <SoftwareSerial.h>

SoftwareSerial miBT(11,10);

//Variable para leer el valor enviado desde la APP
char DATO = 0;
//PIN para Relé de cambio de dirección (Simulación LED Rojo)
int LEDROJO = 2;
//PIN para intensidad del motor del patinete (Simulación LED Verde)
int LEDVERDE = 3;
//Variables para niveles de intensidad
int INTENSIDAD_LOW = 75;
int INTENSIDAD_MEDIUM = 100;
int INTENSIDAD_HIGH = 250;
int INTENSIDAD_PARADA = 0;


void setup() {
  //Inicializamos el puerto serie de Arduino para usar el Serial Monitor
  Serial.begin(9600);
  Serial.println("Ready");
  //Inicializamos la comunicación en serie del dispositivo Bluetooth
  miBT.begin(9600);
  pinMode(LEDROJO, OUTPUT);
  pinMode(LEDVERDE, OUTPUT);
}

void loop() {
  // Si recibimos algún dato desde el módulo Bluetooth escribimos en el monitor de Serie de Arduino
  if (miBT.available()){
    DATO = miBT.read();
    Serial.write(DATO);
    //Control de relé para cambio de dirección
    if(DATO == 'D'){
      Serial.write('0');
      digitalWrite(LEDROJO, !digitalRead(LEDROJO));
    }
    //Intensidad baja
    if(DATO == 'L'){
      Serial.write('1');
      analogWrite(LEDVERDE, INTENSIDAD_LOW);
    }
    //Intensidad media
    if(DATO == 'M'){
      Serial.write('2');
      analogWrite(LEDVERDE, INTENSIDAD_MEDIUM);
    }
    //Intensidad alta
    if(DATO == 'H'){
      Serial.write('3');
      analogWrite(LEDVERDE, INTENSIDAD_HIGH);
    }
    //Detener carrito
    if(DATO == 'S'){
      Serial.write('3');
      analogWrite(LEDVERDE, INTENSIDAD_PARADA);
    }
  }

  // Si recibimos algún dato desde el Serie de Arduino se envía al dispositibo Bluetooth
  if (Serial.available()){
    miBT.write(Serial.read());
  }
}
