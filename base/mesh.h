#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string path;
};

class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    Texture      texture;
    unsigned int VAO;

    Mesh(string path_obj, Texture texture)
    {
        loadObj(path_obj, &vertices, &indices);
        this->texture = texture;
        setupMesh();
    }

    Mesh(string path_obj){
        loadObj(path_obj, &vertices, &indices);
        setupMesh();
    }

    void Draw()
    {
        // Usando a textura
        glBindTexture(GL_TEXTURE_2D, texture.id);

        // Desenhando o mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void loadObj(string path, vector<Vertex> *vertices, vector<unsigned int> *indices)
    {
        Assimp::Importer import;

        // Load file
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if( !scene )
            throw std::runtime_error("Erro ao carregar malha");

        // Retrieve aiMesh
        aiMesh *mesh = scene->mMeshes[scene->mRootNode->mMeshes[0]];

        // Set the vertex properties
        for(unsigned int i = 0; i < mesh->mNumVertices; i++){
            Vertex vertex;

            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;

            vertices->push_back(vertex);
        }

        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices->push_back(face.mIndices[j]);
        }
    }

    unsigned int VBO, EBO;

    void setupMesh()
    {
        // Criando os buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Passando os dados aos buffers
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Especificando Stride e offset
        // Vec3 de posição
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Vec2 de coordenada de textura
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};
#endif
