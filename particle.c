/*
 * Particle System Simulation
 * 
 * COMP37111 Advanced Computer Graphics
 * School of Computer Science
 * The University of Manchester
 * 
 */
#include "particle.h"
#include "keyboard_and_display.h"

// Return random double within range [-a,a].
// Uniform random distribution.
double myRandom(double limit)
{
  return (rand()/(double)RAND_MAX * 2.0 - 1.0) * limit; 
} // myRandom

// Normal random distribution - obtain a Gaussian random.
double normalDistribution(double mean, double std)
{
  double u1, u2, w;
  w = 1.0;
  while (w >= 1.0)
  {
    u1 = myRandom(1.0);
    u2 = myRandom(1.0);
    w = u1 * u1 + u2 * u2;
  } // while
  w = sqrt((-2.0 * log(w)) / w);
  return u1 * w * std + mean;
} // normalDistribution

// Draw the particle system.
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
    smoke.particles[i].y_velocity = normalDistribution(SPEED_MEAN, SPEED_VAR);
    smoke.particles[i].z_velocity = 0.0;

    // Set the colour of each particle
    smoke.particles[i].r = normalDistribution(smoke.r, SHADE_VAR);
    smoke.particles[i].g = normalDistribution(smoke.g, SHADE_VAR);
    smoke.particles[i].b = normalDistribution(smoke.b, SHADE_VAR);
    //smoke.particles[i].a = gaussianDistribution(ALPHA_MEAN, ALPHA_VAR);
    smoke.alive_no_of_particles++;
  } // for

  if(render_as_point)
  {
    glBegin(GL_POINTS);
    for(i = 0; i < smoke.alive_no_of_particles; i++)
    {
      glColor3f(smoke.particles[i].r, 
                smoke.particles[i].g, 
                smoke.particles[i].b);
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
      glVertex3f(smoke.particles[i].x, 
                 smoke.particles[i].y,
                 smoke.particles[i].z);
      glVertex3f(smoke.particles[i].x + 2.0 * smoke.particles[i].x_velocity, 
                 smoke.particles[i].y + 2.0 * smoke.particles[i].y_velocity,
                 smoke.particles[i].z + 2.0 * smoke.particles[i].z_velocity);
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
    if (((smoke.r - smoke.particles[i].r) > lifetime)
     && ((smoke.g - smoke.particles[i].g) > lifetime)
     && ((smoke.b - smoke.particles[i].b) > lifetime))   
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
        smoke.particles[i].y = EMISSION_SOURCE_Y;

      // Modify the velocity of each (relevant) component
      smoke.particles[i].x_velocity += windSpeed * wind_u_component; 
      smoke.particles[i].y_velocity += windSpeed * wind_v_component;
      smoke.particles[i].z_velocity += mass * gravity;
      if (add_randomness)
      {
    		smoke.particles[i].x_velocity += normalDistribution(RANDOM_DIRECTION_MEAN, RANDOM_DIRECTION_VAR);
      	smoke.particles[i].z_velocity += normalDistribution(RANDOM_DIRECTION_MEAN, RANDOM_DIRECTION_VAR);
      }

      // Decrease the intensity of the colour at each step
      double decrease_amount = 0.0001;
      double shadeChange;
      shadeChange = normalDistribution(COLOUR_MEAN, COLOUR_VAR);
      smoke.particles[i].r -= shadeChange;
      smoke.particles[i].g -= shadeChange;
      smoke.particles[i].b -= shadeChange;
      //smoke.particles[i].a -= decrease_amount;
    } // else
  } // for
} // moveParticle

// Display function, called every time the scene needs to be redrawn
void display()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 240.0, 500.0,
            0.0, 240.0, 0.0,
            0.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  drawParticles();
  moveParticles();
  glutPostRedisplay();
  printDataOnScreen();
  glutSwapBuffers();
} // display

// Reshape function - called every time the window is reshaped
void reshape(int width, int height)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
} // reshape

// Reset the simulation to its initial values. 
// Also used when first initializign the system.
void resetSimulation()
{
  smoke.alive_no_of_particles = 0;
  smoke.total_no_of_particles = PARTICLES;
  smoke.r = 0.9;
  smoke.g = 0.6;
  smoke.b = 0.3;
  gravity = GRAVITY;
  mass = MASS;
  render_as_point = 1;
  add_randomness = 0;
  point_size = POINT_SIZE;
  glPointSize(point_size);
  emission_size = EMISSION_SIZE;
  lifetime = LIVING_THRESHOLD;
  windSpeed = WIND_SPEED;
  theta = WIND_ANGLE;
  random_speed = RANDOM_DIRECTION_VAR;
  windComponents();
} // reset

// Create the menu selection entries
void menu (int selection) {
  switch (selection) 
  {
    // Reset parameters to starting values
    case 1: resetSimulation(); break;
    case 2: render_as_point = 1; break;
    case 3: render_as_point = 0; break;
    case 4: add_randomness = !add_randomness; break;
    case 5: displayMenu(); break;
    case 6: exit(0); break;
  }
} // menu

// Create a menu
void createMenu()
{
  glutCreateMenu(menu);
  glutAddMenuEntry("Reset simulation", 1);
  glutAddMenuEntry("Render as point", 2);
  glutAddMenuEntry("Render as trail", 3);
  glutAddMenuEntry ("Add randomness", 4);
  glutAddMenuEntry ("Keyboard options", 5);
  glutAddMenuEntry ("Quit", 6);
  glutAttachMenu (GLUT_RIGHT_BUTTON);
} // createMenu

// Initialize the graphics in the scene
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

// Main Method
int main(int argc, char *argv[])
{
  srand(time(NULL));
  resetSimulation();
  initGraphics(argc, argv);
  glutMainLoop();
  return 0;
} // main