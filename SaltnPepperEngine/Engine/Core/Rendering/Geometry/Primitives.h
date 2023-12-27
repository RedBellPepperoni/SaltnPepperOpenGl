#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
        class Mesh;
        class Material;


        enum class PrimitiveType : uint8_t
        {
            None        = 0,
            Plane       = 1,
            Quad        = 2,
            Sphere      = 3,
            Cube        = 4,
            Cylinder    = 5,
            Capsule     = 6,
            Terrian     = 7,
            External    = 8
            
        };

        Mesh* GeneratePrimitive(PrimitiveType type);

        
        // Creates a default Quad at -1,-1 (so that center is at 0,0) with a size of 2,2
       // Creates a Quad with the specifed params with the lower most vertex at the give nposition
        Mesh* GenerateQuad(const Vector2& position = Vector2(-1.0f), const Vector2& size = Vector2(2.0f));
        Mesh* GenerateCube();
        Mesh* GenerateSphere(uint32_t horizontalSegments = 32, uint32_t verticalSegments = 32);
        Mesh* GenerateCapsule(float radius = 0.5f, float midHeight = 2.0f, int radialSegments = 64, int rings = 8);
        Mesh* GeneratePlane(float width, float height, const glm::vec3& normal);
        Mesh* GenerateCylinder(float radius = 0.5f, float height = 1.0f, int radialSegments = 64, int rings = 8);
        Mesh* CreateTerrain();
    }
}

#endif // !PRIMITIVES_H

