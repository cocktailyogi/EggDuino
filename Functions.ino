
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
	SCmd.addCommand("QB",queryButton);  //"PRG" Button,
	SCmd.setDefaultHandler(unrecognized); // Handler for command that isn't matched (says "What?")
}

void queryPen() {
	char state;
	if (penState==penUpPos)
		state='1';
	else
		state='0';
	Serial.print(String(state)+"\r\n");
	sendAck();
}

void queryButton() {
	Serial.print(String(prgButtonState) +"\r\n");
	sendAck();
	prgButtonState = 0;
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

void stepperMove() {
	uint16_t duration=0; //in ms
	int penStepsEBB=0; //Pen
	int rotStepsEBB=0; //Rot

	moveToDestination();

	if (!parseSMArgs(&duration, &penStepsEBB, &rotStepsEBB)) {
		sendError();
		return;
	}

	sendAck();

	if ( (penStepsEBB==0) && (rotStepsEBB==0) ) {
		delay(duration);
		return;
	}

	prepareMove(duration, penStepsEBB, rotStepsEBB);
}

void setPen(){
	int cmd;
	int value;
	char *arg;

	moveToDestination();

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
				break;

			default:
				sendError();
		}
	}
	char *val;
	val = SCmd.next();
	if (val != NULL) {
		value = atoi(val);
		sendAck();
		delay(value);
	}
	if (val==NULL && arg !=NULL)  {
		sendAck();
		delay(500);
	}
	//	Serial.println("delay");
	if (val==NULL && arg ==NULL)
		sendError();
}  

void togglePen(){
	int value;
	char *arg;

	moveToDestination();

	arg = SCmd.next();
	if (arg != NULL)
		value = atoi(arg);
	else
		value = 500;

	doTogglePen();
	sendAck();
	delay(value);
}

void doTogglePen() {
	if (penState==penUpPos) {
		penServo.write(penDownPos);
		penState=penDownPos;
	} else   {
		penServo.write(penUpPos);
		penState=penUpPos;
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
			case 0: motorsOff();
				sendAck();
				break;
			case 1: motorsOn();
				sendAck();
				break;
			default:
				sendError();
		}
	}
	//the following implementaion is a little bit cheated, because i did not know, how to implement different values for first and second argument.
	if ((arg != NULL) && (val != NULL)){
		switch (value) {
			case 0: motorsOff();
				sendAck();
				break;
			case 1: motorsOn();
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
				storePenDownPosInEE();
				sendAck();
				break;
			case 5: penUpPos= (int)((float) (value-6000)/(float) 133.3); // transformation from EBB to PWM-Servo
				storePenUpPosInEE();
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
