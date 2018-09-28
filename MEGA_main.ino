#define PIR 3
#define BUZ 2

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
  
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);
}

void loop() {
  //se estado mudar, enviar isso para o UNO
  if(presMEGA != presMEGA_anterior)
  {
    if(presMEGA)  Serial3.write("PM1|"); //Presença Mega = true
    else          Serial3.write("PM0|"); //Presença Mega = false
  }

  //recebendo mudança de estado do UNO
  if( Serial3.available() )
  {
    comandoRecebido = Serial3.readStringUntil('|');
    if( comandoRecebido == "PU1" )  presUNO = true;
    if( comandoRecebido == "PU0" )  presUNO = false;
  }

  if( !( presMEGA || presUNO ) ) //NAND (só dá 1 se as duas forem 0)
  {
    iniciarDesligamento();
  }

}

void pirISR()
{
  presMEGA = digitalRead(PIR);
}

void iniciarDesligamento()
{
  delay(300000); //esperar 5 minutos
  beepAtivo(3, 1000, 500, BUZ);
  delay(60000);
  beepAtivo(1, 4000, 0, BUZ);
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

