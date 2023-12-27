#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
#include <stdint.h>

namespace SaltnPepperEngine
{

    namespace Rendering
    {
		enum class UsageType;
		enum class BufferType;


		class IndexBuffer
		{


		private:

			uint32_t bufferId = 0;
			size_t indiciesCount = 0;

			UsageType bufferUsage;// = UsageType::STATIC_DRAW;



		public:

			IndexBuffer(size_t count, const uint32_t* data, UsageType usage);
			~IndexBuffer();

			void Bind();
			void UnBind();
			size_t GetIndiciesCount();

		};
    }
}


#endif // !INDEXBUFFER_H

