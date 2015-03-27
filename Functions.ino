
void makeComInterface(){
  SCmd.addCommand("v",sendVersion); 
  SCmd.addCommand("EM",enableMotors);
  SCmd.addCommand("SC",stepperModeConfigure);
  SCmd.addCommand("SP",setPen);
  SCmd.addCommand("SM",stepperMove);
  SCmd.addCommand("SE",ignore);
  SCmd.addCommand("TP",togglePen);
  SCmd.addCommand("PO",ignore);    //Engraver command, not implemented, gives fake answer
  SCmd.addCommand("NI",nodeCountIncrement);
  SCmd.addCommand("ND",nodeCountDecrement);
  SCmd.addCommand("SN",setNodeCount);
  SCmd.addCommand("QN",queryNodeCount);
  SCmd.addCommand("SL",setLayer);
  SCmd.addCommand("QL",queryLayer);
  SCmd.addCommand("QP",queryPen);
  SCmd.addCommand("QB",queryButton); //preparation for "PRG" Button, actually gives fake answer, 
  SCmd.setDefaultHandler(unrecognized); // Handler for command that isn't matched (says "What?") 
  }

void queryPen() {
	char state;
	if (penState=penUpPos)
		state='1';
	else
		state='0';
	Serial.print(String(state)+"\r\n");
	sendAck();
}

void queryButton() {
	Serial.print(String(prgButtonState) +"\r\n");
	sendAck();
}
  
void queryLayer() {
  Serial.print(String(layer) +"\r\n");
  sendAck();
  } 
  
void setLayer() {
	  uint32_t value=0;
	  char *arg1;
	  arg1 = SCmd.next();
	  if (arg1 != NULL) {
		  value = atoi(arg1);
		  layer=value;
		  sendAck();
	  }
	  else
	  sendError();
  }
  
void queryNodeCount() {
    Serial.print(String(nodeCount) +"\r\n");
	sendAck();
	
}

void setNodeCount() {
	uint32_t value=0;
	char *arg1;
	arg1 = SCmd.next();
	if (arg1 != NULL) {
		value = atoi(arg1);
		nodeCount=value;
		sendAck();
	}
	else
		sendError();
}

void nodeCountIncrement() {
	nodeCount=nodeCount++;
	sendAck();	
}

void nodeCountDecrement() {
	nodeCount=nodeCount--;
	sendAck();
}

void stepperMove(){
  uint16_t duration=0; //in ms
  int penStepsEBB=0; //Pen
  int rotStepsEBB=0; //Rot
  char *arg1;
  char *arg2;
  char *arg3;
  arg1 = SCmd.next();
  if (arg1 != NULL) {
      duration = atoi(arg1);
      arg2 = SCmd.next();
      }
   if (arg2 != NULL) {
      penStepsEBB = atoi(arg2);
      arg3 = SCmd.next();
      }
   if (arg3 != NULL) {
      rotStepsEBB = atoi(arg3);
	  //sendAck();
      
       if ( (penStepsEBB==0) && (rotStepsEBB==0) ) {
          delay(duration);
		  sendAck();
	   }
       if ( (penStepsEBB!=0) || (rotStepsEBB!=0) )  {
//################### Move-Code Start ############################################################
           //Turn on Motors, if they are off....
		   digitalWrite(enableRotMotor, LOW) ;
                   digitalWrite(enablePenMotor, LOW) ;
		   //incoming EBB-Steps will be multiplied by 16, then Integer-maths is done, result will be divided by 16
		   // This make thinks here really complicated, but floating point-math kills performance and memory, believe me... I tried...
		   long rotSteps =   (  (long)rotStepsEBB * 16 / rotStepCorrection) + (long)rotStepError;	//correct incoming EBB-Steps to our microstep-Setting and multiply  by 16 to avoid floatingpoint...
		   long penSteps =   (  (long)penStepsEBB * 16 / penStepCorrection) + (long)penStepError;
		   int rotStepsToGo = (int) (rotSteps/16);		//Calc Steps to go, which are possible on our machine
		   int penStepsToGo = (int) (penSteps/16);
		   rotStepError = (long)rotSteps - ((long) rotStepsToGo * (long)16);	// calc Position-Error, if there is one
		   penStepError = (long)penSteps - ((long) penStepsToGo * (long)16);
		   long temp_rotSpeed =  ((long)rotStepsToGo * (long)1000 / (long)duration );	// calc Speed in Integer Math
		   long temp_penSpeed =  ((long)penStepsToGo * (long)1000 / (long)duration ) ;
		   if (temp_rotSpeed <0 ) //remove sign, there is no negative Speed....
			   temp_rotSpeed= -temp_rotSpeed;
		   if (temp_penSpeed <0 ) //remove sign, there is no negative Speed....
			   temp_penSpeed= -temp_penSpeed;
		   float rotSpeed= (float) temp_rotSpeed;	// type cast 
		   float penSpeed= (float) temp_penSpeed;
           rotMotor.move(rotStepsToGo);		// finally, let us set the target position...
           rotMotor.setSpeed(rotSpeed);		// and the Speed!
           penMotor.move(penStepsToGo);
           penMotor.setSpeed( penSpeed );
           while ( penMotor.distanceToGo() || rotMotor.distanceToGo() ) { 
				   penMotor.runSpeedToPosition(); // Moving.... moving... moving....
				   rotMotor.runSpeedToPosition();
			     }
//################### Move-Code End ############################################################
		   sendAck();     //Mission completed
            }
	  }
   else
      sendError();
       
}

  
void setPen(){
  int cmd;
  int value;
  char *arg;
  arg = SCmd.next(); 
  if (arg != NULL) {
      cmd = atoi(arg);
	  switch (cmd) {
		  case 0:
		  penServo.write(penUpPos);
		  penState=penUpPos;
		  break;
		  
		  case 1:
		  penServo.write(penDownPos);
		  penState=penDownPos;
		  //Serial.println("case 1");
		  break;
		  
		  default:
		  sendError();
	  }
  }
  char *val; 
  val = SCmd.next(); 
  if (val != NULL) {
		  value = atoi(val);
		//  Serial.println("delayvalue");
	      delay(value);
		  sendAck();
		  }
  if (val==NULL && arg !=NULL)  
			delay(500);
			sendAck();
		//	Serial.println("delay");
  if (val==NULL && arg ==NULL)
			sendError();
}  

void togglePen(){
  int value;
  char *arg;
  arg = SCmd.next(); 
  if (arg != NULL) 
      value = atoi(arg);
  if (penState==penUpPos) {
            penServo.write(penDownPos);
            penState=penDownPos;
            if (arg != NULL) 
                      delay(value);
            else   
                      delay(500);
			sendAck();
            }
			
  else   {
            penServo.write(penUpPos);
            penState=penUpPos;
            if (arg != NULL) 
                      delay(value);
            else   
                      delay(500);
		    sendAck();
        }    
}  
  
  
void enableMotors(){
  int cmd;
  int value;
  char *arg;
  char *val; 
  arg = SCmd.next(); 
  if (arg != NULL) 
      cmd = atoi(arg);
  val = SCmd.next(); 
  if (val != NULL) 
      value = atoi(val);
      //values parsed
  if ((arg != NULL) && (val == NULL)){
     switch (cmd) { 
       case 0: digitalWrite(enableRotMotor, HIGH) ;
               digitalWrite(enablePenMotor, HIGH) ;
               sendAck();
               break;       
       case 1: rotStepMode=16; 
               penStepMode=16;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 2: rotStepMode=8; 
               penStepMode=8;
			   rotStepCorrection = rotStepMode/rotMicrostep;
			   penStepCorrection = penStepMode/penMicrostep;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 3: rotStepMode=4; 
               penStepMode=4;
			   rotStepCorrection = rotStepMode/rotMicrostep;
			   penStepCorrection = penStepMode/penMicrostep;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 4: rotStepMode=2; 
               penStepMode=2;
			   rotStepCorrection = rotStepMode/rotMicrostep;
			   penStepCorrection = penStepMode/penMicrostep;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 5: rotStepMode=1; 
               penStepMode=1;
			   rotStepCorrection = rotStepMode/rotMicrostep;
			   penStepCorrection = penStepMode/penMicrostep;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       default:
               sendError();
      }
}
//the following implementaion is a little bit cheated, because i did not know, how to implement different values for first and second argument.
  if ((arg != NULL) && (val != NULL)){
     switch (value) {    
       case 0: digitalWrite(enableRotMotor, HIGH) ;
               digitalWrite(enablePenMotor, HIGH) ;
               sendAck();
               break;  
       case 1: rotStepMode=16; 
               penStepMode=16;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 2: rotStepMode=8; 
               penStepMode=8;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 3: rotStepMode=4; 
               penStepMode=4;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 4: rotStepMode=2; 
               penStepMode=2;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       case 5: rotStepMode=1; 
               penStepMode=1;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               sendAck();
               break;
       
       default:
               sendError();
      }
  }
}

  
void stepperModeConfigure(){
  int cmd;
  int value;
  char *arg;
  arg = SCmd.next(); 
  if (arg != NULL) 
      cmd = atoi(arg);
  char *val; 
  val = SCmd.next(); 
  if (val != NULL) 
      value = atoi(val);
  if ((arg != NULL) && (val != NULL)){
     switch (cmd) {      
       case 4: penDownPos= (int) ((float) (value-6000)/(float) 133.3); // transformation from EBB to PWM-Servo
               sendAck();
               break;
       case 5: penUpPos= (int)((float) (value-6000)/(float) 133.3); // transformation from EBB to PWM-Servo
               sendAck();
               break;
       case 6: //rotMin=value;    ignored
               sendAck();
               break;
       case 7: //rotMax=value;    ignored
               sendAck();
               break;
       case 11: servoRateUp=value;
                sendAck();
                break;
       case 12: servoRateDown=value;
                sendAck();
                break;
       default:
               sendError();
      }
  }
}

void sendVersion(){
  Serial.print(initSting);
  Serial.print("\r\n");
  }
  
  void unrecognized(const char *command){
  sendError();
  }
  
  void ignore(){
  sendAck();
  }
