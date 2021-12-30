#ifndef SCENE_H_
#define SCENE_H_

#include "element.h"
#include <vector>

struct Scene {
  vector<Element> elements;
  mat4 projection;
  mat4 view;

  void draw(int n);
  void addElement(string path_obj, string path_texture);
  void setView(mat4 v);
  void setProjection(mat4 p);
};

void Scene::draw(int n){
  // Cada elemento terá um shader program diferente
  for(auto i = elements.begin(); i != elements.end(); i++){
    glUseProgram(i->program);
    Uniform("projection") = projection;
    Uniform("view") = view;
    i->draw(n);
  }
}

void Scene::setProjection(mat4 p){
    projection = p;
}

void Scene::setView(mat4 v){
    view = v;
}

void Scene::addElement(string path_obj, string path_texture){
  elements.push_back({path_obj, path_texture});
  
  elements.back().model = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  };
}

#endif // SCENE_H_
