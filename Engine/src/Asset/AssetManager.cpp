#include "AssetManager.hpp"

#include "AssetLoader.hpp"

namespace SW
{

	void AssetManager::Initialize()
	{
		AssetLoader::Initialize();
	}

	void AssetManager::Shutdown()
	{
		AssetLoader::Shutdown();
	}

} // namespace SW
