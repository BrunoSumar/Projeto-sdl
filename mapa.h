#include <iostream>
#include <string>
#include <sstream>
#include "unidade.h"

using namespace std;

// Position representa cada posição do mapa
struct Position {
  vector <Unidade*> unidades;
  int equipe;
  int estado;

  void draw(int x, int y);
  void removeUnidade();
  void addUnidade(Unidade *u);
};

// Mapa o posicionamente estado do jogo
struct Mapa {
  const int dim1, dim2;  // Dimensões da matriz e matriz de posições
  Position **mat;
  vector<Unidade> unidades;
  Personagem *personagem;

  Mapa(const int n, const int m,string path_tex="sprito.png");
  std::string matToString();
  void draw();
  void moverPersonagem(int x, int y);
  void initPersonagem(ShaderProgram *sp);
};

void Position::removeUnidade()
{
  unidades.pop_back();
};

void Position::addUnidade(Unidade *u)
{
  unidades.push_back(u);
}

void Position::draw(int x, int y){
  for(int i = 0; i < unidades.size(); i++){
    unidades[i]->draw();
  }
}

void Mapa::moverPersonagem(int x, int y)
{
  mat[personagem->posx][personagem->posy].removeUnidade();
  
  personagem->posx = max(0, min(dim1-1, personagem->posx - y));
  personagem->posy = max(0, min(dim2-1, x + personagem->posy));

  mat[personagem->posx][personagem->posy].addUnidade(personagem);
};

void Mapa::initPersonagem(ShaderProgram *sp)
{
  personagem = new Personagem(sp);
  mat[personagem->posx][personagem->posy].addUnidade(personagem);
}

Mapa::Mapa(const int n, const int m, string path_tex)
  : mat{new Position*[n]},
    dim1{n},
    dim2{m}
{
  for(int i=0; i<n; i++)
    mat[i] = new Position[m];
};
