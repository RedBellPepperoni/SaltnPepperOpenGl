#ifndef RENDERDEFINITIONS_H
#define RENDERDEFINITIONS_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/GLUtils.h"

namespace SaltnPepperEngine
{	

	namespace Rendering
	{
		enum class UsageType
		{
			STREAM_DRAW,
			STREAM_READ,
			STREAM_COPY,
			STATIC_DRAW,
			STATIC_READ,
			STATIC_COPY,
			DYNAMIC_DRAW,
			DYNAMIC_READ,
			DYNAMIC_COPY,
		};

		enum class BufferType
		{
			UNKNOWN = 0,
			ARRAY,
			ELEMENT_ARRAY,
			SHADER_STORAGE,
		};

		// Returns the correct GL value for the usage
		static uint32_t GetGLBufferUsage(UsageType usage)
		{
			switch (usage)
			{
			case UsageType::STREAM_DRAW: return GL_STREAM_DRAW;
				break;
			case UsageType::STREAM_READ: return GL_STREAM_READ;
				break;
			case UsageType::STREAM_COPY: return GL_STREAM_COPY;
				break;
			case UsageType::STATIC_DRAW: return GL_STATIC_DRAW;
				break;
			case UsageType::STATIC_READ: return GL_STATIC_READ;
				break;
			case UsageType::STATIC_COPY: return GL_STATIC_COPY;
				break;
			case UsageType::DYNAMIC_DRAW: return GL_DYNAMIC_DRAW;
				break;
			case UsageType::DYNAMIC_READ: return GL_DYNAMIC_READ;
				break;
			case UsageType::DYNAMIC_COPY: return GL_DYNAMIC_COPY;
				break;
			default: return GL_FALSE;
				break;
			}
		}

		struct Vertex
		{
			Vector3 position{ 0.0f };
			Vector2 texCoord{ 0.0f };
			Vector3 normal{ 0.0f };
			Vector3 tangent{ 0.0f };
			Vector3 bitangent{ 0.0f };

			static const size_t Size = 3 + 2 + 3 + 3 + 3;
		};

		struct Triangle
		{
			Vertex v0;
			Vertex v1;
			Vertex v2;
		};


	}

}

#endif // !RENDERDEFINITIONS_H

