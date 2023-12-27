#include "Shader.h"
#include "Engine/Utils/GLUtils.h"
#include "Engine/Utils/Logging/Log.h"


namespace SaltnPepperEngine
{
	namespace Rendering
	{
		uint32_t Shader::CurrentlyBindedShader = 0;


		struct ShaderSubInfo
		{
			Shader::ShaderType type;
			std::string source;
			FilePath path;
		};

		// getting native 
		inline static int GetGLShaderType(Shader::ShaderType type)
		{
			switch (type)
			{
			case Shader::VERTEX: return GL_VERTEX_SHADER;
				break;
			case Shader::GEOMETRY: return GL_GEOMETRY_SHADER;
				break;
			case Shader::FRAGMENT: return GL_FRAGMENT_SHADER;
				break;
			case Shader::UNKNOWN: return GL_FALSE;
				break;
			default: return GL_FALSE;
				break;
			}
		}


		// Shader  Function Implementaion

		void Shader::DeleteShader(uint32_t shaderId)
		{
			GLDEBUG(glDeleteShader(shaderId));
		}



		void Shader::DeleteProgram()
		{
			if (m_Id != 0)
			{
				//deleted shader program with id 
				glDeleteProgram(m_Id);
			}

			m_Id = 0;
		}

		void Shader::Bind()
		{
			GLDEBUG(glUseProgram(m_Id));
			CurrentlyBindedShader = m_Id;

		}

		void Shader::UnBind()
		{
			GLDEBUG(glUseProgram(0));
			CurrentlyBindedShader = 0;
		}

		Shader::Shader()
			: m_Id(0)
		{
		}

		Shader::~Shader()
		{
			DeleteProgram();
		}

		void Shader::SetProgramID(uint32_t newId)
		{
			DeleteProgram();

			m_Id = newId;

		}


		uint32_t Shader::CreateShaderProgram(const ShaderSubInfo* info, const size_t shaderCount)
		{
			// Creating a container to store the compiled sub shaders
			std::vector<uint32_t> subShaders;

			// Resizing the vector
			subShaders.resize(shaderCount);

			// Storing a new Id for the shader for each of the compiled shader types
			for (uint32_t iterator = 0; iterator < shaderCount; iterator++)
			{
				ShaderSubInfo subInfo = info[iterator];
				subShaders[iterator] = CreateShader(GetGLShaderType(subInfo.type), subInfo.source, subInfo.path);
			}

			// Creating the  Shader Program
			GLDEBUG(uint32_t program = glCreateProgram());

			// Attaching each of the sub shader to the program ID
			for (uint32_t iterator = 0; iterator < shaderCount; iterator++)
			{
				GLDEBUG(glAttachShader(program, subShaders[iterator]));
			}

			// Linking the program
			GLDEBUG(glLinkProgram(program));


			GLint success;
			GLDEBUG(glGetProgramiv(program, GL_LINK_STATUS, &success));

			if (success == 0)
			{
				GLchar ErrorLog[1024];
				GLDEBUG(glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog));
				LOG_ERROR("Error linking shader program : {0}", ErrorLog);

			}





			GLDEBUG(glValidateProgram(program));


			for (uint32_t iterator = 0; iterator < shaderCount; iterator++)
			{
				GLDEBUG(glDetachShader(program, subShaders[iterator]));
				DeleteShader(subShaders[iterator]);
			}

			return program;
		}

		// Just to make sure the correct Filepath is used

		uint32_t Shader::CreateShader(int glType, std::string& source, FilePath& path)
		{
			// Declaring the temp id for teh sahder
			uint32_t shaderId;
			GLDEBUG(shaderId = glCreateShader((GLenum)glType));

			// Unloading the source Data
			const char* processedShaderCodePtr = source.c_str();

			// Setting the Shader Source
			GLDEBUG(glShaderSource(shaderId, 1, &processedShaderCodePtr, nullptr));

			// Compiling the Shader
			GLDEBUG(glCompileShader(shaderId));

			// Checking fo Compile Errors
			GLint success;
			GLDEBUG(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));
			if (!success)
			{
				GLchar InfoLog[1024];
				GLDEBUG(glGetShaderInfoLog(shaderId, sizeof(InfoLog), NULL, InfoLog));
				LOG_ERROR("Error compiling shader type {0}: {1}'", glType, InfoLog);
			}


			return shaderId;
		}

		/*bool Shader::GetShaderError(uint32_t programId, uint32_t errorStage)
		{
			return false;
		}*/





		bool Shader::Load(const std::string& vertPath, const std::string& fragPath)
		{
			if (!FileSystem::Exists(vertPath))
			{
				std::string errorPath = vertPath;

				LOG_WARN("Vertex Shader not found : {0}", errorPath);

				return false;
			}

			if (!FileSystem::Exists(fragPath))
			{
				LOG_ERROR("Fragment Shader not found : {0}", fragPath);
				return false;
			}

			// Setting vertex and fragment data
			std::array<ShaderSubInfo, 2> subInfo =
			{
				ShaderSubInfo
				{	// Defining the type
					ShaderType::VERTEX,
					// Pulling the Shader vertex data into a continuos string
					FileSystem::ReadFileToString(vertPath),
					// Storing the path
					FileSystem::GetFilePathfromString(vertPath)
				},
				ShaderSubInfo
				{
					// Defining the typr
					ShaderType::FRAGMENT,
					// Pulling the Shader fragment data into a continuos string
					FileSystem::ReadFileToString(fragPath),
					// Storing the path
					FileSystem::GetFilePathfromString(fragPath)
				}
			};

			const size_t subShaderCount = subInfo.size();


			// Generating a program id from the given sub shader infos
			uint32_t glProgram = CreateShaderProgram(subInfo.data(), subShaderCount);


			SetProgramID(glProgram);

			return true;

		}

		uint32_t Shader::GetProgramId() const
		{
			return m_Id;
		}

		void Shader::Load(const std::string& vertPath, const std::string& fragPath, const std::string& geoPath)
		{

			// Implement the loading for vert frag and geomerty shader here
		}

		int Shader::GetUniformLocation(const std::string& uniformName)
		{
			typename UniformMap::iterator itr = m_uniformData.find(uniformName);

			if (itr != m_uniformData.end())
			{
				return itr->second;
			}

			uint32_t location = GLDEBUG(glGetUniformLocation(m_Id, uniformName.c_str()));

			if (location == -1)
			{
				LOG_ERROR("{0} : Uniform not found", uniformName);

			}

			m_uniformData[uniformName] = location;

			return location;
		}





		void Shader::SetUniform(const std::string& uniformName, float invalue)
		{

			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it

			GLDEBUG(glUniform1f(location, invalue));

		}

		void Shader::SetUniform(const std::string& uniformName, int invalue)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it

			GLDEBUG(glUniform1i(location, invalue));
		}

		void Shader::SetUniform(const std::string& uniformName, bool invalue)
		{
			SetUniform(uniformName, (int)invalue);
		}

		void Shader::SetUniform(const std::string& uniformName, Vector2 invector)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniform2f(location, invector[0], invector[1]));
		}

		void Shader::SetUniform(const std::string& uniformName, Vector3 invector)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniform3f(location, invector[0], invector[1], invector[2]));
		}

		void Shader::SetUniform(const std::string& uniformName, Vector4 invector)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniform4f(location, invector[0], invector[1], invector[2], invector[3]));
		}

		void Shader::SetUniform(const std::string& uniformName, Vector2Int invector)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniform2i(location, invector[0], invector[1]));

		}

		void Shader::SetUniform(const std::string& uniformName, Vector3Int invector)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniform3i(location, invector[0], invector[1], invector[2]));

		}

		void Shader::SetUniform(const std::string& uniformName, Vector4Int invector)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniform4i(location, invector[0], invector[1], invector[2], invector[3]));
		}


		void Shader::SetUniform(const std::string& uniformName, Matrix2 inmat)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniformMatrix2fv(location, 1, false, &inmat[0][0]));
		}

		void Shader::SetUniform(const std::string& uniformName, Matrix3 inmat)
		{
			//
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniformMatrix3fv(location, 1, false, &inmat[0][0]));
		}



		void Shader::SetUniform(const std::string& uniformName, Matrix4 inmat)
		{
			if (m_Id != Shader::CurrentlyBindedShader)
			{
				LOG_ERROR("Binded Shader {0} and this Shader {1} don't match", Shader::CurrentlyBindedShader, m_Id);
				return;
			}

			int location = GetUniformLocation(uniformName);

			if (location == -1)
			{
				// Invalid Uniform
				return;
			}

			// Found the Uniform, so update it
			GLDEBUG(glUniformMatrix4fv(location, 1, false, &inmat[0][0]));
		}



	}
}
