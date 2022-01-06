#ifndef UNIDADE_H_
#define UNIDADE_H_

#include "figura.h"

struct Unidade{
  Cartao cartao;
  int posx, posy;

Unidade( string path_tex, int x=0, int y=0 )
  : cartao{path_tex},
    posx{x},
    posy{y}
  {};

  const float intervalo = .3f;

  void draw();
};

void Unidade::draw(){
  float tranX = (posx ) * intervalo; 
  float tranZ = (posy - 3.5 ) * intervalo; 

  cartao.model = {
    1.f, 0.f, 0.f, tranX,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, tranZ,
    0.f, 0.f, 0.f, 1.f
  };
  
  cartao.model = translate(-.5, .0, .0) * cartao.model;
  cartao.draw();
};

struct Personagem : Unidade {
  int hp;

  Personagem() : Unidade("resources/personagem.png") {};
};



#endif // UNIDADE_H_
