#include "Int0.h"
#include <reg52.h>

void main() {
    Int0_Init();
  while (1) {
  }
}

void Int0_Routine(void) __interrupt(0) { 
    
    // P21 = ~P21; 
}