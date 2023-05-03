#include "nbody.h"
#include <math.h>
#include <pthread.h>
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

int classify(vector pos){ //classifiy points into quadrants
    return (pos.x > 0 ? (pos.y > 0 ? 0 : 3) : (pos.y > 0 ? 1 : 2)); 
}

void calculate_step_mt(body* bodies, double dt, unsigned int count, double G_const){
    pthread_t threads[4];
    args arg[4];
    int quadrant;
    body** sections = calloc(4,sizeof(body*));
    vector c_g_q[4] = {V_0, V_0, V_0, V_0};
    double masses[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; ++i) {
        sections[i] = calloc(1, sizeof(body));
        arg[i].count = 0;
    }

    for (int i = 0; i < count; ++i) { //allocate memory and compute r^_cm * M for each quadrant
        quadrant = classify(bodies[i].pos);
        sections[quadrant][arg[quadrant].count] = bodies[i];
        c_g_q[quadrant] = v_add(c_g_q[quadrant], v_sprod(bodies[i].pos, bodies[i].mass));
        masses[quadrant] += bodies[i].mass;
        arg[quadrant].count++;
        sections[quadrant] = realloc(sections[quadrant], arg[quadrant].count * sizeof(body));
    }

    for (int i = 0; i < 4; ++i) {
        c_g_q[i] = v_sprod(c_g_q[i], 1/masses[i]);
    }


    for (int i = 0; i < 4; ++i) { //start a thread for each quadrant
        for (int j = 0; j < 4; ++j) {
            arg[i].cg[j] = c_g_q[j];
            arg[i].cg_mass[j] = masses[j];
        }
        arg[i].dt = dt;
        arg[i].G_const = G_const;
        pthread_create(&threads[i], NULL, mt_calc, (void*)&arg[i]);
    }

    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], NULL);
    }

}

void* mt_calc(void* arg_struct){
    args* arg = (args*)arg_struct;
    vector force;
    double radius;

    for (int i = 0; i < arg->count; ++i) {
        arg->bodies[i].force = V_0;
    }

    for (int i = 0; i < (arg->count - 1); ++i) {
        if(arg->bodies[i].mass != 0){
            for (int j = 0; j < arg->count; ++j) {
                if(j == i) continue;
                
                if(arg->bodies[j].mass != 0){
                    radius = cbrt(arg->bodies[i].mass/arg->bodies[i].dens);
                    if(v_mag(v_sub(arg->bodies[i].pos, arg->bodies[j].pos)) < radius){
                        arg->bodies[i].vel = v_sprod(v_add(v_sprod(arg->bodies[i].vel, arg->bodies[i].mass), v_sprod(arg->bodies[j].vel, 
                        arg->bodies[j].mass)), 1/(arg->bodies[i].mass + arg->bodies[j].mass));

                        arg->bodies[i].pos = v_sprod(v_add(v_sprod(arg->bodies[i].pos, arg->bodies[i].mass), v_sprod(arg->bodies[j].pos, 
                        arg->bodies[j].mass)), 1/(arg->bodies[i].mass + arg->bodies[j].mass));

                        arg->bodies[i].mass += arg->bodies[j].mass;
                        arg->bodies[j] = NULL_BODY;

                    }else{
                        body cg_temp = NULL_BODY;
                        int quadrant = classify(arg->bodies[i].pos);
                        for (int j = 0; j < 4; ++j) {
                            if(j != quadrant){
                                cg_temp.pos = v_add(cg_temp.pos, v_sprod(arg->cg[j], arg->cg_mass[j]));
                                cg_temp.mass += arg->cg_mass[j];
                            }
                            cg_temp.pos = v_sprod(cg_temp.pos, 1/cg_temp.mass);
                        }
                        force = v_add(bpair_force(arg->bodies[i], arg->bodies[j]), bpair_force(arg->bodies[i], cg_temp));
                        arg->bodies[i].force = v_add(arg->bodies[i].force, force);
                        arg->bodies[j].force = v_sub(arg->bodies[j].force, force);
                    }
                }
            }
        }
    }

    for (int i = 0; i < arg->count; ++i) {
        arg->bodies[i].accel = v_sprod(arg->bodies[i].force, 1.0/arg->bodies[i].mass);
        arg->bodies[i].vel = v_add(v_sprod(arg->bodies[i].accel, arg->dt), arg->bodies[i].vel);
        arg->bodies[i].pos = v_add(v_sprod(arg->bodies[i].vel, arg->dt), arg->bodies[i].pos);
    }

    return (void*)arg;
}