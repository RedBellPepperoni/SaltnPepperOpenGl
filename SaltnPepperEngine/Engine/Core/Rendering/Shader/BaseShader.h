#ifndef  BASESHADER_H
#define  BASESHADER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Loading/FileSystem.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class BaseShader
		{
		public:

			using UniformIdType = int;
			using ShaderId = unsigned int;
			using BindableId = unsigned int;
			using ShaderTypeEnum = int;

			class UniformMap
			{
			private:
				std::unordered_map<std::string, UniformIdType> m_cache;
				BindableId m_shaderId;

			public:
				constexpr static UniformIdType InvalidLocation = -1;

				UniformMap(BindableId shaderId);

				UniformIdType GetUniformLocation(const std::string& uniformName, bool debug = false);
			};


		private:

			static BindableId CurrentlyAttachedShader;

			BindableId m_id = 0;
			mutable UniformMap m_uniformMap;

			void FreeProgram();

		protected:

			static BindableId CreateProgram(const ShaderId* ids, size_t shaderCount);
			template<typename FilePath> static ShaderId CreateShader(ShaderTypeEnum type, const std::string& sourceCode, const FilePath& filepath);
			template<typename FilePath> static std::vector<std::string> GetShaderIncludeFiles(const std::string& sourceCode, const FilePath& filepath);
			static void DeleteShader(ShaderId id);

			void SetNewNativeHandle(BindableId id);

		public:


			static std::string GetShaderVersionString();

			BaseShader();
			~BaseShader();
			BaseShader(const BaseShader&) = delete;
			BaseShader(BaseShader&& shader) noexcept;
			BaseShader& operator=(const BaseShader&) = delete;
			BaseShader& operator=(BaseShader&& shader) noexcept;

			void Bind() const;
			void Unbind() const;
			BindableId GetNativeHandle() const;

			void InvalidateUniformCache();
			void IgnoreNonExistingUniform(const std::string& name) const;
			void IgnoreNonExistingUniform(const char* name) const;
			UniformIdType GetUniformLocation(const std::string& name) const;
			
			void SetUniform(const std::string& name, float f) const;
			void SetUniform(const std::string& name, const Vector2& v) const;
			void SetUniform(const std::string& name, const Vector3& v) const;
			void SetUniform(const std::string& name, const Vector4& v) const;
			void SetUniform(const std::string& name, const Vector2Int& v) const; 
			void SetUniform(const std::string& name, const Vector3Int& v) const; 
			void SetUniform(const std::string& name, const Vector4Int& v) const; 
			void SetUniform(const std::string& name, const Matrix2& m) const; 
			void SetUniform(const std::string& name, const Matrix3& m) const;
			void SetUniform(const std::string& name, const Matrix4& m) const; 
			void SetUniform(const std::string& name, int i) const;
			void SetUniform(const std::string& name, bool b) const;


		};



	}
}

#endif // ! BASESHADER_H

