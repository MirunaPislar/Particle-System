/*
 * Particle System Simulation
 * 
 * COMP37111 Advanced Computer Graphics
 * School of Computer Science
 * The University of Manchester
 * 
 * Author: Miruna Pislar
 */
#include "particle.h"

/*
 * Return random double within range [-a,a].
 * Uniform random distribution.
 */
double myRandom(double limit)
{
  return (rand()/(double)RAND_MAX * 2.0 - 1.0) * limit; 
} // myRandom

/*
 * Return 
 * Gaussian random distribution.
 */
double gaussianDistribution(double mean, double std)
{
  double uniform1, uniform2, w;
 
  do {
    uniform1 = myRandom(1.0);
    uniform2 = myRandom(1.0);
    w = uniform1 * uniform1 + uniform2 * uniform2;
  } while ( w >= 1.0 );

  w = sqrt((-2.0 * log(w)) / w);
  boxMuller2Rand = uniform2 * w * std + mean;
  return uniform1 * w * std + mean;
} // gaussianDistribution

/*
 * Update u and v components of the wind (coresponding to x and z direction)
 * This needs to be done as we either changed the wind speed or the angle theta
 */
void updateWindComponents()
{
  wind_u_component = mass * windSpeed * cos(DEG_TO_RAD * theta) / 10;
  wind_v_component = mass * windSpeed * sin(DEG_TO_RAD * theta) / 10;
} // updateWindComponents

/*
 * Initialize the particle system and draw the particles.
 */
void drawParticles(void)
{
  int i;

  for(i = smoke.alive_no_of_particles; i < smoke.total_no_of_particles; i++)
  {
    // Set the emission point for each particle
    smoke.particles[i].x = EMISSION_SOURCE_X + myRandom(emission_size);
    smoke.particles[i].y = EMISSION_SOURCE_Y;
    smoke.particles[i].z = EMISSION_SOURCE_Z + myRandom(emission_size);

    // Set the velocity of each particle
    smoke.particles[i].x_velocity = 0.0;
    smoke.particles[i].z_velocity = gaussianDistribution(DEFAULT_SPEED_MEAN, DEFAULT_SPEED_VAR);
    smoke.particles[i].y_velocity = 0.0;

    // Set the colour of each particle
    smoke.particles[i].r = gaussianDistribution(smoke.r, DEFAULT_SHADE_VAR);
    smoke.particles[i].g = gaussianDistribution(smoke.g, DEFAULT_SHADE_VAR);
    smoke.particles[i].b = gaussianDistribution(smoke.b, DEFAULT_SHADE_VAR);
    smoke.particles[i].a = gaussianDistribution(ALPHA_MEAN, ALPHA_VAR);
    smoke.alive_no_of_particles++;
  } // for

  if(render_as_point)
  {
    glBegin(GL_POINTS);
    for(i = 0; i < smoke.alive_no_of_particles; i++)
    {
      // Attach colour to smoke
      glColor3f(smoke.particles[i].r, 
                smoke.particles[i].g, 
                smoke.particles[i].b);
      // Set the position of the particle
      glVertex3f(smoke.particles[i].x, 
                 smoke.particles[i].y,
                 smoke.particles[i].z);
    } // for
    glEnd();
  } // if
  else
  {
    glLineWidth(2.5); 
    glBegin(GL_LINES);
    for(i = 0; i < smoke.alive_no_of_particles; i++)
    {
      glColor3f(smoke.particles[i].r, 
                smoke.particles[i].g, 
                smoke.particles[i].b);

      // Set the position of the particle
      glVertex3f(smoke.particles[i].x, 
                 smoke.particles[i].y,
                 smoke.particles[i].z);
      glVertex3f(smoke.particles[i].x + smoke.particles[i].x_velocity, 
                 smoke.particles[i].y + smoke.particles[i].y_velocity,
                 smoke.particles[i].z + smoke.particles[i].z_velocity);
    } // for
    glEnd();
  } // else
} // drawParticles

// Move the particles in the system
void moveParticles() 
{
  int i;
  for (i = 0; i < smoke.alive_no_of_particles; i++) 
  {
    if ((smoke.particles[i].r <= LIVING_THRESHOLD 
      && smoke.particles[i].g <= LIVING_THRESHOLD 
      && smoke.particles[i].b <= LIVING_THRESHOLD) 
      || smoke.particles[i].a <= LIVING_THRESHOLD) 
    {
      smoke.particles[i] = smoke.particles[smoke.alive_no_of_particles - 1];
      smoke.alive_no_of_particles--;
    } // if
    else // if the particle is still alive, move it
    {
      smoke.particles[i].x += smoke.particles[i].x_velocity;
      smoke.particles[i].y += smoke.particles[i].y_velocity;
      smoke.particles[i].z += smoke.particles[i].z_velocity;

      if(smoke.particles[i].y < EMISSION_SOURCE_Y)
        smoke.particles[i]. y = EMISSION_SOURCE_Y;

      // Modify the velocity of each (relevant) component
      smoke.particles[i].x_velocity += (gaussianDistribution(RANDOM_DIRECTION_MEAN, RANDOM_DIRECTION_VAR) + smoke.particles[i].y * wind_u_component) * 1.5;

      smoke.particles[i].z_velocity += (boxMuller2Rand + smoke.particles[i].y * wind_v_component) * 1.5;

      smoke.particles[i].y_velocity += mass * gravity + gaussianDistribution(RANDOM_DIRECTION_MEAN, RANDOM_DIRECTION_MEAN * 2.0);

      // Decrease the intensity of the colour at each step (fade it away)
      double decrease_amount = 0.0001;
      double shadeChange;
      shadeChange = gaussianDistribution(SMOKE_SHADE_CHANGE_MEAN, SMOKE_SHADE_CHANGE_VAR);
      smoke.particles[i].r -= shadeChange;
      smoke.particles[i].g -= shadeChange;
      smoke.particles[i].b -= shadeChange;
      smoke.particles[i].a -= decrease_amount;
    } // else
  } // for
} // moveParticle

/*
 * Display function, called every time the scene needs to be redrawn
 */
void display()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 240.0, 500.0, // eyeX, eyeY, eyeZ
            0.0, 240.0, 0.0, // centerX, centerY, centerZ
            0.0, 1.0, 0.0); // upX, upY, upZ
  glClear(GL_COLOR_BUFFER_BIT);
  drawParticles();
  moveParticles();
  glutPostRedisplay(); // Mark the current window to be redisplayed
  glutSwapBuffers();
} // display

/*
 * Use keyboard keys to control the simulation
 */
void keyboard(unsigned char key, int x, int y)
{
  switch(key) 
  {
    // Quit the program
    case 27: exit(0); break;

    // Play with the colour of a particle
    case 'c': 
      if (smoke.r - 0.05 > 0.6)
        smoke.r = smoke.r - 0.05;
      if (smoke.g - 0.05 > 0.3)
        smoke.g = smoke.g - 0.05;
      if (smoke.b - 0.05> 0.0)
        smoke.b = smoke.b - 0.05;
      break;
    case 'C':
      if (smoke.r + 0.05 <= 1.0)
        smoke.r = smoke.r + 0.05;
      if (smoke.g + 0.05 <= 1.0)
        smoke.g = smoke.g + 0.05;
      if (smoke.b + 0.05 <= 1.0)
        smoke.b = smoke.b + 0.05;
      break;

    // Change the rendering technique (use points or lines)
    case '1': render_as_point = 1; break;
    case '2': render_as_point = 0; break;

    // Play with the point size of a particle
    case 's': 
      point_size -= 1;
      glPointSize(point_size); break; 
    case 'S': 
      point_size += 1;
      glPointSize(point_size); break;

    // Play with the random speed of a particle
    case 'e': emission_size -= 2; break; 
    case 'E': emission_size += 2; break;  
    
    // Play with the no. of particles (add or remove 500 particles at each key press)
    case 'p': if (smoke.total_no_of_particles >= 1000)
      smoke.total_no_of_particles -= 500; break;
    case 'P': if (smoke.total_no_of_particles <= (MAX_PARTICLES - 500))
      smoke.total_no_of_particles += 500; break;

    // Play with the speed of the wind
    case 'w': 
      if(windSpeed > 0.1) 
        windSpeed *= 0.8;
      printf("Decreased wind %f\n", windSpeed);
      updateWindComponents(); break;
    case 'W': 
      if(windSpeed < 1000000.0) 
        windSpeed *= 1.2;
      printf("Increased wind %f\n", windSpeed);
      updateWindComponents(); break;

    // Play with the wind direction
    case 'd': 
      theta = ((int)(theta - 10)) % 360;
      printf("Theta %f\n", theta);
      updateWindComponents(); break;
    case 'D': 
      theta = ((int)(theta + 10)) % 360;
      printf("Theta %f\n", theta);
      updateWindComponents(); break;

    // Play with gravity value
    case 'g': gravity *= 1.1; 
      printf("Decreased gravity is %f\n", gravity); break;
    // Decrease gravity
    case 'G': gravity *= 0.9;
      printf("Increased gravity is %f\n", gravity); break;

  }
  glutPostRedisplay();
} // keyboard

/*
 * Reshape function - called every time the window is reshaped
 */
void reshape(int width, int height)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
} // reshape

/*
 * Reset the simulation to its initial values.
 */
void resetSimulation()
{
  // Second, initialize the smoke system
  smoke.total_no_of_particles = DEFAULT_NO_OF_PARTICLES;
  smoke.alive_no_of_particles = 0;
  smoke.r = 0.9;
  smoke.g = 0.6;
  smoke.b = 0.3;

  random_speed = RANDOM_DIRECTION_VAR;
  theta = 60.0;
  mass = MASS;
  gravity = GRAVITY;
  windSpeed = WIND_SPEED;
  point_size = POINT_SIZE;
  glPointSize(point_size);
  emission_size = EMISSION_SIZE;
  updateWindComponents();
} // reset

/*
 * Create the menu selection entries
 */
void menu (int selection) {
  switch (selection) 
  {
    // Reset parameters to starting values
    case 1: resetSimulation(); break;
    case 7: exit(0); 
  }
} // menu

/*
 * Create a menu
 */
void createMenu()
{
  glutCreateMenu(menu);
  glutAddMenuEntry("Reset simulation", 1);
  glutAddMenuEntry ("Quit", 7);
  glutAttachMenu (GLUT_RIGHT_BUTTON);
} // createMenu

/*
 * Initialize the graphics in the scene
 */
 void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("Particle System - COMP37111");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  createMenu();

  glEnable(GL_POINT_SMOOTH);
  glPointSize(point_size);
} // initGraphics

/*
 * Main Method
 */
int main(int argc, char *argv[])
{
  srand(time(NULL));
  resetSimulation();
  initGraphics(argc, argv);
  glutMainLoop();
  return 0;
} // main