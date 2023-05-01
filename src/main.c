#include "nbody.h"

int main(void){
    body b1 = {{1, 1}, {0, 0}, {0, 0}, {0, 0}, 10, 100}, b2 = {{2, 2}, {0, 0}, {0, 0}, {0, 0}, 10, 100};
    vector force = bpair_force(b1, b2);
    printf("Force of b2 on b1: [%lf, %lf]", force.x, force.y);
}