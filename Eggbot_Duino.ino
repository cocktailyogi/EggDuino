/* Eggduino-Firmware by Joachim Cerny, 2014

Thanks for the nice libs ACCELSTEPPER and SERIALCOMMAND, which made this project much easier.
Thanks to the Eggbot-Team for such a funny and enjoable concept!
Thanks to my wife and my daughter for their patience. :-)

*/

// implemented Eggbot-Protocol-Version 2.1.0
// EBB-Command-Referenc, I sourced from: http://www.schmalzhaus.com/EBB/EBBCommands.html
// no homing sequence, switch-on position of pen will be taken as reference point.
// No collision-detection!!
// Supported Servos: I do not know, I use Arduino Servo Lib with TG9e- standard servo.

/* TODOs:
1	collision control via penMin/penMax
2	implement homing sequence via microswitch or optical device
3	implement hardware-button , EGGBOT-Guys call it "PRG-Button"
4	Replace Stepper Control ACCELSTEPPER --> DDS, interruptbased control to improve step-precision and smoothness.
*/

#include "AccelStepper.h" // nice lib from http://www.airspayce.com/mikem/arduino/AccelStepper/
#include <Servo.h>
#include "SerialCommand.h" //nice lib from Stefan Rado, https://github.com/kroimon/Arduino-SerialCommand

#define initSting "EBB 2.1.0+ Protocol emulated by Eggduino-Firmware V1.0"
//Rotational Stepper
  #define step1 11
  #define dir1 10
  #define enableRotMotor 9
  #define rotMicrostep 8
//Pen Stepper
  #define step2 8
  #define dir2 7
  #define enablePenMotor 5
  #define penMicrostep 8
//Servo
  #define servoPin 3

//make Objects
  AccelStepper rotMotor(1, step1, dir1);
  AccelStepper penMotor(1, step2, dir2);
  Servo penServo; 
  SerialCommand SCmd;
// Variables
  int penMin=0;
  int penMax=0;
  int penUpPos=5;  //can be overwritten from EBB-Command SC
  int penDownPos=30; //can be overwritten from EBB-Command SC
  int servoRateUp=0; //from EBB-Protocol not implemented on machine-side
  int servoRateDown=0;//from EBB-Protocol not implemented on machine-side
  float rotStepMode=16; //1/16 by default, can be changed by EBB protocol, used as correction factor
  float penStepMode=16; //1/16 by default, can be changed by EBB protocol, used as correction factor
  int penState=penUpPos;
  uint32_t nodeCount=0;
  unsigned int layer=0;
  boolean prgButtonState=0;

void setup() {   
    Serial.begin(9600);
    makeComInterface();
    initHardware();
}

void loop() {
     SCmd.readSerial();  
}
