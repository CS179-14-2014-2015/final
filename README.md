#Finals  (deadline: 2015-03-XX)

Submit your finals here. Here are a couple of reminders:

1. Do not commit compiler or IDE-generated files (the *.o, *.pdb among others)
2. Documentation, especially on how to build and play the game, is appreciated.
3. Also commit a PDF version of the [Certificate of Authorship for groups](http://www.admu.edu.ph/ls/sose/iscs/downloads).

to build the game you'll need to get SDL2 and SDL_gfx running for your project here is one way to do it:


Using eclipse kepler on mac OSX

1.) Download SDL framework from http://www.libsdl.org/download-2.0.php then copy paste the SDL framework in your Library/Frameworks
2.) make a new C++ project with executable Hello World C++ Project and name it main.cpp
3.) right click your project then click properties->c/c++ Build->Settings->Tool Settings tab->Miscellaneous and this to the linker flags "-framework SDL2 -framework Cocoa" without the quotes
4.) Download SDL2_gfx from http://cms.ferzkopp.net/index.php/software/13-sdl-gfx
5.) unzip your SDL2_gfx and add these files to where your project directory is
    SDL2_gfxPrimitives.h
    SDL2_gfxPrimitives.c
    SDL2_gfxPrimitives_font.h
    SDL2_rotozoom.c
    SDL2_rotozoom.h
    
6.) change SDL2_gfxPrimitives.h and SDL2_rotozoom.h's line where it says #include "SDL.h" to
#include "SDL2/SDL.h"
7.) Build and run it





This game has 3 classes, FIGHTER, ALIEN, BOSS

all 3 have a method called move() that lets them move around and shoot bullets based on their shootTimer but only the fighter is controlled by the player. The FIGHTER has a handleEvent() method to know how it should move around and to fire its bullets towards the enemy

all 3 have hp and a method gotHit so that they lose 1 hp if the code detected that a bullet touched them. 

all 3 have render() so that they can load the respective bmp file as their avatar

using boxRGBA of the SDL_gfx and the getHp() method of the boss and fighter I show their respective health on the right most part of the game
