/*
  InterfaceGps - an interface for the VehicleGps library.
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

#ifndef InterfaceGps_h
#define InterfaceGps_h

#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_UART.h>
#include <VehicleGps.h>

// Software version of this library
#define INTERFACE_GPS_VERSION 0.1
//#define UART

class InterfaceGps {
private:  
  // Objects
  VehicleGps * gps;
#ifndef UART
  LiquidCrystal_I2C * lcd;
#else
  LiquidCrystal_UART * lcd;
#endif
  
  byte testRate(unsigned long baudrate);

public:
#ifndef UART
  InterfaceGps(LiquidCrystal_I2C * lcd, VehicleGps * gps);
#else
  InterfaceGps(LiquidCrystal_UART * lcd, VehicleGps * gps);
#endif
  
  boolean checkGps();
  boolean detectGps();
};
#endif