#ifndef UNIDADE_H_
#define UNIDADE_H_

//#include "figura.h" 

#define INTERVALO (.3f)

/* Unidade é a classe genérica para elementos com os quais é possível interagir durante o jogo */
struct Unidade{
  Cartao cartao;
  int posx, posy;
  int id;

  static int count;

  Unidade( string path_tex, int x=0, int y=0 ) :
    cartao{path_tex},
    posx{x},
    posy{y},
    id{count++}
  {};

  void draw(float time);

  virtual Unidade* action(float time) { return NULL;}
};

void Unidade::draw(float time){
  float tranX = (3.5 - posy) * INTERVALO;
  /* float tranZ = (posx - MAPA_WIDTH/2. - .5 ) * INTERVALO; */
  float tranZ = (3.5 - posx) * INTERVALO;

  cartao.model = translate(tranX, 0., tranZ) * scale(1.5, 1.5, 1.5);

  cartao.draw(time);
};

/* Personagem representa a unidade referente ao personagem principal  */
/* controlado pelo usuário */
struct Personagem : Unidade {
  int hp;
  float cooldown = 0.2f;

  // Para implementar cooldown
  float last_shot = 0.;

  Personagem(ShaderProgram* sp)
    : Unidade("resources/sprito2.png")
  {
    cartao.program = sp;
  };

  Unidade* fire(float t);
};

struct Projetil : Unidade {
  // Direção
  int x, y;

  float last_time = 0;
  float vel = 2.f;

  Projetil(string path, int x=0, int y=0)
    : Unidade(path, x, y) {};
  
  virtual Unidade* action(float t);
};

Unidade* Personagem::fire(float t){
  if ((t - last_shot) > cooldown) {
    last_shot = t;
    return new Projetil{"resources/1_0.png", posx + 1, posy};
  };
  return NULL;
};

Unidade* Projetil::action(float t){
  //if ((t - last_time) < (1.f/vel))
  if ((t - last_time) < .05)
    return this;
  posx += 1;
  
  if (posx > 5)
    return NULL;
  last_time = t;
  return this;
};
 
struct Piso : Unidade {
  vec3 color = {1.0, .0, .0};
  Piso(ShaderProgram* sp)
    : Unidade("resources/tex.jpeg")
  {
    cartao.program = sp;
  };

  void draw(int posx, int posy);
};

void Piso::draw(int posx, int posy){
  float tranX = (3.5 - posy) * INTERVALO;
  float tranZ = (3 - posx) * INTERVALO;

  float sc = (INTERVALO / 2.);

  /* cartao.model = translate(tranX, 0., tranZ) * rotate_x(90) * scale(.5, .5, .5); */
  cartao.model = translate(tranX, 0., tranZ)* scale(sc, sc, sc) * rotate_x(M_PI/2.) ;

  Uniform("color") = color;
  cartao.draw(0.);
};

/* Inicializando contador de unidades */
int Unidade::count = 0;

#endif // UNIDADE_H_
