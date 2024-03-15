/**
 * @file FileSystem.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-21
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	/**
	 * @brief Struct representing single item in file dialog filter.
	 */
	struct FileDialogFilterItem {
		const char* Label;			///< Label of the filter.
		const char* Filter;			///< File filter.
	};

	/**
	 * @brief Class responsible for simple file system operations.
	 */
	class FileSystem final
	{
	public:
		/**
		 * @brief Initializes file system.
		 * @warning Must be called before any other file system operation.
		 */
		static void Initialize();

		/**
		 * @brief Shuts down file system.
		 * @warning Must be called before application exit.
		 */
		static void Shutdown();

		/**
		 * @brief Checks if file exists.
		 *
		 * @param path Path to file.
		 * @return File path to chosen file or empty path if no file was chosen.
		 */
		static std::filesystem::path OpenFileDialog(const std::initializer_list<FileDialogFilterItem> filters);

		/**
		 * @brief Opens file dialog.
		 *
		 * @param filters List of filters.
		 * @return File path to chosen file or empty path if no file was chosen.
		 */
		static std::filesystem::path SaveFileDialog(const std::initializer_list<FileDialogFilterItem> filters);

		/**
		 * @brief Reveals the specified folder in the file explorer.
		 *
		 * @param path The path of the folder to be revealed.
		 */
		static void RevealFolderInFileExplorer(const std::filesystem::path& path);

		/**
		 * @brief Reveals the specified folder in the file explorer.
		 *
		 * @param path The path of the folder to be revealed.
		 */
		static void RevealFolderInFileExplorer(const char* path);

		static void OpenFolderAndSelectItem(const std::filesystem::path& path);
	};

}
