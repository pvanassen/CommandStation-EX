/*
 *  © 2020, Chris Harlow. All rights reserved.
 *  
 *  This file is part of CommandStation-EX
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CommandStation.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef RMFT2_H
#define RMFT2_H
#include "RMFTMacros.h"
 

 class RMFT2 {
   public:
    static void begin();
    static void loop();
    RMFT2(byte route);
    ~RMFT2();
    static void readLocoCallback(int cv);
    static void runMySetup();
  private: 
    static void ComandFilter(Print * stream, byte & opcode, byte & paramCount, int p[]);
    static bool parseSlash(Print * stream, byte & paramCount, int p[]) ;
    static void setupCommands(const __FlashStringHelper* input...);
    
    static int locateRouteStart(short _route);
    static int progtrackLocoId;
    static RMFT2 * loopTask;
    static RMFT2 * pausingTask;
    void delayMe(int millisecs);
    void driveLoco(byte speedo);
    bool readSensor(short id);
    void skipIfBlock();
    bool readLoco();
    void showManual();
    void showProg(bool progOn);
    bool doManual();
    void loop2();          
    
  static bool diag;
  static const  PROGMEM  byte RouteCode[];
 
 
 // Local variables
    int progCounter;    // Byte offset of next route opcode in ROUTES table
    unsigned long delayStart; // Used by opcodes that must be recalled before completing
    unsigned long waitAfter; // Used by OPCODE_AFTER
    unsigned int  delayTime;
    int loco;
    bool forward;
    bool invert;
    int speedo;
    RMFT2 *next;   // loop chain 
};
#endif
