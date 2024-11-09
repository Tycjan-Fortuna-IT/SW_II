#include "Texture2DAsset.hpp"

namespace SW
{
	Texture2DAsset::Texture2DAsset(const char* filepath, bool flipped /* = true*/)
	    : OpenGL::Texture2D(filepath, flipped)
	{
	}

	Texture2DAsset::Texture2DAsset(const std::filesystem::path& filepath, bool flipped /* = true*/)
	    : OpenGL::Texture2D(filepath, flipped)
	{
	}

	Texture2DAsset::Texture2DAsset(int width, int height) : OpenGL::Texture2D(width, height)
	{
	}

	Texture2DAsset::Texture2DAsset(const OpenGL::TextureSpecification& spec) : OpenGL::Texture2D(spec)
	{
	}

} // namespace SW
