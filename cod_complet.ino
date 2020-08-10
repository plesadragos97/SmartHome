//comenzile trimise din aplicatie corespund pinilor componentelor

#include <Stepper.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//am definit led-urile pe pinii 2-6
#define led1 2
#define led2 3
#define led3 4
#define led4 5
#define led5 6
#define ledPIR A0
#define ledCafea A1
#define ONE_WIRE_BUS 7

byte recievedData;
//variabile care retin daca becul este aprins sau stins
boolean aprins1 = false;
boolean aprins2 = false;
boolean aprins3 = false;
boolean aprins4 = false;
boolean aprins5 = false;

#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)
  
Stepper stepper(STEPS, 8, 10, 9, 11);


int inputPIR = 12;
int pirState = LOW;
int val = 0;
boolean deschis = false; //variabila care retine daca usa garajului este deschisa sau inchisa

//senzor temp
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  //led-urile sunt de tipul IESIRE
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(ledPIR, OUTPUT);
  pinMode(ledCafea, OUTPUT);

  pinMode(inputPIR, INPUT);

  //valoarea initiala a led-urilor este LOW, adica sunt stinse
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(ledPIR, 0);
  digitalWrite(ledCafea, 0);

  

  //se porneste interfata seriala pentru transmiterea datelor
  Serial.begin(9600);

  sensors.begin();
}

void loop() {
//codul pentru PIR
  val = digitalRead(inputPIR);
  if (val == HIGH) {            
      
    if (pirState == LOW) {
      digitalWrite(ledPIR, 255);
      pirState = HIGH;
    }
  } else {
    
    if (pirState == HIGH){
      digitalWrite(ledPIR, 0); 
      pirState = LOW;
    }
  }

  
  if(Serial.available()>0) {  
    recievedData = Serial.read();
//codul pentru led-uri
    if(recievedData == 2) { //mesajul "2" corespunde aprinderii/stingerii led-ului 1
      if(!aprins1){
        digitalWrite(led1, HIGH);
        aprins1 = !aprins1;
      }else {
        digitalWrite(led1, LOW);
        aprins1 = !aprins1;
      }
    }
    if(recievedData == 3) { //mesajul "3" corespunde aprinderii/stingerii led-ului 2
      if(!aprins2){
        digitalWrite(led2, HIGH);
        aprins2 = !aprins2;
      }else {
        digitalWrite(led2, LOW);
        aprins2 = !aprins2;
      }
    }
    if(recievedData == 4) { //mesajul "4" corespunde aprinderii/stingerii led-ului 3
      if(!aprins3){
        digitalWrite(led3, HIGH);
        aprins3 = !aprins3;
      }else {
        digitalWrite(led3, LOW);
        aprins3 = !aprins3;
      }
    }
    if(recievedData == 5) { //mesajul "5" corespunde aprinderii/stingerii led-ului 4
      if(!aprins4){
        digitalWrite(led4, HIGH);
        aprins4 = !aprins4;
      }else {
        aprins4 = !aprins4;
      }
    }
    if(recievedData == 6) { //mesajul "6" corespunde aprinderii/stingerii led-ului 5
      if(!aprins5){
        digitalWrite(led5, HIGH);
        aprins5 = !aprins5;
      }else {
        digitalWrite(led5, LOW);
        aprins5 = !aprins5;
      }
    }



    //codul pentru senzorul de temperatura
    if(recievedData == 7) {
     sensors.requestTemperatures(); 
     int temperatura = sensors.getTempCByIndex(0);
     // Am folosit ByIndex deoarece putem lega mai multi senzori pe acelasi fir 
     // 0 se refera la primul senzor
     Serial.println(temperatura); 
      

    }

    //codul pentru motor
    if(recievedData == 8) {
      if(deschis == false) {  //daca usa este inchisa
        stepper.setSpeed(13); 
        stepper.step(-10190); // 2038 pasi -- o rotatie completa => 20380 = 10 rotatii, in sensul acelor de ceas, pentru a deschide usa
        deschis = true;
      }else {
        stepper.setSpeed(13); 
        stepper.step(10190); //10 rotatii complete in sensul opus acelor de ceas, pentru a inchide usa
        deschis = false;
      }
      
    }

    //codul pentru led-ul aparatului de cafea
    if(recievedData == 10) {  //espresso
      digitalWrite(ledCafea, 255);
      delay(3000); 
      digitalWrite(ledCafea, 0);
    }   
    if(recievedData == 11) {  //espresso macchiato
      digitalWrite(ledCafea, 255);
      delay(3000); 
      digitalWrite(ledCafea, 0);
      delay(1000);
      digitalWrite(ledCafea, 255);
      delay(2000);
      digitalWrite(ledCafea, 0);
    } 
    if(recievedData == 12) {  //black coffee
      digitalWrite(ledCafea, 255);
      delay(5000); 
      digitalWrite(ledCafea, 0);
    } 
    if(recievedData == 13) {  //cappuccino
      digitalWrite(ledCafea, 255);
      delay(3000); 
      digitalWrite(ledCafea, 0);
      delay(1000);
      digitalWrite(ledCafea, 255);
      delay(3000);
      digitalWrite(ledCafea, 0);
    } 
    if(recievedData == 14) {  //iced coffee
      digitalWrite(ledCafea, 255);
      delay(5000);
      digitalWrite(ledCafea, 0);
    } 
    if(recievedData == 15) {  //cafe latte
      digitalWrite(ledCafea, 255);
      delay(2500);
      digitalWrite(ledCafea, 0);
      delay(1000);
      digitalWrite(ledCafea, 255);
      delay(3500);
      digitalWrite(ledCafea, 0);
    }  
    


 
  }
}
