#include "nbody.h"

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
    double scalar = -G * b1.mass * b2.mass / pow(v_mag(v_sub(b1.pos, b2.pos)), 2);
    return (vector){scalar * r.x, scalar * r.y};
}