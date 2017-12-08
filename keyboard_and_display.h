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

// Update u and v components of the wind (coresponding to x and z direction)
// This needs to be done as we either changed the wind speed or the angle theta
// http://colaweb.gmu.edu/dev/clim301/lectures/wind/wind-uv.html
void windComponents()
{
	double theta_in_radians = theta * PI / 180.0;
  wind_u_component = windSpeed * cos(theta_in_radians);
  wind_v_component = windSpeed * sin(theta_in_radians);
} // WindComponents

// Print the options available from keyboard.
void displayMenu()
{
	printf("\n---------------------------------------------------\n");
	printf("\nKeyboard options\n");
	printf("\n---------------------------------------------------\n");
	printf("p - decrease number of particles (min. 500)\n");
	printf("P - increase number of particles (max. 1,000,000)\n\n");
	printf("c - decrease colour values\n");
	printf("C - increase colour value\n\n");
	printf("s - decrease point size (if render as point)\n");
	printf("S - increase point size (if render as point)\n\n");
	printf("e - decrease emission size\n");
	printf("E - increase emission size\n\n");
	printf("l - decrease lifetime of a particle (min. 0.0)\n");
	printf("L - increase lifetime of a particle (max. 1.0)\n\n");
	printf("w - decrease wind speed\n");
	printf("W - increase wind speed\n\n");
	printf("d - decrease wind direction (min. 0.0 angle)\n");
	printf("D - increase wind direction (max. 180.0 angle)\n\n");
	printf("g - decrease gravity\n");
	printf("G - increase gravity\n\n");
	printf("---------------------------------------------------\n");
} // displayMenu

// Use keyboard keys to control the simulation
void keyboard(unsigned char k, int x, int y)
{
  switch(k) 
  {
    // Play with the colours of the particle system
    case 'c': 
    	if (fire.r - 0.025 >= 0.599)
        fire.r = fire.r - 0.05;
      if (fire.g - 0.025 >= 0.299)
        fire.g = fire.g - 0.05;
      if (fire.b - 0.025 >= 0.000)
        fire.b = fire.b - 0.025;
      if(fire.r > 0.6001 || fire.g > 0.3001 || fire.b > 0.0001)
      	printf("Decreased RGB colours to r = %f, g = %f, b = %f.\n",
      					fire.r, fire.g, fire.b);
      else
      	printf("Cannot change colour further.\n");
      break;
    case 'C':
    	if (fire.r + 0.025 <= 1.01)
        fire.r = fire.r + 0.025;
      if (fire.g + 0.025 <= 0.91)
        fire.g = fire.g + 0.025;
      if (fire.b + 0.025 <= 0.61)
        fire.b = fire.b + 0.025;
      if(fire.r < 1.01 || fire.g < 0.91 || fire.b < 0.61)
      	printf("Increased RGB colours to r = %f, g = %f, b = %f.\n",
      					fire.r, fire.g, fire.b);
      else
      	printf("Cannot change colour further.\n");
      break;

    // Play with the point size (depending on rendering type)
    case 's': 
      if (render_as_point || render_as_texture)
      {
      	if(point_size > 1)
      	{
      		point_size -= 1;
      		glPointSize(point_size); 
      		printf("Decreased point size to %f\n", point_size);
      	} // if
      	else
      	{
      		printf("Cannot decrease point size further.\n");
      	} // else
    	} // if render as point
    	break;
    case 'S': 
      if (render_as_point || render_as_texture)
      {
      	if(point_size < 100)
      	{
      		point_size += 1;
      		glPointSize(point_size); 
      		printf("Increased point size to %f\n", point_size);
      	}	// if
      	else
      	{
      		printf("Cannot increase point size further.\n");
      	} // else
    	} // if render as point
    	break;

    // Play with the emission size of the particle system
    case 'e': 
    	if(emission_size > 2)
    	{
    		emission_size -= 2;
      	printf("Decreased emission size to %f\n", emission_size);
    	} // if
    	else
    	{
      	printf("Cannot decreased emission size further.\n");
    	} // else
      break; 
    case 'E': 
    	if(emission_size < 800)
    	{
    		emission_size += 2; 
      	printf("Increased emission size to %f\n", emission_size);
    	} // if
    	else
    	{
    		printf("Cannot increased emission size further.\n");
    	} // else
    	break; 

    // Play with the no. of particles (add or remove 500 particles at each key press)
    case 'p': 
      if (fire.total_no_of_particles >= 1000)
      {
        fire.total_no_of_particles -= 500; 
        printf("Decreased number of particles to %d\n", fire.total_no_of_particles);
      } // if
      else
      {
      	printf("Reached minimum number of particles. Can't decrease further.\n");
      } // else
      break;
    case 'P': 
      if (fire.total_no_of_particles <= (MAX_PARTICLES - 500))
      {
      	fire.total_no_of_particles += 500;
      	printf("Increased no. of particles to %d\n", fire.total_no_of_particles);
      } // if 
      else
      {
      	printf("Reached maximum number of particles. Can't increase further.\n");
      } // else
      break;

     // Play with the lifetime of a particle. (needs to be between 0.0 and 1.0)
     case 'l':
      if (lifetime > 0.002)
     	{
     		lifetime -= 0.002;
     		printf("Decreased lifetime to %f\n", lifetime);
     	}	// if
     	else
     		printf("Cannot decrease lifetime further.\n");
     	break;
     case 'L':
      if (lifetime < 1.0)
      {
      	lifetime += 0.002;
     		printf("Increased lifetime to %f\n", lifetime);
      } // if
      else
      	printf("Cannot increase lifetime further.\n");
     	break;

    // Play with the speed of the wind
    case 'w': 
      if(windSpeed > 0.000001) 
        windSpeed *= 0.911;
      printf("Decreased wind %f\n", windSpeed);
      windComponents(); 
      break;
    case 'W': 
      if(windSpeed < 50.0) 
        windSpeed *= 1.089;
      printf("Increased wind %f\n", windSpeed);
      windComponents(); 
      break;

    // Play with the wind angle (direction)
    case 'd': 
    	if(theta >= 10)
	    {
	      theta = (int)(theta - 10);
      	printf("Decreased wind direction, theta, to %f\n", theta);
      	windComponents();
      } // if
      else
      {
      	printf("Cannot decrease wind direction further.\n");
      } // else
      break;
    case 'D': 
    	if(theta <= 170)
    	{
    		theta = (int)(theta + 10);
      	printf("Increased wind direction, theta, to %f\n", theta);
      	windComponents();
    	} // fi
    	else
    	{
				printf("Cannot increase wind direction further.\n");
    	} // else
      break;

    // Play with gravity
    case 'g': 
      gravity -= 0.5;
      printf("Decreased gravity to %f\n", gravity); 
      break;
    case 'G': 
      gravity += 0.5;
      printf("Increased gravity to %f\n", gravity); 
      break;

    // Quit the program
    case 27: 
      exit(0); 
      break;
  }
  glutPostRedisplay();
} // keyboard

// Calculate how many frames per second
// inspired from http://stackoverflow.com/questions/5627803/how-to-calculate-fps-in-opengl
void calculateFrameRate()
{
  counter++;
  end = glutGet(GLUT_ELAPSED_TIME);
  int interval = end - start;

  if(interval > 1000)
  {
    framesPerSecond = counter / (interval / 1000.0f);  
    start = end;                   
    counter = 0;                         
  }
} // calculateFrameRate

// Print data on screen
void drawString(void* font, float x, float y, char* str) 
{
  char* ch;
  glRasterPos3f(x, y, 0.0);
  for (ch = str; *ch; ch++)
    glutBitmapCharacter(font, (int) *ch);
} // drawString

void printDataOnScreen()
{
  glColor3f(1.0, 1.0, 1.0);
  
  sprintf(string, "Number of particles: %d", fire.total_no_of_particles);
  drawString(GLUT_BITMAP_9_BY_15, -350, 500 - 1 * 14, string);
  
  sprintf(string, "Wind speed: %.2f m/s", windSpeed);
  drawString(GLUT_BITMAP_9_BY_15, -350, 500 - 2 * 14, string);
  
  sprintf(string, "Wind direction: %.2f degrees", theta);
  drawString(GLUT_BITMAP_9_BY_15, -350, 500 - 3 * 14, string);
  
  sprintf(string, "Gravity: %.2f m/s^2", gravity);
  drawString(GLUT_BITMAP_9_BY_15, -350, 500 - 4 * 14, string);
  
  sprintf(string, "Lifetime: %.2f", lifetime);
  drawString(GLUT_BITMAP_9_BY_15, -350, 500 - 5 * 14, string);

  calculateFrameRate();
  sprintf(string, "Frame Rate: %.2f", framesPerSecond);
  drawString(GLUT_BITMAP_9_BY_15, -350, 500 - 6 * 14, string);
} // printDataOnScreen