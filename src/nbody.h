#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

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

typedef struct mt_calc_arg{
    body* bodies;
    vector cg[4];
    double cg_mass[4];
    double dt;
    unsigned int count;
    double G_const;
}args;

typedef struct cg_adj{

    double cgs[4];
    unsigned int id;
}cg_adj;

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
void calculate_step_mt(body* bodies, double dt, unsigned int count, double G_const);

void* mt_calc(void* arg_struct);

int classify(vector pos);