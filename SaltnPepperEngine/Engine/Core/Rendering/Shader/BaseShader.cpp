#include "BaseShader.h"
#include "Engine/Utils/GLUtils.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Core/Rendering/Shader/ShaderPreprocessor.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		BaseShader::BindableId BaseShader::CurrentlyAttachedShader = 0;

		// Checks if there are any errors during preprocessing
		bool CheckShaderErrors(BaseShader::BindableId shaderId, GLenum stage)
		{
			GLint result;
			GLDEBUG(glGetShaderiv(shaderId, stage, &result));
			if (result == GL_FALSE)
			{
				GLint length;
				GLDEBUG(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));
				std::string msg;
				msg.resize(length);
				GLDEBUG(glGetShaderInfoLog(shaderId, length, &length, &msg[0]));
				if (!msg.empty())
				{
					msg.pop_back(); // extra \n character
					LOG_ERROR("ShaderError : {0}", msg);
				}
				return true;
			}
			return false;
		}

		// returns true if any errors occured
		bool CheckProgramErrors(BaseShader::BindableId programId, GLenum stage)
		{
			GLint result;
			GLDEBUG(glGetProgramiv(programId, stage, &result));
			if (result == GL_FALSE)
			{
				GLint length;
				GLDEBUG(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length));
				std::string msg;
				msg.resize(length);
				GLDEBUG(glGetProgramInfoLog(programId, length, &length, &msg[0]));
				if (!msg.empty())
				{
					msg.pop_back(); // extra \n character
					LOG_ERROR("OpenGL::ErrorHandler : {0}", msg);
				}
				return true;
			}
			return false;
		}



		BaseShader::UniformMap::UniformMap(BindableId shaderId)
			: m_shaderId(shaderId)
		{
		}

		BaseShader::UniformIdType BaseShader::UniformMap::GetUniformLocation(const std::string& uniformName, bool debug)
		{
			auto itr = m_cache.find(uniformName);

			if (itr != m_cache.end())
			{
				return itr->second;
			}

			uint32_t location = GLDEBUG(glGetUniformLocation(m_shaderId, uniformName.c_str()));

			if (location == UniformMap::InvalidLocation && debug)
			{
				LOG_ERROR("{0} : Uniform not found", uniformName);

			}

			m_cache[uniformName] = location;

			return location;
		}

		void BaseShader::FreeProgram()
		{
			if (m_id != 0)
			{
				LOG_TRACE("Shader : deleted shader program with id : {0}", m_id);
				GLDEBUG(glDeleteProgram(m_id));
			}
			m_id = 0;
		}

		BaseShader::BindableId BaseShader::CreateProgram(const ShaderId* ids, size_t shaderCount)
		{
			GLDEBUG(BindableId program = glCreateProgram());

			for (size_t i = 0; i < shaderCount; i++)
			{
				GLDEBUG(glAttachShader(program, ids[i]));
			}

			GLDEBUG(glLinkProgram(program));
			if (CheckProgramErrors(program, GL_LINK_STATUS))
			{
				LOG_WARN("Shader : failed to link shader program with id = {0}" ,std::to_string(program));
			}

			GLDEBUG(glValidateProgram(program));
			if (CheckProgramErrors(program, GL_VALIDATE_STATUS))
			{
				LOG_WARN("Shader : failed to validate shader program with id {0} ",std::to_string(program));
			}

			for (size_t i = 0; i < shaderCount; i++)
			{
				GLDEBUG(glDetachShader(program, ids[i]));
			}

			return program;
		}

		template<>
		std::vector<std::string> BaseShader::GetShaderIncludeFiles<FilePath>(const std::string& sourceCode, const FilePath& path)
		{
			ShaderPreprocessor preprocessor(sourceCode);
			return preprocessor
				.LoadIncludes(path.parent_path())
				.GetIncludeFiles();
		}

		template<>
		BaseShader::ShaderId BaseShader::CreateShader<FilePath>(ShaderTypeEnum type, const std::string& sourceCode, const FilePath& path)
		{
			GLDEBUG(ShaderId shaderId = glCreateShader((GLenum)type));

			ShaderPreprocessor preprocessor(sourceCode);

			auto modifiedSourceCode = preprocessor
				.LoadIncludes(path.parent_path())
				.EmitPrefixLine(BaseShader::GetShaderVersionString())
				.GetResult();

			auto sourceptr = modifiedSourceCode.c_str();
			GLDEBUG(glShaderSource(shaderId, 1, &sourceptr, nullptr));
			GLDEBUG(glCompileShader(shaderId));
			if (CheckShaderErrors(shaderId, GL_COMPILE_STATUS))
			{
				LOG_WARN("Shader : failed to compile shader stage: " + (path).u8string());
			}

			return shaderId;
		}


		void BaseShader::DeleteShader(ShaderId id)
		{
			GLDEBUG(glDeleteShader(id));
		}

		void BaseShader::SetNewNativeHandle(BindableId id)
		{
			FreeProgram();
			m_id = id;
			m_uniformMap = UniformMap{ m_id };
		}

		std::string BaseShader::GetShaderVersionString()
		{
			// Just incase 
			return "#version 450";
		}

		BaseShader::BaseShader()
			: m_id(0)
			, m_uniformMap(m_id)
		{

		}

		BaseShader::~BaseShader()
		{
			FreeProgram();
		}

		BaseShader::BaseShader(BaseShader&& shader) noexcept
			: m_id(shader.m_id), m_uniformMap(std::move(shader.m_uniformMap))
		{
			shader.m_id = 0;
		}

		BaseShader& BaseShader::operator=(BaseShader&& shader) noexcept
		{
			FreeProgram();

			m_id = shader.m_id;
			m_uniformMap = std::move(shader.m_uniformMap);

			shader.m_id = 0;

			return *this;

		}

		void BaseShader::Bind() const
		{
			GLDEBUG(glUseProgram(m_id));
			BaseShader::CurrentlyAttachedShader = m_id;
		}

		void BaseShader::Unbind() const
		{
			GLDEBUG(glUseProgram(0));
			BaseShader::CurrentlyAttachedShader = 0;
		}

		BaseShader::BindableId BaseShader::GetNativeHandle() const
		{
			return m_id;
		}

		void BaseShader::InvalidateUniformCache()
		{
			m_uniformMap = UniformMap{ m_id };
		}

		void BaseShader::IgnoreNonExistingUniform(const std::string& name) const
		{
			GetUniformLocation(name.c_str());
		}
		void BaseShader::IgnoreNonExistingUniform(const char* name) const
		{
			(void)m_uniformMap.GetUniformLocation(name, true);
		}
		BaseShader::UniformIdType BaseShader::GetUniformLocation(const std::string& name) const
		{
			return m_uniformMap.GetUniformLocation(name);;
		}
	

		void BaseShader::SetUniform(const std::string& name, float f) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform1f(location, f));
		}

		void BaseShader::SetUniform(const std::string& name, const Vector2& v) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform2f(location, v[0], v[1]));
		}

		void BaseShader::SetUniform(const std::string& name, const Vector3& v) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform3f(location, v[0], v[1], v[2]));
		}

		void BaseShader::SetUniform(const std::string& name, const Vector4& v) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform4f(location, v[0], v[1], v[2], v[3]));
		
		}

		void BaseShader::SetUniform(const std::string& name, const Vector2Int& v) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform2i(location, v[0], v[1]));
		}

		void BaseShader::SetUniform(const std::string& name, const Vector3Int& v) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform3i(location, v[0], v[1], v[2]));
		}

		void BaseShader::SetUniform(const std::string& name, const Vector4Int& v) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform4i(location, v[0], v[1], v[2], v[3]));
		}

		void BaseShader::SetUniform(const std::string& name, const Matrix2& m) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniformMatrix2fv(location, 1, false, &m[0][0]));
		}

		void BaseShader::SetUniform(const std::string& name, const Matrix3& m) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniformMatrix3fv(location, 1, false, &m[0][0]));
		}

		void BaseShader::SetUniform(const std::string& name, const Matrix4& m) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniformMatrix4fv(location, 1, false, &m[0][0]));
		}

		void BaseShader::SetUniform(const std::string& name, int i) const
		{
			if (m_id != BaseShader::CurrentlyAttachedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", BaseShader::CurrentlyAttachedShader, m_id);
			}

			UniformIdType location = this->GetUniformLocation(name);
			if (location == BaseShader::UniformMap::InvalidLocation)
				return;

			GLDEBUG(glUniform1i(location, i));
		}

		void BaseShader::SetUniform(const std::string& name, bool b) const
		{
			SetUniform(name, (int)b);
		}
	}
}