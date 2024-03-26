#ifndef ENCAMINHAMETO_H
#define ENCAMINHAMENTO_H


void showRouting(int ***table, int dest);

void showPath(int ***tables, int dest);

void showForwarding (int ***tables, int personal);

void initPersonalpath(int ***tables, int personal);

void initTables(int ****tables);

void freeTables(int ***tables);

void destinationInit(int ***table, int dest, int* indexDest);

void stringToPathInt(char* string, int **path, int *pathSize);

int shortestPathCalc(int *old, int *new, int dest);

void routingRecive(int ***tables, char* pathString, int neighbour, int destination, int personalId);

void routingSendAll(int ***tables, int neighbourFd);


#endif // ENCAMINHAMENTO_H
