#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Textures/CubeMap.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
		enum class Attachment : uint8_t
		{
			COLOR_ATTACH_0,
			COLOR_ATTACH_1,
			COLOR_ATTACH_2,
			COLOR_ATTACH_3,
			COLOR_ATTACH_4,
			COLOR_ATTACH_5,
			COLOR_ATTACH_6,
			COLOR_ATTACH_9,
			COLOR_ATTACH_10,
			COLOR_ATTACH_11,
			COLOR_ATTACH_12,
			COLOR_ATTACH_13,
			COLOR_ATTACH_14,
			COLOR_ATTACH_15,
			DEPTH_ATTACH,
			STENCIL_ATTACH,
			DEPTH_STENCIL_ATTACH
		};

		class FrameBuffer
		{

		private:

			enum class AttachmentType : uint8_t
			{
				NONE,
				TEXTURE,
				CUBEMAP
			};

			using BindableID = uint32_t;

			BindableID m_id = 0;
			AttachmentType m_currentAttachment = AttachmentType::NONE;

			SharedPtr<Texture> m_textureRef;
			SharedPtr<CubeMap> m_cubemapRef;


		private:

			void OnTextureAttach(const SharedPtr<Texture>& texture, Attachment attachment);
			void OnCubeMapAttach(const SharedPtr<CubeMap>& cubemap, Attachment attachment);
			void DeleteFrameBuffer();



		public:

			FrameBuffer();
			~FrameBuffer();

			void Validate() const;
			void DetachRenderTarget();
			void DetachExtraTarget(Attachment attachment);

			bool IsTextureAttached() const;
			bool IsCubeMapAttached() const;

			void UseDrawBuffers(std::vector<Attachment> attachments) const;
			void UseOnlyDepth() const;


			size_t GetWidth() const;
			size_t GetHeight() const;


			void Bind() const;
			void UnBind() const;

			BindableID GetHandle() const;


			void AttachTexture(const SharedPtr<Texture>& texture, Attachment attachment = Attachment::COLOR_ATTACH_0)
			{
				DetachRenderTarget();

				m_currentAttachment = AttachmentType::TEXTURE;

				OnTextureAttach(texture, attachment);


			}

			void AttachTextureExtra(const SharedPtr<Texture>& texture, Attachment attachment)
			{
				OnTextureAttach(texture, attachment);
			}

			void AttachTextureExtra(const SharedPtr<CubeMap>& cubemap, Attachment attachment)
			{
				OnCubeMapAttach(cubemap, attachment);
			}



		};
    }
}

#endif // !FRAMEBUFFER_H

