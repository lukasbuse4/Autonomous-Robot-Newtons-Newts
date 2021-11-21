// This file works.
// Connect the board to both your computer and the circuit
// Open in Arduino IDE as it is easiest to upload onto the board from there
// Press save then Upload to board, should be second button in top left.
// After uploading to board please reset board
// Turn power on in the circuit and watch the magic happen


//Functions
void Move_Forward();
void Move_Backwards();
void Move_Right();
void Move_Left();
void Stop();



//Keypad setup
#include <Keypad.h>
const byte ROWS = 4; 
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {52, 50, 48, 46}; //blue goes in 52
byte colPins[COLS] = {44, 42, 40, 38}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


//Wheels setup

#include <Servo.h>

Servo FR_Motor; //Front Right goes in pin 5
Servo FL_Motor; //Front Left goes in pin 3
Servo BR_Motor; //Back Right goes in pin 6
Servo BL_Motor; //Back Left goes in pin 8

void setup() 
{
  // put your setup code here, to run once:
  FL_Motor.attach(3);
  FR_Motor.attach(5);
  BR_Motor.attach(6);
  BL_Motor.attach(8);
  Serial.begin(9600);
}

void loop() 
{
  char customKey = customKeypad.getKey();
  if (customKey == '1')
  {
    Move_Forward();
    delay(1000);
    Move_Left();
    delay(6500);
    Move_Forward();
    delay(2000);
    Move_Right();
    delay(6500);
    Stop();
    }
}

//0 spd is 90
//Right move same, back move same
void Move_Forward()
{
  int spd = 70;
  int frontR_spd = spd + 7;
  int backR_spd = spd + 6;
  
  int frontL_spd = (180 - spd);
  int backL_spd = (180 - spd);
  
  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
}

//0 spd is 90
//Right wheels move same direction, left wheels move same direction


void Move_Backwards()
{
  //This works at about 12.4V
  int spd = 70;
  int frontR_spd = (180 - spd);
  int backR_spd = (180 - spd);
  
  int frontL_spd = spd;
  int backL_spd = spd;
  
  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
}

//0 spd is 90
//Front move same, back move same
void Move_Left()
{
  int spd = 70;
  int frontR_spd = spd + 5;
  int frontL_spd = spd;
  
  int backR_spd = (180 - spd);
  int backL_spd = (180 - spd) + 4;

  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
}

//0 spd is 90
//Front move same, back move same
void Move_Right()
{
  int spd = 70;
  int frontR_spd = (180 - spd);
  int frontL_spd = (180 - spd);
  
  int backR_spd = spd;
  int backL_spd = spd;

  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
}

void Stop()
{
  FR_Motor.write(90);
  FL_Motor.write(90);
  
  BR_Motor.write(90);
  BL_Motor.write(90);
}
