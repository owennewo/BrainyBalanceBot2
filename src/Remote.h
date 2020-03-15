#ifndef _REMOTE_H

#define _REMOTE_H

void RemoteSetup();
bool RemoteHasCommands();
long RemoteGetSteeringCommand();
long RemoteGetSpeedCommand();

#endif