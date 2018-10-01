#define debug true

#define PIR 3
#define BUZ 8
#define lamp 4

volatile bool presUNO = true;
volatile bool presMEGA = true;
volatile bool presUNO_anterior = true; //estado anterior
volatile bool presMEGA_anterior = true;//estado anteiror
         bool estadoSala = true;

String comandoRecebido;

void setup() {
  #ifdef debug
  Serial.begin(9600);
  #endif
  Serial3.begin(9600);
  pinMode(BUZ,OUTPUT);
  pinMode(lamp,OUTPUT);
  digitalWrite(lamp,HIGH);
  presMEGA = digitalRead(PIR);
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);
}

void loop() {
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
  if( !( presMEGA || presUNO ) && estadoSala == 1 )
  {
    iniciarDesligamento();
  }
  else if(( presMEGA || presUNO ) && estadoSala == 0)
  {
    estadoSala = 1;
    digitalWrite(lamp,HIGH);
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

void iniciarDesligamento()
{
  #ifdef debug
  Serial.println("Desligamento iniciado!");
  #endif
  for(int i=1; i<=10+5; i++)
  {
    if( Serial3.available() )
    {
    comandoRecebido = Serial3.readStringUntil('|');
    if( comandoRecebido == "PU1" )  presUNO = true;
    else if( comandoRecebido == "PU0" )  presUNO = false;
    #ifdef debug
    Serial.println(comandoRecebido);
    #endif
    }
    if(presMEGA || presUNO)
    {
      #ifdef debug
      Serial.println("desligamento cancelado");
      #endif
      break;
    }
    if(i == 10) beepAtivo(5, 100, 200, BUZ);
    if(i == 15)
    {
      beepAtivo(1, 1000, 0, BUZ);
      digitalWrite(lamp,LOW);
      estadoSala = 0;
    }
    delay(1000);
  }
}

void beepAtivo(int beeps, int tempoON, int tempoOFF, int pin)
{
  for(int i=0; i<beeps; i++)
  {
    digitalWrite(pin,HIGH);
    delay(tempoON);
    digitalWrite(pin,LOW);
    delay(tempoOFF);
  }
}

