#ifndef INTERFACE_H
#define INTERFACE_H

bool join(char *IP, int TCP, char *regIP, char *regUDP, char *ring, char *id);

bool leave(char *IP, int TCP, char *regIP, int regUDP, char *ring, char *id);

#endif