/*
 * Particle System Implementation for COMP37111 Advanced Computer Graphics
 * This header file contains all the parameters and prototypes needed by 
 * the particle system simulation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <float.h>
#ifdef MACOSX
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

/*
 * Declare the simulation parameters
 */
 #define WINDOW_WIDTH 1450
 #define WINDOW_HEIGHT 800
 #define POINT_SIZE 20

 #define MAX_PARTICLES 1000000
 #define DEFAULT_NO_OF_PARTICLES 500

 /*
  * Declare all attributes needed for the particle system
  */
#define GRAVITY -9.81
#define MASS 0.00002
#define WIND_SPEED 0.1
#define EMISSION_SOURCE_X 0.0
#define EMISSION_SOURCE_Y 100.0
#define EMISSION_SOURCE_Z 0.0
#define EMISSION_SIZE 25.0//800.0

#define DEFAULT_SPEED_MEAN 0.5
#define DEFAULT_SPEED_VAR 0.1

#define RANDOM_DIRECTION_MEAN 0.01
#define RANDOM_DIRECTION_VAR 0.02

#define SMOKE_SHADE_CHANGE_MEAN 0.003
#define SMOKE_SHADE_CHANGE_VAR 0.007 
#define DEFAULT_SHADE_VAR 0.02
#define ALPHA_MEAN 0.3
#define ALPHA_VAR 0.1  

#define LIVING_THRESHOLD 0.0000001
#define DEG_TO_RAD 0.017453293

/*
 * Define a particle and a system of particles that will constitute the smoke
 */
 typedef struct 
 {
    double x, y, z;                                     // Position
    double x_velocity, y_velocity, z_velocity;          // Velocity
    double r, g, b, a;                                  // RGBA
 } Particle;

 typedef struct
 {
    Particle particles[MAX_PARTICLES];
    int total_no_of_particles;
    int alive_no_of_particles;
    double r, g, b;
 } Smoke;

Smoke smoke;
int render_as_point  = 1;

/*
 * Define some variables for the environment
 */
double gravity;
double mass;
double random_speed;
double theta = 180;
double windSpeed;
double wind_u_component;
double wind_v_component;
double boxMuller2Rand;
double emission_size;
double point_size;