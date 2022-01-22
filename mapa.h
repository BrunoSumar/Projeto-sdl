#ifndef MAPA_H_
#define MAPA_H_

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

  void removeUnidade(Unidade *u);
  void addUnidade(Unidade *u);
};

// Contém o estado de uma batalha no que diz
// respeito às posições
struct Mapa {
  const int dim1, dim2; // Dimensões da matriz de posições
  Position **mat; // Contém uma matriz de Positions que correspondem às células do tabuleiro
  vector<Unidade*> unidades;  // Ponteiros para as unidades que estão sendo utilizadas na batalha
  Personagem *personagem;
  Piso *piso;

  Mapa(const int n, const int m,string path_tex="sprito.png");
  void moverPersonagem(int x, int y);
  void moverUnidade(Unidade *u, int x, int y);
  void initPersonagem(ShaderProgram *sp);
  void initPiso(ShaderProgram *sp);
  void actions(float time);
  void addUnidade(Unidade *u);
  void tiroPersonagem(float time);
  void processAction(float time, Unidade *u);
  void remUnidade( Unidade *u , int x, int y);
};

void Mapa::remUnidade( Unidade *u, int x, int y){
  // mat[x][y].removeUnidade(u);
  mat[u->posx][u->posy].removeUnidade(u);

  auto remove_id = std::remove_if(
    unidades.begin(),
    unidades.end(),
    [u](Unidade *x){return (x->id == u->id);}
  );
  unidades.erase( remove_id, unidades.end());
};

void Mapa::tiroPersonagem(float time){
  if (!personagem) return;

  Unidade* u = personagem->fire( time );
  if (!u) return;
  addUnidade( u );
};

void Position::removeUnidade(Unidade *u)
{
  auto remove_id = std::remove_if(
    unidades.begin(),
    unidades.end(),
    [u](Unidade *x){return (x->id == u->id);}
  );
  unidades.erase( remove_id, unidades.end());
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

  u->posx = posx;
  u->posy = posy;

  std::cout << "id: " << u->id << ' ' <<  posx << ' ' << posy << '\n';
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

// Função para lidar com os ponteiros de
// unidade gerados pela função Unidade::action
void Mapa::processAction( float time /*action() recebe tempo como parâmetro*/, Unidade *u /*Unidade que possui action()*/)
//                                     para poder calcular quando realizar ação             
{
  if(!u)
    return;

  int old_x = u->posx;
  int old_y = u->posy;
  Unidade *new_u = u->action(time);

  if(!new_u){
    u->posx = old_x;
    u->posy = old_y;
    remUnidade( u, old_x, old_y );
  }
  else if(new_u == u){
    int diffx = u->posx - old_x, diffy = u->posy - old_y;
    if ( !diffx || !diffy ) {
      u->posx = old_x;
      u->posy = old_y;
      moverUnidade(u, diffx, diffy);
    }
  }
  else{
    addUnidade( new_u );
  }
};

void Mapa::actions(float time){
  for(int i = 0; i < unidades.size(); i++){
    processAction(time, unidades[i]);
  }
}

void Mapa::addUnidade(Unidade *u){
  unidades.push_back(u);

  if(dim1 > u->posx && dim2 > u->posy)
    mat[u->posx][u->posy].addUnidade(u);
}

#endif // MAPA_H_
