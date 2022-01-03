#ifndef SCENE_H_
#define SCENE_H_
#include <vector>
#include <map>

#include "figura.h"
#include "mapa.h"

struct Scene {
  vector<Figura> figuras;
  /* map<string, *ShaderProgram> shaders; */
  mat4 projection;
  mat4 view;
  Mapa mapa;

  Scene() : mapa{5, 8} {};

  void draw();
  void addFigura(string path_texture);
  void addFigura(string path_obj, string path_texture);
  void addProgram(string name, string path_vertex, string path_fragment);
  void setView(mat4 v);
  void setProjection(mat4 p);
};

void Scene::draw(){
  int dim1 = mapa.dim1;
  int dim2 = mapa.dim2;

  // Cenario
  for( int i = 0; i < figuras.size() ; i++ ){
    glUseProgram(figuras[i].program->id);
    Uniform("projection") = projection;
    Uniform("view") = view;
    figuras[i].draw();
  }

  //Desenha as unidades de cada posição do mapa
  for(int i=0; i<dim1; i++) {
    for(int j=0; j<dim2; j++) {
      for(int k = 0 ; k < mapa.mat[i][j].unidades.size(); k++){
        /* glUseProgram(shaders[i->program]); */
        glUseProgram(mapa.mat[i][j].unidades[k].cartao.program->id);
        Uniform("projection") = projection;
        Uniform("view") = view;
        mapa.mat[i][j].unidades[k].draw(i, j);
      }
    }
  }
}

void Scene::setProjection(mat4 p){
    projection = p;
}

void Scene::setView(mat4 v){
    view = v;
}

void Scene::addFigura(string path_texture){
  figuras.push_back(Cartao{path_texture});
}

void Scene::addFigura(string path_obj, string path_texture){
  figuras.push_back({path_obj, path_texture});
}

void Scene::addProgram(string name, string path_vertex, string path_fragment){
  /* shaders[name] = new {path_vertex, path_fragment}; */
}

#endif // SCENE_H_
