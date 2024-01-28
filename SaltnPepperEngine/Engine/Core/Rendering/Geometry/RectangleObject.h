#ifndef RECTANGLEOBJECT_H
#define RECTANGLEOBJECT_H

#include "Engine/Core/Rendering/Buffers/VertexBuffer.h"
#include "Engine/Core/Rendering/Buffers/IndexBuffer.h"
#include "Engine/Core/Rendering/Buffers/VertexArray.h"

#include "Engine/Core/Memory/MemoryDefinitions.h"


namespace SaltnPepperEngine
{
	using namespace Rendering;

	class RectangleObject
	{
		SharedPtr<VertexBuffer> VBO;
		SharedPtr<IndexBuffer> IBO;
		SharedPtr<VertexArray> VAO;

	public:

		

		// 2 Triangles , each having 3 vertices
		static constexpr size_t IndexCount = 2 * 3;
		void Init(float HalfSize);
		SharedPtr<VertexArray>& GetVAO();
	};

}

#endif // !RECTANGLEOBJECT_H

