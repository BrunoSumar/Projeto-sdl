#ifndef UNIDADE_H_
#define UNIDADE_H_

#include "figura.h"

struct Unidade{
  Cartao cartao;

  Unidade( string path_tex ) : cartao{path_tex} {};

  const float intervalo = 4.f;

  void draw(int i, int j);
};

void Unidade::draw(int i, int j){
  float tranX = (i - 2) * intervalo;
  float tranZ = (j - 3.5 ) * intervalo;
  cartao.model = {
    1.f, 0.f, 0.f, tranX,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, tranZ,
    0.f, 0.f, 0.f, 1.f
  };
  cartao.draw();
}


#endif // UNIDADE_H_
