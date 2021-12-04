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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* glm::mat4 Model = glm::scale(glm::identity(), glm::vec3(0.5f)); */ //tralate, scale, rotate, identity
/* return glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 100.f); */

struct Texture {
    unsigned int id;
    string path;
};

struct Elemento{
    Mesh mesh;
    Texture texture;
    glm::mat4 model;

    Elemento(const string path_obj):
        mesh{path_obj},
        model{ glm::mat4(1.f) } // Matriz identidade
        {}

    Elemento(const string path_obj, glm::mat4 model):
        mesh{ path_obj},
        model{ model }
        {}

    void Draw(Shader &shader){
        shader.use();
        shader.setMat("model", model);
        mesh.Draw(texture.id);
    }

    void loadTexture(Shader &shader, string path_tex){
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path_tex.c_str(), &width, &height, &nrChannels, 0);
        if(!data){
            std::cout << "ERROR loading texture image with stb_image" << std::endl;
            return;
        }

        std::cout << "Texture loaded successfully" << std::endl;

        unsigned int t;

        glGenTextures(1, &t);

        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        glUniform1i(glGetUniformLocation(shader.ID, "tex"), 0);

        texture.id = t;
        texture.path = path_tex;
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
    void addElementAndTex( Elemento e, string path ){
        e.loadTexture(shader, path);
        elementos.push_back(e);
    }
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
