#include "Arduino.h"

extern bool initComms();
extern bool piInit(), needToShutdown(), isDeployed();


extern void setCommAltitude(int alt), setCommDistance(int dist), setDofError(bool er), setGpsError(bool er), setGpsHasFix(bool er);
extern void setCommLat(float lat), setCommLon(float lon), setCommOrientation(float zero, float one, float two);
