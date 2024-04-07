/**
 * @file Sprite.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Asset/Asset.hpp"

namespace SW {

	class Texture2D;

	class Sprite final : public Asset
	{
	public:
		Sprite() = default;
		~Sprite() = default;

		static AssetType GetStaticType() { return AssetType::Sprite; }
		AssetType GetAssetType() const override { return AssetType::Sprite; }

		Texture2D* GetTexture() const { return *m_Texture; }
		Texture2D** GetTextureRaw() const { return m_Texture; }

		void SetTexture(Texture2D** texture) { m_Texture = texture; }

	public:
		glm::vec2 TexCordLeftDown = glm::vec2(0.f);
		glm::vec2 TexCordRightDown = glm::vec2(0.f);
		glm::vec2 TexCordUpRight = glm::vec2(0.f);
		glm::vec2 TexCordUpLeft = glm::vec2(0.f);

	private:
		Texture2D** m_Texture = nullptr;
	};

}
