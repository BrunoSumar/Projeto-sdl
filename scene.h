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

struct Elemento{
    Mesh mesh;
    glm::mat4 model;

    Elemento(const string path_obj):
        mesh{path_obj},
        model{ glm::mat4(1.f) } // Matriz identidade
        {}

    Elemento(const string path_obj, glm::mat4 model):
        mesh{ path_obj },
        model{ model }
        {}

    void Draw(Shader &shader){
        shader.use();
        shader.setMat("model", model);
        mesh.Draw();
    }
};

struct Camera {
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    glm::mat4 view;

    Camera() {}

    Camera(glm::vec3 e, glm::vec3 c, glm::vec3 u):
        eye{e},
        center{c},
        up{u},
        view{1.f}
    {
        updateView();
    }

    void updateView(){
        view = glm::lookAt(eye, center, up);
    }
};

struct Scene{
    Shader shader;
    Camera cam;

    float aRatio;
    float near;
    float far;
    float ang;
    glm::mat4 Projection;

    vector<Elemento> elementos;

    Scene() {}

    void updateProjection() {
        Projection = glm::perspective(glm::radians(ang), aRatio, near, far);
        shader.use();
        shader.setMat("projection", Projection);
    }

    void Draw(){
        for(std::vector<Elemento>::iterator i = elementos.begin(); i < elementos.end(); i++){
            i->Draw(shader);
        }
    }

    void addElement( Elemento e ) { elementos.push_back(e); }
    void removeElement( int i ) { elementos.erase(elementos.begin() + i); }

    void updateView(){
        cam.updateView();
        shader.use();
        shader.setMat("view", cam.view);
    }

    void updateMatrixes(){
        updateView();
        updateProjection();
    }
};

#endif // SCENE_H_
