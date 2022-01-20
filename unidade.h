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
  float cooldown = 0.1f;

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
  float last_time = 0;
  float vel = .01f;
  int last_posx;

  Projetil(string path, int x=0, int y=0)
    : Unidade(path, x, y),
      last_posx{x-1}
  {};
  
  virtual Unidade* action(float t);
};

Unidade* Personagem::fire(float t){
  if ((t - last_shot) > cooldown) {
    last_shot = t;

    // Personagem e seu projétil estão usando o mesmo shaderProgram
    Unidade *u = new Projetil{"resources/1_0.png", posx + 1, posy};
    u->cartao.program = this->cartao.program;
    return u;
  };
  return NULL;
};

Unidade* Projetil::action(float t){
  // Se a ultima posição está igual a posição atual
  // é porque o mapa diminuiu a posição, então chegou a parede
  if (last_posx == posx)
    return NULL;

  // Caso tenha se passado um tempo maior que vel
  // pode-se mover novamente.
  if ((t - last_time) > vel) {
    last_posx = posx;
    posx += 1;
    last_time = t;
  }

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
