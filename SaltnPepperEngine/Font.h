#ifndef FONT_H
#define FONT_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <string>

#include <msdf-atlas-gen.h>

namespace SaltnPepperEngine
{
	namespace Rendering
	{

		struct MSDFData
		{
			msdf_atlas::FontGeometry fontGeometry;
			std::vector<msdf_atlas::GlyphGeometry> glyphs;
		};


		class Texture;

		class Font
		{

		public:

			Font(const std::string& filepath);
			Font(uint8_t* data, uint32_t dataSize, const std::string& name);

			~Font();

			SharedPtr<Texture> GetFontAtlas() const;
			const MSDFData* GetMSDFData() const;
			const std::string& GetFilePath() const;

			void Init();

			static void InitDefaultFont();
			static void ReleaseDefaultFont();
			static SharedPtr<Font> GetDefaultFont();

		private:

			std::string m_filePath;
			SharedPtr<Texture> m_textureAtlas;
			MSDFData* m_msdfData = nullptr;
			uint8_t* m_fontData;
			uint32_t* m_fontDataSize;

			static SharedPtr<Font> defaultFont;

		};


	}
}

#endif // !FONT_H

