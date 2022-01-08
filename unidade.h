#ifndef UNIDADE_H_
#define UNIDADE_H_

#include "figura.h"

#define INTERVALO (.3f)

/* Unidade é a classe genérica para elementos com os quais é possível interagir durante o jogo */
struct Unidade{
  Cartao cartao;
  int posx, posy;

Unidade( string path_tex, int x=0, int y=0 )
  : cartao{path_tex},
    posx{x},
    posy{y}
  {};

  void draw();
};

void Unidade::draw(){
  float tranX = (posx - 2.) * INTERVALO;
  float tranZ = (posy - 3.5 ) * INTERVALO;

  cartao.model = translate(tranX, 0., tranZ) * scale(1.5, 1.5, 1.5);

  cartao.draw();
};

/* Personagem representa a unidade referente ao personagem principal  */
/* controlado pelo usuário */
struct Personagem : Unidade {
  int hp;

  Personagem(ShaderProgram* sp)
    : Unidade("resources/sprito.png")
  {
    cartao.program = sp;
  };
};

#endif // UNIDADE_H_
