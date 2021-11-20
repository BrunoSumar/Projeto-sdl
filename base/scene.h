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

class Elemento{
public:
    Mesh mesh;
    glm::mat4 model;

    Elemento(const string path_obj):
        mesh{path_obj},
        model{ glm::mat4(1.f) } // Matriz identidade
        {}

    void Draw(Shader *shader){
        shader->use();
        shader->setMat("model", model);
        mesh.Draw();
    }
};

class Scene{
public:
    Scene(glm::vec3 e, glm::vec3 c, glm::vec3 u, float aRatio):
        eye{e},
        center{c},
        up{u},
        Projection{ glm::mat4(1.f) }
        {
            Projection = glm::perspective(glm::radians(45.0f), aRatio, .1f, 100.0f);
            Shader sh("./shaders/vertexShader", "./shaders/fragmentShader");
            shader = &sh;
            updateView();
            shader->use();
            shader->setMat("projection", Projection);
            Elemento elem("./untitled.obj");
            elementos.push_back(elem);
        }

    void Draw(){
        for(std::vector<Elemento>::iterator i = elementos.begin(); i < elementos.end(); i++){
            (*i).Draw(shader);
        }
    }

private:
    /* Camera */
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    glm::mat4 view;

    glm::mat4 Projection;

    vector<Elemento> elementos;
    Shader *shader;

    void updateView(){
        view = glm::mat4(1.f);
        //view = glm::lookAt(eye, center, up);
        view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));
        shader->use();
        shader->setMat("view", view);
    }
};

#endif // SCENE_H_
