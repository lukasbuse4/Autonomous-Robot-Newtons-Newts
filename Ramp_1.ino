#include <Keypad.h>
#include <Servo.h>

#include <infraredsensor.h>
int IRSensorPin = 33;

void Deploy_Probe();
int Get_Distance(int, int); //passes in pins you want to use to take measurement
void Left_Correction(int);
void Right_Correction(int);
void around_wall();


const byte ROWS = 4; 
const byte COLS = 4; 

void Move_Forward();
void Move_Backwards();
void Move_Right();
void Move_Left();
void Stop();
void Turn_Right();
void Turn_Left();
int backR_spd = 90;
int backL_spd = 90;
int frontR_spd = 90;
int frontL_spd = 90;
int direct = 0; //for stopping logic
                //1 for forward, 2 = back, 3 = left, 4 = right
double getInclination();

#include <conductivity.h>
double SalinityCalibrationFunction(int);

#include <LiquidCrystal_I2C.h>

//no clue why this works but it does
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//used for maze logic
int delay_cntr = 0;


//Keypad Setup
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {52, 50, 48, 46}; //blue goes in 38
byte colPins[COLS] = {44, 42, 40, 38}; //purple goes in 52

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


//Right wheel setup
Servo FR_Motor; //Front Right goes in pin 5
Servo FL_Motor; //Front Left goes in pin 3
Servo BR_Motor; //Back Right goes in pin 6
Servo BL_Motor; //Back Left goes in pin 8

Servo Arm_Motor; // arm motor goes in pin 2
Servo Spool; //spool 360 servo goes in pin 9


//Ping sensor setup


// defines variables
int distance;
long duration; // variable for the duration of sound wave travel

void setup() 
{
  
  pinMode(10, OUTPUT); // Sets the trigPin(11) as an OUTPUT
  pinMode(11, INPUT); // Sets the echoPin(10) as an INPUT

  //need to add back sensor with different pins (basically uncomment below and change 10/11)
  pinMode(31, OUTPUT); // Sets the trigPin(11) as an OUTPUT
  pinMode(33, INPUT); // Sets the echoPin(10) as an INPUT

  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  //Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  //Serial.println("with Arduino UNO R3");

  //FL goes in pin 3
  FL_Motor.attach(3);

  //FR goes in 5
  FR_Motor.attach(5);

  //BR goes in 6
  BR_Motor.attach(6);

  //BL goes in 8
  BL_Motor.attach(8);

  //The 360 servo goes in pin 9
  Spool.attach(9);

  //starts moving automatically so set to spd 90 (no movement)
  Spool.write(90);   //< 90 is CCW, > 90 is CW

  //attach the salamander picker upper to pin 2
  Arm_Motor.attach(2);


  lcd.begin(16, 2); // initialize the lcd. Don't worry about the int values above.
  //lcd.backlight();
  lcd.home();       // go to the top line  
  lcd.setCursor(0, 0);
}

void loop() 
{
  //gets key input
  char customKey = customKeypad.getKey();

  //do if statements and run code based on input
  if (customKey == '1')
  {
    
    
  }
  else if(customKey == '2')
  {
    lcd.print("This thing does work");
//    Move_Forward();
//    delay(2000);
//    Stop();
  }
  else if(customKey == '3')
  {
    Deploy_Probe();
    double salinity = Get_Salinity();
    Serial.print("Salinity: ");
    Serial.print(salinity);
    lcd.setCursor(0, 0);
    lcd.print("Salinity: ");
    lcd.print(salinity);
    lcd.print("%");
    
  }
  else if(customKey == '4')
  {
    double inclination = Get_Inclination();
    lcd.setCursor(0, 1);       
    lcd.print("Incline: ");
    lcd.print(inclination);
  }
  else if(customKey == '5')
  {
    
  }
  else if(customKey == '6')
  {
    Around_Wall();
    Move_Right();
    delay(4750);
    Turn_Right();
    delay(200);
    Turn_Left();
    delay(200);
    Move_Forward();
    delay(5000);
    delay(3000);
    Stop();
    double inclination = Get_Inclination();
    lcd.setCursor(0, 0);       
    lcd.print("Incline: ");
    lcd.print(inclination);
    Move_Forward();
    delay(3500);
    Turn_Left();
    delay(1000);
//    Move_Forward();
//    delay(150);
    Stop();
    Deploy_Probe();
    delay(1000);
    double salinity = Get_Salinity();
    lcd.setCursor(0, 1);
    lcd.print("Salinity: ");
    lcd.print(salinity);
    lcd.print("%");
  }
  else if(customKey == '7')
  {
    Move_Left();
    while(true)
    {
      customKey = customKeypad.getKey();
      if(customKey == '8')
      {
        break;
      }
      Left_Correction(20);
      delay(20);
    }
  }
  else if(customKey == '9')
  {
    Move_Forward();
  }
}

//calibrated and should work properly
double Get_Inclination()
{
    const int conductivityAnalogPin1 = 1;

    int reading;
    
    reading = analogRead(conductivityAnalogPin1);

    double slope = 0.2390276187;
    double inter = -108.2381394;
    double angle = (slope * reading) + inter;

    return angle;
}

//return the salinity
double Get_Salinity()
{
  int con = getConductivity();
  double salin = SalinityCalibrationFunction(con);
  return salin;
}


//this is good for the time being, probably needs to recalibrated sooner or later after cleaning
double SalinityCalibrationFunction(int reading)
{
  double slope = 0.02993;
  double intercept = -6.76356;
  double answer = (1 / (slope * reading + intercept));
  return answer;
}

//basically 
void Deploy_Probe()
{
  Spool.write(120);
  delay(2000);
  Spool.write(90);
}

//basically same logic as wall
void Around_Wall()
{
  //move forward until within 20cm of wall
  Move_Forward();
  distance = 1000;
  while(true)
  { 
    distance = Get_Distance(10,11);
    if(distance < 20)
    {
      break;
    }

  }

  //need to edit direction depending on starting box location (depending on what course)
  Move_Left();

  //ADD Correction logic, probably easiest to create function (and have it use the "direct" variable

  
  int cntr = 0;
  while(true)
  {
    distance = Get_Distance(10,11); // Speed of sound wave divided by 2 (go and back)

    //take multiple measurements to make sure the hole is found
    if(distance > 70)
    {
      cntr++;
    }
    if(cntr >= 20)
    {
      Stop();
      
      break;
      
    }
  }
  Move_Forward();
  delay(3000);
}

//returns distance depending on what ping sensor you want to use (
int Get_Distance(int trigPin, int echoPin)
{
    //pulled this from online. It works
    int distance;
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    return distance;
}


//Movement calibration

//Movement is confusing for the robot, so hopefully my comments help
//Note: Motors are still when at 90 spd


//Right move same, left move same
void Move_Forward()
{
  //if drifting left decrease right spds by making it closer to 90
  //if drifting right decrease left speed (need to subtract more) - alternative make the right side faster (decrease the additions)
  int spd = 70;
  frontR_spd = spd + 7;
  backR_spd = spd + 7;
  
  frontL_spd = (180 - spd);
  backL_spd = (180 - spd);
  
  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);

  //direction used for stopping logic
  direct = 1;
}


//0 spd is 90
//Right wheels move same direction, left wheels move same direction
void Move_Backwards()
{

  //if drifting left decrease right spds by making it closer to 90
  //if drifting right decrease left speed (need to subtract more) - alternative make the right side faster (decrease the additions
  int spd = 70;
  frontR_spd = (180 - spd) + 7;
  backR_spd = (180 - spd) + 6;
  
  frontL_spd = spd;
  backL_spd = spd;
  
  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);

  //used for stopping logic
  direct = 2;
}

//0 spd is 90
//Front move same, back move same
void Move_Left()
{
  
  int spd = 70;
  frontR_spd = spd + 5;
  frontL_spd = spd + 5;
  
  backR_spd = (180 - spd);
  backL_spd = (180 - spd) + 2;

  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);

  //used for stopping logic
  direct = 3;
}

//0 spd is 90
//Front move same, back move same
void Move_Right()
{
  int spd = 70;
  frontR_spd = (180 - spd) + 5;
  frontL_spd = (180 - spd) + 3;
  
  backR_spd = spd - 3;
  backL_spd = spd + 3;

  FR_Motor.write(frontR_spd);
  FL_Motor.write(frontL_spd);
  
  BR_Motor.write(backR_spd);
  BL_Motor.write(backL_spd);

  //used for stopping logic
  direct = 4;
}


//When stopping the robot tends to rotate left
void Stop()
{

  //from moving forward basically make it turn right to counteract the left drift
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

    //edit this delay if stop is not working from moving forward.
    //make larger if starts turning left, smaller if starts turning right
    delay(20);
  }
  //from moving backward basically make it turn right to counteract the left drift
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
    
    //edit this delay if stop is not working from moving forward.
    //make larger if starts turning left, smaller if starts turning right
    delay(25);
  }
  //from moving left basically make it turn right to counteract the left drift
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
    
    //edit this delay if stop is not working from moving forward.
    //make larger if starts turning left, smaller if starts turning right
    delay(20);
  }

  //moving right
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
    
    //edit this delay if stop is not working from moving forward.
    //make larger if starts turning left, smaller if starts turning right
    delay(20);
  }
  //already stopped then make it stopped again, don't know why you would do Stop 
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

//This turns left
//delay(623ish to get 90º)
void Turn_Left()
{
  //make everything 
  int spd = 70;
  
  FR_Motor.write(spd);
  FL_Motor.write(spd);
  
  BR_Motor.write(spd);
  BL_Motor.write(spd);
  
}

//delay(623ish to get 90º)
void Turn_Right()
{
  int spd = 110;
  
  FR_Motor.write(spd);
  FL_Motor.write(spd);
  
  BR_Motor.write(spd);
  BL_Motor.write(spd);
}

//while moving left it sometimes drifts
void Left_Correction(int ideal_distance)
{
  //This is extremely hard to fine tune. May be best to scrap the idea

  //May want to trasition to have it rotate left or right then back up slightly if it gets within 15cm of wall
  //Just a thought

  
    int dist = Get_Distance(10, 11);
    //error correction, since moving left frontL and backR needs to slow down if getting closer
    if((dist < ideal_distance) && (frontL_spd != 92) && (backR_spd != 88) && (backL_spd != 92) && (frontR_spd != 88))
    {
      //decrease frontL, increase backR(if getting closer) basically make it rotate left
      frontL_spd -= 2;
      backR_spd += 2;
      backL_spd += 2;
      frontR_spd -= 2;
      FL_Motor.write(frontL_spd);
      BR_Motor.write(backR_spd);
      FR_Motor.write(frontR_spd);
      BL_Motor.write(backL_spd);
    }
    //same logic as above but backward
    if((dist > ideal_distance) && (frontL_spd != 88) && (backR_spd != 92) && (backL_spd != 88) && (frontR_spd != 92))
    {
      frontL_spd += 2;
      backR_spd -= 2;
      backL_spd -= 2;
      frontR_spd += 2;
      FL_Motor.write(frontL_spd);
      BR_Motor.write(backR_spd);
      FR_Motor.write(frontR_spd);
      BL_Motor.write(backL_spd);
    }
//    if(frontL_spd == 90)
//    {
//      frontL_spd = 75;
//    }
//    if(backL_spd == 90)
//    {
//      backL_spd = 75;
//    }
//    if(frontR_spd == 90)
//    {
//      frontR_spd = 75;
//    }
//    if(backL_spd == 90)
//    {
//      backL_spd = 112;
//    }

    //move left
    //FL = 75, FR = 75
    //BL = 112, BR = 110
}
