#ifndef SCENE_H_
#define SCENE_H_
#include <vector>
#include <map>

#include "figura.h"
#include "mapa.h"

#define MAPA_WIDTH 8
#define MAPA_HEIGHT 5

// Scene é a classe reposável por gerenciar todas as informações necessáras para renderição da cena do jogo
struct Scene {
  vector<Figura*> figuras;
  mat4 projection;
  mat4 view;
  Mapa mapa;

  Scene() : mapa{MAPA_WIDTH, MAPA_HEIGHT} {};

  void draw(float time);
  void addFigura(string path_texture);
  void addFundo(string path_texture);
  void addFigura(string path_obj, string path_texture);
  void setView(mat4 v);
  void setProjection(mat4 p);
  void addShaderToMapa(ShaderProgram *sp);
  void setMatrices();
};

void Scene::draw(float time){
  int dim1 = mapa.dim1;
  int dim2 = mapa.dim2;
  Figura *f = NULL;

  // Cenario (modelos não relacionado ao gameplay)
  for( int i = 0; i < figuras.size() ; i++ ){
    // f = &figuras[i];
    glUseProgram(figuras[i]->program->id);
    setMatrices();
    figuras[i]->draw(time);
    // f->draw();
  }

  //Desenha as unidades de cada posição do mapa
  for(int i=0; i<dim1; i++) {
    for(int j=0; j<dim2; j++) {
      glUseProgram(mapa.piso->cartao.program->id);
      setMatrices();
      mapa.piso->draw(i, j);
      for(int k = 0 ; k < mapa.mat[i][j].unidades.size(); k++){
        glUseProgram(mapa.mat[i][j].unidades[k]->cartao.program->id);
        setMatrices();
        mapa.mat[i][j].unidades[k]->draw(time);
      }
    }
  }
}

void Scene::setMatrices(){
  Uniform("projection") = projection;
  Uniform("view") = view;
}

void Scene::setProjection(mat4 p){
    projection = p;
}

void Scene::setView(mat4 v){
    view = v;
}

void Scene::addFigura(string path_texture){
  figuras.push_back(new Cartao{path_texture});
}

void Scene::addFundo(string path_texture){
  figuras.push_back(new PlanoDeFundo{path_texture});
}

void Scene::addFigura(string path_obj, string path_texture){
  figuras.push_back(new Figura{path_obj, path_texture});
}

#endif // SCENE_H_
