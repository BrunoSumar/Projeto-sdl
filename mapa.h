#include <iostream>
#include <string>
#include <sstream>
#include "unidade.h"

using namespace std;

struct Position {
  vector <Unidade*> unidades;
  int team;
  int estado;
  bool hasPersonagem;

  void draw(int x, int y);
  void addUnidade( Unidade *u );
  void removePersonagem();
  void addPersonagem(Personagem *p);
};

struct Mapa {
  // Dimensões da matriz e matriz de posições
  const int dim1, dim2;
  Position **mat;
  vector<Unidade> unidades;
  Personagem personagem;

  // Funções
  Mapa(const int n, const int m, string path_tex="personagem.png");
  std::string matToString();
  void draw();
  void moverPersonagem(int x, int y);
  // void addUnidade(string path_tex, ShaderProgram *sp, int posx, int posy);
  void initPersonagem(ShaderProgram *sp);
};

void Position::removePersonagem()
{
  // Caso for mudar a maneira de remover o personagem
  // Essa função deverá ser modificada, para que não
  // seja necessário modificar anywhere else.

  // Por enquanto está considerando que personagem é o
  // último item adcionado no vecotr com push_back()
  unidades.pop_back();
};

void Position::addPersonagem(Personagem *p)
{
  // Mesmo que Position::removePersonagme()
  unidades.push_back(p);
}

void Position::addUnidade( Unidade* u ){
   unidades.push_back(u);
};

void Position::draw(int x, int y){
  // tile.draw(x, y, estado);
  for(int i = 0; i < unidades.size(); i++){
    unidades[i]->draw();
  }
}


void Mapa::moverPersonagem(int x, int y)
{
  mat[personagem.posx][personagem.posy].removePersonagem();
  
  personagem.posx = max(0, min(dim1-1, x + personagem.posx));
  personagem.posy = max(0, min(dim2-1, y + personagem.posy));

  mat[personagem.posx][personagem.posy].addPersonagem(&personagem);
};

// void Mapa::addUnidade(string path_tex, ShaderProgram *sp, int posx, int posy){
//   unidades.push_back({path_tex, posx, posy});
//   unidades.back().cartao.program = sp;

//   mat[posx][posy].unidades.push_back(&unidades.back());
// }

void Mapa::initPersonagem(ShaderProgram *sp)
{  
  personagem.cartao.program = sp;
  mat[personagem.posx][personagem.posy].addPersonagem(&personagem);
}

Mapa::Mapa(const int n, const int m, string path_tex)
  : mat{new Position*[n]},
    dim1{n},
    dim2{m},
    personagem{}
{
  for(int i=0; i<n; i++)
    mat[i] = new Position[m];
};