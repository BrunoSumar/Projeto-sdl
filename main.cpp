#include <ctime>
#include <iostream>
#include <string>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "frases.h"

#include "scene.h"

using namespace std;

// Global declarations
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow = NULL;
SDL_GLContext gContext;
SDL_Event e;

Scene scene;

float rot = 0.f;
float rot_idle = 0.f;
float dist = 0.f;
float rot_x = 0.f;

ShaderProgram *cartaoSP;
ShaderProgram *fundoSP;
ShaderProgram *cenarioSP;
ShaderProgram *pisoSP;

clock_t begin_time;
float tempo_atual = 0;
float tempo_total = 0;
float tempo_pausa = 0;
float tempo_jogo = 0;

int estado_combate = 0;

// Inicializa-se o objeto Scene scene.
void setupScene() {
  scene.setProjection(
      scale(1., 1., -1.) *
      perspective(
        45.0f,                                      // fov
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, // aspect ratio
        0.1f,                                       // near
        100.f                                       // far
      )
  );

  scene.setView(lookAt(
    {5.5f, 2.f, 0.f}, // eye
    {-2.f, .0f, 0.f}, // center
    { 0.f, 1.f, 0.f}   // up
  ));

  cartaoSP = new ShaderProgram{
    Shader{"shaders/cartao.vert", GL_VERTEX_SHADER},
    Shader{"shaders/cartao.frag", GL_FRAGMENT_SHADER}
  };
  return;

  fundoSP = new ShaderProgram{
    Shader{"shaders/fundo.vert", GL_VERTEX_SHADER},
    Shader{"shaders/fundo.frag", GL_FRAGMENT_SHADER}
  };

  cenarioSP = new ShaderProgram{
    Shader{"shaders/vertex_shader", GL_VERTEX_SHADER},
    Shader{"shaders/fragment_shader", GL_FRAGMENT_SHADER}
  };

  pisoSP = new ShaderProgram{
    Shader{"shaders/piso.vert", GL_VERTEX_SHADER},
    Shader{"shaders/piso.frag", GL_FRAGMENT_SHADER}
  };

  scene.addFundo("resources/folha.png", fundoSP);

  scene.addFigura("resources/cenario5.obj", "resources/cenario4.png");
  scene.figuras.back()->program = cenarioSP;
  scene.figuras.back()->model = scale(1.7, 1.7, 1.7);

  scene.mapa.initPersonagem(cartaoSP);
  scene.mapa.initPiso(pisoSP);

}

ImFont* font1;
ImFont* font1_grande;

ImFont* font2;
ImFont* font3;
// Inicialização do sdl e opengl
bool init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // GL 3.3 + GLSL 130
  const char *glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#ifdef __EMSCRIPTEN__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

  // Criando janela
  gWindow = SDL_CreateWindow(
    "Nature's Ghost", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );
  if (gWindow == NULL) {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Criando contexto opengl
  gContext = SDL_GL_CreateContext(gWindow);
  if (gContext == NULL) {
    printf("OpenGL context could not be created! SDL Error: %s\n",
           SDL_GetError());
    return false;
  }

  SDL_GL_MakeCurrent(gWindow, gContext);

  if (SDL_GL_SetSwapInterval(1) < 0) {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }

  // Inicializando GLEW
  glewExperimental = GL_FALSE;
  if (glewInit() != GLEW_OK)
    throw std::runtime_error("glewInit failed");

  // Inicializando o DEPTH buffer
  glEnable(GL_DEPTH_TEST);

  // Inicializando OpenGL
  glClearColor(.3f, 0.6f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glCullFace(GL_FRONT);

  // Inicialização do Imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  font1 = io.Fonts->AddFontFromFileTTF("./resources/ghouls.ghosts.and.goblins-[fontvir.us].ttf", 15);
  font1_grande = io.Fonts->AddFontFromFileTTF("./resources/ghouls.ghosts.and.goblins-[fontvir.us].ttf", 40);
  font2 = io.Fonts->AddFontFromFileTTF("./resources/alphbeta.ttf", 10);
  font3 = io.Fonts->AddFontFromFileTTF("./resources/alphbeta.ttf", 15);
  // font3 = io.Fonts->AddFontFromFileTTF("imgui/misc/fonts/Roboto-Medium.ttf", 15);
  io.Fonts->Build();

  ImGui::StyleColorsLight();

  ImGui_ImplSDL2_InitForOpenGL(gWindow, gContext);
  ImGui_ImplOpenGL3_Init(glsl_version);

  srand(time(NULL));

  return true;
}

// Estado geral do programa inteiro
ImGuiWindowFlags window_flags = 0;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool quit = false;
bool isPaused = true;
bool onBattle = false;
bool afterBattle = false;

void togglePause() {
  isPaused = !isPaused;
  if (isPaused)
    tempo_total += tempo_atual - tempo_pausa;
  tempo_pausa = tempo_atual;
}

int estadoCombate() {
  if (scene.mapa.personagem->hp <= 0)
    return -1;
  if (scene.mapa.numInimigos() <= 0)
    return 1;

  return 0;
}

int nivel = 0;
void avancarNivel(int estado){
  togglePause();
  if( !estado ){
    return;
  }
  else{
    afterBattle = nivel == 0 ? false : true;
    estado_combate = estado;

    scene.mapa.resetPersonagem();
    scene.mapa.cleanUnidades();

    nivel = (estado_combate > 0 ) ? nivel + 1 : 0;
    if(nivel > 5)
      nivel = 1;

    for( int i=0; i<nivel; i++){
      int dim1 = scene.mapa.dim1, dim2 = scene.mapa.dim2;
      int posx = dim1/2 + rand() % ((dim1 - 1) /2);
      int posy = dim2/2 + rand() % ((dim2 - 1) /2);
      Inimigo *cubo = new Cubinho(cartaoSP, posx, posy);
      scene.mapa.addUnidade(cubo);
    }
  }
}

void eventHandler(SDL_Event &e) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  ImGui_ImplSDL2_ProcessEvent(&e);
  if (e.type == SDL_KEYDOWN) {
    if (e.key.keysym.sym == SDLK_RETURN){
      afterBattle = false;
      togglePause();
    }

    // Controle da câmera
    //
    // <- e -> Giram em torno do centro em direção horizontal
    else if (e.key.keysym.sym == SDLK_RIGHT)
      rot += .02;
    else if (e.key.keysym.sym == SDLK_LEFT)
      rot -= .02;

    // /\ e \/ Aproximam e afastam do centro
    else if (e.key.keysym.sym == SDLK_DOWN)
      dist += .08;
    else if (e.key.keysym.sym == SDLK_UP)
      dist -= .08;

    // f-\/ e r-/\ rotaciona torno do centro em direção vertical
    else if (e.key.keysym.sym == SDLK_r)
      rot_x += .02;
    else if (e.key.keysym.sym == SDLK_f)
      rot_x -= .02;

    // movimentação
    else if (!isPaused) {
      if (e.key.keysym.sym == SDLK_w)
        scene.mapa.moverPersonagem(0, 1);
      else if (e.key.keysym.sym == SDLK_s)
        scene.mapa.moverPersonagem(0, -1);
      else if (e.key.keysym.sym == SDLK_a)
        scene.mapa.moverPersonagem(-1, 0);
      else if (e.key.keysym.sym == SDLK_d)
        scene.mapa.moverPersonagem(1, 0);

      else if (e.key.keysym.sym == SDLK_j)
        scene.mapa.tiroPersonagem(tempo_atual);
    }
  }

  if (e.type == SDL_WINDOWEVENT) {
    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
      SDL_SetWindowSize(gWindow, e.window.data1, e.window.data2);
      glViewport(0, 0, e.window.data1, e.window.data2);
    }
  }

  if (e.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) {
    quit = true;
  }
}

// Para testes principalmente
ImGuiWindowFlags setupJanelaCustomizavel() {

  static ImGuiWindowFlags window_flags;

  // Estado da janela
  static bool no_titlebar = true;
  static bool no_scrollbar = true;
  static bool no_menu = true;
  static bool no_move = false;
  static bool no_resize = false;
  static bool no_collapse = false;
  static bool no_close = false;
  static bool no_nav = false;
  static bool no_background = true;
  static bool no_bring_to_front = false;
  static bool unsaved_document = false;

  window_flags = 0;
  if (no_titlebar)
    window_flags |= ImGuiWindowFlags_NoTitleBar;
  if (no_scrollbar)
    window_flags |= ImGuiWindowFlags_NoScrollbar;
  if (!no_menu)
    window_flags |= ImGuiWindowFlags_MenuBar;
  if (no_move)
    window_flags |= ImGuiWindowFlags_NoMove;
  if (no_resize)
    window_flags |= ImGuiWindowFlags_NoResize;
  if (no_collapse)
    window_flags |= ImGuiWindowFlags_NoCollapse;
  if (no_nav)
    window_flags |= ImGuiWindowFlags_NoNav;
  if (no_background)
    window_flags |= ImGuiWindowFlags_NoBackground;
  if (no_bring_to_front)
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  if (unsaved_document)
    window_flags |= ImGuiWindowFlags_UnsavedDocument;

  if (ImGui::BeginTable("split", 3)) {
    ImGui::TableNextColumn();
    ImGui::Checkbox("No titlebar", &no_titlebar);
    ImGui::TableNextColumn();
    ImGui::Checkbox("No scrollbar", &no_scrollbar);
    ImGui::TableNextColumn();
    ImGui::Checkbox("No menu", &no_menu);

    ImGui::TableNextColumn();
    ImGui::Checkbox("No move", &no_move);
    ImGui::TableNextColumn();
    ImGui::Checkbox("No resize", &no_resize);
    ImGui::TableNextColumn();
    ImGui::Checkbox("No collapse", &no_collapse);

    ImGui::TableNextColumn();
    ImGui::Checkbox("No close", &no_close);
    ImGui::TableNextColumn();
    ImGui::Checkbox("No nav", &no_nav);
    ImGui::TableNextColumn();
    ImGui::Checkbox("No background", &no_background);

    ImGui::TableNextColumn();
    ImGui::Checkbox("No bring to front", &no_bring_to_front);
    ImGui::TableNextColumn();
    ImGui::Checkbox("Unsaved document", &unsaved_document);
    ImGui::EndTable();
  }

  return window_flags;
}

void menuPrincipal() {
  static bool use_work_area = true;
  static ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
  flags |= ImGuiWindowFlags_NoBackground ;

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
  ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

  ImGui::Begin("Nome do Jogo", NULL, flags);
  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1., 1., 1., 1.));
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0., 0., 0., 0.8));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.4, 0.1, 1.));

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 20.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 10.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

  auto windowheight = ImGui::GetWindowSize().y;
  auto windowWidth = ImGui::GetWindowSize().x;

  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65, 0.1, 0.1, 1.));
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (windowheight / 8));
  ImGui::PushFont(font1_grande);
  ImGui::SetWindowFontScale(4);
  string txt = "Nature's Ghost";
  auto textWidth   = ImGui::CalcTextSize(txt.c_str()).x;
  ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
  
  ImGui::Text(txt.c_str());
  ImGui::PopFont();
  ImGui::PopStyleColor();

  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 1., 1., 1.));

  ImGui::SetWindowFontScale(3);

  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (windowheight / 4));

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (windowWidth  / 3));
  if (ImGui::Button("Iniciar jogo")){
    onBattle = true;
  }

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (windowWidth  / 3));
  if (ImGui::Button("Sair"))
    quit = true;

  ImGui::PopStyleVar(4);
  ImGui::PopStyleColor(4);

  ImGui::SetCursorPosY(windowheight * 11/12);

  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 1., 1., 1.));
  ImGui::SetWindowFontScale(2);
  ImGui::Text("\t\tFeito por Bruno Macedo e Bruno Sumar.");
  ImGui::PopStyleColor();

  ImGui::End();

  // ImGui::PushFont(font3);
  // ImGui::ShowDemoWindow();
  // ImGui::PopFont();
}

void janelaAfter() {
  static ImGuiWindowFlags window_flags;
  static bool hasWindowPadding = false;
  bool had_just_now = false;

  // Configurando a janela
  window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoScrollbar;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_NoMove;

  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1., 1., 1., 1.));
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0., 0., 0., 0.8));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.4, 0.1, 1.));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 1., 1., 1.));

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 20.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 10.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(100.0f, 100.f));

  ImGui::Begin("Nome do Jogo", NULL, window_flags);

  ImGui::PushFont(font2);

  ImGui::SetWindowFontScale(5);
  if(nivel > 1)
    ImGui::Text("Estágio %d :", nivel - 1);
  if( estado_combate > 0 )
    ImGui::Text("Parabéns você venceu a batalha!!!");
  else
    ImGui::Text("Tente novamente. :(");

  ImGui::SetWindowFontScale(3);

  ImGui::Text("\n\n\n\n");
  ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 700);
  int index = (int)tempo_pausa % 8;
  ImGui::Text(frases[index].c_str());
  ImGui::PopTextWrapPos();
  ImGui::Text("\n\n\n\n");

  char botao[50];
  if( nivel == 1)
    sprintf(botao,"Reiniciar jogo", nivel);
  else
    sprintf(botao,"Continuar para estágio %d", nivel);
  if (ImGui::Button(botao)){
    afterBattle = false;
    togglePause();
  }

  if (ImGui::Button("Sair"))
   quit = true;

  ImGui::PopStyleColor(4);
  ImGui::PopStyleVar(5);

  ImGui::PopFont();


  ImGui::End();
}

void janelaDePause() {
  static ImGuiWindowFlags window_flags;
  static bool hasWindowPadding = false;
  bool had_just_now = false;

  // Configurando a janela
  window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoScrollbar;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_NoMove;

  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1., 1., 1., 1.));
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0., 0., 0., 0.8));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.4, 0.1, 1.));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 1., 1., 1.));

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 20.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 10.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(100.0f, 100.f));

  ImGui::Begin("Nome do Jogo", NULL, window_flags);

  ImGui::PushFont(font2);

  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 1., 1., 1.));
  ImGui::SetWindowFontScale(5);
  ImGui::Text("Jogo pausado.\n\n");
  ImGui::SetWindowFontScale(3);
  ImGui::Text("Estágio %d :", nivel - 1);
  ImGui::PopStyleColor();

  if (ImGui::Button("Continuar"))
    togglePause();

  if (ImGui::Button("Sair"))
    quit = true;

  auto windowheight = ImGui::GetWindowSize().y;
  auto windowWidth = ImGui::GetWindowSize().x;
  ImGui::SetCursorPosY(windowheight * 11/12);

  ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

  ImGui::PopStyleColor(4);
  ImGui::PopStyleVar(5);
  ImGui::PopFont();

  ImGui::End();
}

// Função de renderização
void main_loop() {
  while (SDL_PollEvent(&e) != 0) {
    eventHandler(e);
    if (quit == true)
      break;
  }

  tempo_atual = float(clock() - begin_time) / CLOCKS_PER_SEC;
  tempo_jogo = tempo_total + ( isPaused ? 0 : tempo_atual - tempo_pausa);

  rot_idle = isPaused ? ((int)tempo_pausa*100 % 2 ? 1 : -1) * sin((tempo_atual - tempo_pausa) * 4.)/2.5 : rot_idle*.8;
  scene.setView(
    lookAt(
      toVec3(rotate_y(rot + rot_idle ) * rotate_z(rot_x) * translate(dist, .0, .0) *
             vec4{4.8f, 2.5f, 0.f, 1.f}), // eye
      toVec3(rotate_y(rot + rot_idle ) * rotate_z(rot_x) * translate(dist, .0, .0) *
             vec4{-.5f, 0.f, 0.f, 1.f}), // center
      {0.f, 1.f, 0.f}                    // up
    )
  );

  // Inicia Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Desenha cena
  int estado = 0;
  static ImGuiWindowFlags window_flags;

  if (onBattle) {

    scene.drawFundo(tempo_jogo);
    estado = estadoCombate();
    if (!isPaused) {
      scene.mapa.actions(tempo_jogo);
    }

    if( estado )
      avancarNivel(estado);
    if( !afterBattle )
      scene.draw(tempo_jogo);

    if (isPaused){
      if( afterBattle )
        janelaAfter();
      else
        janelaDePause();
    }
    else {
      // Configurando a janela
      window_flags = 0;
      window_flags |= ImGuiWindowFlags_NoTitleBar;
      window_flags |= ImGuiWindowFlags_NoScrollbar;
      window_flags |= ImGuiWindowFlags_NoCollapse;
      window_flags |= ImGuiWindowFlags_NoBackground;
      window_flags |= ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

      ImGui::Begin("Batalha", NULL, window_flags);

      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1., 1., 1., 1.));

      ImGui::SetWindowFontScale(3.5);
      ImGui::Text("hp: %.f\nEstágio: %d\nInimigos: %d",
                  scene.mapa.personagem->hp, nivel,
                  scene.mapa.numInimigos());

      ImGui::PopStyleColor();

      ImGui::End();
    }
  }
  else{
    scene.drawFundo(tempo_atual);
    menuPrincipal();
  }

  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Atualização da janela
  SDL_GL_SwapWindow(gWindow);
};

// Libera recursos
void close() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_GL_DeleteContext(gContext);
  SDL_Quit();
}

int main(int argc, char *argv[]) {

  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    quit = false;

    SDL_StartTextInput();

    setupScene();

    begin_time = clock();
#ifdef __EMSCRIPTEN__

    emscripten_set_main_loop(main_loop, 0, 1);

#else

    while (!quit) {
      main_loop();
      SDL_Delay(20);
    }

#endif
  }
  close();

  return 0;
}
