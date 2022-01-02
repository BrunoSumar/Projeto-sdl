#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Position {
  int p;
};

struct Map {
  // Dimensões da matriz e matriz de posições
  const int dim1, dim2;
  Position **mat;

  // Funções
  Map(const int n, const int m);
  std::string matToString();
};

struct Tile {
  float vertex[4];
};

// A ideia aqui é gerar os vertices de Tile
// a partir das informações em mapa.
// E talvez associar Position com Tile.
struct Tabuleiro {
  Map &mapa;
  Tile *tiles;

  //mat4 model

  //Tabuleiro(Map &m) : mapa{ m } {}
};

Map::Map(const int n, const int m)
  : mat{new Position*[n]},
    dim1{n},
    dim2{m} {

  for(int i=0; i<n; i++)
    mat[i] = new Position[m];

};

std::string Map::matToString() {
  std::stringstream r;

  for(int i=0; i<dim1; i++) {

    for(int j=0; j<dim2; j++) {
      r << mat[i][j].p;
      r << ' ';
    }

    r << '\n';
  }

  // for( auto i : mat)  // Não entendo o porque isso não funciona
  //   for( auto j : i )
  //     r << j;

  return r.str();
};

int main () {
  Map map{10, 10};
  std::cout << map.matToString() << std::endl;
};
