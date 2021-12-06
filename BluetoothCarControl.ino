
#include <ESP32Servo.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//Define Pins for Motors
const int MOTOR_1F = 12;
const int MOTOR_1B = 27;
const int MOTOR_1PWM = 33;
const int MOTOR_2F = 15;
const int MOTOR_2B = 32;
const int MOTOR_2PWM = 14;

//Define Pins for Electromagnet and LED light
const int ELECTRO = 4;
const int LEDLIGHT = 21;


//Define servos for the plow and arm
Servo plowServo;
Servo armServo;
int plowPos = 0;
int armPos = 0;

//Command variable to get character sent in from android phone app
char command;


void setup() {

  SerialBT.begin("MiningRig"); //Bluetooth device name
  SerialBT.println("Ready to Pair with Bluetooth.");

  plowServo.write(180);
  armServo.write(180);
  
  pinMode(LED_BUILTIN, OUTPUT);


 //Set output for Motor Pins
  pinMode(MOTOR_1F, OUTPUT);
  pinMode(MOTOR_1B, OUTPUT);
  pinMode(MOTOR_1PWM, OUTPUT);
  pinMode(MOTOR_2F, OUTPUT);
  pinMode(MOTOR_2B, OUTPUT);
  pinMode(MOTOR_2PWM, OUTPUT);

  //Set output for electromagnet and LED pins
  pinMode(ELECTRO, OUTPUT);
  pinMode(LEDLIGHT, OUTPUT);

  digitalWrite(ELECTRO, LOW);
}

void loop() {
  
  //Case statements to make function call based on what character is sent to board from bluetooth android app
    if (SerialBT.available()>0) {
      command = SerialBT.read();
      switch(command) {
        case 'q' :
          forwardSlow();
          break;
        case 'f' : 
          forward(); 
          break;
        case 'g' :
          forwardFast();
          break;  
        case 'b' : 
          backward(); 
          break;
        case 'l' : 
          turnLeft(); 
          break;
        case 'r' : 
          turnRight(); 
           break;
        case 's' : 
          stopCar(); 
          break;
        case 'm' : 
          magnetOn(); 
          break;
        case 'n' :
          magnetOff();
          break; 
        case 'i' : 
          lights(); 
          break; 
        case 'u' : 
          plowUp();
          break;
        case 'd' :
          plowDown();
          break;
        case 'c' :
          armUp();
          break;
        case 'v' :
          armDown();
          break;
          
        // Code to test the led that is built into the board to test communication between phone/micro controller.
        case 't' :  
          if (digitalRead(LED_BUILTIN) == LOW) {
            digitalWrite(LED_BUILTIN, HIGH);
          }
          else {
            digitalWrite(LED_BUILTIN, LOW);
          }
          break;
          default : break;
        //case 'o' : digitalWrite(LED_BUILTIN, LOW); break;
      } 
    //SerialBT.print(command);
  }
}


//Function for moving slow speed forward
void forwardSlow() {
    digitalWrite(MOTOR_1F, HIGH);
    digitalWrite(MOTOR_2F, HIGH);   
    digitalWrite(MOTOR_1B, LOW);
    digitalWrite(MOTOR_2B, LOW);
    analogWrite(MOTOR_1PWM, 125);
    analogWrite(MOTOR_2PWM, 125);
}

//Function for moving medium speed forward
void forward() {
    digitalWrite(MOTOR_1F, HIGH);
    digitalWrite(MOTOR_2F, HIGH);   
    digitalWrite(MOTOR_1B, LOW);
    digitalWrite(MOTOR_2B, LOW);
    analogWrite(MOTOR_1PWM, 175);
    analogWrite(MOTOR_2PWM, 175);
}

//Function for moving fast speed forward
void forwardFast() {
   digitalWrite(MOTOR_1F, HIGH);
   digitalWrite(MOTOR_2F, HIGH);   
   digitalWrite(MOTOR_1B, LOW);
   digitalWrite(MOTOR_2B, LOW);
   analogWrite(MOTOR_1PWM, 255);
   analogWrite(MOTOR_2PWM, 255); 
}

//Function for backwards movement
void backward() {
    digitalWrite(MOTOR_1F, LOW);
    digitalWrite(MOTOR_2F, LOW);   
    digitalWrite(MOTOR_1B, HIGH);
    digitalWrite(MOTOR_2B, HIGH);
    analogWrite(MOTOR_1PWM, 150);
    analogWrite(MOTOR_2PWM, 150);  
}

//Turning left, sets right motor forward and left motor backward for track like turning
void turnLeft() {
  digitalWrite(MOTOR_1F, LOW);
  digitalWrite(MOTOR_1B, HIGH);
  digitalWrite(MOTOR_2B, LOW);
  digitalWrite(MOTOR_2F, HIGH);
  analogWrite(MOTOR_1PWM, 150);
  analogWrite(MOTOR_2PWM, 250);
}

//Turning left, sets left motor forward and right motor backward for track like turning
void turnRight() {
  digitalWrite(MOTOR_2F, LOW);
  digitalWrite(MOTOR_2B, HIGH);
  digitalWrite(MOTOR_1B, LOW);
  digitalWrite(MOTOR_1F, HIGH);
  analogWrite(MOTOR_1PWM, 250);
  analogWrite(MOTOR_2PWM, 150);
}

//Function to stop car, turns all motors off
void stopCar() {
  digitalWrite(MOTOR_1F, LOW);
  digitalWrite(MOTOR_1B, LOW);
  digitalWrite(MOTOR_2F, LOW);
  digitalWrite(MOTOR_2B, LOW);
  analogWrite(MOTOR_1PWM, 0);
  analogWrite(MOTOR_2PWM, 0);
}

//Set electromagnet high to pick things up
void magnetOn() {
  digitalWrite(ELECTRO, HIGH);
}

//Set electromagnet low to drop things
void magnetOff() {
  digitalWrite(ELECTRO, LOW);  
}

//Turn LED light on if it is off, or turn LED light off if it is on
void lights() {
  if (digitalRead(LEDLIGHT) == LOW) {
    digitalWrite(LEDLIGHT, HIGH);
   }
  else {
    digitalWrite(LEDLIGHT, LOW);
   }
}

///Lower plow using servo with delay so it lowers slowly
void plowDown() {
  plowServo.attach(26); //Pin 26 is labeled as A0 on the board
  for (plowPos = 90; plowPos <= 180; plowPos += 1) {
    plowServo.write(plowPos);
    delay(15);
   }
   plowServo.detach();
}

///Raise plow using servo with delay so it raises slowly
void plowUp() {
  plowServo.attach(26);
  for (plowPos = 180; plowPos >= 90; plowPos -= 1) {
    plowServo.write(plowPos);
    delay(15);  
  }
  plowServo.detach();
}

///Lower arm using servo with delay so it lowers slowly
void armDown() {
 armServo.attach(25);
 for (armPos = 90; armPos <= 180; armPos += 1) {
    armServo.write(armPos);
    delay(15);  
 }
 armServo.detach();
}

///Raises arm using servo with delay so it raises slowly
void armUp() {
 armServo.attach(25);
 for (armPos = 180; armPos >=90; armPos -= 1) {
    armServo.write(armPos);
    delay(15);  
 }
 armServo.detach();
}
