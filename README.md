# Particle-System

Needs installation fo the SOIL library:

sudo apt-get install libsoil-dev

Compile:

gcc particle.c -o particle -lGL -lGLU -lglut -lm -lSOIL

Run:

./particle

And in case you face the error: 

undefined reference to 'SOIL_load_OGL_texture' 

then link -lSOIL before linking any other library 

(https://stackoverflow.com/questions/17497497/undefined-reference-to-soil-load-ogl-texture

https://stackoverflow.com/questions/18886598/how-do-i-install-soil-simple-opengl-image-loader/18887019#18887019)


Resources:

Textures from OpenGameArt: https://opengameart.org/content/explosion-particles-sprite-atlas

Simple OpenGL Image Library http://www.lonesock.net/soil.html
