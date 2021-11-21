/**
 * This file is a small sample showing some basic setup and functionality for the
 * LCD display. The setup() function sets up the LCD display, and the loop()
 * function shows how many iterations have been performed on the display.
 * 
 * Be sure to check the wiring guide for how to hook up the LCD.
 * 
 * IMPORTANT: Make sure you bridge the two pins specified in the diagram.
 * One side of the display connector has 4 pins that connect to the arduino.
 * The other side has 2 pins that MUST be bridged.
 * 
 * The first few lines of the setup() function can be copied directly into
 * your program. Don't worry about the seemingly random sequence of numbers;
 * those have been pre-determined and don't need to be changed.
 * 
 * The LCD is made up of 2 rows and 16 columns. Like a text document, there's
 * a "cursor" that you can move. When you write a value, like an int, float,
 * or string of characters, it starts writing wherever your cursor currently
 * sits. You'll see in the loop() function that we specify exactly where
 * we want to print values on the LCD screen.
 * 
 * NOTE: rows and columns are 0-index-based. As in, to print on column 1 row 1,
 * you'll move your cursor to 0 and 0. To print on column 8 row 2, move the
 * cursor to 7 and 1. COLUMN GOES FIRST, FOLLOWED BY ROW.
 * 
 * The print() function lets you print the following types: int, double, char*, long.
 */

#include <LiquidCrystal_I2C.h>
int loopCount = 0;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  
  lcd.begin(16, 2); // initialize the lcd. Don't worry about the int values above.
  lcd.backlight();
  
  lcd.home();       // go to the top line  
  
  lcd.setCursor(0, 0);       // Moves the cursor to column 0, row 0
  lcd.print("Salinity: "); // Print out a title on row 0
  lcd.print(loopCount + 2);
  lcd.print("%");
  lcd.setCursor(0, 1);       // Moves the cursor to column 0, row 1
  lcd.print("Inclination: ");      // Print out the number of times we've looped
  lcd.print(loopCount + 5);
  delay(2000);    // Delay for two seconds
}

void loop() {
}
