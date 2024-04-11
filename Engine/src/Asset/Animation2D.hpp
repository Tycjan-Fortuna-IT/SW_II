/**
 * @file Animation.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset.hpp"
#include "Sprite.hpp"

namespace SW {

	class Animation2D final : public Asset
	{
	public:
		Animation2D() = default;
		~Animation2D() = default;

		static AssetType GetStaticType() { return AssetType::Animation2D; }
		AssetType GetAssetType() const override { return AssetType::Animation2D; }
	
		f32 Speed = 1.0f;

		std::vector<Sprite**> Sprites;

	private:

	};

}
