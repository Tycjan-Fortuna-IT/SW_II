#pragma once

#include "Core/Asset/Asset.hpp"

namespace SW {

	class Spritesheet;

	class Sprite final : public Asset
	{
	public:
		Sprite() = default;
		~Sprite() = default;

	private:
		Spritesheet** m_Spritesheet = nullptr;
	};

}
