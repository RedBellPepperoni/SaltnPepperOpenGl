#include "ShaderStorageBuffer.h"
namespace SaltnPepperEngine
{
	namespace Rendering
	{
		ShaderStorageBuffer::ShaderStorageBuffer()
		{
			GLDEBUG(glGenBuffers(1, &id));
			LOG_INFO("ShaderStorageBuffer Created with id : {0}", id);
		}
		ShaderStorageBuffer::~ShaderStorageBuffer()
		{
			FreeBuffer();
		}
		void ShaderStorageBuffer::Bind() const
		{
			GLDEBUG(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
		}

		void ShaderStorageBuffer::UnBind() const
		{
			GLDEBUG(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
		}

		ShaderStorageBuffer::BindableId ShaderStorageBuffer::GetHandle() const
		{
			return id;
		}
		UsageType ShaderStorageBuffer::GetUsageType() const
		{
			return usageType;
		}
		size_t ShaderStorageBuffer::GetByteSize() const
		{
			return byteSize;
		}

		void ShaderStorageBuffer::SetUsageType(UsageType usage)
		{
			usageType = usage;
		}

		void ShaderStorageBuffer::FreeBuffer()
		{
			if (id != 0)
			{
				GLDEBUG(glDeleteBuffers(1, &id));
				LOG_TRACE("Deleted Shader Buffer with id : {0}", id);
				id = 0;
			}
		}
	}
}