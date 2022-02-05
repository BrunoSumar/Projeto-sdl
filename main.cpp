#include <string>
#include <iostream>
#include <ctime>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

#else
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#endif

#include "scene.h"

using namespace std;

// Global declarations
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
SDL_Event e;

Scene scene;

float rot = 0.f;
float dist = 0.f;
float rot_x = 0.f;
// int sprite = 1;

ShaderProgram *cartaoSP;
ShaderProgram *fundoSP;
ShaderProgram *cenarioSP;
ShaderProgram *pisoSP;

clock_t begin_time;
float tempo_atual = 0;

// Inicializa-se o objeto Scene scene.
void setupScene(){
  scene.setProjection(
    scale(1., 1., -1.)*perspective(
      45.0f,                                    // fov
      (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, // aspect ratio
      0.1f,                                     // near
      100.f                                     // far
    ));

  scene.setView(
    lookAt(
      {5.5f, 2.f, 0.f},  // eye
      {-2.f, .0f, 0.f},  // center
      {0.f, 1.f, 0.f}    // up
    ));

  cartaoSP = new ShaderProgram{
    Shader{"shaders/cartao.vert", GL_VERTEX_SHADER},
    Shader{ "shaders/cartao.frag", GL_FRAGMENT_SHADER}
  };

  fundoSP = new ShaderProgram{
    Shader{"shaders/fundo.vert", GL_VERTEX_SHADER},
    Shader{ "shaders/fundo.frag", GL_FRAGMENT_SHADER}
  };

  cenarioSP = new ShaderProgram {
    Shader{"shaders/vertex_shader", GL_VERTEX_SHADER},
    Shader{"shaders/fragment_shader", GL_FRAGMENT_SHADER}
  };

  pisoSP = new ShaderProgram{
    Shader{"shaders/piso.vert", GL_VERTEX_SHADER},
    Shader{ "shaders/piso.frag", GL_FRAGMENT_SHADER}
  };

  scene.addFundo("resources/folha.png", fundoSP);

  scene.addFigura("resources/cenario4.obj", "resources/cenario4.png");
  scene.figuras.back()->program = cenarioSP;
  scene.figuras.back()->model = scale(2., 2., 2.);

  scene.mapa.initPersonagem(cartaoSP);
  scene.mapa.initPiso(pisoSP);

  Inimigo *cubo = new Cubinho(cartaoSP, 7, 2);
  scene.mapa.addUnidade(cubo);
}

// Inicialização do sdl e opengl
bool init()
{
  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
      printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
      return false;
    }

  // GL 3.3 + GLSL 130
  const char* glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

  //Criando janela
  gWindow = SDL_CreateWindow( "Jogo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
			      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if( gWindow == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      return false;
    }

  //Criando contexto opengl
  gContext = SDL_GL_CreateContext( gWindow );
  if( gContext == NULL )
    {
      printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
      return false;
    }

  SDL_GL_MakeCurrent(gWindow, gContext);

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

  //Inicializando OpenGL
  glClearColor(.3f, 0.6f, 1.f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glCullFace(GL_FRONT);
  // Inicialização do Imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(gWindow, gContext);
  ImGui_ImplOpenGL3_Init(glsl_version);
  return true;
}
static bool quit = false;
static bool isPaused = true;
static bool no_titlebar = false;
static bool no_scrollbar = false;
static bool no_menu = false;
static bool no_move = false;
static bool no_resize = false;
static bool no_collapse = false;
static bool no_close = false;
static bool no_nav = false;
static bool no_background = false;
static bool no_bring_to_front = false;
static bool unsaved_document = false;
ImGuiWindowFlags window_flags = 0;

void eventHandler( SDL_Event &e ) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  ImGui_ImplSDL2_ProcessEvent(&e);
  if( e.type == SDL_KEYDOWN )
  {
    if( e.key.keysym.sym == SDLK_PAUSE)
      isPaused = true;

    // Controle da câmera
    //
    // <- e -> Giram em torno do centro em direção horizontal
    else if( e.key.keysym.sym == SDLK_RIGHT)
      rot += .02;
    else if ( e.key.keysym.sym == SDLK_LEFT)
      rot -= .02;

    // /\ e \/ Aproximam e afastam do centro
    else if ( e.key.keysym.sym == SDLK_DOWN )
      dist += .08;
    else if ( e.key.keysym.sym == SDLK_UP )
      dist -= .08;

    // f-\/ e r-/\ rotaciona torno do centro em direção vertical
    else if ( e.key.keysym.sym == SDLK_r )
      rot_x += .02;
    else if ( e.key.keysym.sym == SDLK_f )
      rot_x -= .02;

    // movimentação
    else if ( e.key.keysym.sym == SDLK_w )
      scene.mapa.moverPersonagem(0, 1);
    else if ( e.key.keysym.sym == SDLK_s )
      scene.mapa.moverPersonagem(0, -1);
    else if ( e.key.keysym.sym == SDLK_a )
      scene.mapa.moverPersonagem(-1, 0);
    else if ( e.key.keysym.sym == SDLK_d )
      scene.mapa.moverPersonagem(1, 0);

    else if ( e.key.keysym.sym == SDLK_j )
      scene.mapa.tiroPersonagem(tempo_atual);
  }

  if (e.type == SDL_WINDOWEVENT)
  {
    if (e.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      SDL_SetWindowSize(gWindow , e.window.data1, e.window.data2);
      glViewport(0, 0, e.window.data1, e.window.data2);
    }
  }

  if( e.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE] )
  {
    quit = true;
  }
}

// the state
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// Função de renderização
void main_loop(){
  while( SDL_PollEvent( &e ) != 0 )
  {
    eventHandler(e);
    if(quit == true)
      break;
  }
scene.setView(
	      lookAt(
		     toVec3( rotate_y(rot) * rotate_z(rot_x) * translate(dist, .0, .0) * vec4{5.8f, 3.f, 0.f, 1.f} ),// eye
		     toVec3( rotate_y(rot) * rotate_z(rot_x) * translate(dist, .0, .0) * vec4{-.5f, 0.f, 0.f, 1.f} ),// center
		     {0.f, 1.f, 0.f}   // up
		     )
	      );
// Start the Dear ImGui frame
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplSDL2_NewFrame();
ImGui::NewFrame();

window_flags = 0;
if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

if (isPaused) 
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Nome do Jogo", NULL, window_flags);                          

    if (ImGui::Button("Resume game"))                            
      isPaused = false;

    if (ImGui::Button("Quit"))                            
      quit = true;

    if (ImGui::BeginTable("split", 3))
      {
	ImGui::TableNextColumn(); ImGui::Checkbox("No titlebar", &no_titlebar);
	ImGui::TableNextColumn(); ImGui::Checkbox("No scrollbar", &no_scrollbar);
	ImGui::TableNextColumn(); ImGui::Checkbox("No menu", &no_menu);
	ImGui::TableNextColumn(); ImGui::Checkbox("No move", &no_move);
	ImGui::TableNextColumn(); ImGui::Checkbox("No resize", &no_resize);
	ImGui::TableNextColumn(); ImGui::Checkbox("No collapse", &no_collapse);
	ImGui::TableNextColumn(); ImGui::Checkbox("No close", &no_close);
	ImGui::TableNextColumn(); ImGui::Checkbox("No nav", &no_nav);
	ImGui::TableNextColumn(); ImGui::Checkbox("No background", &no_background);
	ImGui::TableNextColumn(); ImGui::Checkbox("No bring to front", &no_bring_to_front);
	ImGui::TableNextColumn(); ImGui::Checkbox("Unsaved document", &unsaved_document);
	ImGui::EndTable();
      }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }
  ImGui::Render();

  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Desenha cena
  tempo_atual = float( clock() - begin_time ) /  CLOCKS_PER_SEC;

  scene.mapa.actions(tempo_atual);
  scene.draw(tempo_atual);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Atualização da janela
  SDL_GL_SwapWindow( gWindow );
};
// Libera recursos
void close()
{
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  SDL_GL_DeleteContext(gContext);
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

  begin_time = clock();
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
