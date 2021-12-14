#include <string>
#include <iostream>

#include <SDL2/SDL.h>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>

#else
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#endif

#include "scene.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
SDL_Event e;

Scene scene;

int sprite = 1;

void setupScene(){
  scene.setProjection(
    scale(1., 1., -1.)*perspective(
      45.0f,                                    // fov
      (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, // aspect ratio
      0.1f,                                     // near
      100.f
    ));                                  // far

  scene.setView(
    lookAt(
      {-2.f, 5.f, -3.f},// eye
      {0.f, 0.f, 0.f},  // center
      {0.f, 1.f, 0.f}   // up
    ));

  scene.addElement("src/shaders/sprite_matriz_vertex_shader", "src/shaders/fragment_shader", "src/resources/faces_tex.obj", "src/resources/megaman2x6.png");

  scene.elements.back().sprite_rows = 2;
  scene.elements.back().sprite_columns = 5;
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

  // glCullFace(GL_FRONT_AND_BACK);

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
        if( e.key.keysym.sym == SDLK_RIGHT){
            sprite = sprite + 1;
        } else if ( e.key.keysym.sym == SDLK_LEFT) {
            sprite = sprite - 1;
        }

      // quit = true;
      glClearColor( 0.f, 0.5f, 0.5f, 1.f );
    }
  }

  // glClearColor( 1.f, 1.f, 1.f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  scene.draw(sprite);
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


    setupScene();

#ifdef __EMSCRIPTEN__

    emscripten_set_main_loop(main_loop, 0, 1);

#else

    while(!quit) {
      main_loop();
      SDL_Delay(20);
    }

#endif
  }

  close();

  return 0;
}
