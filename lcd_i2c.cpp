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

#include "lcd_i2c.h"
#include <inttypes.h>
#include <Arduino.h>
#include <Wire.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LcdI2c constructor is called).
LcdI2c::LcdI2c()
{
    sda_ = 0;
    scl_ = 0;
    charsize_ = LCD_5x8DOTS;
    backlightval_ = LCD_NOBACKLIGHT;
}

void LcdI2c::begin()
{
    if (sda_ != 0 && scl_ != 0)
        Wire.begin(sda_, scl_);
    else
        Wire.begin();
    displayfunction_ = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

    if (rows_ > 1) {
        displayfunction_ |= LCD_2LINE;
    }

    // for some 1 line displays you can select a 10 pixel high font
    if ((charsize_ != 0) && (rows_ == 1)) {
        displayfunction_ |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    delay(50);

    // Now we pull both RS and R/W low to begin commands
    expanderWrite(backlightval_);   // reset expanderand turn backlight off (Bit 8 =1)
    delay(1000);

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    // third go!
    write4bits(0x03 << 4);
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02 << 4);

    // set # lines, font size, etc.
    command(LCD_FUNCTIONSET | displayfunction_);

    // turn the display on with no cursor or blinking default
    displaycontrol_ = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    displaycontrol_ |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | displaycontrol_);

    // clear it off
    clear();

    // Initialize to default text direction (for roman languages)
    displaymode_ = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    command(LCD_ENTRYMODESET | displaymode_);

    setCursor(0, 0);
}

/********** high level commands, for the user! */
void LcdI2c::clear()
{
    command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
    delayMicroseconds(2000);  // this command takes a long time!
}

void LcdI2c::home()
{
    command(LCD_RETURNHOME);  // set cursor position to zero
    delayMicroseconds(2000);  // this command takes a long time!
}

void LcdI2c::setCursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > rows_) {
        row = rows_ - 1;    // we count rows starting w/0
    }
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LcdI2c::createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

void LcdI2c::setBacklight(bool isOn)
{
    if (isOn) {
        backlightval_ = LCD_BACKLIGHT;
        expanderWrite(0);
    }
    else {
        backlightval_ = LCD_NOBACKLIGHT;
        expanderWrite(0);
    }
}

/*********** mid level commands, for sending data/cmds */

inline void LcdI2c::command(uint8_t value)
{
    send(value, 0);
}

inline size_t LcdI2c::write(uint8_t value)
{
    send(value, Rs);
    return 1;
}


/************ low level data pushing commands **********/

// write either command or data
void LcdI2c::send(uint8_t value, uint8_t mode)
{
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

void LcdI2c::write4bits(uint8_t value)
{
    expanderWrite(value);
    pulseEnable(value);
}

void LcdI2c::expanderWrite(uint8_t _data)
{
    Wire.beginTransmission(addr_);
    Wire.write((int)(_data) | backlightval_);
    Wire.endTransmission();
}

void LcdI2c::pulseEnable(uint8_t _data)
{
    expanderWrite(_data | En);  // En high
    delayMicroseconds(1);       // enable pulse must be >450ns

    expanderWrite(_data & ~En); // En low
    delayMicroseconds(50);      // commands need > 37us to settle
}