#include "RectangleObject.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"

namespace SaltnPepperEngine
{
    


    void RectangleObject::Init(float HalfSize)
	{

        std::array vertecies =
        {
            Vector4(-HalfSize, -HalfSize, 0.5f, 1.0f),
            Vector4(HalfSize, -HalfSize, 0.5f, 1.0f),
            Vector4(-HalfSize,  HalfSize, 0.5f, 1.0f),
            Vector4(HalfSize,  HalfSize, 0.5f, 1.0f),
        };

        std::array indicies = {
            0u, 1u, 2u,
            2u, 1u, 3u,
        };

        VBO = MakeShared<VertexBuffer>(

            
            vertecies.size() * (sizeof(Vector4) / sizeof(float)),
            (float*)vertecies.data(),
            UsageType::STATIC_DRAW
        );

        IBO = MakeShared<IndexBuffer>(
            indicies.size(),
            indicies.data(),
            UsageType::STATIC_DRAW
        );

        std::vector vertexLayout = {
            VertexAttribute::Attribute<Vector4>()
        };

        VAO = MakeShared<VertexArray>();

        VAO->Bind();
        VBO->Bind();

        VAO->AddVertexAttribLayout(vertexLayout);
        
        IBO->Bind();
        VAO->UnBind();

	}

	const VertexArray& RectangleObject::GetVAO() const
	{
		return *VAO;
	}
}