#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>

#include "lib/MeshTexture.h"
#include "lib/MeshBuffers.h"
#include "lib/obj_buffers.h"

using namespace std;

struct Element{
    ShaderProgram program;
    MeshBuffers   mesh;
    MeshTexture   texture;
    mat4          model;

    Element(string p_vs, string p_fs, string p_obj, string p_tex){
        setObj(p_obj);
        setShaderProgram(p_vs, p_fs);
        setTexture(p_tex);
    }

    void setObj(string path_obj){
        mesh = obj_buffers(path_obj.c_str());
    }

    void setShaderProgram(string path_vertex, string path_fragment) {
        program = ShaderProgram{
            Shader{path_vertex, GL_VERTEX_SHADER},
            Shader{path_fragment, GL_FRAGMENT_SHADER}
        };
    }

    void setTexture(string path_tex){
        texture = {path_tex.c_str()};
    }

    void setModel(mat4 model){
        this->model = model;
    }

    void draw(){
        //glUseProgram(program); coloquei na scene.h
        Uniform{"Model"} = model;
        texture.bind();
        mesh.draw();
    }
};

#endif // ELEMENT_H_
