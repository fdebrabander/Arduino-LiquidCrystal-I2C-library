/*****************************************************************************
 *
 * Liquid Crystal Display Arduino library
 *
 * Copyright (C) joaopedrosgs and fdebrabander
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com) 2018
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 *****************************************************************************/

#include <Wire.h> 
#include <lcd_i2c.h>
#include <assert.h>


#define CFG_LCD_ADDR 0x27
#define CFG_LCD_COLS 16
#define CFG_LCD_ROWS 2


class App
{
public:
    App(ILcdI2c *lcd): lcd_(lcd)
    {
        assert(lcd != nullptr);
    }

    void setup()
    {
        lcd_->setAddress(CFG_LCD_ADDR);
        lcd_->setResolution(CFG_LCD_COLS, CFG_LCD_ROWS);
        lcd_->begin();
        lcd_->setBacklight(true);
    }

    void loop()
    {
        unsigned uptime = millis() / 1000;

        lcd_->clear();
        lcd_->setCursor(0, 0);
        lcd_->print("Hello World!");
        lcd_->setCursor(0, 1);
        lcd_->print("Uptime: ");
        lcd_->print(uptime);
        lcd_->print("s");

        delay(1000);
    }

private:
    ILcdI2c *lcd_;
};


LcdI2c lcd;
App app(&lcd);


void setup()
{
    app.setup();
}

void loop()
{
    app.loop();
}