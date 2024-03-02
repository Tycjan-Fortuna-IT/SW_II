#include "Font.hpp"

#include "Core/OpenGL/Texture2D.hpp"

namespace SW {

	Font::Font(const char* path)
		: m_Data(new MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();

		ASSERT(ft, "MSDFGEN failed to initialize");

		msdfgen::FontHandle* font = msdfgen::loadFont(ft, path);

		std::string filePath = path;
		std::string error = "Failed to load the font: ";
		ASSERT(font, (filePath + error).c_str());
	
		msdf_atlas::Charset charset;
		for (i32 i = 0x0020; i <= 0x00FF; i++) // range [0x0020, 0x00FF]
			charset.add(i);

		m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
		m_Data->FontGeometry.loadCharset(font, 1.0, charset);

		const f64 maxCornerAngle = 3.0; 
		for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs) // Apply MSDF edge coloring.
			glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

		msdf_atlas::TightAtlasPacker packer;
		packer.setPixelRange(2.0);
		packer.setMiterLimit(1.0);
		packer.setPadding(0);
		packer.setScale(40.f);
		packer.pack(m_Data->Glyphs.data(), (i32)m_Data->Glyphs.size());

		i32 width = 0, height = 0;
		packer.getDimensions(width, height);

		msdf_atlas::ImmediateAtlasGenerator<
			f32, // pixel type of buffer for individual glyphs depends on generator function
			3, // number of atlas color channels
			msdf_atlas::msdfGenerator, // function to generate bitmaps for individual glyphs
			msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3> // class that stores the atlas bitmap
			// For example, a custom atlas storage class that stores it in VRAM can be used.
		> generator(width, height);

		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		generator.setAttributes(attributes);
		generator.setThreadCount(4);
		generator.generate(m_Data->Glyphs.data(), (i32)m_Data->Glyphs.size());

		msdfgen::BitmapConstRef<u8, 3> bitmap = (msdfgen::BitmapConstRef<u8, 3>)generator.atlasStorage();

		TextureSpecification spec;
		spec.Width = bitmap.width;
		spec.Height = bitmap.height;
		spec.Format = ImageFormat::RGB8;

		m_AtlasTexture = new Texture2D(spec);
		m_AtlasTexture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);

		SW_INFO("Font `{}` created successfully!", path);
	}

	Font::~Font()
	{
		delete m_Data;
		delete m_AtlasTexture;
	}

}
