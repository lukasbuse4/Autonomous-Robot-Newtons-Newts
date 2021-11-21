#include <Keypad.h>
#include <Servo.h>

#include <infraredsensor.h>
int IRSensorPin = 33;

void navigate_maze();
void find_hole(bool);
void reset_pos(int);
void arm_down();
void Find_Salamanders();
void Collect_Salamanders();
void arm_up();
void Deploy_Probe();

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
double myCalibrationFunction(int);

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


//int delay_cntr


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
Servo FL_Motor; //Front Left goes in pin 4
Servo BR_Motor; //Back Right goes in pin 6
Servo BL_Motor; //Back Left goes in pin 8

Servo Arm_Motor; // arm motor goes in pin 9
Servo Spool; //spool 360 servo goes in pin 2


//Ping sensor setup
#define echoPin 11 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 10 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance = 1000; // variable for the distance measurement

void setup() 
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  //Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  //Serial.println("with Arduino UNO R3");
  
  FL_Motor.attach(3);
  FR_Motor.attach(5);
  BR_Motor.attach(6);
  BL_Motor.attach(8);

  Spool.attach(9);
  Spool.write(90);   //< 90 is CCW, > 90 is CW
  Arm_Motor.attach(2);


  lcd.begin(16, 2); // initialize the lcd. Don't worry about the int values above.
  lcd.backlight();
  lcd.home();       // go to the top line  
  lcd.setCursor(0, 0);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  char customKey = customKeypad.getKey();
  
  if (customKey == '1')
  {
    navigate_maze();
  }
  else if(customKey == '3')
  {
    Find_Salamanders();
  }
  else if(customKey == '4')
  {
    Deploy_Probe();
    
  }
  else if(customKey == '5')
  {
    double inclination = Get_Inclination();
    lcd.setCursor(0, 1);       
    lcd.print("Incline: ");
    lcd.print(inclination);
  }

  
  else if(customKey == '6')
  {
    double salinity = Get_Salinity();
    lcd.setCursor(0, 0);
    lcd.print("Salinity: ");
    lcd.print(salinity);
    lcd.print("%");
  }

  
  else if(customKey == '7')
  {
    Around_Wall();
    Move_Right();
    delay(4500);
    Move_Forward();
    delay(6500);
    Stop();
    double inclination = Get_Inclination();
    lcd.setCursor(0, 1);       
    lcd.print("Incline: ");
    lcd.print(inclination);
    Move_Forward();
    delay(2000);
    Turn_Left();
    delay(1000);
//    Move_Forward();
//    delay(150);
    Stop();
    Deploy_Probe();
    delay(1000);
    double salinity = Get_Salinity();
    lcd.setCursor(0, 0);
    lcd.print("Salinity: ");
    lcd.print(salinity);
    lcd.print("%");
  }
}


void navigate_maze()
{
  Move_Forward();
  distance = 1000;
  while(distance > 15)
  {
    
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
    // Displays the distance on the Serial Monitor
    Serial.println(distance);
    if(distance < 20)
    {
      break;
    }

  }

  
  find_hole(true);
  
  Move_Forward();
  while(distance > 20)
  {
    
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
    // Displays the distance on the Serial Monitor
    Serial.println(distance);

  }
  find_hole(false);
  Move_Forward();
  delay(3000);

  Stop();
  
}


void find_hole(bool RL)
{
  int delay_cntr;
  if(RL == 1)
  {
    Move_Left();
  }
  else
  {
    Move_Right();
  }
  int cntr = 0;
  while(true)
  {
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
    // Displays the distance on the Serial Monitor
    
    
    delay(50);
    if(distance > 40)
    {
      cntr++;
    }
    if(cntr >= 25)
    {
      Stop();
      
      break;
      
    }
  }
  Move_Forward();
  delay(2500);
  cntr = 0;
  return delay_cntr;
}


//Arm stuffs

void arm_down()
{
  
  Arm_Motor.write(110);
  delay(500);
  Arm_Motor.write(90);
  
}

void arm_up()
{
  Arm_Motor.write(70);
  delay(500);
  Arm_Motor.write(90);
}

void Find_Salamanders()
{
  Move_Left();
  bool breaker = true;
  while(breaker)
  {
    int numCharsReadFromIR = scanIR(IRSensorPin);
    char* IRCharacters = getIR();
    for(int i = 0; i < numCharsReadFromIR; i++)
    {
      if(IRCharacters[i] == 'K')
      {
        Stop();
        breaker = false;
      }
    }
  }
  
  Move_Forward();
  while(distance > 30)
  {
    
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
    // Displays the distance on the Serial Monitor
    Serial.println(distance);

  }
//  Move_Left();
//  delay(2500);
//  Turn_Right(); // if on left side
//  delay(623);
//  Move_Left();
//  delay(4500);
//  Turn_Right(); // if on back side
//  delay(623);
//  Move_Left();
//  delay(4500);
//  Turn_Right(); //if on right side
//  delay(623);
//  Move_Left();
  
}

void Collect_Salamanders()
{
  arm_down();
  Move_Forward();
  delay(750);
  Stop();
  Move_Left();
  delay(200);
  Stop();
  Move_Backwards();
  delay(50);
  Stop();
  Move_Left();
  delay(50);
  Stop();
  Move_Forward();
  delay(60);
  Stop();
  arm_up();
}

double Get_Inclination()
{
    const int conductivityAnalogPin1 = 1;

    int reading;
    
    reading = analogRead(conductivityAnalogPin1);

    double slope = 0.2390276187;
    double inter = -108.2381394;
    double angle = (slope * reading) +- inter;

    return angle;
}

double Get_Salinity()
{
  int con = getConductivity();
  double salin = myCalibrationFunction(con);
  return salin;
}

double myCalibrationFunction(int reading)
{
  double slope = 0.02993;
  double intercept = -6.76356;
  double answer = (1 / (slope * reading + intercept));
  return answer;
}

void Deploy_Probe()
{
  Spool.write(120);
  delay(2000);
  Spool.write(90);
}


void Around_Wall()
{
  Move_Right();
  int cntr = 0;
  while(true)
  {
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



//Movement calibration

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
  frontL_spd = spd + 5;
  
  backR_spd = (180 - spd);
  backL_spd = (180 - spd) + 2;

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
  frontL_spd = (180 - spd) + 3;
  
  backR_spd = spd - 3;
  backL_spd = spd + 3;

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

void Turn_Left()
{
  int spd = 70;
  
  FR_Motor.write(spd);
  FL_Motor.write(spd);
  
  BR_Motor.write(spd);
  BL_Motor.write(spd);
  
}

void Turn_Right()
{
  int spd = 110;
  
  FR_Motor.write(spd);
  FL_Motor.write(spd);
  
  BR_Motor.write(spd);
  BL_Motor.write(spd);
}
