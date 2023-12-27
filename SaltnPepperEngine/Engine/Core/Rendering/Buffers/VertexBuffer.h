#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include <stdint.h>

namespace SaltnPepperEngine
{

    namespace Rendering
    {
		enum class UsageType;
		enum class BufferType;

		class VertexBuffer
		{

		private:

			uint32_t bufferID = 0;
			size_t byteSize = 0;
			UsageType bufferUsage;

		public:

			VertexBuffer(UsageType usage);
			VertexBuffer(size_t size, const void* data, UsageType usage);


			~VertexBuffer();

			void Bind();
			void UnBind();
			// This should only be called once when initailizing a new buffer
			void SetData(size_t size, const void* data);

			// This should be used to update/override the data in the buffer
			void SetSubData(size_t offset, size_t size, const void* data);

			// Getters
			uint32_t GetBufferID();
			size_t GetSize();

			// Setters
			void SetBufferUsage(UsageType usage);



		};
    }
}



#endif // !VERTEXBUFFER_H

