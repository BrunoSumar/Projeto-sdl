#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>

#include "MeshBuffers.h"
#include "MeshTexture.h"
#include "obj_buffers.h"

using namespace std;

// Por compatibilidade vou deixar Element mas o nome no futuro-
// a gente usa o Figura mesmo
struct Element{
  ShaderProgram program;
  MeshBuffers   mesh;
  MeshTexture   texture;
  mat4          model;
  int           sprite_rows;
  int           sprite_columns;
  
  Element(string p_obj, string p_tex){
    mesh = obj_buffers( p_obj.c_str() );
    texture = { p_tex.c_str() };
  }
  
  void setModel(mat4 model){
    this->model = model;
  }
  
  void setSprite(int spriteNum){
    Uniform{"sprite_rows"} = sprite_rows;
    Uniform{"sprite_columns"} = sprite_columns;
    Uniform{"nSprite"} = spriteNum;
  }
  
  void draw (int sprite=1){
    //glUseProgram(program); coloquei na scene.h
    Uniform{"model"} = model;
    setSprite(sprite);
    texture.bind();
    mesh.draw();
  }
};

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

  void draw (){
    // glUseProgram(*program);
    Uniform{"model"} = model;
    texture.bind();
    mesh.draw();
  }
};

struct Cartao : Figura{
  Cartao() {}

  Cartao(string p_tex){
    mesh = card_mesh();
    texture = { p_tex.c_str() };
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
      {{0., 2., 0.}, {0.,1.}, {0., 0., 1.}},
      {{0., 0., 0.}, {0.,0.}, {0., 0., 1.}}
      // {{1., 0., 0.}, {0.,1.}, {0., 0., 1.}},
      // {{1., 2., 0.}, {0.,0.}, {0., 0., 1.}},
      // {{0., 2., 0.}, {1.,0.}, {0., 0., 1.}},
      // {{0., 0., 0.}, {1.,1.}, {0., 0., 1.}}
    };

    vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    return MeshBuffers{V, indices};
  }
};


#endif // ELEMENT_H_
