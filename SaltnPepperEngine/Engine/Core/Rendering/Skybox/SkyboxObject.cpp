#include "SkyboxObject.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"

namespace SaltnPepperEngine
{

    void SkyboxObject::Init()
    {
        constexpr float size = 1.0f;

        constexpr std::array vertices =
        {
            Vector3(-size, -size, -size),
            Vector3(-size, -size,  size),
            Vector3(-size,  size, -size),
            Vector3(-size,  size,  size),
            Vector3(size, -size, -size),
            Vector3(size, -size,  size),
            Vector3(size,  size, -size),
            Vector3(size,  size,  size),
        };
        constexpr std::array indices =
        {
            1u, 7u, 5u,
            7u, 1u, 3u,
            4u, 2u, 0u,
            2u, 4u, 6u,
            4u, 7u, 6u,
            7u, 4u, 5u,
            2u, 1u, 0u,
            1u, 2u, 3u,
            0u, 5u, 4u,
            5u, 0u, 1u,
            6u, 3u, 2u,
            3u, 6u, 7u,
        };

        VBO = MakeShared<VertexBuffer>(vertices.size() * sizeof(Vector3), (float*)vertices.data(), UsageType::STATIC_DRAW);

        IBO = MakeShared<IndexBuffer>(indices.size(), indices.data(), UsageType::STATIC_DRAW);

        std::vector<VertexAttribute> vertexLayout =
        {
            VertexAttribute::Attribute<Vector3>()
        };

        VAO = MakeShared<VertexArray>();

        VAO->Bind();

        VBO->Bind();

        VAO->AddVertexAttribLayout(vertexLayout);

        IBO->Bind();

        VAO->UnBind();


    }



    const VertexArray& SkyboxObject::GetVAO() const
    {
        return *VAO;
    }
}
