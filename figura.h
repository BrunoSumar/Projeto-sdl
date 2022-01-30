#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>

#include "MeshBuffers.h"
#include "MeshTexture.h"
#include "obj_buffers.h"

using namespace std;

// Figura é a classe genérica para qualquer modelo desenhado
struct Figura{
  ShaderProgram *program;
  MeshBuffers   mesh;
  MeshTexture   texture;
  mat4          model;

  Figura() {}

  Figura(string p_obj, string p_tex){
    program = NULL;
    mesh = obj_buffers( p_obj.c_str() );
    texture = { p_tex.c_str() };
    model = {
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f
    };
  }

  virtual void draw(float time){
    Uniform{"model"} = model;
    texture.bind();
    mesh.draw();
  }
};

// Cartao é um quad desenhado sempre voltado na direção da camera
// principal elemento na coposição de cenas
struct Cartao : Figura{
  mat4 position;
  Cartao() {}

  Cartao(string p_tex){
    mesh = card_mesh();
    texture = { p_tex.c_str() };
    position = {
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f
    };
    model = {
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f
    };
  }

  MeshBuffers card_mesh(){
    struct Vertex{
      vec3 position;
      vec2 texCoords;
      vec3 normal;
    };
    vector<Vertex> V = {
      {{1., 0., 0.}, {1.,0.}, {0., 0., 1.}},
      {{1., 2., 0.}, {1.,1.}, {0., 0., 1.}},
      {{-1., 2., 0.}, {0.,1.}, {0., 0., 1.}},
      {{-1., 0., 0.}, {0.,0.}, {0., 0., 1.}}
    };

    vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    return MeshBuffers{V, indices};
  }

  virtual void draw(float time){
    Uniform("position") = position;
    Figura::draw(time);
  }
};

struct PlanoDeFundo : Figura{
  PlanoDeFundo() {}

  PlanoDeFundo(string p_tex, ShaderProgram *sp){
    program = sp;
    mesh = fundo_mesh();
    texture = { p_tex.c_str() };
    model = {
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f
    };
  }

  MeshBuffers fundo_mesh(){
    struct Vertex{
      vec3 position;
      vec2 texCoords;
      vec3 normal;
    };
    vector<Vertex> V = {
      {{1., -1., 0.}, {1.,0.}, {0., 0., 1.}},
      {{1., 1., 0.}, {1.,1.}, {0., 0., 1.}},
      {{-1., 1., 0.}, {0.,1.}, {0., 0., 1.}},
      {{-1., -1., 0.}, {0.,0.}, {0., 0., 1.}}
    };

    vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    return MeshBuffers{V, indices};
  }

  virtual void draw(float time){
    glDisable(GL_DEPTH_TEST);
    Uniform("Time") = time;
    Figura::draw(time);
    glEnable(GL_DEPTH_TEST);
  }
};


#endif // ELEMENT_H_
