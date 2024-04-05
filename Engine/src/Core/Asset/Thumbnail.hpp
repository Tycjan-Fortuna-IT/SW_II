#pragma once

namespace SW {

	class Texture2D;

	struct Thumbnail
	{
		f32 Width = 64.f;
		f32 Height = 64.f;

		ImVec2 TexCoordMin = { 0, 1 };
		ImVec2 TexCoordMax = { 1, 0 };

		Texture2D** Texture = nullptr;

		operator bool() const { return Texture != nullptr && *Texture != nullptr; }
	};

}
