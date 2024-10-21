/**
 * @file AssetPanel.hpp
 * @version 0.2.2
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include <filesystem>

#include "Asset/Cache/ThumbnailCache.hpp"
#include "GUI/Panel.hpp"

namespace SW
{
	class Texture2D;
	class AssetDirectoryTree;
	struct AssetSourceItem;

	/**
	 * @brief The AssetPanel class represents a panel that displays the assets of the project
	 *        in a tree-like structure.
	 */
	class AssetPanel final : public Panel
	{
	public:
		AssetPanel();
		~AssetPanel() override;

		AssetPanel(const AssetPanel& other)            = delete;
		AssetPanel(AssetPanel&& other)                 = delete;
		AssetPanel& operator=(const AssetPanel& other) = delete;
		AssetPanel& operator=(AssetPanel&& other)      = delete;

		PanelType GetPanelType() const override { return PanelType::AssetPanel; }

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(Timestep dt) override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

	private:
		std::filesystem::path m_AssetsDirectory; /** @brief The path to the assets directory. */

		bool m_IsTableHovered         = false;
		bool m_OpenDeleteWarningModal = false;
		bool m_OpenNewFileModal       = false;
		bool m_RenameEntryModal       = false;

		int m_ThumbnailSize = 220;

		std::filesystem::path m_FilesystemEntryToDelete = "";
		std::filesystem::path m_FilesystemEntryToRename = "";

		AssetDirectoryTree* m_AssetTree       = nullptr;
		AssetSourceItem* m_SelectedItem       = nullptr;
		AssetSourceItem* m_QueuedSelectedItem = nullptr;

		ThumbnailCache m_Cache;

		f32 m_CurrentTime = 0u; // for animation2d thumbnails

		/**
		 * @brief Loads the directory entries.
		 */
		void LoadDirectoryEntries();

		/**
		 * @brief Draws the header of the asset panel.
		 */
		void DrawHeader();

		/**
		 * @brief Draws the directory tree view recursively.
		 *
		 * @param path The path of the directory to draw.
		 */
		void DrawDirectoryTreeViewRecursive(AssetSourceItem* item);

		/**
		 * @brief Draws the side view of the asset panel.
		 */
		void DrawSideView();

		/**
		 * @brief Draws the body of the asset panel.
		 */
		void DrawBody();

		void DrawItemOperationsPopup(const AssetSourceItem* item);

		void HandleItemOnDoubleClick(AssetSourceItem* item, bool* refreshDirectory);

		/**
		 * @brief Draws the popup for the asset panel.
		 */
		void DrawAssetPanelPopup();
	};

} // namespace SW
