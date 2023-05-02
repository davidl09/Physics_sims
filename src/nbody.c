#include "nbody.h"
#include <math.h>
#include <stdlib.h>

double G;

double v_mag(vector v){
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

vector v_add(vector v1, vector v2){
    return (vector){v1.x + v2.x, v1.y + v2.y};
}

vector v_sub(vector v1, vector v2){
    return (vector){v1.x - v2.x, v1.y - v2.y};
}

vector v_unit(vector v){
    double mag = v_mag(v);
    return (vector){v.x/mag, v.y/mag};
}

vector v_sprod(vector v, double scalar){
    return (vector){scalar * v.x, scalar * v.y};
}

vector bpair_force(body b1, body b2) 
//force between two massive bodies, returns force vector v applied on b1 by b2
{ 
    vector r = v_unit(v_sub(b1.pos, b2.pos));
    double scalar = -G * b1.mass * b2.mass / (pow(v_mag(v_sub(b2.pos, b1.pos)), 2));
    return v_sprod(r, scalar);
}

void fill_arr(body* bodies, double dv, unsigned int mass, unsigned int dens, unsigned int count){
    srand(time(NULL));
    for (int i = 0; i < count; ++i) {
        int pos_x = (rand()%WIDTH) - WIDTH/2, pos_y = (-(rand()%HEIGHT) + HEIGHT/2);
        bodies[i] = (body){(vector){pos_x, pos_y}, (vector){- dv * pos_y, dv * pos_x}, 
        (vector){0, 0}, (vector){0, 0}, (mass == 0? 100 : mass), (dens == 0 ? 10 : dens)};
    }
}

void calculate_step(body* bodies, double dt, unsigned int count, double G_const){
    G = G_const;
    vector force;
    double radius;
    for (int i = 0; i < count; ++i) {
        bodies[i].force = V_0;
    }

    for (int i = 0; i < (count - 1); ++i) {
        if(bodies[i].mass != 0){
            for (int j = i + 1; j < count; ++j) {
                if(bodies[j].mass != 0){
                    radius = cbrt(bodies[i].mass/bodies[i].dens);
                    if(v_mag(v_sub(bodies[i].pos, bodies[j].pos)) < radius){
                        bodies[i].vel = v_sprod(v_add(v_sprod(bodies[i].vel, bodies[i].mass), v_sprod(bodies[j].vel, 
                        bodies[j].mass)), 1/(bodies[i].mass + bodies[j].mass));
                        bodies[i].pos = v_sprod(v_add(v_sprod(bodies[i].pos, bodies[i].mass), v_sprod(bodies[j].pos, 
                        bodies[j].mass)), 1/(bodies[i].mass + bodies[j].mass));
                        bodies[i].mass += bodies[j].mass;
                        bodies[j] = NULL_BODY;
                    }else{
                        force = bpair_force(bodies[i], bodies[j]);
                        bodies[i].force = v_add(bodies[i].force, force);
                        bodies[j].force = v_sub(bodies[j].force, force);
                    }
                }
            }
        }
    }

    for (int i = 0; i < count; ++i) {
        bodies[i].accel = v_sprod(bodies[i].force, 1.0/bodies[i].mass);
        bodies[i].vel = v_add(v_sprod(bodies[i].accel, dt), bodies[i].vel);
        bodies[i].pos = v_add(v_sprod(bodies[i].vel, dt), bodies[i].pos);
    }
}