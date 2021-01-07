#ifndef RMFT_H
#define RMFT_H
// Prepare the startup process
#include "RMFT2.h"

class RMFT {
  public:
   static void inline begin() {RMFT2::begin();}
   static void inline loop() {RMFT2::loop();}
};

void RMFT2::runMySetup() {
       RMFT2::setupCommands(
       #if __has_include ( "mySetup.h")
        #define SETUP(cmd) F(cmd), 
        #include "mySetup.h"
        #undef SETUP
       #endif
       NULL);
}

#if __has_include ( "myAutomation.h")
  #include "myAutomation.h"
  #define RMFT_ACTIVE
#endif

#endif
