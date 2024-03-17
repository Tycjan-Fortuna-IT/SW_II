/**
 * @file AssetPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.0
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <filesystem>

#include "GUI/Panel.hpp"

namespace SW {

	class Texture2D;

	/**
	 * @brief The FileType enum class represents the type of a file.
	 */
	enum class FileType : int
	{
		Unknown = 0,	///< Unknown file type.
		Texture,		///< Texture file type.
		Directory,		///< Directory file type.
		Scene,			///< Scene file type
		Font,			///< Font file type.
		Prefab,			///< Prefab file type.
		Script,			///< Script file type.
	};

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
		void OnUpdate(Timestep dt) final override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() final override;

		void InvalidateAssetDirectory();
	
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
		 * @brief Loads the directory entries.
		 */
		void LoadDirectoryEntries();

		void DrawHeader();

		void DrawDirectoryTreeViewRecursive(const std::filesystem::path& path);

		void DrawSideView();

		void DrawBody();

		void DrawDirectoryEntryPopup(const std::filesystem::path& entry);

		void DrawAssetPanelPopup();
	};

}
