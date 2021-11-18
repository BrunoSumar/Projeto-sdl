#include <GL/glew.h>
// #include <stdio.h>
#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include "scene.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

#else
#include <SDL2/SDL_opengl.h>

#endif

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
SDL_Event e;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
        glClearColor( 1.f, 0.5f, 0.5f, 1.f );
			}
		}
	}

	return success;
}

static bool quit = false;
void main_loop(){
  while( SDL_PollEvent( &e ) != 0 )
  {
    //User requests quit
    if( e.type == SDL_QUIT )
    {
      quit = true;
    }
    //Handle keypress with current mouse position
    else if( e.type == SDL_TEXTINPUT )
    {
      int x = 0, y = 0;
      // SDL_GetMouseState( &x, &y );
      // handleKeys( e.text.text[ 0 ], x, y );
    }
  }

  glClear( GL_COLOR_BUFFER_BIT );
  //Update screen
  SDL_GL_SwapWindow( gWindow );
};

void close()
{
  //Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char *argv[]) {
    //teste();

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		quit = false;

		SDL_StartTextInput();


    #ifdef __EMSCRIPTEN__

      emscripten_set_main_loop(main_loop, 0, 1);

    #else

      while(!quit) {
        main_loop();
        SDL_Delay(16);
      }

    #endif
	}

	close();

  return 0;
}
