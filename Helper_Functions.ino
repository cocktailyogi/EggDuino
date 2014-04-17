void initHardware(){
  pinMode(enableRotMotor, OUTPUT); 
  pinMode(enablePenMotor, OUTPUT);  
  rotMotor.setMaxSpeed(1000.0);
  rotMotor.setAcceleration(10000.0);
  penMotor.setMaxSpeed(1000.0);
  penMotor.setAcceleration(10000.0);
  digitalWrite(enableRotMotor, HIGH) ;
  digitalWrite(enablePenMotor, HIGH) ;
  penServo.attach(servoPin);
  penServo.write(penUpPos);
  }
