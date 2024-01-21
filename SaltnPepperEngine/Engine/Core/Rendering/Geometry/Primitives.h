#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

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
            Terrain     = 7,
            External    = 8
            
        };

       SharedPtr<Mesh> GeneratePrimitive(PrimitiveType type);

        
        // Creates a default Quad at -1,-1 (so that center is at 0,0) with a size of 2,2
       // Creates a Quad with the specifed params with the lower most vertex at the give nposition
       SharedPtr<Mesh> GenerateQuad(const Vector2& position, const Vector2& size);
       SharedPtr<Mesh> GenerateQuad();
       SharedPtr<Mesh> GenerateCube();
       SharedPtr<Mesh> GenerateSphere(uint32_t horizontalSegments = 32, uint32_t verticalSegments = 32);
       SharedPtr<Mesh> GenerateCapsule(float radius = 0.5f, float midHeight = 2.0f, int radialSegments = 64, int rings = 8);
       SharedPtr<Mesh> GeneratePlane(float width, float height, const Vector3& normal);
       SharedPtr<Mesh> GenerateCylinder(float radius = 0.5f, float height = 1.0f, int radialSegments = 64, int rings = 8);
       SharedPtr<Mesh> CreateTerrain();
    }
}

#endif // !PRIMITIVES_H

