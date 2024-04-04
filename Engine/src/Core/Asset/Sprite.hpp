#pragma once

#include "Core/Asset/Asset.hpp"
#include "GUI/Editor/EditorResources.hpp"

namespace SW {

	class Texture2D;

	class Sprite final : public Asset
	{
	public:
		Sprite() = default;
		~Sprite() = default;

		static AssetType GetStaticType() { return AssetType::Sprite; }
		AssetType GetAssetType() const override { return AssetType::Sprite; }

		Texture2D* GetTexture() const { return m_Texture ? *m_Texture : EditorResources::MissingAssetIcon; }
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
