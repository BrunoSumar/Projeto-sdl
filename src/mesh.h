#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cstddef>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

struct Mesh {
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    unsigned int VAO;

    Mesh() {}
    Mesh(string path_obj)
        {
            loadObj(path_obj);
            setupMesh();
        }

    void Draw(unsigned int texture)
        {
            // Desenhando o mesh

            glBindTexture(GL_TEXTURE_2D, texture);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }

    void loadObj(string path)
        {
            Assimp::Importer import;

            // Load file
            const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
                return;
            }

            aiNode *node = scene->mRootNode;
            node = node->mChildren[0];
            aiMesh *mesh = NULL;
            // Retrieve aiMesh
            if (node->mNumMeshes > 0){
                mesh = scene->mMeshes[node->mMeshes[0]];

                // Set the vertex properties
                for(unsigned int i = 0; i < mesh->mNumVertices; i++){
                    //Vertex vertex;
                    vertices.push_back({
                        { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z  },
                        { 0., 0.}
                    });

                    if(mesh->mTextureCoords[0])
                    {
                        // glm::vec2 vec;
                        // vertices[i].TexCoords.x = mesh->mTextureCoords[0][i].x;
                        // vertices[i].TexCoords.y = mesh->mTextureCoords[0][i].y;

                        vertices[i].TexCoords = {
                            (float) mesh->mTextureCoords[0][i].x,
                            (float) mesh->mTextureCoords[0][i].y
                        };
                    }
                }


                for(unsigned int i = 0; i < mesh->mNumFaces; i++)
                {
                    aiFace face = mesh->mFaces[i];
                    for(unsigned int j = 0; j < face.mNumIndices; j++)
                        indices.push_back(face.mIndices[j]);
                }
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);

            // Vec2 de coordenada de textura
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }
};
#endif