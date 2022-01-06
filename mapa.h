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
  vector<Unidade*> unidades;
  Personagem personagem;

  // Funções
  Mapa(const int n, const int m, string path_tex="personagem.png");
  std::string matToString();
  void draw();
  void moverPersonagem(int x, int y);
  // void addUnidade(string path_tex, ShaderProgram *sp, int posx, int posy);
  void addPersonagem(ShaderProgram *sp);
};

