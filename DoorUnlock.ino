#include <Servo.h>                          // include the Servo library
Servo myServo;                              // create a servo object called myServo
const int TrigPin = 2;
const int EchoPin = 3;
const int Light = 7;
const int Buttons = 8;
float cm;
bool onOff = false;

void setup()
{
  myServo.attach(6);                          // attaches the servo on pin 8 to the myServo object
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(Light,OUTPUT);
  pinMode(Buttons,INPUT);
  Serial.begin(9600);
}
void loop()
{
  digitalWrite(TrigPin, LOW);       
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
  cm = (int(cm * 100.0)) / 100.0;     //Keep two decimal places
  Serial.println();
  if(cm<15){ //if you are puting the sensor behind the door use ">" sysmbole insted of ">" p.s. make sure tat the door nob is less than 15 cm away
      digitalWrite(Light,HIGH);
      myServo.write(75);
      delay(2000);
      onOff = true;
    }

   if(digitalRead(Buttons) == HIGH){
    digitalWrite(Light,HIGH);
      myServo.write(75);
      delay(2000);
      onOff = true;
   }

    if(onOff == true){
      digitalWrite(Light,LOW);
      myServo.write(25);
      delay(200); 
      onOff = false; 
    }
   
  delay(20);
}
