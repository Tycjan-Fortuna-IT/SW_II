/**
 * @file AssetPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.1
 * @date 2024-03-29
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <filesystem>

#include "GUI/Panel.hpp"
#include "Core/Asset/Asset.hpp"

namespace SW {

	class Texture2D;
	class AssetDirectoryTree;
	struct AssetSourceItem;

	/**
	 * @brief The AssetPanel class represents a panel that displays the console window.
	 *        in a tree-like structure.
	 */
	class AssetPanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a AssetPanel object with an optional name.
		 * 
		 * @param name The name of the panel. Default is "Assets".
		 */
		explicit AssetPanel(const char* name = "Assets");

		/**
		 * @brief Default destructor for the AssetPanel class.
		 */
		~AssetPanel() override;

		/** @brief Copy constructor (deleted). */
		AssetPanel(const AssetPanel& other) = delete;
		/** @brief Move constructor (deleted). */
		AssetPanel(AssetPanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		AssetPanel& operator=(const AssetPanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		AssetPanel& operator=(AssetPanel&& other) = delete;

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
		std::filesystem::path m_AssetsDirectory;	/** @brief The path to the assets directory. */

		bool m_OpenDeleteWarningModal = false;
		bool m_OpenNewFileModal = false;
		bool m_RenameEntryModal = false;

		int m_ThumbnailSize = 256;

		std::filesystem::path m_FilesystemEntryToDelete = "";
		std::filesystem::path m_FilesystemEntryToRename = "";

		AssetDirectoryTree* m_AssetTree = nullptr;
		AssetSourceItem* m_SelectedItem = nullptr;
		AssetSourceItem* m_QueuedSelectedItem = nullptr;

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

}
