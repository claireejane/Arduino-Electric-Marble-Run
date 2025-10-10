
//#include <Adafruit_LiquidCrystal.h>  
#include <LiquidCrystal_PCF8574.h>     
#include <Wire.h>
#include <Stepper.h>

//Adafruit_LiquidCrystal lcd(0x27, 16, 2);  
LiquidCrystal_PCF8574 lcd(0x27);           



const int motor1ENA = 8;
const int motor1in1 = 13;
const int motor1in2 = 12;

const int sensorPin1 = A1;
const int sensorPin2 = A2;
const int entrySwitchPin = A3;
const int lightUnobstructed = 500;

const int shiftDataPin = 2;
const int shiftClockPin = 4;
const int shiftLatchPin = 7;
const int echoPin = A0;

const int sdaPin = A4;
const int sclPin = A5;
byte shiftOutState = 0b00000000;

const int stepsPer = 2080;
Stepper stepperMotor(stepsPer, 3, 5, 6, 9);


int distance;
int ballsThroughGSpot = 0;
int ballsGrabbed = 0;
int ballsTrapped = 0;
int ballsSeen = 0;
int unseenBalls = 0;

bool motor1ON = false;  
//bool lastTrapState = HIGH;
unsigned long motor1StartTime = 0;
unsigned long lastTrapTime = 0;
unsigned long lastGrabTime = 0;
unsigned long firstballGrabTime = 0;
unsigned long secondballGrabTime = 0;
unsigned long timePulley1Ball = 8500;
unsigned long timePulley2Balls = 9000;

long pingUltrasonic();
void stopMotor();
void rotateStepper(int steps, int delayTime = 5);


void setup() {
  //rotateStepper(2000); 
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT_PULLUP);
  pinMode(entrySwitchPin, INPUT_PULLUP);
  pinMode(echoPin, INPUT);
  pinMode(motor1ENA, OUTPUT);
  pinMode(motor1in1, OUTPUT);
  pinMode(motor1in2, OUTPUT); 
  pinMode(shiftDataPin, OUTPUT);
  pinMode(shiftClockPin, OUTPUT);
  pinMode(shiftLatchPin, OUTPUT);

  Wire.begin();  // Explicitly use A4 and A5 for I2C

  lcd.begin(16, 2);
  lcd.setBacklight(255);  // Max brightness
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Balls Through: 0");
  Serial.begin(9600);
}

void loop() {
 
  long duration = pingUltrasonic();
  distance = duration * 0.034 / 2;
  if (distance > 1 && distance < 10) {
    unsigned long now = millis();
    ballsSeen++;
    
    unsigned long travelTime = (now - firstballGrabTime) + 2000;
    unseenBalls = ballsThroughGSpot - ballsSeen;

    if (unseenBalls == 1) {
      timePulley2Balls = travelTime + (secondballGrabTime - firstballGrabTime);
      firstballGrabTime = secondballGrabTime;
      secondballGrabTime = lastGrabTime;
    }else{
      timePulley1Ball = travelTime;
    }
     if (ballsSeen == ballsThroughGSpot) {
      stopMotor();
     }
  }
  
  
  int lightVal1 = analogRead(sensorPin1);
  bool trapTime = !digitalRead(sensorPin2);

  Serial.print("Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.print("S1 Light Value: ");
  Serial.print(lightVal1);
  Serial.print(" - S2 Voltage Status: ");
  Serial.println(trapTime);

  Serial.print("B: ");
  Serial.print(ballsGrabbed);
  Serial.print(" - T: ");
  Serial.print(ballsTrapped); 
  Serial.print(" - S: ");
  Serial.println(ballsSeen);

 
  
//Entry Switch Logic
  if (digitalRead(entrySwitchPin) == LOW) {
    lcd.setCursor(0,0);
    lcd.print("Balls through:");
    lcd.setCursor(0,1);
    lcd.print(ballsThroughGSpot);
  }

//Pulley Motor Logic
  if(lightVal1 < lightUnobstructed){
    unsigned long now = millis();
    unseenBalls = ballsThroughGSpot - ballsSeen;
     if (unseenBalls == 1) {
      firstballGrabTime = now;
      lastGrabTime = now;
    } else if (unseenBalls == 2) {
      secondballGrabTime = now;
      lastGrabTime = now;
    }
    ballsGrabbed++;
    if (ballsGrabbed > ballsThroughGSpot){
      ballsGrabbed--;
    }
  
    Serial.println(" - Pulley is ON!");
    digitalWrite(motor1in1, HIGH);
    digitalWrite(motor1in2, LOW);
    analogWrite(motor1ENA, 255);
    motor1StartTime = millis();
    motor1ON = true;
  }  
  //Trapdoor Motor Logic
  if (trapTime && ballsTrapped < ballsThroughGSpot) {
    ballsTrapped++;
    stepperMotor.step(stepsPer);
    delay(300);
  }

} 
  
    


  


void stopMotor() {
  digitalWrite(motor1in1, LOW);
  digitalWrite(motor1in2, LOW);
  analogWrite(motor1ENA, 0);
  motor1ON = false;
}

long pingUltrasonic() {
  setShiftPin(7, LOW);  // Q7 (Trig)
  delayMicroseconds(2);
  setShiftPin(7, HIGH);
  delayMicroseconds(10);
  setShiftPin(7, LOW);
  return pulseIn(echoPin, HIGH, 20000); // Q6 = ECHO wired to Arduino pin 6
}
void setShiftPin(int qPin, bool state) {
  bitWrite(shiftOutState, qPin, state);
  digitalWrite(shiftLatchPin, LOW);
  shiftOut(shiftDataPin, shiftClockPin, MSBFIRST, shiftOutState);
  digitalWrite(shiftLatchPin, HIGH);
}

//void stepStepperMotor(int step) {
  //byte stepSequence[4] = {0b0001, 0b0010, 0b0100, 0b1000};
  //shiftOutState &= 0b11110000;
  //shiftOutState |= stepSequence[step % 4];
  //digitalWrite(shiftLatchPin, LOW);
  //shiftOut(shiftDataPin, shiftClockPin, MSBFIRST, shiftOutState);
  //digitalWrite(shiftLatchPin, HIGH);
//}

//void stepStepperMotor(int step){
  // int stepSequence[4][4] = {
    //{HIGH, LOW, LOW, LOW},
    //{LOW, HIGH, LOW, LOW},
    //{LOW, LOW, HIGH, LOW},
    //{LOW, LOW, LOW, HIGH}
  //};

//  digitalWrite(3, stepSequence[step][0]);
//  digitalWrite(5, stepSequence[step][1]);
//  digitalWrite(6, stepSequence[step][2]);
//  digitalWrite(9, stepSequence[step][3]);

//}
//void rotateStepper(int steps, int delayTime = 5) {
//  for (int i = 0; i < steps; i++) {
//    stepStepperMotor(i);
//    delay(delayTime);
//  }
////  delay(100);
//}
