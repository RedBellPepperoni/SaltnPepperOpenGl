#ifndef SHADERSTORAGEBUFFER_H
#define SHADERSTORAGEBUFFER_H

#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class ShaderStorageBuffer
		{
			using BindableId = uint32_t;

		public:
			template<typename T>
			ShaderStorageBuffer(const T* data, size_t count, UsageType usage)
			{
				Load<T>(data, count, usage);
			}

			template<typename T>
			size_t GetSize() const
			{
				return GetByteSize() / sizeof(T);
			}

			template<typename T>
			void Load(const T* data, size_t count, UsageType usage)
			{
				byteSize = count * sizeof(T);
				usageType = usage;
				Bind();

				GLDEBUG(glBufferData(GL_SHADER_STORAGE_BUFFER, byteSize, (const uint8_t*)data, GetGLBufferUsage(usage)));
			}


			template<typename T>
			void BufferSubData(const T* data, size_t count, size_t offsetCount = 0)
			{
				
			
				size_t size = count * sizeof(T);
				if (size + offsetCount >= byteSize)
				{
					LOG_ERROR("Shader Storage Buffer Overflow : ID :{0}",id);
				}

				Bind();
				GLDEBUG(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetCount, size, (const uint32_t*)data));
			
			}

			void BindBase(size_t index) const
			{
				GLDEBUG(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLint)index, id));
			}


		public:

			ShaderStorageBuffer();
			~ShaderStorageBuffer();

			void Bind() const;
			void UnBind() const;

			//void Clear();

			BindableId GetHandle() const;
			UsageType GetUsageType() const;
			size_t GetByteSize() const;


			void SetUsageType(UsageType usage);

		private:

			

			BindableId id = 0;
			UsageType usageType = UsageType::STATIC_DRAW;
			size_t byteSize = 0;

			void FreeBuffer();



		};
	}

}


#endif // !SHADERSTORAGEBUFFER_H


