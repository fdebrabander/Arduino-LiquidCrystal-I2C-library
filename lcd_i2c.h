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

#ifndef LIQUID_CRYSTAL_I2C_H
#define LIQUID_CRYSTAL_I2C_H

#include <inttypes.h>
#include <Print.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit


class ILcdI2c
{
public:
    virtual void setAddress(uint8_t addr) = 0;
    virtual void setResolution(uint8_t cols, uint8_t rows) = 0;
    virtual void setWirePins(uint8_t sda, uint8_t scl) = 0;
    virtual void setCharSize(uint8_t size) = 0;
    virtual void setBacklight(bool isOn) = 0;
    virtual void begin() = 0;
    virtual void clear() = 0;
    virtual void home() = 0;
    virtual void createChar(uint8_t, uint8_t[]) = 0;
    virtual void setCursor(uint8_t pos, uint8_t line) = 0;
    virtual size_t print(const String &val) = 0;
    virtual size_t print(const char val[]) = 0;
    virtual size_t print(char val) = 0;
    virtual size_t print(unsigned val) = 0;
    virtual size_t print(int val) = 0;
    virtual size_t print(double val) = 0;
};


/**
 * This is the driver for the Liquid Crystal LCD displays that use the I2C bus.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */
class LcdI2c: public ILcdI2c, public Print
{
public:
    /**
     * Constructor
     */
    LcdI2c();

    /**
     * Set I2C address of LCD
     *
     * @param addr I2C address
     */
    inline void setAddress(uint8_t addr) { addr_ = addr; }

    /**
     * Set cols and rows count of LCD
     *
     * @param cols Cols count of LCD
     * @param row Rows count of LCD
     */
    inline void setResolution(uint8_t cols, uint8_t rows) { cols_ = cols; rows_ = rows; }

    /**
     * Set Wire pins
     *
     * @param sda Data pin
     * @param scl Clock pin
     */
    inline void setWirePins(uint8_t sda, uint8_t scl) { sda_ = sda; scl_ = scl; }

    /**
     * Set char size of LCD
     *
     * @param size Char size of LCD
     */
    inline void setCharSize(uint8_t size) { charsize_ = size; }

    /**
     * Set backlight state
     *
     * @param state Backlight On/Off state
     */
    void setBacklight(bool isOn);

    /**
     * Set the LCD display in the correct begin state, must be called before anything else is done.
     */
    void begin();

     /**
      * Remove all the characters currently shown. Next print/write operation will start
      * from the first position on LCD display.
      */
    void clear();

    /**
     * Next print/write operation will will start from the first position on the LCD display.
     */
    void home();

    /**
     * Create new char for LCD
     *
     * @param location Symbol number
     * @param charmap Symbol defenition
     */
    void createChar(uint8_t location, uint8_t charmap[]);

    /**
     * Set cursor position
     *
     * @param pos Position in line
     * @param line Line number of lcd
     */
    void setCursor(uint8_t pos, uint8_t line);

    /* Print funcs */
    inline size_t print(const String &val) { Print::print(val); }
    inline size_t print(const char val[]) { Print::print(val); }
    inline size_t print(char val) { Print::print(val); }
    inline size_t print(unsigned val) { Print::print(val); }
    inline size_t print(int val) { Print::print(val); }
    inline size_t print(double val) { Print::print(val); }

private:
    void send(uint8_t, uint8_t);
    void write4bits(uint8_t);
    void expanderWrite(uint8_t);
    void pulseEnable(uint8_t);
    virtual size_t write(uint8_t);
    void command(uint8_t);

    uint8_t addr_;
    uint8_t displayfunction_;
    uint8_t displaycontrol_;
    uint8_t displaymode_;
    uint8_t cols_;
    uint8_t rows_;
    uint8_t charsize_;
    uint8_t backlightval_;
    uint8_t sda_;
    uint8_t scl_;
};


#endif