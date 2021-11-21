
#include <conductivity.h>
double myCalibrationFunction(int);

//Yellow and Green go in Analog 2 and 3 (A2, A3)
//Blues go in pin12 and 13



#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 


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


#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);



void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2); // initialize the lcd. Don't worry about the int values above.
  lcd.backlight();
  
  lcd.home();       // go to the top line  
  
  lcd.setCursor(0, 0);       // Moves the cursor to column 0, row 0
}

void loop() 
{
  char customKey = customKeypad.getKey();
  
  if (customKey == '1'){
    int conductivityReading = getConductivity();
    double salinity = myCalibrationFunction(conductivityReading);
    lcd.print("Salinity: ");
    lcd.println(salinity);
    lcd.println("%");
  }
    
}

double myCalibrationFunction(int reading)
{
  double slope = 0.02993;
  double intercept = -6.76356;
  double answer = (1 / (slope * reading + intercept));
  return answer;
}
