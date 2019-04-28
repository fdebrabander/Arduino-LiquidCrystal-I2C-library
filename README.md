# Basic info
This is library for I2C display. It wokrs with Teensy 3.6.

# Installation
Create a new folder called "LiquidCrystal_I2C" under the folder named "libraries" in your Arduino sketchbook folder.
Create the folder "libraries" in case it does not exist yet. Place all the files in the "LiquidCrystal_I2C" folder.

# Usage
To use the library in your own sketch, select it from *Sketch > Import Library*.

## Basic example

```c++
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //Set LCD to 16chars and 2 lines

void setup() {

  lcd.begin(); //init display 
  lcd.backlight(); //light up backlight

  
  lcd.clearhome(); //Clear display and return to home
  lcd.print("Hello world!");//Print characters on display
  delay(5000);
}

void loop(){

  lcd.clear();
  lcd.setCursor(5, 0); //set cursor to 5th character on first line
  lcd.print("Sup!");
  delay(1000);

  lcd.clear(); //clear tge display
  lcd.setCursor(0, 1); //set cursor to second line
  lcd.print("Sup!");
  delay(1000);
}

```

# Credits

Based on (fdebrabander's library)[https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library]