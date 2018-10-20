
#define LedIR 11
 
void setup() {
  pinMode(LedIR, OUTPUT); 
  Serial.begin(9600);
}

void loop() {
  Serial.println("Enviando sinal IR (DESLIGAR PROJETOR)");
  EnviarCodigo();
  delay(2000);
}
 
void pulseIR(long microsegs) // Utilização dos interrupts Cli() e Sei()
{
  cli();  
    while (microsegs > 0) 
    {
    digitalWrite(LedIR, HIGH);  
    delayMicroseconds(10);       
    digitalWrite(LedIR, LOW);   
    delayMicroseconds(10);        
    microsegs -= 26; 
    }
  sei(); 
}

 
void EnviarCodigo() {  //DESLIGAR PROJETOR 

delayMicroseconds(39160);
pulseIR(8940);
delayMicroseconds(4660);
pulseIR(460);
delayMicroseconds(1900);
pulseIR(360);
delayMicroseconds(1820);
pulseIR(440);
delayMicroseconds(1920);
pulseIR(360);
delayMicroseconds(840);
pulseIR(280);
delayMicroseconds(820);
pulseIR(320);
delayMicroseconds(800);
pulseIR(340);
delayMicroseconds(780);
pulseIR(340);
delayMicroseconds(1840);
pulseIR(440);
delayMicroseconds(1820);
pulseIR(440);
delayMicroseconds(780);
pulseIR(360);
delayMicroseconds(1920);
pulseIR(340);
delayMicroseconds(700);
pulseIR(440);
delayMicroseconds(780);
pulseIR(360);
delayMicroseconds(780);
pulseIR(340);
delayMicroseconds(2000);
pulseIR(280);
delayMicroseconds(880);
pulseIR(240);
delayMicroseconds(2020);
pulseIR(260);
delayMicroseconds(2020);
pulseIR(240);
delayMicroseconds(1920);
pulseIR(360);
delayMicroseconds(800);
pulseIR(320);
delayMicroseconds(1920);
pulseIR(360);
delayMicroseconds(780);
pulseIR(340);
delayMicroseconds(880);
pulseIR(260);
delayMicroseconds(740);
pulseIR(400);
delayMicroseconds(840);
pulseIR(280);
delayMicroseconds(780);
pulseIR(360);
delayMicroseconds(780);
pulseIR(360);
delayMicroseconds(2000);
pulseIR(260);
delayMicroseconds(780);
pulseIR(360);
delayMicroseconds(1880);
pulseIR(380);
delayMicroseconds(1940);
pulseIR(320);
delayMicroseconds(1900);
pulseIR(380);
delayMicroseconds(41880);
pulseIR(9160);
delayMicroseconds(2260);
pulseIR(560);
}
