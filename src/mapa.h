#include <iostream>
#include <string>
#include <sstream>

#include "unidade.h"

using namespace std;

struct Position {
  vector<Unidade> unidades;
  int time;
  int estado;

  void draw(int x, int y);
};

void Position::draw(int x, int y){
  // tile.draw(x, y, estado);
  for(int i = 0; i < unidades.size(); i++){
    unidades[i].draw(x, y);
  }
}

struct Mapa {
  // Dimensões da matriz e matriz de posições
  const int dim1, dim2;
  Position **mat;

  // Funções
  Mapa(const int n, const int m);
  std::string matToString();
  void draw();
};

Mapa::Mapa(const int n, const int m)
  : mat{new Position*[n]},
    dim1{n},
    dim2{m} {

  for(int i=0; i<n; i++)
    mat[i] = new Position[m];

};

// std::string Mapa::matToString() {
//   std::stringstream r;

//   for(int i=0; i<dim1; i++) {

//     for(int j=0; j<dim2; j++) {
//       r << mat[i][j].p;
//       r << ' ';
//     }

//     r << '\n';
//   }

//   return r.str();
// };


// int main () {
//   Mapa mapa{10, 10};
//   std::cout << mapa.matToString() << std::endl;
// };
