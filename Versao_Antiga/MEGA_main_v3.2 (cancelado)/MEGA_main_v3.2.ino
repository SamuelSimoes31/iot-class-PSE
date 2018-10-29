#define debug true
//-------TIMER-----------
#include <Event.h>
#include <Timer.h>
int id1,id2,id3,id4;
//id1(5min) / id2(1min) /id2(desligar tomadas) / id4(soliciar presença UNO)
Timer t;

//-------INFRARED----------------
#define LedIR 11

//-------BLUETOOTH----------
bool BluetoothOk = false;

#define PIR 3
#define BUZ 8
#define lamp 4
#define tomadas 5

volatile bool presUNO;
volatile bool presMEGA;
volatile bool presUNO_anterior = true; //estado anterior
volatile bool presMEGA_anterior = true;//estado anteiror
         int  estadoSala = LIGADA;

enum estadosSala{LIGADA, DESLIGANDO, DESLIGADA};

String comandoRecebido;

void setup() {
  #ifdef debug
  Serial.begin(9600);
  #endif
  Serial3.begin(9600);
  
  pinMode(BUZ,OUTPUT);
  pinMode(lamp,OUTPUT);
  digitalWrite(lamp,HIGH);
  pinMode(tomadas,OUTPUT);
  digitalWrite(tomadas,HIGH);
  
  presMEGA = digitalRead(PIR);
  presMEGA_anterior = presMEGA;
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);
  pinMode(LedIR, OUTPUT); 

  id4 = t.every(250)
  while(!BluetoothOk)t.update();
}

void loop() {
  t.update();
  
  if(presMEGA != presMEGA_anterior)
  {
    if(presMEGA)  Serial3.write("PM1|"); //Presença Mega = true
    else          Serial3.write("PM0|"); //Presença Mega = false
    presMEGA_anterior = presMEGA;
  }

  //recebendo mudança de estado do UNO
  if( Serial3.available() )
  {
    comandoRecebido = Serial3.readStringUntil('|');
    if( comandoRecebido == "PU1" )  presUNO = true;
    else if( comandoRecebido == "PU0" )  presUNO = false;
    #ifdef debug
    Serial.println(comandoRecebido);
    #endif
  }

  if( !( presMEGA || presUNO ) && estadoSala == LIGADA )
  {
    estadoSala = DESLIGANDO;
    Serial3.write("PM0|");
    iniciarDesligamento();
  }
  else if(( presMEGA || presUNO ) && estadoSala == DESLIGANDO)
  {
      estadoSala = LIGADA;
      t.stop(id1);
      t.stop(id2);
      t.stop(id3);
  }
  else if(( presMEGA || presUNO ) && estadoSala == DESLIGADA)
  {
    estadoSala = LIGADA;
    digitalWrite(lamp,HIGH);
    digitalWrite(tomadas,HIGH);
  }
  
  #ifdef debug
  Serial.print("presMEGA = ");
  Serial.print(presMEGA);
  Serial.print("   presUNO = ");
  Serial.println(presUNO);
  #endif
}

void pirISR()
{
  presMEGA = digitalRead(PIR);
}

