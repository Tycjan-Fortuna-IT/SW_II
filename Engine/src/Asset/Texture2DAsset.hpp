/**
 * @file Texture2DAsset.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-09-11
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Asset.hpp"
#include "OpenGL/Rendering/Texture2D.hpp"

namespace SW
{

	class Texture2DAsset : public Asset, public OpenGL::Texture2D
	{
	public:
		Texture2DAsset(const char* filepath, bool flipped = true);
		Texture2DAsset(const std::filesystem::path& filepath, bool flipped = true);
		Texture2DAsset(int width, int height);
		Texture2DAsset(const OpenGL::TextureSpecification& spec);

		static AssetType GetStaticType() { return AssetType::Texture2D; }
		AssetType GetAssetType() const override { return AssetType::Texture2D; }

		bool operator==(const Texture2DAsset& other) const
		{
			return Asset::GetHandle() == ((Texture2DAsset&)other).Asset::GetHandle();
		}

	private:
	};

} // namespace SW
