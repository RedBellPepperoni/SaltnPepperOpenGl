#include "Font.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/System/Application/Application.h"

using namespace msdf_atlas;

namespace SaltnPepperEngine
{
	namespace Rendering
	{

		struct FontInput
		{
			const char* fontFilename;
			GlyphIdentifierType glyphIdentifierType;
			const char* charsetFilename;
			double fontScale;
			const char* fontName;
			uint32_t* data;
			uint32_t dataSize;
		};

		struct Configuration
		{
			

			ImageType imageType;
			msdf_atlas::ImageFormat imageFormat;
			YDirection yDirection;
			int width, height;
			double emSize;
			double pxRange;
			double angleThreshold;
			double miterLimit;
			void (*edgeColoring)(msdfgen::Shape&, double, unsigned long long);
			bool expensiveColoring;
			unsigned long long coloringSeed;
			GeneratorAttributes generatorAttributes;
		};

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define DEFAULT_MITER_LIMIT 1.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREADS 8

		Font::Font(const std::string& filepath)
		{

		}
		Font::Font(uint8_t* data, uint32_t dataSize, const std::string& name)
		{
		}
		Font::~Font()
		{
		}
		SharedPtr<Texture> Font::GetFontAtlas() const
		{
			return SharedPtr<Texture>();
		}
		const MSDFData* Font::GetMSDFData() const
		{
			return nullptr;
		}
		const std::string& Font::GetFilePath() const
		{
			return m_filePath;
		}
		void Font::Init()
		{
		}
		void Font::InitDefaultFont()
		{
		}
		void Font::ReleaseDefaultFont()
		{
		}
		SharedPtr<Font> Font::GetDefaultFont()
		{
			return SharedPtr<Font>();
		}
	}
}