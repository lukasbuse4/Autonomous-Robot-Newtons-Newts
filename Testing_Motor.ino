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
void Turn_Right();
int backR_spd = 90;
int backL_spd = 90;
int frontR_spd = 90;
int frontL_spd = 90;
int direct = 0; //for stopping logic
                //1 for forward, 2 = back, 3 = left, 4 = right


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


Servo Arm_Motor; // arm motor goes in pin 2
Servo Spool; //spool 360 servo goes in pin 9


void setup() 
{
  // put your setup code here, to run once:
  FL_Motor.attach(3);
  FR_Motor.attach(5);
  BR_Motor.attach(6);
  BL_Motor.attach(8);
  Serial.begin(9600);

  Spool.attach(9);
  Spool.write(90); //< 90 is CCW, > 90 is CW
  Arm_Motor.attach(2);
//  Arm_Motor.write(45); arm speed need to be above 90 to rotate out
}

void loop() 
{
  Arm_Motor.write(110);
  delay(500);
  Arm_Motor.write(90);
  Arm_Motor.write(70);
  delay(500);
  Arm_Motor.write(90);
  
  char customKey = customKeypad.getKey();


  
  if (customKey == '1')
  {
    Serial.println("Moving Forward");
    Move_Forward();
  }
  else if(customKey == '2')
  {
    Serial.println("Moving Backwards");
    Move_Backwards();
  }
  else if(customKey == '3')
  {
    Serial.println("Moving Right");
    Move_Right();
  }
  else if(customKey == '4')
  {
    Serial.println("Moving Left");
    Move_Left();
  }
  else if(customKey == '5')
  {
    Serial.println("Stopping");
    Stop();
  }
  else if(customKey == '6')
  {
    Serial.println("Turning");
    Turn_Right();
    delay(623); //600 is about 85º
    Stop();
  }
  else if(customKey == '7')
  {
    Arm_Motor.write(110);
    delay(500);
    Arm_Motor.write(90);
  }
  else if(customKey == '8')
  {
    Arm_Motor.write(70);
    delay(750);
    Arm_Motor.write(90);
  }
  else if(customKey == '9')
  {
    Arm_Motor.write(110);
    delay(500);
    Arm_Motor.write(90);
    
    Collect();
    Move_Backwards();
    delay(600);
    Move_Left();
    delay(100);
    Collect();
    Stop();
    
  }
  
}


void Collect()
{
    Move_Forward();
    delay(750);
    Arm_Motor.write(70);
    delay(750);
    Arm_Motor.write(90);
    Arm_Motor.write(110);
    delay(1000);
    Arm_Motor.write(90);
}

//0 spd is 90
//Right move same, back move same
void Move_Forward()
{
  int spd = 70;
  frontR_spd = spd + 7;
  backR_spd = spd + 7;
  
  frontL_spd = (180 - spd);
  backL_spd = (180 - spd);
  
  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
  direct = 1;
}


//0 spd is 90
//Right wheels move same direction, left wheels move same direction
void Move_Backwards()
{
  //This works at about 12.4V
  int spd = 70;
  frontR_spd = (180 - spd) + 7;
  backR_spd = (180 - spd) + 6;
  
  frontL_spd = spd;
  backL_spd = spd;
  
  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
  direct = 2;
}

//0 spd is 90
//Front move same, back move same
void Move_Left()
{
  int spd = 70;
  frontR_spd = spd + 5;
  frontL_spd = spd;
  
  backR_spd = (180 - spd);
  backL_spd = (180 - spd) + 4;

  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
  direct = 3;
}

//0 spd is 90
//Front move same, back move same
void Move_Right()
{
  int spd = 70;
  frontR_spd = (180 - spd) + 5;
  frontL_spd = (180 - spd);
  
  backR_spd = spd + 2;
  backL_spd = spd + 1;

  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);
  direct = 4;
}

void Stop()
{
  if(direct == 1)
  {
    frontR_spd = 110;
    backR_spd = 110;
    frontL_spd = 110;
    backL_spd = 110;
    
    FR_Motor.write(frontR_spd);
    FL_Motor.write(frontL_spd);

    BL_Motor.write(backL_spd);
    BR_Motor.write(backR_spd);
    delay(20);
  }
  else if(direct == 2)
  {
    frontR_spd = 110;
    backR_spd = 110;
    frontL_spd = 110;
    backL_spd = 110;
    FR_Motor.write(frontR_spd);
    FL_Motor.write(frontL_spd);

    BL_Motor.write(backL_spd);
    BR_Motor.write(backR_spd);
    delay(25);
  }
  else if(direct == 3)
  {
    frontR_spd = 110;
    backR_spd = 110;
    frontL_spd = 110;
    backL_spd = 110;
    
    FR_Motor.write(frontR_spd);
    FL_Motor.write(frontL_spd);

    BL_Motor.write(backL_spd);
    BR_Motor.write(backR_spd);
    delay(20);
  }
  else if(direct == 4)
  {
    frontR_spd = 110;
    backR_spd = 110;
    frontL_spd = 110;
    backL_spd = 110;
    FR_Motor.write(frontR_spd);
    FL_Motor.write(frontL_spd);

    BL_Motor.write(backL_spd);
    BR_Motor.write(backR_spd);
    delay(20);
  }
  else
  {
    frontR_spd = 90;
    backR_spd = 90;
    frontL_spd = 90;
    frontL_spd = 90;
  }
  
  FR_Motor.write(90);
  FL_Motor.write(90);

  BL_Motor.write(90);
  BR_Motor.write(90);
  direct = 0;
}

void Turn_Right()
{
  int spd = 70;
  
  FR_Motor.write(spd);
  FL_Motor.write(spd);
  
  BR_Motor.write(spd);
  BL_Motor.write(spd);
}
