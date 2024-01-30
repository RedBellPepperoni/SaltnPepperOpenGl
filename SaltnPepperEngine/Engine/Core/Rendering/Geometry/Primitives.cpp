#include "Primitives.h"
#include "Engine/Utils/Logging/Log.h"
#include "Mesh.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
        SharedPtr<Mesh> GeneratePrimitive(PrimitiveType type)
        {
            switch (type)
            {
            case PrimitiveType::Cube:
                return GenerateCube();
            case PrimitiveType::Plane:
                return GeneratePlane(1.0f, 1.0f, Vector3(0.0f, 1.0f, 0.0f));
            case PrimitiveType::Quad:
                return GenerateQuad();
            case PrimitiveType::Sphere:
                return GenerateSphere();
            case PrimitiveType::Capsule:
                return GenerateCapsule();
            case PrimitiveType::Cylinder:
                return GenerateCylinder();
            case PrimitiveType::Terrain:
                return CreateTerrain();
            case PrimitiveType::External:
               LOG_WARN("Trying to create primitive of type File");
                return nullptr;
            }

            LOG_ERROR("Primitive not supported");
            return nullptr;
        }

        SharedPtr<Mesh> GenerateQuad(const Vector2& position, const Vector2& size)
        {
            std::vector<Vertex> vertices(4);

            vertices[0].position = Vector3(position.x, position.y, 0.0f);
            vertices[0].texCoord = Vector2(0.0f, 1.0f);

            vertices[1].position = Vector3(position.x + size.x, position.y, 0.0f);
            vertices[1].texCoord = Vector2(0, 0);

            vertices[2].position = Vector3(position.x + size.x, position.y + size.y, 0.0f);
            vertices[2].texCoord = Vector2(1, 0);

            vertices[3].position = Vector3(position.x, position.y + size.y, 0.0f);
            vertices[3].texCoord = Vector2(1, 1);

            std::vector<uint32_t> indices = {
                0,
                1,
                2,
                2,
                3,
                0,
            };
            return MakeShared<Mesh>(vertices, indices);
        }

        SharedPtr<Mesh> GenerateQuad()
        {
            std::vector<Vertex> vertices(4);

            vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
            vertices[0].texCoord = Vector2(0.0f, 0.0f);
           

            vertices[1].position = Vector3(1.0f, -1.0f, 0.0f);          
            vertices[1].texCoord = Vector2(1.0f, 0.0f);

            vertices[2].position = Vector3(1.0f, 1.0f, 0.0f);
            vertices[2].texCoord = Vector2(1.0f, 1.0f);

            vertices[3].position = Vector3(-1.0f, 1.0f, 0.0f);          
            vertices[3].texCoord = Vector2(0.0f, 1.0f);

            std::vector<uint32_t> indices = {
                0,
                1,
                2,
                2,
                3,
                0,
            };

            return MakeShared<Mesh>(vertices, indices); 
        }

        SharedPtr<Mesh> GenerateCube()
        {
            //    v6----- v5
           //   /|      /|
           //  v1------v0|
           //  | |     | |
           //  | |v7---|-|v4
           //  |/      |/
           //  v2------v3
            
            std::vector<Vertex> vertices(24);

            vertices[0].position = Vector3(0.5f, 0.5f, 0.5f);       
            vertices[0].normal = Vector3(0.0f, 0.0f, 1.0f);

            vertices[1].position = Vector3(-0.5f, 0.5f, 0.5f);           
            vertices[1].normal = Vector3(0.0f, 0.0f, 1.0f);

            vertices[2].position = Vector3(-0.5f, -0.5f, 0.5f);          
            vertices[2].normal = Vector3(0.0f, 0.0f, 1.0f);

            vertices[3].position = Vector3(0.5f, -0.5f, 0.5f);
            vertices[3].normal = Vector3(0.0f, 0.0f, 1.0f);

            vertices[4].position = Vector3(0.5f, 0.5f, 0.5f);
            vertices[4].normal = Vector3(1.0f, 0.0f, 0.0f);

            vertices[5].position = Vector3(0.5f, -0.5f, 0.5f);
            vertices[5].normal = Vector3(1.0f, 0.0f, 0.0f);

            vertices[6].position = Vector3(0.5f, -0.5f, -0.5f);
            vertices[6].normal = Vector3(1.0f, 0.0f, 0.0f);

            vertices[7].position = Vector3(0.5f, 0.5f, -0.5f);
            vertices[7].normal = Vector3(1.0f, 0.0f, 0.0f);

            vertices[8].position = Vector3(0.5f, 0.5f, 0.5f);
            vertices[8].normal = Vector3(0.0f, 1.0f, 0.0f);

            vertices[9].position = Vector3(0.5f, 0.5f, -0.5f);
            vertices[9].normal = Vector3(0.0f, 1.0f, 0.0f);

            vertices[10].position = Vector3(-0.5f, 0.5f, -0.5f);
            vertices[10].normal = Vector3(0.0f, 1.0f, 0.0f);

            vertices[11].position = Vector3(-0.5f, 0.5f, 0.5f);
            vertices[11].normal = Vector3(0.0f, 1.0f, 0.0f);

            vertices[12].position = Vector3(-0.5f, 0.5f, 0.5f);
            vertices[12].normal = Vector3(-1.0f, 0.0f, 0.0f);

            vertices[13].position = Vector3(-0.5f, 0.5f, -0.5f);
            vertices[13].normal = Vector3(-1.0f, 0.0f, 0.0f);

            vertices[14].position = Vector3(-0.5f, -0.5f, -0.5f);
            vertices[14].normal = Vector3(-1.0f, 0.0f, 0.0f);

            vertices[15].position = Vector3(-0.5f, -0.5f, 0.5f);
            vertices[15].normal = Vector3(-1.0f, 0.0f, 0.0f);

            vertices[16].position = Vector3(-0.5f, -0.5f, -0.5f);
            vertices[16].normal = Vector3(0.0f, -1.0f, 0.0f);

            vertices[17].position = Vector3(0.5f, -0.5f, -0.5f);
            vertices[17].normal = Vector3(0.0f, -1.0f, 0.0f);

            vertices[18].position = Vector3(0.5f, -0.5f, 0.5f);
            vertices[18].normal = Vector3(0.0f, -1.0f, 0.0f);

            vertices[19].position = Vector3(-0.5f, -0.5f, 0.5f);
            vertices[19].normal = Vector3(0.0f, -1.0f, 0.0f);

            vertices[20].position = Vector3(0.5f, -0.5f, -0.5f);
            vertices[20].normal = Vector3(0.0f, 0.0f, -1.0f);

            vertices[21].position = Vector3(-0.5f, -0.5f, -0.5f);
            vertices[21].normal = Vector3(0.0f, 0.0f, -1.0f);

            vertices[22].position = Vector3(-0.5f, 0.5f, -0.5f);
            vertices[22].normal = Vector3(0.0f, 0.0f, -1.0f);

            vertices[23].position = Vector3(0.5f, 0.5f, -0.5f);
            vertices[23].normal = Vector3(0.0f, 0.0f, -1.0f);

            for (int i = 0; i < 6; i++)
            {
                vertices[i * 4 + 0].texCoord = Vector2(0.0f, 0.0f);
                vertices[i * 4 + 1].texCoord = Vector2(1.0f, 0.0f);
                vertices[i * 4 + 2].texCoord = Vector2(1.0f, 1.0f);
                vertices[i * 4 + 3].texCoord = Vector2(0.0f, 1.0f);
            }

            std::vector<uint32_t> indices = {
                0, 1, 2,
                0, 2, 3,
                4, 5, 6,
                4, 6, 7,
                8, 9, 10,
                8, 10, 11,
                12, 13, 14,
                12, 14, 15,
                16, 17, 18,
                16, 18, 19,
                20, 21, 22,
                20, 22, 23
            };

            return MakeShared<Mesh>(vertices, indices);
        }

        SharedPtr<Mesh> GenerateSphere(uint32_t horizontalSegments, uint32_t verticalSegments)
        {
            
            std::vector<Vertex> vertices = std::vector<Vertex>();

            float sectorCount = static_cast<float>(horizontalSegments);
            float stackCount = static_cast<float>(verticalSegments);
            float sectorStep = 2 * PI / sectorCount;
            float stackStep = PI / stackCount;
            float radius = 0.5f;

            for (int i = 0; i <= stackCount; ++i)
            {
                float stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
                float xy = radius * cos(stackAngle);        // r * cos(u)
                float z = radius * sin(stackAngle);        // r * sin(u)

                // add (sectorCount+1) vertices per stack
                // the first and last vertices have same position and normal, but different tex coords
                for (int j = 0; j <= sectorCount; ++j)
                {
                    float sectorAngle = j * sectorStep; // starting from 0 to 2pi

                    // vertex position (x, y, z)
                    float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                    float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

                    // vertex tex coord (s, t) range between [0, 1]
                    float s = static_cast<float>(j / sectorCount);
                    float t = static_cast<float>(i / stackCount);

                    Vertex vertex;
                    vertex.position = Vector3(x, y, z);
                    vertex.texCoord = Vector2(s, t);
                    vertex.normal = Normalize(Vector3(x, y, z));

                    vertices.emplace_back(vertex);
                }
            }

            std::vector<uint32_t> indices;
            uint32_t k1, k2;
            for (uint32_t i = 0; i < stackCount; ++i)
            {
                k1 = i * (static_cast<uint32_t>(sectorCount) + 1U); // beginning of current stack
                k2 = k1 + static_cast<uint32_t>(sectorCount) + 1U;  // beginning of next stack

                for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2)
                {
                    // 2 triangles per sector excluding first and last stacks
                    // k1 => k2 => k1+1
                    if (i != 0)
                    {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }

                    // k1+1 => k2 => k2+1
                    if (i != (stackCount - 1))
                    {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }

            return MakeShared<Mesh>(vertices, indices);
        }

        SharedPtr<Mesh> GenerateCapsule(float radius, float midHeight, int radialSegments, int rings)
        {
            return SharedPtr<Mesh>();
        }

        SharedPtr<Mesh> GeneratePlane(float width, float height, const Vector3& normal)
        {
            Vector3 vec = normal * 90.0f;
            Quaternion rotation = Quaternion(vec.z, Vector3(1.0f, 0.0f, 0.0f)) * Quaternion(vec.y, Vector3(0.0f, 1.0f, 0.0f)) * Quaternion(vec.x, Vector3(0.0f, 0.0f, 1.0f));

            std::vector<Vertex> vertices(4);

            vertices[0].position = rotation * Vector3(-width * 0.5f, -1.0f, -height * 0.5f);
            vertices[0].normal = normal;
            vertices[0].texCoord = Vector2(0.0f, 0.0f);

            vertices[1].position = rotation * Vector3(-width * 0.5f, -1.0f, height * 0.5f);
            vertices[1].normal = normal;
            vertices[1].texCoord = Vector2(0.0f, 1.0f);

            vertices[2].position = rotation * Vector3(width * 0.5f, 1.0f, height * 0.5f);
            vertices[2].normal = normal;
            vertices[2].texCoord = Vector2(1.0f, 1.0f);

            vertices[3].position = rotation * Vector3(width * 0.5f, 1.0f, -height * 0.5f);
            vertices[3].normal = normal;
            vertices[3].texCoord = Vector2(1.0f, 0.0f);

            std::vector<uint32_t> indices = {
                0, 1, 2,
                2, 3, 0
            };


            return MakeShared<Mesh>(vertices, indices);
        
        }

        SharedPtr<Mesh> GenerateCylinder(float radius, float height, int radialSegments, int rings)
        {
            return SharedPtr<Mesh>();
        }

        SharedPtr<Mesh> CreateTerrain()
        {
            return SharedPtr<Mesh>();
        }

    }
}