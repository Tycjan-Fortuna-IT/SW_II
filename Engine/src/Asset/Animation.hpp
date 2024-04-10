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

namespace SW {

	class Animation final : public Asset
	{
		Animation() = default;
		~Animation() = default;

		static AssetType GetStaticType() { return AssetType::Animation; }
		AssetType GetAssetType() const override { return AssetType::Animation; }
	};

}
