#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "unidade.h"

using namespace std;

// Position representa cada posição do mapa
struct Position {
  vector <Unidade*> unidades;
  int equipe;
  int estado;
  bool bloqueado = false;

  void draw(int x, int y);
  void removeUnidade(Unidade *u);
  void addUnidade(Unidade *u);
};

// Mapa o posicionamente estado do jogo
struct Mapa {
  const int dim1, dim2;  // Dimensões da matriz e matriz de posições
  Position **mat;
  vector<Unidade> unidades;
  Personagem *personagem;
  Piso *piso;

  Mapa(const int n, const int m,string path_tex="sprito.png");
  void moverPersonagem(int x, int y);
  void moverUnidade(Unidade *u, int x, int y);
  void initPersonagem(ShaderProgram *sp);
  void initPiso(ShaderProgram *sp);
};

void Position::removeUnidade(Unidade *u)
{
  std::remove_if(
    unidades.begin(),
    unidades.end(),
    [u](Unidade *x){return (x->id == u->id);}
 );
};

void Position::addUnidade(Unidade *u)
{
  unidades.push_back(u);
}

void Mapa::moverUnidade(Unidade *u, int x, int y)
{
  int posx = max(0, min(dim1-1, x + u->posx));
  int posy = max(0, min(dim2-1, y + u->posy));

  if( mat[posx][posy].bloqueado )
    return;

  mat[u->posx][u->posy].removeUnidade(u);

  personagem->posx = posx;
  personagem->posy = posy;

  mat[posx][posy].addUnidade(u);
};

void Mapa::moverPersonagem(int x, int y)
{
  moverUnidade(personagem, x, y);
};

void Mapa::initPiso(ShaderProgram *sp)
{
  piso = new Piso(sp);
}

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
