/* Eggduino-Firmware by Joachim Cerny, 2014

Thanks for the nice libs ACCELSTEPPER and SERIALCOMMAND, which made this project much easier.
Thanks to the Eggbot-Team for such a funny and enjoable concept!
Thanks to my wife and my daughter for their patience. :-)

*/

// implemented Eggbot-Protocol-Version v13
// EBB-Command-Reference, I sourced from: http://www.schmalzhaus.com/EBB/EBBCommands.html
// no homing sequence, switch-on position of pen will be taken as reference point.
// No collision-detection!!
// Supported Servos: I do not know, I use Arduino Servo Lib with TG9e- standard servo.
// Note: Maximum-Speed in Inkscape is 1000 Steps/s. You could enter more, but then Pythonscript sends nonsense.
// EBB-Coordinates are coming in for 16th-Microstepmode. The Coordinate-Transforms are done in weired integer-math. Be careful, when you diecide to modify settings.

/* TODOs:
1	collision control via penMin/penMax
2	implement homing sequence via microswitch or optical device
3	implement hardware-button , EGGBOT-Guys call it "PRG-Button"
*/
#include "AccelStepper.h"
//#include "libs\AccelStepper.h" // nice lib from http://www.airspayce.com/mikem/arduino/AccelStepper/
#include <Servo.h>
#include "SerialCommand.h" //nice lib from Stefan Rado, https://github.com/kroimon/Arduino-SerialCommand

#define initSting "EBBv13_and_above Protocol emulated by Eggduino-Firmware V1.3"
//Rotational Stepper
  #define step1 11
  #define dir1 10
  #define enableRotMotor 9
  #define rotMicrostep 16  //MicrostepMode, only 1,2,4,8,16 allowed, because of Integer-Math in this Sketch
//Pen Stepper
  #define step2 8
  #define dir2 7
  #define enablePenMotor 6
  #define penMicrostep 16 //MicrostepMode, only 1,2,4,8,16 allowed, because of Integer-Math in this Sketch
//Servo
  #define servoPin 3

//make Objects
  AccelStepper rotMotor(1, step1, dir1);
  AccelStepper penMotor(1, step2, dir2);
  Servo penServo; 
  SerialCommand SCmd;
  
// Variables... be careful, by messing around here, evrything has a reason and crossrelations...
  int penMin=0;
  int penMax=0;
  int penUpPos=5;  //can be overwritten from EBB-Command SC
  int penDownPos=30; //can be overwritten from EBB-Command SC
  int servoRateUp=0; //from EBB-Protocol not implemented on machine-side
  int servoRateDown=0;//from EBB-Protocol not implemented on machine-side
  uint8_t rotStepMode=16; //1/16 by default, can be changed by EBB protocol, used as correction factor
  uint8_t penStepMode=16; //1/16 by default, can be changed by EBB protocol, used as correction factor
  long rotStepError=0;
  long penStepError=0;
  int penState=penUpPos;
  uint32_t nodeCount=0;
  unsigned int layer=0;
  int prgButtonState=0;
  uint8_t rotStepCorrection = rotStepMode/rotMicrostep ; //devide EBB-Coordinates by this factor to get EGGduino-Steps
  uint8_t penStepCorrection = penStepMode/penMicrostep ; //devide EBB-Coordinates by this factor to get EGGduino-Steps
  float rotSpeed=0; 
  float penSpeed=0; // these are local variables for Function SteppermotorMove-Command, but for performance-reasons it will be initialized here

void setup() {   
    Serial.begin(9600);
    makeComInterface();
    initHardware();
}

void loop() {
     SCmd.readSerial();  
}
