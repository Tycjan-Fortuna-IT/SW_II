#pragma once

#include "Core/OpenGL/Texture2D.hpp"
#include "Asset.hpp"

namespace SW {

	class AssetRegistry
	{
	public:
		/**
		 * @brief Initializes asset registry.
		 * @warning Must be called before any other asset manager system operation.
		 */
		void Initialize();

		/**
		 * @brief Shuts down asset registry.
		 * @warning Must be called before application exit.
		 */
		void Shutdown();

		// If asset didn't exist previously creates a new entry
		Asset* operator[](AssetHandle handle);

		u64 Count() const { return m_AssetRegistry.size(); }
		bool Contains(AssetHandle handle) const { return m_AssetRegistry.find(handle) != m_AssetRegistry.end(); }

		auto begin() { return m_AssetRegistry.begin(); }
		auto end() { return m_AssetRegistry.end(); }
		auto begin() const { return m_AssetRegistry.cbegin(); }
		auto end() const { return m_AssetRegistry.cend(); }

	private:
		std::unordered_map<AssetHandle, Asset*> m_AssetRegistry; // contains all loaded assets

		//std::unordered_map<AssetHandle, AssetMetaData> m_AvailableAssets;
	};

}
