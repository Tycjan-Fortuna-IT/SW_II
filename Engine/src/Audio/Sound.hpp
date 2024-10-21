#pragma once

#include "Asset/Asset.hpp"
#include "Audio/AudioEngine.hpp"

namespace SW
{

	class Sound : public Asset
	{
	public:
		Sound(const std::filesystem::path& path);
		~Sound();

		/**
		 * @brief Gets the handle of the sound.
		 */
		const ma_sound* GetAudioHandle() const { return &m_Handle; }

		/**
		 * @brief Gets the static type of the asset.
		 */
		static AssetType GetStaticType() { return AssetType::Audio; }

		/**
		 * @brief Gets the type of the asset.
		 */
		AssetType GetAssetType() const override { return AssetType::Audio; }

	private:
		ma_sound m_Handle;
	};

} // namespace SW
