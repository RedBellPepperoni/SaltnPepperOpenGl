#include "VertexAttribute.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/GLUtils.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
        template<>
        VertexAttribute VertexAttribute::Attribute<float>()
        {
            return { GL_FLOAT, 1, 1, sizeof(float) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Vector2>()
        {
            return { GL_FLOAT, 2, 1, sizeof(Vector2) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Vector3>()
        {
            return { GL_FLOAT, 3, 1, sizeof(Vector3) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Vector4>()
        {
            return { GL_FLOAT, 4, 1, sizeof(Vector4) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<int32_t>()
        {
            return { GL_INT, 1, 1, sizeof(int32_t) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Vector2Int>()
        {
            return { GL_INT, 2, 1, sizeof(Vector2Int) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Vector3Int>()
        {
            return { GL_INT, 3, 1, sizeof(Vector3Int) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Vector4Int>()
        {
            return { GL_INT, 4, 1, sizeof(Vector4Int) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Matrix2>()
        {
            return { GL_FLOAT, 2, 2, sizeof(Matrix2) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Matrix3>()
        {
            return { GL_FLOAT, 3, 3, sizeof(Matrix3) };
        }

        template<>
        VertexAttribute VertexAttribute::Attribute<Matrix4>()
        {
            return { GL_FLOAT, 4, 4, sizeof(Matrix4) };
        }
    }
}