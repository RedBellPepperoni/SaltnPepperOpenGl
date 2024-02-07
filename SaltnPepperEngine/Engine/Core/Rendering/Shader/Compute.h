#ifndef COMPUTE_H
#define COMPUTE_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Rendering/Shader/ComputeShader.h"

namespace SaltnPepperEngine
{
	namespace Rendering 
	{
        struct BarrierType
        {
            enum Bits : uint64_t
            {
                VERTEX_ARRAY = 1 << 0,
                INDEX_BUFFER = 1 << 1,
                UNIFORM_BUFFER = 1 << 2,
                TEXTURE_FETCH = 1 << 3,
                IMAGE_ACCESS = 1 << 4,
                COMMAND = 1 << 5,
                PIXEL_BUFFER = 1 << 6,
                TEXTURE_UPDATE = 1 << 7,
                BUFFER_UPDATE = 1 << 8,
                CLIENT_MAPPED_BUFFER = 1 << 9,
                FRAMEBUFFER = 1 << 10,
                TRANSFORM_FEEDBACK = 1 << 11,
                ATOMIC_COUNTER = 1 << 12,
                SHADER_STORAGE_BUFFER = 1 << 13,
                QUERY_BUFFER = 1 << 14,
                ALL = 1 << 15,
            };
        };

        class Compute
        {
        public:
            static void Dispatch(const SharedPtr<ComputeShader>& computeShader, size_t x, size_t y, size_t z);
            static void SetMemoryBarrier(BarrierType::Bits barriers);
        };
	}
}

#endif // !COMPUTE_H


