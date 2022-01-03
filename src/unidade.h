#ifndef UNIDADE_H_
#define UNIDADE_H_

#include "figura.h"

struct Unidade{
  Cartao cartao;

  Unidade( string path_tex ) : cartao{path_tex} {};

  const float intervalo = 1.f;

  void draw(int x, int y);
};

void Unidade::draw(int x, int y){
  float tranX = (x + .5) * intervalo;
  float tranY = (y + .5) * intervalo;
  cartao.model = {
    1.f, 0.f, 0.f, tranX,
    0.f, 1.f, 0.f, tranY,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  };
  cartao.draw();
}


#endif // UNIDADE_H_
