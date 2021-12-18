#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>

#include "MeshBuffers.h"
#include "MeshTexture.h"
#include "obj_buffers.h"

using namespace std;

struct Element{
    ShaderProgram program;
    MeshBuffers   mesh;
    MeshTexture   texture;
    mat4          model;
    int           sprite_rows;
    int           sprite_columns;

    Element(string p_obj, string p_tex, ShaderProgram sp){
        mesh = obj_buffers( p_obj.c_str() );
        texture = { p_tex.c_str() };
        program = sp;
        // sprite_rows=1; //Caso tenha mais sprites tem que trocar na mão
        // sprite_columns=1;
    }

    // void setShaderProgram(string path_vertex, string path_fragment) {
    //     program = ShaderProgram{
    //         Shader{path_vertex, GL_VERTEX_SHADER},
    //         Shader{path_fragment, GL_FRAGMENT_SHADER}
    //     };
    // }

    // void setTexture(string path_tex){
    //     texture = {path_tex.c_str()};
    // }

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

#endif // ELEMENT_H_
