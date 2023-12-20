//    FILE: I2C_LCD_demo_spectrum_row.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo I2C_LCD library
//     URL: https://github.com/RobTillaart/I2C_LCD

//  WARNING: do not overfeed your display with too much data
//           too fast as it may not be able to handle 
//           (mine got corrupted)


#include "Arduino.h"
#include "Wire.h"

#include "I2C_LCD.h"


//  test 20x4 + 16x2
#define BACKLIGHT_PIN   3
#define En_pin          2
#define Rw_pin          1
#define Rs_pin          0
#define D4_pin          4
#define D5_pin          5
#define D6_pin          6
#define D7_pin          7


I2C_LCD lcd(39);

uint8_t block[5][8] =
{
  { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
  { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
  { 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C },
  { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E },
  { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }
};


void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println(__FILE__);
  Serial.print("I2C_LCD_LIB_VERSION: ");
  Serial.println(I2C_LCD_LIB_VERSION);
  Serial.println();

  lcd.config(39, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin, BACKLIGHT_PIN, POSITIVE);

  Wire.begin();
  Wire.setClock(100000);
  lcd.begin(20, 4);

  lcd.display();
  lcd.clear();
  lcd.setCursor(0, 0);

  for (int i = 0; i < 5; i++)
  {
    lcd.createChar(i, block[i]);
  }
  delay(10);
  lcd.clear();
  for (int i = 0; i < 5; i++)
  {
    lcd.special(i);
  }
  lcd.clear();

}


void loop()
{
  for (int i = 0; i < 4; i++)
  {
    spectrumRow(i, random(80));
    delay(100);
  }
  delay(1000);
}


void spectrumRow(uint8_t row, int value)
{
  lcd.setCursor(0, row);
  lcd.print(value);
  lcd.setCursor(4, row);
  for (uint8_t col = 4; col < 20; col++)
  {
    if (value <= 0)      lcd.print(' ');
    else if (value >= 5) lcd.special(4);
    else                 lcd.special(value - 1);
    value -= 5;
    delay(1);
  }
}


//  -- END OF FILE --
