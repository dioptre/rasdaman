#ifndef _RASGEO_SIGNAL_HH_
#define _RASGEO_SIGNAL_HH_

#include <signal.h>

//signalCleanup function is called when a signal is received by the program. 
//You should write your function in order to have signal management
void signalCleanup();

//signalHandler function is called when a signal occurs
void
signalHandler(int sig);

//installSignalHandlers function should be called first in main function
//in order to receive signal in your program

void
installSignalHandlers();

#endif _RASGEO_SIGNAL_HH_

