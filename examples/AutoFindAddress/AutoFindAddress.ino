#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Auto detect LCD address between 0x3F or 0x27
byte autolcdaddr(){
Wire.begin();
Wire.beginTransmission(0x27);
return Wire.endTransmission()? 0x3F:0x27;
}


// Set the LCD for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(autolcdaddr(), 16, 2);

void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("Hello, world!");
}

void loop()
{
	// Do nothing here...
}
