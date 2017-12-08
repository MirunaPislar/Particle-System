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
void drawParticles()
{
  int i;

  for(i = fire.alive_no_of_particles; i < fire.total_no_of_particles; i++)
  {
    // Set the emission point for each particle
    fire.particles[i].x = EMISSION_SOURCE_X + myRandom(emission_size);
    fire.particles[i].y = EMISSION_SOURCE_Y;
    fire.particles[i].z = EMISSION_SOURCE_Z + myRandom(emission_size);

    // Set the velocity of each particle
    fire.particles[i].x_velocity = 0.0;
    fire.particles[i].y_velocity = normalDistribution(SPEED_MEAN, SPEED_VAR);
    fire.particles[i].z_velocity = 0.0;

    // Set the colour of each particle
    fire.particles[i].r = normalDistribution(fire.r, SHADE_VAR);
    fire.particles[i].g = normalDistribution(fire.g, SHADE_VAR);
    fire.particles[i].b = normalDistribution(fire.b, SHADE_VAR);
    fire.particles[i].a = normalDistribution(ALPHA_MEAN, ALPHA_VAR);
    fire.alive_no_of_particles++;
  } // for

  if(render_as_point)
  {
    glBegin(GL_POINTS);
    for(i = 0; i < fire.alive_no_of_particles; i++)
    {
      glColor3f(fire.particles[i].r, 
                fire.particles[i].g, 
                fire.particles[i].b);
      glVertex3f(fire.particles[i].x, 
                 fire.particles[i].y,
                 fire.particles[i].z);
    } // for
    glEnd();
  } // if
  else if(render_as_lines)
  {
    glBegin(GL_LINES);
    for(i = 0; i < fire.alive_no_of_particles; i++)
    {
      glColor3f(fire.particles[i].r, 
                fire.particles[i].g, 
                fire.particles[i].b);
      glVertex3f(fire.particles[i].x, 
                 fire.particles[i].y,
                 fire.particles[i].z);
      glVertex3f(fire.particles[i].x + 5.0 * fire.particles[i].x_velocity, 
                 fire.particles[i].y + 5.0 * fire.particles[i].y_velocity,
                 fire.particles[i].z + 5.0 * fire.particles[i].z_velocity);
    } // for
    glEnd();
  } // else
  else if (render_as_texture)
  {
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_TEXTURE_2D);
    for (i = 0; i < fire.alive_no_of_particles; i++) 
    {
    	if(explosion)
    		glBindTexture(GL_TEXTURE_2D, explosion_texture);
    	if(steam)
      	glBindTexture(GL_TEXTURE_2D, steam_textures[i % 9]);
      glBegin(GL_POINTS);
      glColor4f(fire.particles[i].r, fire.particles[i].g, fire.particles[i].b, fire.particles[i].a);
      glVertex3f(fire.particles[i].x, fire.particles[i].y, fire.particles[i].z);
      glEnd();
    } // for
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_POINT_SPRITE);
  } // else
} // drawParticles

// Move the particles in the system
void moveParticles() 
{
  int i;
  for (i = 0; i < fire.alive_no_of_particles; i++) 
  {
    if ((((fire.r - fire.particles[i].r) > lifetime)
     && ((fire.g - fire.particles[i].g) > lifetime)
     && ((fire.b - fire.particles[i].b) > lifetime))
     || (fire.particles[i].a < ALPHA_LIFETIME))
    {
      fire.particles[i] = fire.particles[fire.alive_no_of_particles - 1];
      fire.alive_no_of_particles--;
    } // if
    else // if the particle is still alive, move it
    {
      fire.particles[i].x += fire.particles[i].x_velocity;
      fire.particles[i].y += fire.particles[i].y_velocity;
      fire.particles[i].z += fire.particles[i].z_velocity;

      if(fire.particles[i].y < EMISSION_SOURCE_Y)
        fire.particles[i].y = EMISSION_SOURCE_Y;

      // Modify the velocity of each (relevant) component
      fire.particles[i].x_velocity += windSpeed * wind_u_component; 
      fire.particles[i].y_velocity += windSpeed * wind_v_component;
      fire.particles[i].z_velocity += mass * gravity;
      if (add_randomness)
      {
    		fire.particles[i].x_velocity += normalDistribution(RANDOM_DIRECTION_MEAN, RANDOM_DIRECTION_VAR);
      	fire.particles[i].z_velocity += normalDistribution(RANDOM_DIRECTION_MEAN, RANDOM_DIRECTION_VAR);
      }

      // Decrease the intensity of the colour at each step
      double decrease_amount = 0.0001;
      double shadeChange;
      shadeChange = normalDistribution(COLOUR_MEAN, COLOUR_VAR);
      fire.particles[i].r -= shadeChange;
      fire.particles[i].g -= shadeChange;
      fire.particles[i].b -= shadeChange;
      fire.particles[i].a -= decrease_amount;
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
  fire.alive_no_of_particles = 0;
  fire.total_no_of_particles = PARTICLES;
  fire.r = 0.9;
  fire.g = 0.6;
  fire.b = 0.3;
  gravity = GRAVITY;
  mass = MASS;
  render_as_point = 0;
  render_as_lines = 0;
  render_as_texture = 1;
  explosion = 1;
  steam = 0;
  add_randomness = 0;
  if(render_as_point)
  	point_size = POINT_SIZE;
  if(render_as_texture)
  	point_size = TEXTURE_POINT_SIZE;
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
    case 1: 
    	resetSimulation(); 
    	break;
    case 2: 
    	render_as_point = 1;
    	point_size = POINT_SIZE;
    	glPointSize(point_size);
    	render_as_lines = 0;
    	render_as_texture = 0;
    	explosion = steam = 0;
    	break;
    case 3: 
    	render_as_point = 0;
    	render_as_lines = 1;
    	render_as_texture = 0;
    	explosion = steam = 0;
    	break;
    case 4: 
    	render_as_point = 0;
    	render_as_lines = 0;
    	render_as_texture = 1;
    	point_size = TEXTURE_POINT_SIZE;
    	glPointSize(point_size);
    	explosion = 1;
    	steam = 0;
    	break;
    case 5: 
    	render_as_point = 0;
    	render_as_lines = 0;
    	render_as_texture = 1;
    	point_size = TEXTURE_POINT_SIZE;
    	glPointSize(point_size);
    	explosion = 0;
    	steam = 1;
    	break;
    case 6:
    	add_randomness = !add_randomness; 
    	break;
    case 7: 
    	displayMenu(); 
    	break;
    case 8: 
    	exit(0); 
    	break;
  }
} // menu

// Create a menu
void createMenu()
{
  glutCreateMenu(menu);
  glutAddMenuEntry("Reset simulation", 1);
  glutAddMenuEntry("Render as point", 2);
  glutAddMenuEntry("Render as line", 3);
  glutAddMenuEntry("Render fire texture", 4);
  glutAddMenuEntry("Render steam texture", 5);
  glutAddMenuEntry ("Add randomness", 6);
  glutAddMenuEntry ("Keyboard options", 7);
  glutAddMenuEntry ("Quit", 8);
  glutAttachMenu (GLUT_RIGHT_BUTTON);
} // createMenu

// Initialize the graphics in the scene
 void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("Particle System - COMP37111");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  createMenu();
  glEnable(GL_POINT_SMOOTH);
  glPointSize(point_size);

  // Textures obatined from OpenGameArt.org (see licence in PNG directory)
  // https://opengameart.org/content/explosion-particles-sprite-atlas
  // Used SOIL (Simple Graphics Image Library) to load PNG images used as textures
  // Details: https://www.khronos.org/opengl/wiki/Image_Libraries
  // and here http://www.lonesock.net/soil.html
  if(render_as_texture)
  {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_POINT_SPRITE);
    sprintf(string, "PNG/Others/fire3_crop.png");
    explosion_texture = SOIL_load_OGL_texture(string, SOIL_LOAD_RGBA, 
                   SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    int i;
		for (i = 0; i < 9; i++)
  	{
  		sprintf(string, "PNG/Fart/fart0%d.png", i);
   		steam_textures[i] = SOIL_load_OGL_texture(string, SOIL_LOAD_RGBA, 
                  				SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
  	} // for
  } // if
} // initGraphics

int main(int argc, char *argv[])
{
  srand(time(NULL));
  resetSimulation();
  initGraphics(argc, argv);
  glutMainLoop();
  return 0;
} // main