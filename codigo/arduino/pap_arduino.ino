#include <SoftwareSerial.h>

SoftwareSerial miBT(11,10);

//Variable para leer el valor enviado desde la APP
char DATO = 0;

//PIN para intensidad del motor del patinete (Simulación LED Verde)
int MOTOR = 3;

//PINs para la gestión de relés
int MAESTRO = 2;
int ESCLAVO_1 = 8;
int ESCLAVO_2 = 9;
int WAITTIME = 500;
bool adelante = true;

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
  pinMode(MOTOR, OUTPUT);

  //Inicializamos los pintes para los relés
  pinMode(MAESTRO, OUTPUT);
  pinMode(ESCLAVO_1, OUTPUT);
  pinMode(ESCLAVO_2, OUTPUT);


  //Logica inversa para desactivar los relés por defecto. High = Desactivado - Low = Activado
  digitalWrite(MAESTRO, HIGH);
  digitalWrite(ESCLAVO_1, HIGH);
  digitalWrite(ESCLAVO_2, HIGH);

}

void loop() {

  controlMotor();
  //controlLed();
  //TEST BORRAR LUEGO
  //delay(10000);
  //DATO = 'D';
}


void controlMotor(){
  // Si recibimos algún dato desde el módulo Bluetooth escribimos en el monitor de Serie de Arduino
  if (miBT.available()){
    DATO = miBT.read();
    Serial.write(DATO);
    //Control de relé para cambio de dirección
    if(DATO == 'D'){
      //Indicaciones para cambiar dirección de motor hacia atrás
      if(adelante == true){
        digitalWrite(MAESTRO, LOW);
        delay(WAITTIME);
        digitalWrite(ESCLAVO_1, LOW);
        digitalWrite(ESCLAVO_2, LOW);
        delay(WAITTIME);
        digitalWrite(MAESTRO, HIGH);
        adelante = false;
      }else{
      //Indicaciones para cambiar dirección de motor hacia adelante
        digitalWrite(MAESTRO, LOW);
        delay(WAITTIME);
        digitalWrite(ESCLAVO_1, HIGH);
        digitalWrite(ESCLAVO_2, HIGH);
        delay(WAITTIME);
        digitalWrite(MAESTRO, HIGH);
        adelante = true;
      }
    }
    //Intensidad baja
    if(DATO == 'L'){
      Serial.write('1');
      analogWrite(MOTOR, INTENSIDAD_LOW);
    }
    //Intensidad media
    if(DATO == 'M'){
      Serial.write('2');
      analogWrite(MOTOR, INTENSIDAD_MEDIUM);
    }
    //Intensidad alta
    if(DATO == 'H'){
      Serial.write('3');
      analogWrite(MOTOR, INTENSIDAD_HIGH);
    }
    //Detener carrito
    if(DATO == 'S'){
      Serial.write('3');
      analogWrite(MOTOR, INTENSIDAD_PARADA);
      //Para los relés de manera controlada
      digitalWrite(MAESTRO, LOW);
      delay(WAITTIME);
      digitalWrite(ESCLAVO_1, HIGH);
      digitalWrite(ESCLAVO_2, HIGH);
      delay(WAITTIME);
      digitalWrite(MAESTRO, HIGH);
      adelante = true;
    }
  }

  // Si recibimos algún dato desde el Serie de Arduino se envía al dispositibo Bluetooth
  if (Serial.available()){
    miBT.write(Serial.read());
  }

}