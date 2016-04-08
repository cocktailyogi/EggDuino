
void makeComInterface(){
	SCmd.addCommand("v",sendVersion);
	SCmd.addCommand("EM",enableMotors);
	SCmd.addCommand("SC",stepperModeConfigure);
	SCmd.addCommand("SP",setPen);
	SCmd.addCommand("SM",stepperMove);
	SCmd.addCommand("SE",setEngraver);
	SCmd.addCommand("TP",togglePen);
	SCmd.addCommand("PD",ignore);
	SCmd.addCommand("PO",pinOutput);
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
		state='0';
	else
		state='1';
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


void setPenUp() {
	penServo.write(penUpPos, servoRateUp, true);
	penState=penUpPos;
}


void setPenDown() {
	penServo.write(penDownPos, servoRateDown, true);
	penState=penDownPos;
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
        sendAck();
				setPenDown();
				break;

			case 1:
        sendAck();
				setPenUp();        
				break;

			default:
				sendError();
		}
	}
	char *val;
	val = SCmd.next();
	if (val != NULL) {
		value = atoi(val);
		delay(value);
	}
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
		setPenDown();
	} else   {
		setPenUp();
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
			case 4: penUpPos= (int) ((float) (value-6000)/(float) 133.3); // transformation from EBB to PWM-Servo
				storePenUpPosInEE();
				sendAck();
				break;
			case 5: penDownPos= (int)((float) (value-6000)/(float) 133.3); // transformation from EBB to PWM-Servo
				storePenDownPosInEE();
				sendAck();
				break;
			case 6: //rotMin=value;    ignored
				sendAck();
				break;
			case 7: //rotMax=value;    ignored
				sendAck();
				break;
			case 11: servoRateUp=value / 5;
        eeprom_update_word(penUpRateEEAddress, servoRateUp);
				sendAck();
				break;
			case 12: servoRateDown=value / 5;
			  eeprom_update_word(penDownRateEEAddress, servoRateDown);     
				sendAck();
				break;
			default:
				 sendError();
		}
	}
}

void pinOutput(){
	char *arg1;
	char *arg2;
	char *arg3;
	int val;

	arg1 = SCmd.next();
	arg2 = SCmd.next();
	arg3 = SCmd.next();
	if (arg1 == NULL || arg2 == NULL || arg3 == NULL) {
		sendError();
		return;
	}
	//PO,B,3,0 = disable engraver
	//PO,B,3,1 = enable engraver
	if (arg1[0] == 'B' && arg2[0] == '3') {
		val = atoi(arg3);
		digitalWrite(engraverPin, val);
	}
	sendAck();
}

//currently inkscape extension is using PO command for engraver instead of SE
void setEngraver(){
	char *arg;
	int val;
	arg = SCmd.next();
	if (arg != NULL) {
		val = atoi(arg);
		digitalWrite(engraverPin, val);
	}
	sendAck();
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
