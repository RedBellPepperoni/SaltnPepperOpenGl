#include "VertexArray.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "VertexBuffer.h"


namespace SaltnPepperEngine
{

    namespace Rendering
    {
		VertexArray::VertexArray()
		{

			GLDEBUG(glGenVertexArrays(1, &bufferId));



		}
		VertexArray::~VertexArray()
		{
			FreeArray();
		}

		void VertexArray::Bind() const
		{
			GLDEBUG(glBindVertexArray(bufferId));

		}
		void VertexArray::UnBind() const
		{
			GLDEBUG(glBindVertexArray(0));

		}


		void VertexArray::FreeArray()
		{
			if (bufferId != 0)
			{
				GLDEBUG(glDeleteVertexArrays(1, &bufferId));
			}

			bufferId = 0;
		}


		void VertexArray::AddVertexAttributelayout(uint32_t shaderId)
		{

			// Should Make this Dynamic later on, actually store the atrribs in the shader itslef and pull data accordingly

			GLsizei stride = (GLsizei)(Vertex::Size * sizeof(float));

			GLDEBUG(glEnableVertexAttribArray(0));
			GLDEBUG(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position)));

			GLDEBUG(glEnableVertexAttribArray(1));
			GLDEBUG(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texCoord)));

			/*GLDEBUG(glEnableVertexAttribArray(color));
			GLDEBUG(glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, Vertex::Stride, (void*)offsetof(Vertex, color)));*/

			GLDEBUG(glEnableVertexAttribArray(2));
			GLDEBUG(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal)));



		}

		void VertexArray::AddVertexAttribLayout(std::vector<VertexAttribute>& layout)
		{


			size_t offset = 0;
			size_t stride = 0;

			uint32_t count = 0;

			for (const VertexAttribute& elemnent : layout)
			{
				stride += elemnent.byteSize;
			}

			for (const VertexAttribute& element : layout)
			{
				for (size_t index = 0; index < element.entries; index++)
				{


					GLDEBUG(glEnableVertexAttribArray(count));
					GLDEBUG(glVertexAttribPointer(count, element.components, (GLenum)element.type, GL_FALSE, (GLsizei)stride, (void*)offset));

					offset += element.byteSize / element.entries;
					count++;
				}
			}



		}

		void VertexArray::AddSkinnedVertexAttributelayout(uint32_t shaderId)
		{

			// Should Make this Dynamic later on, actually store the atrribs in the shader itslef and pull data accordingly

			GLsizei stride = (GLsizei)sizeof(SkinnedVertex);

			GLDEBUG(glEnableVertexAttribArray(0));
			GLDEBUG(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(SkinnedVertex, position)));

			GLDEBUG(glEnableVertexAttribArray(1));
			GLDEBUG(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(SkinnedVertex, texCoord)));


			GLDEBUG(glEnableVertexAttribArray(2));
			GLDEBUG(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(SkinnedVertex, normal)));


			GLDEBUG(glEnableVertexAttribArray(3));
			GLDEBUG(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(SkinnedVertex, tangent)));


			GLDEBUG(glEnableVertexAttribArray(4));
			GLDEBUG(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(SkinnedVertex, bitangent)));
		
			
		}
    }
}