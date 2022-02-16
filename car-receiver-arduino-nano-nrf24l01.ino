/*
   TATOO CAR
*/

#include <Servo.h>
Servo peck;


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define PIN_CE  10
#define PIN_CSN 9
RF24 radio(PIN_CE, PIN_CSN);
uint64_t writeAddress = 700;
uint64_t readAddress = 100;

char textBuffer[6];

//Right Engine
int RPWM_Output_Right = 5; // Arduino PWM output pin 3; connect to IBT-2 pin 1 (RPWM); Blue wire color; forward
int LPWM_Output_Right = 4; // Arduino PWM output pin 5; connect to IBT-2 pin 2 (LPWM); Orange wire color; reverse
//Left Engine
int RPWM_Output_Left = 2; // Arduino PWM output pin 6; connect to IBT-2 pin 1 (RPWM); Blue-Black wire color; forward
int LPWM_Output_Left = 3; // Arduino PWM output pin 9; connect to IBT-2 pin 2 (LPWM); Orange-Black wire color
//Engine speed
//int velRight = 255, velLeft = 255;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_2XCLOCK_MASK);
  radio.begin();
  radio.setRetries(0, 15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(writeAddress);
  radio.openReadingPipe(1, readAddress);
  radio.startListening();

  peck.attach(A1);
  peck.write(180);

  pinMode(RPWM_Output_Right, OUTPUT);
  pinMode(LPWM_Output_Right, OUTPUT);
  pinMode(RPWM_Output_Left, OUTPUT);
  pinMode(LPWM_Output_Left, OUTPUT);
}

void loop() {
  while (radio.available()) {
    radio.read( textBuffer, sizeof(textBuffer));
    //Serial.println(textBuffer);
    String message = textBuffer;

    if (message == "front")forward();

    else if (message == "down")reverse();

    else if (message == "right")turnRight();

    else if (message == "left")turnLeft();

    else if (message == "stop")stopEngine();

    if (message == "peck")peck.write(120);

    else if (message == "stop")peck.write(180);
  }
}

//Function for stop the car
void stopEngine() {
  analogWrite(LPWM_Output_Right, 0);
  analogWrite(RPWM_Output_Right, 0);
  analogWrite(LPWM_Output_Left, 0);
  analogWrite(RPWM_Output_Left, 0);
  //Serial.println("Rooster stopped!");
}

//Function to accelerate the car forward
void forward() {
  analogWrite(LPWM_Output_Right, 255);
  analogWrite(RPWM_Output_Right, 0);
  analogWrite(LPWM_Output_Left, 255);
  analogWrite(RPWM_Output_Left, 0);
  peck.write(120);
  //Serial.println("Rooster to forward!");
}

//Function for turn right
void turnRight() {
  analogWrite(LPWM_Output_Right, 0);
  analogWrite(RPWM_Output_Right, 255);
  analogWrite(LPWM_Output_Left, 255);
  analogWrite(RPWM_Output_Left, 0);
  //Serial.println("Rooster turning right!");
}

//Function for the car to go back
void reverse() {
  analogWrite(LPWM_Output_Right, 0);
  analogWrite(RPWM_Output_Right, 255);
  analogWrite(LPWM_Output_Left, 0);
  analogWrite(RPWM_Output_Left, 255);
  //Serial.println("Rooster to back!");
}

//Function for the car turn left
void turnLeft() {
  analogWrite(LPWM_Output_Right, 255);
  analogWrite(RPWM_Output_Right, 0);
  analogWrite(LPWM_Output_Left, 0);
  analogWrite(RPWM_Output_Left, 255);
  //Serial.println("Rooster turning left!");
}
