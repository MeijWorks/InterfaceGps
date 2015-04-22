/*
  InterfaceGps - an interface for the VehicleGps library
Copyright (C) 2011-2014 J.A. Woltjer.
All rights reserved.
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "InterfaceGps.h"

// -----------
// Constructor
// -----------
InterfaceGps::InterfaceGps(LiquidCrystal_I2C * _lcd,
                     VehicleGps * _gps){
  lcd = _lcd;
  gps = _gps;
  
  //if(!checkGps()){
  detectGps();
  //}
}

// ---------------------------------------
// Methods for autobauding and serial port
// ---------------------------------------
/*boolean InterfaceGps::checkGps(){
  byte rate = gps->getBaudrate();
  
  byte rates[8] = {
    1, 2, 3, 4, 6, 8, 12, 24};
  byte success = 0;
  unsigned long baudrate = long(4800) * rates[rate];
      
  lcd->write_buffer("GPS:                ", 0);
  lcd->write_buffer("GGA string: --      ", 1);
  lcd->write_buffer("VTG string: --      ", 2);
  lcd->write_buffer("XTE string: --      ", 3);
  
  lcd->write_screen(-1);

  success = testRate(baudrate);
  
  if (success == 7) {
    lcd->write_buffer("GPS: OK             ", 0);
  }
  else {
    lcd->write_buffer("GPS: Check settings ", 0);
  }
  
  lcd->write_buffer("                    ", 1);
  lcd->write_buffer("                    ", 2);
  lcd->write_buffer("                    ", 3);
  
  lcd->write_screen(-1);

#if defined(__AVR_ATmega32U4__)
  Serial1.begin(baudrate);
#else
  Serial.begin(baudrate);
#endif
  
  return (success == 7);
}*/

boolean InterfaceGps::detectGps () {
  byte rate = gps->getBaudrate();
  byte gpsrate = 0;
  
  byte rates[8] = {
    1, 2, 3, 4, 6, 8, 12, 24 };
  int success = 0;
  unsigned long baudrate = 0;

  lcd->write_buffer("GPS:                ", 0);
  lcd->write_buffer("GGA string: --      ", 1);
  lcd->write_buffer("VTG string: --      ", 2);
  lcd->write_buffer("XTE string: --      ", 3);
  
  lcd->write_screen(-1);
  //delay(3000);
  
  for (int i = rate; i < rate + 8; i++) {
    baudrate = long(4800) * rates[i%8];
     
    lcd->setCursor(5, 0);
    lcd->print("@");
    lcd->print(baudrate);
    lcd->print(" baud  ");

    success = testRate(baudrate);
    
    if (success != 0) {
      gpsrate = i%8;
      break;
    }
  }
 
  lcd->write_screen(-1);
       
  if (success == 7) {
#if defined(__AVR_ATmega32U4__)   
    Serial1.begin(baudrate);
#else
    Serial.begin(baudrate);
#endif
    gps->commitBaudrate(gpsrate);
    return true;
  }
  else if (success != 0) {
#if defined(__AVR_ATmega32U4__)   
    Serial1.begin(baudrate);
#else
    Serial.begin(baudrate);
#endif
    delay(1000);
    return false;
  }
  else {
#if defined(__AVR_ATmega32U4__)   
    Serial1.begin(long(4800) * rates[rate]);
#else
    Serial.begin(long(4800) * rates[rate]);
#endif
    delay(1000);
    return false;
  }
}

byte InterfaceGps::testRate(unsigned long baudrate){
  unsigned long starttime = millis();
  boolean gga = false;
  boolean vtg = false;
  boolean xte = false;
  byte result = 0;

#if defined(__AVR_ATmega32U4__)
  Serial1.begin(baudrate);
#else
  Serial.begin(baudrate);
#endif

  while ((millis() - starttime < 2000 || result != 0) && millis() - starttime < 10000 && result != 7) {
    gps->update();

    if (millis() - gps->getGgaFixAge() < 2000){
      lcd->setCursor(12, 1);
      lcd->print("OK");
      gga = true;
    }
    else{
      lcd->setCursor(12, 1);
      lcd->print("--");
      gga = false;
    }
    if (millis() - gps->getVtgFixAge() < 2000){
      lcd->setCursor(12, 2);
      lcd->print("OK");
      vtg = true;
    }
    else{
      lcd->setCursor(12, 2);
      lcd->print("--");
      vtg = false;
    }
    if (millis() - gps->getXteFixAge() < 2000){
      lcd->setCursor(12, 3);
      lcd->print("OK");
      xte = true;
    }
    else{
      lcd->setCursor(12, 3);
      lcd->print("--");
      xte = false;
    }
    result = gga * 4 + vtg * 2 + xte * 1;
    lcd->write_screen(1);
  }
  lcd->write_screen(-1);

#if defined(__AVR_ATmega32U4__)  
  Serial1.end();
#else
  Serial.end();
#endif

  return result;
}