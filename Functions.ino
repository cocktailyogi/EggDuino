
void makeComInterface(){
  SCmd.addCommand("v",sendVersion); // Converts two arguments to integers and echos them back
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
  SCmd.addCommand("QB",queryButton); //preparation for "PRG" Button, actually gives fake answer, 
  SCmd.setDefaultHandler(unrecognized); // Handler for command that isn't matched (says "What?") 
  }
  
void queryButton() {
	Serial.print(String(prgButtonState) +"\r\n" + "OK\r\n");
}
  
void queryLayer() {
  Serial.print(String(layer) +"\r\n" + "OK\r\n");
  } 
  
void setLayer() {
	  uint32_t value=0;
	  char *arg1;
	  arg1 = SCmd.next();
	  if (arg1 != NULL) {
		  value = atoi(arg1);
		  layer=value;
		  Serial.print("OK\r\n");
	  }
	  else
	  Serial.print("unknown CMD\r\n");
  }
  
void queryNodeCount() {
	Serial.print(String(nodeCount) +"\r\n" + "OK\r\n");
}

void setNodeCount() {
	uint32_t value=0;
	char *arg1;
	arg1 = SCmd.next();
	if (arg1 != NULL) {
		value = atoi(arg1);
		nodeCount=value;
		Serial.print("OK\r\n");
	}
	else
		Serial.print("unknown CMD\r\n");
}

void nodeCountIncrement() {
	nodeCount=nodeCount++;
	Serial.print("OK\r\n");	
}

void nodeCountDecrement() {
	nodeCount=nodeCount--;
	Serial.print("OK\r\n");
}

void stepperMove(){
  uint16_t duration=0; //in ms
  long penStepsEBB=0; //Pen
  long rotStepsEBB=0; //Rot
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
      Serial.print("OK\r\n");
      
       if ( (penStepsEBB==0) && (rotStepsEBB==0) )
          delay(duration);
          
       if ( (penStepsEBB!=0) || (rotStepsEBB!=0) )  {
        //Move-Code:
           digitalWrite(enableRotMotor, LOW) ;
           digitalWrite(enablePenMotor, LOW) ;
		   long rotSteps = round( ((float) (rotMicrostep/rotStepMode)) * rotStepsEBB ); //transform to local coordiantes
		   long penSteps = round( ((float) (penMicrostep/penStepMode)) * penStepsEBB ); //transform to local coordiantes
		   float rotSpeed = (float) ( ((long) rotSteps * (long)1000) / (long) duration ) ;
		   float penSpeed = (float) ( ((long) penSteps * (long)1000) / (long) duration ) ;
           rotMotor.move(rotSteps);
           rotMotor.setSpeed(rotSpeed);
           penMotor.move(penSteps);
           penMotor.setSpeed( penSpeed );
           while ( penMotor.distanceToGo() || rotMotor.distanceToGo() ) { 
				   penMotor.runSpeedToPosition();
				   rotMotor.runSpeedToPosition();
			     }     
         //end Move-Code
            }
	  }
   else
      Serial.print("unknown CMD\r\n");
       
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
		  Serial.print("OK\r\n");
		  break;
		  
		  case 1:
		  penServo.write(penDownPos);
		  penState=penDownPos;
		  //Serial.println("case 1");
		  Serial.print("OK\r\n");
		  break;
		  
		  default:
		  Serial.print("unknown CMD");
	  }
  }
  char *val; 
  val = SCmd.next(); 
  if (val != NULL) {
		  value = atoi(val);
		//  Serial.println("delayvalue");
	      delay(value);
		  }
  if (val==NULL && arg !=NULL)  
			delay(500);
		//	Serial.println("delay");
  if (val==NULL && arg ==NULL)
		Serial.print("unknown CMD\r\n");
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
            Serial.print("OK\r\n");
            if (arg != NULL) 
                      delay(value);
            else   
                      delay(500);
            }
  else   {
            penServo.write(penUpPos);
            penState=penUpPos;
            Serial.print("OK\r\n");
            if (arg != NULL) 
                      delay(value);
            else   
                      delay(500);
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
               Serial.print("OK\r\n");
               break;       
       case 1: rotStepMode=16; 
               penStepMode=16;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 2: rotStepMode=8; 
               penStepMode=8;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 3: rotStepMode=4; 
               penStepMode=4;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 4: rotStepMode=2; 
               penStepMode=2;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 5: rotStepMode=1; 
               penStepMode=1;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       default:
               Serial.print("unknown CMD\r\n");
      }
}
//the following implementaion is a little bit cheated, because i did not know, how to implement different values for first and second argument.
  if ((arg != NULL) && (val != NULL)){
     switch (value) {    
       case 0: digitalWrite(enableRotMotor, HIGH) ;
               digitalWrite(enablePenMotor, HIGH) ;
               Serial.print("OK\r\n");
               break;  
       case 1: rotStepMode=16; 
               penStepMode=16;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 2: rotStepMode=8; 
               penStepMode=8;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 3: rotStepMode=4; 
               penStepMode=4;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 4: rotStepMode=2; 
               penStepMode=2;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       case 5: rotStepMode=1; 
               penStepMode=1;
               digitalWrite(enableRotMotor, LOW) ;
               digitalWrite(enablePenMotor, LOW) ;
               Serial.print("OK\r\n");
               break;
       
       default:
               Serial.print("unknown CMD\r\n");
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
       case 4: penDownPos= (int) ((float) (value-6000)/(float) 94.18); // transformation from EBB to PWM-Stepper
               Serial.print("OK\r\n");
               break;
       case 5: penUpPos= (int)((float) (value-6000)/(float) 94.18); // transformation from EBB to PWM-Stepper
               Serial.print("OK\r\n");
               break;
       case 6: //rotMin=value;    ignored
               Serial.print("OK\r\n");
               break;
       case 7: //rotMax=value;    ignored
               Serial.print("OK\r\n");
               break;
       case 11: servoRateUp=value;
                Serial.print("OK\r\n");
                break;
       case 12: servoRateDown=value;
                Serial.print("OK\r\n");
                break;
       default:
               Serial.print("unknown CMD\r\n");
      }
  }
}

void sendVersion(){
  Serial.print(initSting);
  Serial.print("\r\n");
  }
  
  void unrecognized(const char *command){
  Serial.print("unknown CMD\r\n");
  }
  
  void ignore(){
  Serial.print("OK\r\n");
  }
