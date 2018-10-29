#define debug true

#include <SoftwareSerial.h>

#define PIR 3

//-------INFRARED----------------
#define LedIR 11

//-------BLUETOOTH----------------
#define BTstatusPIN 2
volatile bool BTstatus = false;

volatile bool presUNO = true;
volatile bool presUNO_anterior = true; //estado anterior

String comandoRecebido;

SoftwareSerial BTserial(4, 5); // RX | TX

void setup() {
  #ifdef debug
  Serial.begin(9600); //Para fins de debug
  #endif
  
  BTserial.begin(9600);
  pinMode(PIR,INPUT);
  pinMode(BTstatusPIN,INPUT);

  presUNO = digitalRead(PIR);
  presUNO_anterior = presUNO;
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);
  
  btISR();
  attachInterrupt(digitalPinToInterrupt(BTstatusPIN), btISR, CHANGE);
  while(!BTstatus)
  {
    Serial.println("AGUARDANDO CONEXÃO");
    Serial.println(BTstatus);
  }
  while(1)
  {
    Serial.println("socorro");
    if ( BTserial.available() )
    {
      comandoRecebido = BTserial.readStringUntil('|');
      Serial.println(comandoRecebido);
      if ( comandoRecebido == "PUOK" )break;
    }
    delay(100);
    if(presUNO)  BTserial.write("PU1|"); //Presença Uno = true
    else         BTserial.write("PU0|"); //Presença Uno = false
  }

  
}

void loop() {
  if(BTstatus)
  {
    //se estado mudar, enviar isso para o MEGA
    if(presUNO != presUNO_anterior)
    {
      if(presUNO)  BTserial.write("PU1|"); //Presença Uno = true
      else         BTserial.write("PU0|"); //Presença Uno = false
      presUNO_anterior = presUNO;
    }
  
    #ifdef debug
    Serial.print("   presUNO = ");
    Serial.println(presUNO);
    delay(1000);
    #endif
  }
  else{Serial.println("Whoops");}
}

void pirISR()
{
  presUNO = digitalRead(PIR);
}

void btISR()
{
  BTstatus = digitalRead(BTstatusPIN);
  digitalWrite(13,BTstatus);
}
