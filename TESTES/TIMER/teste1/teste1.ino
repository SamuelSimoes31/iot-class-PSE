#include <Event.h>
#include <Timer.h>

#define pin 12
#define pir 2

//bool state = 0;
int id1;
Timer t;

void setup() {
  pinMode(pin,OUTPUT);
  pinMode(pir,INPUT_PULLUP);
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  //id1 = t.every(200,callbackfuc,1);
  //t.after(1000,callbackfuc,1);
  
  attachInterrupt(digitalPinToInterrupt(pir),pirISR,CHANGE);
}

void loop() {
  t.update();
  
}

void callbackfuc()
{
  digitalWrite(pin,!digitalRead(pin));
}

void pirISR()
{
  digitalWrite(3,!digitalRead(3));
  bool st = digitalRead(pir);
  if(!st)
  {
    t.stop(id1);
  }
  else
  {
    id1 = t.every(200,callbackfuc,1);
  }
}

