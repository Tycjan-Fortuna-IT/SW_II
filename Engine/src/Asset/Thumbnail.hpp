/**
 * @file Thumbnail.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	class Texture2D;

	struct Thumbnail
	{
		f32 Width  = 64.f;
		f32 Height = 64.f;

		ImVec2 TexCoordMin = {0, 1};
		ImVec2 TexCoordMax = {1, 0};

		Texture2D** Texture = nullptr;

		// For Animation2D thumbnail
		// TODO: In the future cleanup the thumbnail system, split it into many derived classes like:
		// Animation2DThumbnail, SpritesheetThumbnail ... etc
		// Each with own OnRender methods etc...
		int CurrentFrame = 0;

		/**
		 * @brief Thumbnail is valid only if the texture is not nullptr.
		 */
		operator bool() const { return Texture != nullptr && *Texture != nullptr; }
	};

} // namespace SW
