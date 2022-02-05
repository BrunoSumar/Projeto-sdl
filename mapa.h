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
  int equipe = 0;
  int estado;
  float impacto = -1.;
  bool bloqueado = false;
  vec3 color = {.5f, .5f, .5f};

  static vec3 cor1;
  static vec3 cor2;

  void removeUnidade(Unidade *u);
  void addUnidade(Unidade *u);
  void setMatrices(float time, int ocupado);
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
  int moverUnidade(Unidade *u, int x, int y);
  void moverPersonagem(int x, int y);
  void initPersonagem(ShaderProgram *sp);
  void initPiso(ShaderProgram *sp);
  void actions(float time);
  void addUnidade(Unidade *u);
  void tiroPersonagem(float time);
  void processAction(float time, Unidade *u);
  void remUnidade( Unidade *u );
};

void Mapa::remUnidade( Unidade *u ){
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

// retorna 0 caso unidade de tenha sido movida e outro número caso contrário
int Mapa::moverUnidade(Unidade *u, int x, int y)
{
  int posx = x + u->posx;
  int posy = y + u->posy;

  if(( posx < 0 || posx >= dim1 ) || ( posy < 0 || posy >= dim2 ))
    return 1;

  if( mat[posx][posy].bloqueado )
    return 2;

  if( u->equipe && ( mat[posx][posy].equipe != u->equipe ))
    return 3;

  mat[u->posx][u->posy].removeUnidade(u);

  u->posx = posx;
  u->posy = posy;

  for(int i = 0; i < mat[posx][posy].unidades.size(); i++){
    mat[posx][posy].unidades[i]->colisao( u );
    u->colisao( mat[posx][posy].unidades[i] );
  }

  mat[posx][posy].addUnidade(u);
  return 0;
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

  for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
      mat[i][j].equipe = i < n/2. ? 1 : 2;
};

void Mapa::processAction( float time, Unidade *u)
{
  if(!u)
    return;

  int old_x = u->posx;
  int old_y = u->posy;
  Unidade *new_u = u->action(time);
  int movimento = 0;

  if(!new_u){
    u->posx = old_x;
    u->posy = old_y;
    remUnidade( u );
  }
  else{
    int diffx = u->posx - old_x, diffy = u->posy - old_y;
    u->posx = old_x;
    u->posy = old_y;
    if ( diffx || diffy ) {
      movimento = moverUnidade(u, diffx, diffy);
    }
    if(new_u != u){
      addUnidade( new_u );
    }
  }
};

void Mapa::actions(float time){
  for(int i = 0; i < unidades.size(); i++){
    if( unidades[i]->hp <= 0 )
      remUnidade( unidades[i] );
  }

  for(int i = 0; i < unidades.size(); i++){
    processAction(time, unidades[i]);
  }
}

void Mapa::addUnidade(Unidade *u){
  if(dim1 <= u->posx || dim2 <= u->posy)
    return;

  unidades.push_back(u);
  mat[u->posx][u->posy].addUnidade(u);
}

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
};

void Position::setMatrices(float time, int ocupado){
  Uniform("Impacto") = impacto;
  Uniform("Equipe") = equipe;
  Uniform("Time") = time;
  Uniform("Color") = equipe == 1 ? Position::cor1 : Position::cor2;
  Uniform("Ocupado") = ocupado;
};

vec3 Position::cor1 = {.0, .3, .2};
vec3 Position::cor2 = {.9, .0, .1};

#endif // MAPA_H_
