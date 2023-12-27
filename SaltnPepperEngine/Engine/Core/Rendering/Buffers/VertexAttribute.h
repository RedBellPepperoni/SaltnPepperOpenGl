#ifndef VERTEXATTRIBUTE_H
#define VERTEXATTRIBUTE_H

#include <cstddef>
#include <cstdint>

namespace SaltnPepperEngine
{

    namespace Rendering
    {
		struct VertexAttribute
		{
			uint32_t type;
			uint16_t components;
			uint16_t entries;
			size_t byteSize;

			template<typename T>
			static VertexAttribute Attribute();

		};
    }
}

#endif // !VERTEXATTRIBUTE_H

