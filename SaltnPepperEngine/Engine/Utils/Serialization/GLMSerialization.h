#ifndef GLMSERIALIZATION_H
#define GLMSERIALIZATION_H

#include "Engine/Utils/Maths/MathDefinitions.h"

using namespace SaltnPepperEngine::Math;

// Weird fix but including the glm namespace fixes issues with custom types
namespace glm 
{
    template <class Archive>
    void serialize(Archive& archive, Vector2& v)
    {
        archive(v.x, v.y);
    }

    template <class Archive>
    void serialize(Archive& archive, Vector3& v)
    {
        archive(v.x, v.y, v.z);
    }

    template <class Archive>
    void serialize(Archive& archive, Vector4& v)
    {
        archive(v.x, v.y, v.z, v.w);
    }

    template <class Archive>
    void serialize(Archive& archive, Vector2Int& v)
    {
        archive(v.x, v.y);
    }

    template <class Archive>
    void serialize(Archive& archive, Vector3Int& v)
    {
        archive(v.x, v.y, v.z);
    }

    template <class Archive>
    void serialize(Archive& archive, Vector4Int& v)
    {
        archive(v.x, v.y, v.z, v.w);
    }


    // glm matrices serialization
    template <class Archive>
    void serialize(Archive& archive, Matrix2& m)
    {
        archive(m[0], m[1]);
    }

    template <class Archive>
    void serialize(Archive& archive, Matrix3& m)
    {
        archive(m[0], m[1], m[2]);
    }

    template <class Archive>
    void serialize(Archive& archive, Matrix4& m)
    {
        archive(m[0], m[1], m[2], m[3]);
    }


    template <class Archive>
    void serialize(Archive& archive, Quaternion& q)
    {
        archive(q.x, q.y, q.z, q.w);
    }
}
   

    



#endif // !GLMSERIALIZATION

