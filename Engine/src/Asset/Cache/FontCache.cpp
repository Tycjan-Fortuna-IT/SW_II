#include "FontCache.hpp"

#include "Core/OpenGL/Texture2D.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW
{

	Texture2D* FontCache::TryGetCachedAtlas(AssetHandle handle, Timestamp lastModified)
	{
		const std::filesystem::path cachePath = ProjectContext::Get()->GetAssetDirectory() / "cache" / "fonts";

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(cachePath))
		{
			const std::filesystem::path& filepath = entry.path();
			const std::string& filename           = filepath.filename().string();

			const std::string handleStr = filename.substr(0, filename.find("_"));
			const std::string modTimeStr =
			    filename.substr(filename.find("_") + 1, filename.find(".cache") - filename.find("_") - 1);

			const u64 itemHandle = std::stoull(handleStr);
			const u64 modTime    = std::stoull(modTimeStr);

			if (itemHandle == handle)
			{

				if (modTime == lastModified)
				{
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
					spec.Width  = width;
					spec.Format = channels == 3 ? ImageFormat::RGB8 : ImageFormat::RGBA8;

					Texture2D* texture = new Texture2D(spec);
					texture->SetData(data, width * height * channels);
					delete[] data;

					return texture;
				}
				else
				{
					std::filesystem::remove(filepath);
				}

				break;
			}
		}

		return nullptr;
	}

	void FontCache::CacheAtlas(const Texture2D* atlas, AssetHandle handle, Timestamp lastModified)
	{
		const std::filesystem::path cachePath = ProjectContext::Get()->GetAssetDirectory() / "cache" / "fonts";
		const std::string newFilename         = std::to_string(handle) + "_" + std::to_string(lastModified) + ".cache";

		std::ofstream file(cachePath / newFilename, std::ios::binary | std::ios::trunc);

		ASSERT(file.is_open());

		i32 width         = atlas->GetWidth();
		i32 height        = atlas->GetHeight();
		i32 channels      = atlas->GetChannels();
		const char* bytes = atlas->GetBytes();

		file.write(reinterpret_cast<const char*>(&width), sizeof(i32));
		file.write(reinterpret_cast<const char*>(&height), sizeof(i32));
		file.write(reinterpret_cast<const char*>(&channels), sizeof(i32));
		file.write(bytes, width * height * channels);
		file.close();
	}

} // namespace SW
