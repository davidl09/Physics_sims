#include <stdlib.h>
#include <math.h>

typedef struct vector{
    double x;
    double y;

}vector;

typedef struct body{
    vector pos;
    vector vel;
    vector accel;
    double mass;
    double dens;
}body;

double mag(vector v);