#include "ThumbnailCache.hpp"

#include "Core/OpenGL/Texture2D.hpp"
#include "Core/Asset/AssetSourceItem.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "Core/Utils/SerializationUtils.hpp"
#include "../Font.hpp"
#include "../AssetManager.hpp"

namespace SW {

	ThumbnailCache::~ThumbnailCache()
	{
		for (std::pair<AssetHandle, ThumbnailCacheData> pair : m_Thumbnails) {
			delete pair.second.Texture;
		}
	}

	Texture2D** ThumbnailCache::GetTextureThumbnail(const std::filesystem::path& itemPath, AssetHandle handle, Timestamp lastModified)
	{
		auto it = m_Thumbnails.find(handle);

		/*
		  Process:
			- Check if the thumbnail was already loaded into the cache.
				- If yes:
					- Check if the last modified timestamp has changed since the thumbnail was loaded.
						- If the timestamp has changed, it means the original asset has been modified, so we need to reload the thumbnail.
						- If the timestamp has not changed, we can return the cached thumbnail as it is still up-to-date.
				- If no:
					- The thumbnail has not been loaded into the cache yet. We need to load the thumbnail from the asset source,
					  resize it to the appropriate dimensions for a thumbnail, and then save it into the cache.
					- We also need to save the last modified timestamp of the original asset with the thumbnail in the cache, so
					  we can check if the thumbnail is up-to-date the next time it is requested.
		*/

		const std::filesystem::path cachePath = ProjectContext::Get()->GetAssetDirectory() / "cache" / "thumbnails";

		if (it != m_Thumbnails.end()) {
			ThumbnailCacheData& data = it->second;

			if (data.LastModified == lastModified)
				return &data.Texture;

			const std::string oldFilename = std::to_string(handle) + "_" + std::to_string(data.LastModified) + ".cache";

			std::filesystem::remove(cachePath / oldFilename);

			return LoadAndCacheTexture(itemPath, cachePath, handle, lastModified);
		}

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(cachePath)) {
			const std::filesystem::path& filepath = entry.path();
			const std::string& filename = filepath.filename().string();

			const std::string handleStr = filename.substr(0, filename.find("_"));
			const std::string modTimeStr = filename.substr(filename.find("_") + 1, filename.find(".cache") - filename.find("_") - 1);

			const u64 itemHandle = std::stoull(handleStr);
			const u64 modTime = std::stoull(modTimeStr);

			if (itemHandle == handle) {

				if (modTime == lastModified) {
					std::ifstream file(filepath, std::ios::binary);

					ASSERT(file.is_open());

					i32 width, height, channels;
					file.read(reinterpret_cast<char*>(&width), sizeof(i32));
					file.read(reinterpret_cast<char*>(&height), sizeof(i32));
					file.read(reinterpret_cast<char*>(&channels), sizeof(i32));

					char* data = new char[width * height * channels];
					file.read(data, width * height * channels);
					file.close();

					TextureSpecification spec;
					spec.Height = height;
					spec.Width = width;
					spec.Format = channels == 3 ? ImageFormat::RGB8 : ImageFormat::RGBA8;

					Texture2D* texture = new Texture2D(spec);
					texture->SetData(data, width * height * channels);
					delete[] data;

					ThumbnailCacheData cacheData = ThumbnailCacheData{ texture, lastModified };
					m_Thumbnails[handle] = cacheData;

					return &m_Thumbnails[handle].Texture;
				} else {
					std::filesystem::remove(filepath);

					return LoadAndCacheTexture(itemPath, cachePath, handle, lastModified);
				}

			}
		}

		return LoadAndCacheTexture(itemPath, cachePath, handle, lastModified);
	}

	Texture2D** ThumbnailCache::GetFontAtlasThumbnail(const std::filesystem::path& itemPath, AssetHandle handle, Timestamp lastModified)
	{
		auto it = m_Thumbnails.find(handle);

		const std::filesystem::path cachePath = ProjectContext::Get()->GetAssetDirectory() / "cache" / "thumbnails";

		if (it != m_Thumbnails.end()) {
			ThumbnailCacheData& data = it->second;

			if (data.LastModified == lastModified)
				return &data.Texture;

			const std::string oldFilename = std::to_string(handle) + "_" + std::to_string(data.LastModified) + ".cache";

			std::filesystem::remove(cachePath / oldFilename);

			return LoadAndCacheFontAtlas(itemPath, cachePath, handle, lastModified);
		}

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(cachePath)) {
			const std::filesystem::path& filepath = entry.path();
			const std::string& filename = filepath.filename().string();

			const std::string handleStr = filename.substr(0, filename.find("_"));
			const std::string modTimeStr = filename.substr(filename.find("_") + 1, filename.find(".cache") - filename.find("_") - 1);

			const u64 itemHandle = std::stoull(handleStr);
			const u64 modTime = std::stoull(modTimeStr);

			if (itemHandle == handle) {

				if (modTime == lastModified) {
					std::ifstream file(filepath, std::ios::binary);

					ASSERT(file.is_open());

					i32 width, height, channels;
					file.read(reinterpret_cast<char*>(&width), sizeof(i32));
					file.read(reinterpret_cast<char*>(&height), sizeof(i32));
					file.read(reinterpret_cast<char*>(&channels), sizeof(i32));

					char* data = new char[width * height * channels];
					file.read(data, width * height * channels);
					file.close();

					TextureSpecification spec;
					spec.Height = height;
					spec.Width = width;
					spec.Format = channels == 3 ? ImageFormat::RGB8 : ImageFormat::RGBA8;

					Texture2D* texture = new Texture2D(spec);
					texture->SetData(data, width * height * channels);
					delete[] data;

					ThumbnailCacheData cacheData = ThumbnailCacheData{ texture, lastModified };
					m_Thumbnails[handle] = cacheData;

					return &m_Thumbnails[handle].Texture;
				} else {
					std::filesystem::remove(filepath);

					return LoadAndCacheFontAtlas(itemPath, cachePath, handle, lastModified);
				}

			}
		}

		return LoadAndCacheFontAtlas(itemPath, cachePath, handle, lastModified);
	}

    void ThumbnailCache::DownscaleTexture(Texture2D* texture)
	{
		i32 originalWidth = texture->GetWidth();
		i32 originalHeight = texture->GetHeight();
		i32 newWidth = originalWidth;
		i32 newHeight = originalHeight;
		i32 clampedSize = std::min(256, std::max(originalWidth, originalHeight));

		if (originalWidth > originalHeight) {
			newWidth = clampedSize;
			newHeight = (i32)(clampedSize * (f32)originalHeight / (f32)originalWidth);
		}
		else {
			newHeight = clampedSize;
			newWidth = (i32)(clampedSize * (f32)originalWidth / (f32)originalHeight);
		}

		texture->ChangeSize(newWidth, newHeight);
	}

	Texture2D** ThumbnailCache::LoadAndCacheTexture(
		const std::filesystem::path& itemPath, const std::filesystem::path& cachePath, AssetHandle handle, Timestamp lastModified
	) {
		const std::string newFilename = std::to_string(handle) + "_" + std::to_string(lastModified) + ".cache";

		Texture2D* loaded = new Texture2D(ProjectContext::Get()->GetAssetDirectory() / itemPath);
		DownscaleTexture(loaded);

		std::ofstream file(cachePath / newFilename, std::ios::binary | std::ios::trunc);

		ASSERT(file.is_open());

		i32 width = loaded->GetWidth();
		i32 height = loaded->GetHeight();
		i32 channels = loaded->GetChannels();
		const char* bytes = loaded->GetBytes();

		file.write(reinterpret_cast<const char*>(&width), sizeof(i32));
		file.write(reinterpret_cast<const char*>(&height), sizeof(i32));
		file.write(reinterpret_cast<const char*>(&channels), sizeof(i32));
		file.write(bytes, width * height * channels);
		file.close();

		m_Thumbnails[handle] = ThumbnailCacheData{ loaded, lastModified };

		return &m_Thumbnails[handle].Texture;
	}

	Texture2D** ThumbnailCache::LoadAndCacheFontAtlas(const std::filesystem::path& itemPath, const std::filesystem::path& cachePath, AssetHandle handle, Timestamp lastModified)
	{
		const std::string newFilename = std::to_string(handle) + "_" + std::to_string(lastModified) + ".cache";

		YAML::Node fontData = YAML::LoadFile((ProjectContext::Get()->GetAssetDirectory() / itemPath).string());

		YAML::Node data = fontData["Font"];

		ASSERT(data);

		const u64 fontSourceHandle = data["FontSourceHandle"].as<u64>();

		const AssetMetaData& sourceMetadata = AssetManager::GetAssetMetaData(fontSourceHandle);

		FontSpecification spec;
		spec.Path = ProjectContext::Get()->GetAssetDirectory() / sourceMetadata.Path;
		spec.Charset = (FontCharsetType)data["CharsetType"].as<int>();;
		spec.ApplyMSDFColoring = false;
		spec.ForceHeight = 512;
		spec.ForceWidth = 512;

		Font* font = new Font(spec);
		Texture2D* atlas = font->GetAtlasTexture();

		std::ofstream file(cachePath / newFilename, std::ios::binary | std::ios::trunc);

		ASSERT(file.is_open());

		i32 width = atlas->GetWidth();
		i32 height = atlas->GetHeight();
		i32 channels = atlas->GetChannels();
		const char* bytes = atlas->GetBytes();

		file.write(reinterpret_cast<const char*>(&width), sizeof(i32));
		file.write(reinterpret_cast<const char*>(&height), sizeof(i32));
		file.write(reinterpret_cast<const char*>(&channels), sizeof(i32));
		file.write(bytes, width * height * channels);
		file.close();

		// Copy the font atlas texture since we delete font afterwards (maybe clean this later on)
		TextureSpecification texSpec;
		texSpec.Height = height;
		texSpec.Width = width;
		texSpec.Format = channels == 3 ? ImageFormat::RGB8 : ImageFormat::RGBA8;

		Texture2D* texture = new Texture2D(texSpec);
		texture->SetData((void*)bytes, width * height * channels);

		delete font;

		m_Thumbnails[handle] = ThumbnailCacheData{ texture, lastModified };

		return &m_Thumbnails[handle].Texture;
	}

}
