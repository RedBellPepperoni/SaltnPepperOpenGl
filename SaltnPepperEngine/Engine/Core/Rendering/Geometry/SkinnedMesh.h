#ifndef SKINNED_MESH_H
#define SKINNED_MESH_H
#include "Engine/Core/Rendering/Buffers/VertexArray.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Shader/Shader.h"
#include "Mesh.h"
#include <map>
#include <unordered_map>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> 



namespace SaltnPepperEngine
{
	



#define MAX_BONE_INFLUENCE 4

        struct SkinVertex 
        {
            // position
            glm::vec3 Position;
            // normal
            glm::vec3 Normal;
            // texCoords
            glm::vec2 TexCoords;
            // tangent
            glm::vec3 Tangent;
            // bitangent
            glm::vec3 Bitangent;
            //bone indexes which will influence this vertex
            int m_BoneIDs[MAX_BONE_INFLUENCE];
            //weights from each bone
            float m_Weights[MAX_BONE_INFLUENCE];
        };


        class SkinnedMesh 
        {
        public:
            // mesh Data
            std::vector<SkinVertex>  vertices;
            std::vector<unsigned int> indices;
            SharedPtr<Material> material;
            unsigned int VAO;

            SkinnedMesh();

            // constructor
            SkinnedMesh(std::vector<SkinVertex> vertices, vector<unsigned int> indices)
            {
                this->vertices = vertices;
                this->indices = indices;
                material = MakeShared<Material>();

                // now that we have all the required data, set the vertex buffers and its attribute pointers.
                setupMesh();
            }

            // render the mesh
            void Render()
            {
                // bind appropriate textures
              
                

                // draw mesh
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);

               
            }



        private:
            // render data 
            unsigned int VBO, EBO;

            // initializes all the buffer objects/arrays
            void setupMesh()
            {
                // create buffers/arrays
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glGenBuffers(1, &EBO);

                glBindVertexArray(VAO);
                // load data into vertex buffers
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                // A great thing about structs is that their memory layout is sequential for all its items.
                // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
                // again translates to 3/2 floats which translates to a byte array.
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SkinVertex), &vertices[0], GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

                // set the vertex attribute pointers
                // vertex Positions
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void*)0);
                // vertex normals
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void*)offsetof(SkinVertex, Normal));
                // vertex texture coords
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void*)offsetof(SkinVertex, TexCoords));
                // vertex tangent
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void*)offsetof(SkinVertex, Tangent));
                // vertex bitangent
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void*)offsetof(SkinVertex, Bitangent));
                // ids
                glEnableVertexAttribArray(5);
                glVertexAttribIPointer(5, 4, GL_INT, sizeof(SkinVertex), (void*)offsetof(SkinVertex, m_BoneIDs));

                // weights
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void*)offsetof(SkinVertex, m_Weights));
                glBindVertexArray(0);
            }
        };
	

}

#endif // !SKINNED_MESH_H


