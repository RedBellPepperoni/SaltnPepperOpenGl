#include "Compute.h"
#include "Engine/Utils/GLUtils.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
       constexpr std::array<uint64_t, 16> BarrierMappings = {
       GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
       GL_ELEMENT_ARRAY_BARRIER_BIT,
       GL_UNIFORM_BARRIER_BIT,
       GL_TEXTURE_FETCH_BARRIER_BIT,
       GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
       GL_COMMAND_BARRIER_BIT,
       GL_PIXEL_BUFFER_BARRIER_BIT,
       GL_TEXTURE_UPDATE_BARRIER_BIT,
       GL_BUFFER_UPDATE_BARRIER_BIT,
       GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT,
       GL_FRAMEBUFFER_BARRIER_BIT,
       GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
       GL_ATOMIC_COUNTER_BARRIER_BIT,
       GL_SHADER_STORAGE_BARRIER_BIT,
       GL_QUERY_BUFFER_BARRIER_BIT,
       GL_ALL_BARRIER_BITS,
        };

        void Compute::Dispatch(const SharedPtr<ComputeShader>& computeShader, size_t x, size_t y, size_t z)
        {
            computeShader->Bind();
            GLDEBUG(glDispatchCompute(x, y, z));
        }

        void Compute::SetMemoryBarrier(BarrierType::Bits barriers)
        {
            GLbitfield barrierBits = 0;
            for (size_t i = 0; i < BarrierMappings.size(); i++)
            {
                bool bit = (uint64_t)barriers & (1ull << i);
                barrierBits |= (bit ? BarrierMappings[i] : 0);
            }
            GLDEBUG(glMemoryBarrier(barrierBits));
        }
	}
}