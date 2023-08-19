#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "web-server-setup.h"

extern const int relayPin;
extern unsigned long startTime;
extern bool relayOn;
extern const int maxDuration;
extern const int minDuration;
extern const int period;

extern unsigned long remainingTimeOn;
extern unsigned long remainingTimeOff;

void relayControl();

#endif
