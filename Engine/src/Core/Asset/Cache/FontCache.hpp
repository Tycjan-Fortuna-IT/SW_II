#pragma once

namespace SW {

	class Texture2D;

	class FontCache
	{
	public:
		using Timestamp = u64;
		using AssetHandle = u64;

		// if exists returns pointer to atlas texture, caller must manage it's lifetime
		static Texture2D* TryGetCachedAtlas(AssetHandle handle, Timestamp lastModified);

		static void CacheAtlas(const Texture2D* atlas, AssetHandle handle, Timestamp lastModified);
	};

}
