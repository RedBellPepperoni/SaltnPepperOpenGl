#include "Primitives.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
        Mesh* GeneratePrimitive(PrimitiveType type)
        {
            switch (type)
            {
            case PrimitiveType::Cube:
                return GenerateCube();
            case PrimitiveType::Plane:
                return GeneratePlane(1.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
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

        Mesh* GenerateQuad(const Vector2& position, const Vector2& size)
        {
            return nullptr;
        }
        Mesh* GenerateCube()
        {
            return nullptr;
        }
        Mesh* GenerateSphere(uint32_t horizontalSegments, uint32_t verticalSegments)
        {
            return nullptr;
        }
        Mesh* GenerateCapsule(float radius, float midHeight, int radialSegments, int rings)
        {
            return nullptr;
        }
        Mesh* GeneratePlane(float width, float height, const glm::vec3& normal)
        {
            return nullptr;
        }
        Mesh* GenerateCylinder(float radius, float height, int radialSegments, int rings)
        {
            return nullptr;
        }
        Mesh* CreateTerrain()
        {
            return nullptr;
        }
    }
}