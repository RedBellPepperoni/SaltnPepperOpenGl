#include "GLUtils.h"

#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/Loading/FileSystem.h"

namespace SaltnPepperEngine
{
	GLenum glCheckError_(const char* file, int line)
	{
		// Taken from https://learnopengl.com/In-Practice/Debugging

		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;

			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}


			std::string simplePath = FileSystem::GetFileNameWithExt(file);


			LOG_ERROR("GL ERROR {0} : | {1} (Line {2})", error, simplePath, line);


		}


		return errorCode;
	}
}
