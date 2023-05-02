#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH 1000
#define HEIGHT 1000

typedef struct vector{
    double x;
    double y;
}vector;

typedef struct body{
    vector pos;
    vector vel;
    vector accel;
    vector force;
    double mass;
    double dens;
}body;

#define V_0 (vector){0, 0}
#define NULL_BODY (body){V_0, V_0, V_0, V_0, 0.0, 1}

double v_mag(vector v);
vector v_add(vector v1, vector v2);
vector v_sub(vector v1, vector v2);
vector v_sprod(vector v, double scalar);
vector v_unit(vector v);

vector bpair_force(body b1, body b2);

void fill_arr(body* bodies, double dv, unsigned int mass, unsigned int dens, unsigned int count);
void calculate_step(body* array, double dt, unsigned int count, double G);