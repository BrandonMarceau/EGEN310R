
#include <ESP32Servo.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


const int MOTOR_1F = 12;
const int MOTOR_1B = 27;
const int MOTOR_1PWM = 33;

const int MOTOR_2F = 15;
const int MOTOR_2B = 32;
const int MOTOR_2PWM = 14;

const int ELECTRO = 4;
const int LEDLIGHT = 21;



Servo plowServo;
Servo armServo;
int plowPos = 0;
int armPos = 0;

short pwm = 0;

char command;


void setup() {

  //Serial.begin(115200);
  SerialBT.begin("MiningRig"); //Bluetooth device name
  SerialBT.println("Ready to Pair with Bluetooth.");

  plowServo.write(180);
  armServo.write(180);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(MOTOR_1F, OUTPUT);
  pinMode(MOTOR_1B, OUTPUT);
  pinMode(MOTOR_1PWM, OUTPUT);

  pinMode(MOTOR_2F, OUTPUT);
  pinMode(MOTOR_2B, OUTPUT);
  pinMode(MOTOR_2PWM, OUTPUT);

  pinMode(ELECTRO, OUTPUT);
  pinMode(LEDLIGHT, OUTPUT);

  digitalWrite(ELECTRO, LOW);
}

void loop() {
  
  //while(SerialBT.available()) {
    if (SerialBT.available()>0) {
      command = SerialBT.read();
      switch(command) {
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


void forward() {
    digitalWrite(MOTOR_1F, HIGH);
    digitalWrite(MOTOR_2F, HIGH);   
    digitalWrite(MOTOR_1B, LOW);
    digitalWrite(MOTOR_2B, LOW);
    analogWrite(MOTOR_1PWM, 150);
    analogWrite(MOTOR_2PWM, 150);

/*
  if (MOTOR_1B == LOW && MOTOR_2B == LOW) {
    digitalWrite(MOTOR_1F, HIGH);
    digitalWrite(MOTOR_2F, HIGH);  
    if (pwm < 255) {
      pwm += 85;
      analogWrite(MOTOR_1PWM, pwm);
      analogWrite(MOTOR_2PWM, pwm);
    }
  }
  else if (MOTOR_1B == HIGH && MOTOR_2B == HIGH) {
    if (pwm == 0){  
      digitalWrite(MOTOR_1B, LOW);
      digitalWrite(MOTOR_2B, LOW);
    }
    pwm -= 85;
    analogWrite(MOTOR_1PWM, pwm);
    analogWrite(MOTOR_2PWM, pwm);
  }
  else {
    digitalWrite(MOTOR_1F, HIGH);
    digitalWrite(MOTOR_2F, HIGH);
    analogWrite(MOTOR_1PWM, 85);
    analogWrite(MOTOR_2PWM, 85);
 }
 */
}

void forwardFast() {
   digitalWrite(MOTOR_1F, HIGH);
   digitalWrite(MOTOR_2F, HIGH);   
   digitalWrite(MOTOR_1B, LOW);
   digitalWrite(MOTOR_2B, LOW);
   analogWrite(MOTOR_1PWM, 250);
   analogWrite(MOTOR_2PWM, 250); 
}


void backward() {
    /*  digitalWrite(MOTOR_1F, LOW);
    digitalWrite(MOTOR_2F, LOW);   
    digitalWrite(MOTOR_1B, HIGH);
    digitalWrite(MOTOR_2B, HIGH);
    */
  if (MOTOR_1F == LOW && MOTOR_2F == LOW) {
    digitalWrite(MOTOR_1B, HIGH);
    digitalWrite(MOTOR_2B, HIGH);  
    if (pwm < 255) {
      pwm += 85;
      analogWrite(MOTOR_1PWM, pwm);
      analogWrite(MOTOR_2PWM, pwm);
    }
  }
  else if (MOTOR_1F == HIGH && MOTOR_2F == HIGH) {
    if (pwm == 0){  
      digitalWrite(MOTOR_1F, LOW);
      digitalWrite(MOTOR_2F, LOW);
    }
    pwm -= 85;
    analogWrite(MOTOR_1PWM, pwm);
    analogWrite(MOTOR_2PWM, pwm);
  }
  else {
    digitalWrite(MOTOR_1B, HIGH);
    digitalWrite(MOTOR_2B, HIGH);
    analogWrite(MOTOR_1PWM, 85);
    analogWrite(MOTOR_2PWM, 85);
  }
  
}

void turnLeft() {
  digitalWrite(MOTOR_1F, HIGH);
  digitalWrite(MOTOR_1B, LOW);
  digitalWrite(MOTOR_2B, HIGH);
  digitalWrite(MOTOR_2F, LOW);
  analogWrite(MOTOR_1PWM, 150);
  analogWrite(MOTOR_2PWM, 150);
}

void turnRight() {
  digitalWrite(MOTOR_2F, HIGH);
  digitalWrite(MOTOR_2B, LOW);
  digitalWrite(MOTOR_1B, HIGH);
  digitalWrite(MOTOR_1F, LOW);
  analogWrite(MOTOR_1PWM, 150);
  analogWrite(MOTOR_2PWM, 150);
}

void stopCar() {
  digitalWrite(MOTOR_1F, LOW);
  digitalWrite(MOTOR_1B, LOW);
  digitalWrite(MOTOR_2F, LOW);
  digitalWrite(MOTOR_2B, LOW);
  analogWrite(MOTOR_1PWM, 0);
  analogWrite(MOTOR_2PWM, 0);
}

void magnetOn() {
  digitalWrite(ELECTRO, HIGH);
}

void magnetOff() {
  digitalWrite(ELECTRO, LOW);  
}

void lights() {
  if (digitalRead(LEDLIGHT) == LOW) {
    digitalWrite(LEDLIGHT, HIGH);
   }
  else {
    digitalWrite(LEDLIGHT, LOW);
   }
}

void plowUp() {
  plowServo.attach(26); //Pin 26 is labeled as A0 on the board
  for (plowPos = 90; plowPos <= 180; plowPos += 1) {
    plowServo.write(plowPos);
    delay(15);
   }
   plowServo.detach();
}

void plowDown() {
  plowServo.attach(26);
  for (plowPos = 180; plowPos >= 90; plowPos -= 1) {
    plowServo.write(plowPos);
    delay(15);  
  }
  plowServo.detach();
}

void armUp() {
 armServo.attach(25);
 for (armPos = 90; armPos <= 180; armPos += 1) {
    armServo.write(armPos);
    delay(15);  
 }
 armServo.detach();
}

void armDown() {
 armServo.attach(25);
 for (armPos = 180; armPos >=90; armPos -= 1) {
    armServo.write(armPos);
    delay(15);  
 }
 armServo.detach();
}
