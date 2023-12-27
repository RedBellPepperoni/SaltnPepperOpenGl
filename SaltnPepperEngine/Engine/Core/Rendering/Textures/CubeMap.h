#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <string>
#include "Engine/Utils/Loading/FileSystem.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {

		class Image;


		class CubeMap
		{
		public:

			using CubeMapBindId = int;

		private:

			using BindableId = uint32_t;
			std::string m_filepath;
			size_t m_width = 0;
			size_t m_height = 0;
			size_t m_channels = 0;
			BindableId m_id = 0;

			mutable CubeMapBindId m_activeId = 0;

			void FreeCubeMap();


		public:

			CubeMap();
			~CubeMap();

			CubeMap(const FilePath& path);

			void Bind() const;
			void Bind(CubeMapBindId id) const;
			void UnBind() const;

			CubeMapBindId getBoundId() const;
			BindableId GetHandle() const;

			// Load from single file (try to Do using .exr or hdri images)
			void Load(const FilePath& filePath);
			void Load(const FilePath& right, const FilePath& left, const FilePath& top,
				const FilePath& bottom, const FilePath& front, const FilePath& back);

			void Load(const std::array<Image, 6>& images);

			void LoadDepth(int width, int height);

			size_t GetHeight() const;
			size_t GetWidth() const;
			size_t GetChannels() const;

			void SetMaxLOD(size_t lod) const;
			void SetMinLOD(size_t lod) const;
			void GenerateMipMaps();





		};
    }
}


#endif // !CUBEMAP_H