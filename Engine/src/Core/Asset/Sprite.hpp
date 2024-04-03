#pragma once

#include "Core/Asset/Asset.hpp"

namespace SW {

	class Spritesheet;

	class Sprite final : public Asset
	{
	public:
		Sprite(const std::string& name);
		~Sprite() = default;

	public:
		std::string Name;
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);
		glm::vec4 Tint = glm::vec4(1.0f);

	private:
		Spritesheet** m_Spritesheet = nullptr;
	};

}
