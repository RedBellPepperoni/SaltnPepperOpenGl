#include "VertexBuffer.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {

		VertexBuffer::VertexBuffer(UsageType usage) : bufferUsage(usage), byteSize(0)
		{
			// generating a new Buffer and Storing the ID acquired from OpenGL
			GLDEBUG(glGenBuffers(1, &bufferID));

		}

		VertexBuffer::VertexBuffer(size_t size, const void* data, UsageType usage)
		{
			// generating a new Buffer and Storing the ID acquired from OpenGL
			GLDEBUG(glGenBuffers(1, &bufferID));

			SetData(size, data);

		}

		VertexBuffer::~VertexBuffer()
		{
			// Deleting the buffer linked to the bufferID
			GLDEBUG(glDeleteBuffers(1, &bufferID));
		}



		void VertexBuffer::Bind()
		{
			bufferUsage = UsageType::STATIC_DRAW;

			GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
		}

		void VertexBuffer::UnBind()
		{
			GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		void VertexBuffer::SetData(size_t size, const void* data)
		{
			// setting the byte size  and binding the buffer
			byteSize = size;
			Bind();

			bufferUsage = UsageType::STATIC_COPY;

			// Setting the Buffer data
			GLDEBUG(glBufferData(GL_ARRAY_BUFFER, size, data, GetGLBufferUsage(bufferUsage)));

		}

		void VertexBuffer::SetSubData(size_t offset, size_t size, const void* data)
		{
			if (size + offset >= byteSize)
			{
				// Error, size is over the data
			}
			Bind();

			// Updating data in a previous set buffer
			GLDEBUG(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
		}


		uint32_t VertexBuffer::GetBufferID()
		{
			return bufferID;
		}

		size_t VertexBuffer::GetSize()
		{
			return byteSize;
		}

		void VertexBuffer::SetBufferUsage(UsageType usage)
		{
			bufferUsage = usage;
		}
    }
}