#include <infraredsensor.h>
#include <KNWRobot.h>
int IRSensorPin = 33;
KNWRobot* myRobot;

void setup()
{
  Serial.begin(9600);
  
}

void loop()
{
   // Now print the reading on the next line of the LCD
  int numCharsReadFromIR = scanIR(IRSensorPin);
  myRobot->printLCD("Chars read: ");
  myRobot->printLCD(charactersReadFromIR);
  char* IRCharacters = getIR();
  myRobot->moveCursor(0, 1);
  myRobot->printLCD(IRCharacters);

  
  delay(1000);
}
