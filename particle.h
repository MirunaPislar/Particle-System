#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "SOIL.h"
#ifdef MACOSX
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#define POINT_SIZE 10
#define TEXTURE_POINT_SIZE 80
#define MAX_PARTICLES 1000000
#define PARTICLES 500
#define EMISSION_SOURCE_X 0.0
#define EMISSION_SOURCE_Y 50.0
#define EMISSION_SOURCE_Z 0.0
#define EMISSION_SIZE 25.0
#define GRAVITY -9.81
#define MASS 0.0005
#define LIVING_THRESHOLD 0.3
#define WIND_SPEED 0.1
#define WIND_ANGLE 90.0
#define PI 3.14159265358979323846
#define SPEED_MEAN 0.5
#define SPEED_VAR 0.1
#define RANDOM_DIRECTION_MEAN 0.0
#define RANDOM_DIRECTION_VAR 0.02
#define COLOUR_MEAN 0.003
#define COLOUR_VAR 0.007 
#define SHADE_VAR 0.03
#define ALPHA_MEAN 0.3
#define ALPHA_VAR 0.1
#define ALPHA_LIFETIME 0.00001

typedef struct 
{
	double x, y, z;                                 
    double x_velocity, y_velocity, z_velocity;
    double r, g, b, a;
} Particle;

typedef struct
{
    Particle particles[MAX_PARTICLES];
    int total_no_of_particles;
    int alive_no_of_particles;
    double r, g, b;
} System;

System fire;
int render_as_point, render_as_lines, render_as_texture;
int explosion, steam;
int explosion_texture, steam_textures[10];
int add_randomness;
double lifetime;
double gravity;
double mass;
double random_speed;
double theta;
double windSpeed;
double wind_u_component;
double wind_v_component;
double emission_size;
double point_size;
double framesPerSecond;
int counter, start, end;
char string[50];