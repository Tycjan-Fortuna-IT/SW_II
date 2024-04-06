#pragma once

#include "Asset.hpp"

namespace SW {

	class Texture2D;
	struct AssetSourceItem;

	struct ThumbnailCacheData
	{
		Texture2D* Texture;
		u64 LastModified;
	};

	class ThumbnailCache
	{
	public:
		ThumbnailCache() = default;
		~ThumbnailCache();

		using Timestamp = u64;
		Texture2D** GetThumbnail(const std::filesystem::path& itemPath, AssetHandle handle, Timestamp lastModified);

	private:
		std::unordered_map<AssetHandle, ThumbnailCacheData> m_Thumbnails;

	private:
		void DownscaleTexture(Texture2D* texture);

		Texture2D** LoadAndCacheTexture(const std::filesystem::path& itemPath, const std::filesystem::path& cachePath,
			AssetHandle handle, Timestamp lastModified);
	};

}
