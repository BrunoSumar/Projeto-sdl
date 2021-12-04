#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#include "scene.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

#else
#include <SDL2/SDL_opengl.h>
#include <GL/glew.h>

#endif

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
SDL_Event e;

Scene scene;

void setupScene(){
    scene.shader = {"./shaders/vertexShader", "./shaders/fragmentShader"};

    scene.cam = {
        {.0f, .0f, 3.0f},
        {.0f, .0f, .0f},
        {.0f, 1.0f, .0f}
    };

    scene.aRatio = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
    scene.near = 0.1f;
    scene.far = 100.0f;
    scene.ang = 45.0f;

    scene.updateMatrixes();

    //scene.addElement({"./untitled.obj", glm::scale(glm::mat4(1.f) , glm::vec3(.3, .3, .3))});
    scene.addElementAndTex({"./untitled.obj",
                            glm::scale(glm::mat4(1.f) , glm::vec3(1, 1, 1))},
                            "./tex.jpg");
}

bool init()
{

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  //Use OpenGL 2.1
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

  //Create window
  gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  if( gWindow == NULL )
  {
    printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  //Create context
  gContext = SDL_GL_CreateContext( gWindow );
  if( gContext == NULL )
  {
    printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  //Use Vsync
  if( SDL_GL_SetSwapInterval( 1 ) < 0 )
  {
    printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
  }

  //Inicializando GLEW
  glewExperimental = GL_FALSE;
  if( glewInit() != GLEW_OK )
    throw std::runtime_error("glewInit failed");

  //Inicializando o DEPTH buffer
  glEnable(GL_DEPTH_TEST);

  //Initialize OpenGL
  glClearColor( 1.f, 0.5f, 0.5f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  return true;
}

static bool quit = false;
void main_loop(){
  while( SDL_PollEvent( &e ) != 0 )
  {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    //User requests quit
    if( e.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE] )
    {
      quit = true;
    }
    //Handle keypress with current mouse position
    else if( e.type == SDL_KEYDOWN )
    {
      // quit = true;
      glClearColor( 0.f, 0.5f, 0.5f, 1.f );
    }
  }

  // glClearColor( 1.f, 1.f, 1.f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  scene.Draw();
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

      setupScene();

      while(!quit) {
        main_loop();
        SDL_Delay(16);
      }

    #endif
	}

	close();

  return 0;
}
