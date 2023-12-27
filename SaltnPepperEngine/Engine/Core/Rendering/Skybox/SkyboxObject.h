#ifndef SKYBOXOBJECT_H
#define SKYBOXOBJECT_H

#include "Engine/Core/Rendering/Buffers/VertexBuffer.h"
#include "Engine/Core/Rendering/Buffers/IndexBuffer.h"
#include "Engine/Core/Rendering/Buffers/VertexArray.h"

#include "Engine/Core/Memory/MemoryDefinitions.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;


	class SkyboxObject
	{
		SharedPtr<VertexBuffer> VBO;
		SharedPtr<IndexBuffer> IBO;
		SharedPtr<VertexArray> VAO;


	public:



		// 6 sides , each side having 2 triangles and 3 vertices
		static constexpr size_t IndexCount = 6 * 2 * 3;

		void Init();
		const VertexArray& GetVAO() const;

	};

}



#endif // !SKYBOXOBJECT_H
