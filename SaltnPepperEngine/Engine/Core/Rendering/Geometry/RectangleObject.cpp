#include "RectangleObject.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"

namespace SaltnPepperEngine
{
    
    struct RenderQuadVertex
    {
        RenderQuadVertex(Vector2 newPos, Vector2 newTexCoords)
            : position(newPos)
            , textureCoord(newTexCoords)
        {

        }

        Vector2 position;
        Vector2 textureCoord;

        static const size_t size = 2 + 2;
    };


    void RectangleObject::Init(float HalfSize)
	{

        std::array vertices =
        {
            Vector4(-HalfSize, -HalfSize, 0.5f, 1.0f),
            Vector4(HalfSize, -HalfSize, 0.5f, 1.0f),
            Vector4(-HalfSize,  HalfSize, 0.5f, 1.0f),
            Vector4(HalfSize,  HalfSize, 0.5f, 1.0f),
        };


       

        //std::vector<RenderQuadVertex> vertices
        //{
        //    RenderQuadVertex(Vector2(-1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        //    RenderQuadVertex(Vector2(-1.0f, -1.0f), Vector2(0.0f, 0.0f)),
        //    RenderQuadVertex(Vector2(1.0f, -1.0f), Vector2(1.0f, 0.0f)),

        //    RenderQuadVertex(Vector2(-1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        //    RenderQuadVertex(Vector2(1.0f, -1.0f), Vector2(1.0f, 0.0f)),
        //    RenderQuadVertex(Vector2(1.0f, 1.0f), Vector2(1.0f, 1.0f)),

        //};


       

        std::array indicies = {
            0u, 1u, 2u,
            1u, 3u, 2u
        };  
        
        


        VBO = MakeShared<VertexBuffer>(

            
           // vertices.size() * 4 * sizeof(float),
            vertices.size() * (sizeof(Vector4)),
            vertices.data(),
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
        VBO->UnBind();
        IBO->UnBind();

	}

	SharedPtr<VertexArray>& RectangleObject::GetVAO() 
	{
		return VAO;
	}
}