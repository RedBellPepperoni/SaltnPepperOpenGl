//#ifndef SHADER_H
//#define SHADER_H
//
//#include <vector>
//#include <string>
//#include <unordered_map>
//#include "Engine/Utils/Maths/MathDefinitions.h"
//#include "Engine/Utils/Loading/FileSystem.h"
//
//namespace SaltnPepperEngine
//{
//	namespace Rendering
//	{
//		struct ShaderSubInfo;
//
//
//		class Shader
//		{
//
//		public:
//
//			static uint32_t CurrentlyBindedShader;
//
//			// The type of Shader to load
//			enum ShaderType
//			{
//				VERTEX,				// 1 
//				GEOMETRY,			// 2
//				FRAGMENT,			// 3
//
//				UNKNOWN // This also defines the total number of Shadertypes
//			};
//
//
//			using UniformMap = std::unordered_map<std::string, uint32_t>;
//
//
//
//		private:
//
//			// The bound ID for the shader
//			uint32_t m_Id;
//
//
//			// Storage to contain all the uniform needed for a particular shader
//			UniformMap m_uniformData;
//
//
//			void DeleteProgram();
//			void DeleteShader(uint32_t shaderId);
//			void SetProgramID(uint32_t newId);
//
//
//
//			//bool CheckShaderError(uint32_t programId, uint32_t errorStage);
//
//			uint32_t CreateShader(int glType, std::string& source, FilePath& path);
//
//			//bool CompileShaderFromSource(ShaderSubInfo& info);
//
//		public:
//
//			Shader();
//			~Shader();
//
//
//
//
//			// Loads all the shader types for a particular shaderprogram
//			uint32_t CreateShaderProgram(const ShaderSubInfo* shaderTypes, const size_t shaderCount);
//
//			uint32_t GetProgramId() const;
//
//			void Bind();
//			void UnBind();
//
//
//			// ===================== SHADER STUFF =================================
//
//			bool Load(const std::string& vertPath, const std::string& fragPath);
//			void Load(const std::string& vertPath, const std::string& geoPath, const std::string& fragPath);
//
//
//			// Get the GL location from the uniform name
//			int GetUniformLocation(const std::string& uniformName);
//
//
//			void SetUniform(const std::string& uniformName, float invalue);
//			void SetUniform(const std::string& uniformName, int invalue);
//			void SetUniform(const std::string& uniformName, bool invalue);
//			void SetUniform(const std::string& uniformName, Vector2 invector);
//			void SetUniform(const std::string& uniformName, Vector3 invector);
//			void SetUniform(const std::string& uniformName, Vector4 invector);
//			void SetUniform(const std::string& uniformName, Vector2Int invector);
//			void SetUniform(const std::string& uniformName, Vector3Int invector);
//			void SetUniform(const std::string& uniformName, Vector4Int invector);
//			void SetUniform(const std::string& uniformName, Matrix2 inmat);
//			void SetUniform(const std::string& uniformName, Matrix3 inmat);
//			void SetUniform(const std::string& uniformName, Matrix4 inmat);
//
//
//
//		};
//	}
//
//}
//
//#endif // !SHADER_H
