#include "IndexBuffer.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
		IndexBuffer::IndexBuffer(size_t count, const uint32_t* data, UsageType usage) : indiciesCount(count)
			, bufferUsage(usage)
		{
			// Put GL Error checking here

			// Generating and getting an ID for a new Index Buffer
			GLDEBUG(glGenBuffers(1, &bufferId));

			// Binding the Generated buffer for use
			GLDEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId));

			byteSize = count * sizeof(uint32_t);

			// Loading Data into the binded buffer
			GLDEBUG(glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, data, GetGLBufferUsage(usage)));


		}
		IndexBuffer::~IndexBuffer()
		{
			GLDEBUG(glDeleteBuffers(1, &bufferId));
		}
		void IndexBuffer::Bind()
		{
			bufferUsage = UsageType::STATIC_DRAW;
			GLDEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId));
		}
		void IndexBuffer::UnBind()
		{
			GLDEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		size_t IndexBuffer::GetIndiciesCount()
		{
			return indiciesCount;
		}
		void IndexBuffer::SetData(size_t size, const void* data)
		{
		}
		void IndexBuffer::SetSubData(size_t offset, size_t size, const void* data)
		{
			if (size + offset >= byteSize)
			{
				// Error, size is over the data
			}
			Bind();

			// Updating data in a previous set buffer
			GLDEBUG(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
		}
    }
}