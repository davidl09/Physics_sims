#include "nbody.h"
#include <stdio.h>

int main(void){
    /*
    body b1 = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, 10, 100}, b2 = {{4, 4}, {0, 0}, {0, 0}, {0, 0}, 10, 100};
    vector force = bpair_force(b1, b2);
    printf("Force of b2 on b1: [%lf, %lf]", force.x, force.y);
    */
    srand(time(NULL));
    unsigned int numbodies = 100;
    body bodies[numbodies];
    

    for (int i = 0; i < numbodies; ++i) {
        bodies[i] = (body){{rand()%WIDTH, rand()%HEIGHT}, {0, 0}, {0, 0}, {0, 0}, 10, 10};
    }

    vector force;
    for (int i = 0; i < numbodies; ++i) {
        for (int j = i; j < numbodies; ++j) {
            force = bpair_force(bodies[i], bodies[j]);
            bodies[i].force = v_add(bodies[i].force, force);
            bodies[j].force = v_sub(bodies[j].force, force);
        }
    }
}