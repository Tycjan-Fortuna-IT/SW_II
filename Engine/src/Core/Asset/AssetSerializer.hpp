#pragma once

namespace SW {

	class Asset;

	class AssetSerializer
	{
		virtual void Serialize(const Asset* asset) = 0;
	
		virtual Asset* TryLoadAsset(const std::filesystem::path& path) = 0;
	};

	class SpritesheetSerializer final : public AssetSerializer
	{
		void Serialize(const Asset* asset) override;

		Asset* TryLoadAsset(const std::filesystem::path& path) override;
	};

}
