#include "FontAsset.hpp"

namespace SW
{

	FontAsset::FontAsset(const FontAssetSpecification& spec)
	    : OpenGL::Font(OpenGL::FontSpecification{
	          .Path              = spec.Path,
	          .Charset           = spec.Charset,
	          .PreloadedAtlas    = spec.PreloadedAtlas,
	          .ApplyMSDFColoring = spec.ApplyMSDFColoring,
	          .ForceWidth        = spec.ForceWidth,
	          .ForceHeight       = spec.ForceHeight,
	      })
	{
	}

} // namespace SW
