// This file works.
// Connect the board to both your computer and the circuit
// Open in Arduino IDE as it is easiest to upload onto the board from there
// Press save then Upload to board, should be second button in top left.
// After uploading to board please reset board
// Turn power on in the circuit and watch the magic happen

#include <Servo.h>

Servo servo1;

void setup() 
{
  // put your setup code here, to run once:
  servo1.attach(2);
  //Serial.begin(9600);
  servo1.write(0);
  
 
}

void loop() 
{

  
  servo1.write(0);
  delay(2000);

  servo1.write(45);
  delay(2000);
  servo1.write(90);
  delay(2000);
  servo1.write(135);
  delay(2000);
  servo1.write(180);
  delay(2000);
  servo1.write(0);
  delay(2000);
  servo1.write(180);
  delay(2000);
  
//  // put your main code here, to run repeatedly:
//  int sensorvalue = analogRead(2);
//  double voltage = sensorvalue * (5.0 / 1023);
//  Serial.println(voltage);
//  servo1.write(0);
//  delay(1000);
//  sensorvalue = analogRead(2);
//  voltage = sensorvalue * (5.0 / 1023);
//  Serial.println(voltage);
//  servo1.write(180);
//  delay(1000);
}
