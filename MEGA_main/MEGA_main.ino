#define PIR 3
#define BUZ 8
#define lamp 4

volatile bool presUNO = true;
volatile bool presMEGA = true;
volatile bool presUNO_anterior = true; //estado anterior
volatile bool presMEGA_anterior = true;//estado anteiror
         bool estadoSala;

String comandoRecebido;

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  
  pinMode(BUZ,OUTPUT);
  pinMode(lamp,OUTPUT);
  digitalWrite(lamp,HIGH);
  
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
    Serial.println(comandoRecebido);
  }

  if( !( presMEGA || presUNO ) ) //NAND (só dá 1 se as duas forem 0)
  {
    iniciarDesligamento();
  }

  Serial.print("presMEGA = ");
  Serial.print(presMEGA);
  Serial.print("   presUNO = ");
  Serial.println(presUNO);
}

void pirISR()
{
  presMEGA = digitalRead(PIR);
}

void iniciarDesligamento()
{
  Serial.println("Desligamento iniciado!");
  //delay(300000); //esperar 5 minutos
  delay(5000);
  beepAtivo(3, 1000, 500, BUZ);
  //delay(60000);
  delay(1000);
  beepAtivo(1, 4000, 0, BUZ);
  digitalWrite(lamp,LOW);
  //Serial3.write("DAC|");
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

