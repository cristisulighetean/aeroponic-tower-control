#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "web-server-setup.h"

extern const int relayPin;
extern bool relayOn;

extern const int maxDuration;
extern const int minDuration;
extern const int period;

extern unsigned long startTime;
extern long remainingTimeOn;
extern long remainingTimeOff;

void relayControl();

#endif
