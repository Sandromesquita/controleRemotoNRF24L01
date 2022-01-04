#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define PIN_CE  10
#define PIN_CSN 9
RF24 radio(PIN_CE, PIN_CSN);

uint64_t writeAddress = 100, readAddress = 700;
int mode = 1;
const int BUFFER_SIZE = 7;
char comandBuffer[7];

#define btnFront 5
#define btnRight 3
#define btnDown 4
#define btnLeft 2
#define btnPeck 7
#define btnLeg 6
#define voltageSensor A3

int readPinSensor;  //Variável para leitura da porta analógica
float volts; //Armazenar o cálculo

void setup() {
  Serial.begin(9600);
  for(byte pin=2; pin<8; pin++){
    pinMode(pin, INPUT_PULLUP);
  }
  for(byte pin=13; pin<20; pin++){
    pinMode(pin, OUTPUT);
  }
  pinMode(1, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A6, INPUT);

  digitalWrite(A3, LOW);
  
  while(!Serial){}
  radio.begin();                      
  radio.setRetries(0, 15);
  radio.setPALevel(RF24_PA_LOW);      
  radio.openWritingPipe(writeAddress);  
  radio.openReadingPipe(1, readAddress);
  radio.stopListening();  
}
void loop() {
  buttonReading();
  readSensor();
  battery();
}

void buttonReading(){
  if      (!digitalRead(btnFront)){
    control("front");
    digitalWrite(A3, HIGH);
  }
  else if (!digitalRead(btnRight)){
    control("right");
    digitalWrite(A3, HIGH);
  }
  else if (!digitalRead(btnDown)){
    control("down");
    digitalWrite(A3, HIGH);
  }
  else if (!digitalRead(btnLeft)){
    control("left");
    digitalWrite(A3, HIGH);
  }
  else if (!digitalRead(btnPeck)){
    control("peck");
    digitalWrite(A3, HIGH);
  }
  else if (!digitalRead(btnLeg)){
    control("leg");
    digitalWrite(A3, HIGH);
  }
  else{
    control("stop");
    digitalWrite(A3, LOW);
  }
}

void control(String comand){
  comand.reserve(7);
  
  comand.toCharArray(comandBuffer, BUFFER_SIZE);
  radio.write(comandBuffer,sizeof(comandBuffer));
  
  //Serial.print("[transmitter] -> ");
  //Serial.println(comand);

}

void readSensor() {
  readPinSensor = analogRead(A6);
  volts = ((readPinSensor * 0.004887585) * 5);
  volts = volts + volts*0.04;
  Serial.println(volts);
}

void battery(){
  if(volts >= 8){
    barGraph(0, 0, 0, 0, 0, 0);
    //Serial.println("Full");
  }
  else if((volts >= 7.5)and(volts < 8)){
    barGraph(0, 0, 0, 0, 0, 1);
    //Serial.println("4");
  }
  else if((volts >= 7)and(volts < 7.5)){
    barGraph(0, 0, 0, 0, 1, 1);
    //Serial.println("3");
  }
  else if((volts >= 6.5)and(volts < 7)){
    barGraph(0, 0, 0, 1, 1, 1);
    //Serial.println("2");
  }
  else if((volts >= 6)and(volts < 6.5)){
    barGraph(0, 0, 1, 1, 1, 1);
    //Serial.println("1");
  }
  else{
    barGraph(0, 1, 1, 1, 1, 1);
    //Serial.println("0");
  }
}

void barGraph(bool statusZero, bool statusOne, bool statusTwo, bool statusThree, bool statusFour, bool statusFive ){
  digitalWrite(8, statusZero);
  digitalWrite(A0, statusOne);
  digitalWrite(A1, statusTwo);
  digitalWrite(A2, statusThree);
  digitalWrite(A4, statusFour);
  digitalWrite(A5, statusFive);
}
