#ifndef SCENE_H_
#define SCENE_H_

#include <stdio.h>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

#include <vector>
#include "mesh.h"
#include "shader.h"

/* glm::mat4 Model = glm::scale(glm::identity(), glm::vec3(0.5f)); */ //tralate, scale, rotate, identity
/* return glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 100.f); */

class Element{
public:
    Mesh mesh;
    glm::mat4 model;

    Element(string path_obj)
        : mesh{ Mesh(path_obj) }
        , model{ glm::mat4() } // Matriz identidade
        {}

    void Draw(){
        mesh.Draw();
    }
};

class Scene{
public:
    Scene(glm::vec3 e, glm::vec3 c, glm::vec3 u, float aRatio)
        : eye{e}
        , center{c}
        , up{u}
        , Projection{ glm::perspective(glm::radians(45.0f), aRatio, .1f, 100.0f) }
        {
            std::cout << "Chegou aqui\n";
            Shader sh("shaders/vertexShader", "shaders/fragmentShader");
            shader = &sh;
            std::cout << "E aqui\n";
            updateView();
            shader->use();
            shader->setMat("projection", Projection);
            elementos.push_back(Element("./untitled.obj"));
        }

    void Draw(){
        for(std::vector<Element>::iterator i = elementos.begin(); i != elementos.end(); i++){
            i->Draw();
        }
    }

private:
    /* Camera */
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    glm::mat4 view;

    glm::mat4 Projection;

    vector<Element> elementos;
    Shader *shader;

    void updateView(){
        view = glm::lookAt(eye, center, up);
        shader->setMat("view", view);
    }
};

#endif // SCENE_H_
