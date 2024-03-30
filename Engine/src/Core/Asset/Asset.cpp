#include "Asset.hpp"

#include "Core/Utils/Random.hpp"

namespace SW {

	Asset::Asset(AssetHandle handle, const std::filesystem::path& path)
		: m_Path(path), m_Handle(handle) {}

}
