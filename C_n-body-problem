#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

size_t rows;
static size_t cols = 10;


FILE* fp;

long double start;

#define MAX_OBJECT_COUNT 2000
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define X_POS 0
#define Y_POS 1
#define X_VEL 2
#define Y_VEL 3
#define X_ACCEL 4
#define Y_ACCEL 5
#define X_FORCE 6
#define Y_FORCE 7
#define RAD 8
#define MASS 9
#define G 10

long double** populate(size_t num_obj, long double **data);
long double** calculate_forces(long double **data, long double dt);
long double** calculate_motion(long double **data, long double dt);
long double** delete_row(int index, long double **data);
long double** increment_data(long double **data);
void print_data(long double **data);

int main() {
    long double** data = NULL;
    long double dt;
    int sim_time;
    int obj_count;

    printf("Enter the object count for the n-body simulation: \n");
    scanf("%d", &obj_count);
    printf("Enter the time span of the simulation in seconds: \n");
    scanf("%d", &sim_time);
    printf("Enter the time delta for each step: \n");
    scanf("%LF", &dt);
    printf("Relative initial complexity score: %.3Lf\n", ((sim_time/dt)*(pow(obj_count, 2)))/1000);

    int frames = sim_time/dt;


    fp = fopen("sim_data.txt", "w");

    start = time(NULL);
    data = populate(obj_count, data);
    fprintf(fp, "%.4Lf,%d,%d,%d,\n", dt, sim_time, SCREEN_WIDTH, SCREEN_HEIGHT);
    for (int i = 0; i < frames; ++i) {
        data = calculate_forces(data, dt);
        for (int j = 0; j < rows; ++j) {
            fprintf(fp, "%Lf,%LF,%LF,\n", data[j][X_POS], data[j][Y_POS], data[j][RAD]);
        }
        fprintf(fp, "newArr,\n");
        if(i%(frames/100) == 0){
            printf("%d%% done\n", 100*i/frames);
        }
    }
    long double end = time(NULL);
    fprintf(fp,"Finished with %zu objects after starting with %d objects, taking %Lf seconds to simulate %d frames. Average of %Lf frames per second.\n", rows, obj_count, end - start, frames, frames/(end-start));
    printf("Finished with %zu objects after starting with %d objects, taking %Lf seconds to simulate %d frames. \nAverage of %Lf frames per second.\n", rows, obj_count, end - start, frames, frames/(end-start));
    free(data);
}


long double** calculate_forces(long double **data, long double dt) {
    long double x_dist;
    long double y_dist;
    long double distance;
    long double force;
    //clear forces and accelerations from previous run, and calculate motion
    for (int i = 0; i < rows; ++i) {
        data[i][X_ACCEL] = data[i][X_FORCE]/data[i][MASS];
        data[i][X_VEL] += data[i][X_ACCEL]*dt;
        data[i][X_POS] += data[i][X_VEL]*dt;
        data[i][Y_ACCEL] = data[i][Y_FORCE]/data[i][MASS];
        data[i][Y_VEL] += data[i][Y_ACCEL]*dt;
        data[i][Y_POS] += data[i][Y_VEL]*dt;
        data[i][X_FORCE] = 0;
        data[i][Y_FORCE] = 0;
        data[i][X_ACCEL] = 0;
        data[i][X_ACCEL] = 0;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = i+1; j < rows; ++j) {
            x_dist = data[j][X_POS] - data[i][X_POS];
            y_dist = data[j][Y_POS] - data[i][Y_POS];
            distance = sqrtl(powl(x_dist, 2) + powl(y_dist, 2));
            if(distance < (data[i][RAD] + data[j][RAD])){
                data[i][X_VEL] = (data[i][X_VEL] * data[i][MASS] + data[j][X_VEL] * data[j][MASS]) /
                                 (data[i][MASS] + data[j][MASS]);
                data[i][Y_VEL] = (data[i][Y_VEL] * data[i][MASS] + data[j][Y_VEL] * data[j][MASS]) /
                                 (data[i][MASS] + data[j][MASS]);
                //conserve center of mass position
                data[i][X_POS] = (data[i][X_POS] * data[i][MASS] + data[j][X_POS] * data[j][MASS]) /
                                 (data[i][MASS] + data[j][MASS]);
                data[i][Y_POS] = (data[i][Y_POS] * data[i][MASS] + data[j][Y_POS] * data[j][MASS]) /
                                 (data[i][MASS] + data[j][MASS]);
                data[i][MASS] += data[j][MASS];
                data[i][RAD] = powl(data[i][MASS], (float)1/(float)3);
                delete_row(j, data);
            }
            else{
                force = G * data[i][MASS] * data[j][MASS] /
                        powl(distance, 2); //calculate accel, vel, pos, data for pair of non-colliding objects
                data[i][X_FORCE] += force * (x_dist / distance);
                data[i][Y_FORCE] += force * (y_dist / distance);
                data[j][X_FORCE] += -data[i][X_FORCE];
                data[j][Y_FORCE] += -data[i][Y_FORCE];
            }
        }
    }
    return data;
}

long double** delete_row(int index, long double **data){ //index is the index of the row to be erased.
    for (int i = index + 1; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i - 1][j] = data[i][j];
        }
    }
    rows--;
    free(data[rows]);
    data = realloc(data, rows*sizeof(long double*));
    return data;
}

long double** increment_data(long double **data){
    rows += 1;
    data = realloc(data, rows*sizeof(long double*));
    data[rows-1] = malloc(cols*sizeof(long double));
    return data;
}

long double** populate(size_t num_obj, long double **data){
    srand(time(NULL));
    for (int i = 0; i < num_obj; ++i) {
        data = increment_data(data);
        data[i][X_POS] = rand()%SCREEN_WIDTH;// /2+SCREEN_WIDTH/4;
        data[i][Y_POS] = rand()%SCREEN_HEIGHT;// /2+SCREEN_HEIGHT/4;
        data[i][X_VEL] = (data[i][Y_POS]-(SCREEN_HEIGHT/2))/80;
        data[i][Y_VEL] = -(data[i][X_POS]-(SCREEN_WIDTH/2))/80;
        data[i][X_ACCEL] = 0;
        data[i][Y_ACCEL] = 0;
        data[i][X_FORCE] = 0;
        data[i][Y_FORCE] = 0;
        data[i][RAD] = 3 + rand()%3;
        data[i][MASS] = powl(data[i][RAD], 3);
    }
    return data;
}
