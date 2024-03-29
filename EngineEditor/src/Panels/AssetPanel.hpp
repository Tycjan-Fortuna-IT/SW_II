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

	/**
	 * @brief The File struct represents a file.
	 */
	struct File final
	{
		std::string Name = "Invalid";		/** @brief The name of the file. */
		std::string FilePath = "Invalid";	/** @brief The path to the file. */
		Texture2D* Thumbnail = nullptr;		/** @brief The thumbnail of the file. */
		FileType Type = FileType::Unknown;	/** @brief The type of the file. */
		std::string TypeString = "Unknown";	/** @brief The type of the file as a string. */
		ImVec4 ColorIndicator;				/** @brief The color indicator of the file. */
	};

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
		~AssetPanel() override = default;

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
		std::filesystem::path m_CurrentDirectory;	/** @brief The path to the current directory fetched by the panel. */
		std::vector<File> m_DirectoryEntries = {};	/** @brief The directory entries. */

		bool m_OpenDeleteWarningModal = false;
		bool m_OpenNewFileModal = false;
		bool m_RenameEntryModal = false;

		int m_ThumbnailSize = 150;

		std::filesystem::path m_FilesystemEntryToDelete = "";
		std::filesystem::path m_FilesystemEntryToRename = "";

		/**
		 * @brief Refreshes current directory to show entries inside the body.
		 */
		void InvalidateAssetDirectory();
		
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
		void DrawDirectoryTreeViewRecursive(const std::filesystem::path& path);

		/**
		 * @brief Draws the side view of the asset panel.
		 */
		void DrawSideView();

		/**
		 * @brief Draws the body of the asset panel.
		 */
		void DrawBody();

		/**
		 * @brief Draws the popup for a directory entry.
		 * 
		 * @param entry The path of the directory entry.
		 */
		void DrawDirectoryEntryPopup(const std::filesystem::path& entry);

		/**
		 * @brief Draws the popup for the asset panel.
		 */
		void DrawAssetPanelPopup();
	};

}
