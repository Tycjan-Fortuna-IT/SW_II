/**
 * @file FileSystem.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.3
 * @date 2024-03-21
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
		 * @brief Creates a new file in a given path to it.
		 *
		 * @param path Path to file. e.g. C:/dev/file.txt
		 * @param content Content that will be put inside this newly created file.
		 * @return Whether the operation was successful.
		 */
		static bool CreateFileWithContent(const std::filesystem::path& path, const std::string& content);

		/**
		 * @brief Creates a new directory in a given path to it.
		 *
		 * @param path Path to directory. e.g. C:/dev/newdir
		 * @return Whether the operation was successful.
		 */
		static bool CreateEmptyDirectory(const std::filesystem::path& path);

		/**
		 * @brief Renames the file to chosen filename.
		 * @warning This method doesn't preserve the file extension so be sure to pass it with newName!
		 *
		 * @param path Path to file. e.g. C:/dev/file.txt
		 * @param newName Name of the file to be used while renaming. e.g. renamed.txt
		 * @return Whether the operation was successful.
		 */
		static bool RenameFile(const std::filesystem::path& path, const std::string& newName);

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
		 * @brief Opens file dialog for folder picking.
		 *
		 * @param initialFolder Initial folder that will be opened.
		 * @return File path to chosen folder or empty path if no folder was chosen.
		 */
		static std::filesystem::path OpenFolderDialog(const char* initialFolder = nullptr);

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
