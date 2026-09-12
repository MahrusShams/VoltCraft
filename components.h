#ifndef COMPONENTS_H
#define COMPONENTS_H

#define MAX_POINTS 100
#define MAX_COMPONENTS 100
#define MAX_NAME 20

typedef struct
{
    char name[MAX_NAME];
    char type;      // R = resistor, V = voltage source, C = capacitor
    int pointA;
    int pointB;
    double value;
} Component;

#endif