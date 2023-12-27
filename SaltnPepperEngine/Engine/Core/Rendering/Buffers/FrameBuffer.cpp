#include "FrameBuffer.h"
#include "Engine/Utils/GLUtils.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
		GLenum OpenGLAttachment[]
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,
			GL_COLOR_ATTACHMENT5,
			GL_COLOR_ATTACHMENT6,
			GL_COLOR_ATTACHMENT9,
			GL_COLOR_ATTACHMENT10,
			GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12,
			GL_COLOR_ATTACHMENT13,
			GL_COLOR_ATTACHMENT14,
			GL_COLOR_ATTACHMENT15,
			GL_DEPTH_ATTACHMENT,
			GL_STENCIL_ATTACHMENT,
			GL_DEPTH_STENCIL_ATTACHMENT,
		};



		FrameBuffer::FrameBuffer()
		{
			GLDEBUG(glGenFramebuffers(1, &m_id));
			LOG_TRACE("FrameBuffer: Created framebuffer with id [{0}]", m_id);
		}


		FrameBuffer::~FrameBuffer()
		{
			DeleteFrameBuffer();
		}

		void FrameBuffer::OnTextureAttach(const SharedPtr<Texture>& texture, Attachment attachment)
		{
			m_textureRef = texture;
			m_cubemapRef = nullptr;
			GLenum mode = OpenGLAttachment[(int)attachment];
			GLint textureId = texture->GetHandle();


			Bind();
			GLDEBUG(glFramebufferTexture2D(GL_FRAMEBUFFER, mode, texture->GetTextureType(), textureId, 0));
		}

		void FrameBuffer::OnCubeMapAttach(const SharedPtr<CubeMap>& cubemap, Attachment attachment)
		{
			m_textureRef = nullptr;
			m_cubemapRef = cubemap;

			GLenum mode = OpenGLAttachment[(int)attachment];
			GLint cubemapId = cubemap->GetHandle();

			Bind();
			GLDEBUG(glFramebufferTexture(GL_FRAMEBUFFER, mode, cubemapId, 0));

		}

		void FrameBuffer::DeleteFrameBuffer()
		{
			DetachRenderTarget();

			if (m_id != 0)
			{
				GLDEBUG(glDeleteFramebuffers(1, &m_id));
				LOG_TRACE("FrameBuffer : deleted framebuffer with id [{0}]", m_id);
			}

			m_id = 0;
		}


		void FrameBuffer::Validate() const
		{
			Bind();
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				LOG_ERROR("FrameBuffer : framebuffer validation failed");
			}
		}

		void FrameBuffer::DetachRenderTarget()
		{
			m_currentAttachment = AttachmentType::NONE;
			m_textureRef = nullptr;
			m_cubemapRef = nullptr;
		}

		void FrameBuffer::DetachExtraTarget(Attachment attachment)
		{
			GLDEBUG(glFramebufferTexture(GL_FRAMEBUFFER, OpenGLAttachment[(int)attachment], 0, 0));
		}

		bool FrameBuffer::IsTextureAttached() const
		{
			return m_currentAttachment == AttachmentType::TEXTURE;
		}

		bool FrameBuffer::IsCubeMapAttached() const
		{
			return m_currentAttachment == AttachmentType::CUBEMAP;
		}

		void FrameBuffer::UseDrawBuffers(std::vector<Attachment> attachments) const
		{
			std::vector<GLenum> attachmentTypes(20);

			size_t size = attachments.size();

			if (size >= attachmentTypes.size())
			{
				LOG_ERROR("FrameBuffer : More attachments than possible");
				return;
			}

			GLDEBUG(glDrawBuffers((GLsizei)size, attachmentTypes.data()));

		}

		void FrameBuffer::UseOnlyDepth() const
		{
			Bind();
			GLDEBUG(glDrawBuffer(GL_NONE));
		}

		size_t FrameBuffer::GetWidth() const
		{
			if (m_textureRef)
			{
				return m_textureRef->GetWidth();
			}
			else if (m_cubemapRef)
			{
				return m_cubemapRef->GetWidth();
			}

			return 0;
		}
		size_t FrameBuffer::GetHeight() const
		{
			if (m_textureRef)
			{
				return m_textureRef->GetHeight();
			}
			else if (m_cubemapRef)
			{
				return m_cubemapRef->GetHeight();
			}

			return 0;
		}

		void FrameBuffer::Bind() const
		{
			GLDEBUG(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
		}
		void FrameBuffer::UnBind() const
		{
			GLDEBUG(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

		FrameBuffer::BindableID FrameBuffer::GetHandle() const
		{
			return m_id;
		}

    }
}