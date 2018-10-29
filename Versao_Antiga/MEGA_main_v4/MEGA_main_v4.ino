#define debug true
//-------TIMER-----------
#include <Event.h>
#include <Timer.h>
int id1,id2,id3;
Timer t;

//-------INFRARED----------------
#define LedIR 11

//-------BLUETOOTH----------------
#define BTstatusPIN 2
volatile bool BTstatus = false;

#define PIR 3
#define BUZ 8
#define lamp 13  //MUDAR PARA 4
#define tomadas 5

enum estadosSala{LIGADA, DESLIGANDO, DESLIGADA};
volatile bool presUNO = false;
volatile bool presMEGA = true;
volatile bool presUNO_anterior = true; //estado anterior
volatile bool presMEGA_anterior = true;//estado anteiror
         int estadoSala = LIGADA;



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
  
  pinMode(PIR,INPUT);
  pinMode(BTstatusPIN,INPUT);
  presMEGA = digitalRead(PIR);

  BTstatus = digitalRead(BTstatusPIN);
  attachInterrupt(digitalPinToInterrupt(BTstatusPIN), btISR, CHANGE);
  while(!BTstatus);
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);
  
  pinMode(LedIR, OUTPUT); 
  
}

void loop() {
  t.update();
  if(BTstatus)
  {
    //se estado mudar, enviar isso para o UNO
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
        Serial.println("DESLIGAMENTO CANCELADO!");
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
    Serial.print("estadoSala = ");
    Serial.print(estadoSala);
    
    Serial.print("  |  presMEGA = ");
    Serial.print(presMEGA);
    Serial.print("  |  presUNO = ");
    Serial.println(presUNO);
    
    delay(1000);
    
    #endif
  }
}

void pirISR()
{
  presMEGA = digitalRead(PIR);
}

void btISR()
{
  BTstatus = digitalRead(BTstatusPIN);
}

