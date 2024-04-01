/**
 * @file Popups.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-01
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW::GUI::Popups {

	/**
	 * Draws a popup window for adding a new file.
	 * 
	 * @param destinationDir The full path of the destination directory.
	 * @param opened A pointer to a boolean indicating whether the popup window is open.
	 * @return True if the file was added successfully, false otherwise.
	 */
	bool DrawAddNewFilePopup(const std::filesystem::path& destinationDir, bool* opened);

	/**
	 * Draws a popup window for deleting a file.
	 * 
	 * @param filepath The full path of the file to be deleted.
	 * @param opened A pointer to a boolean indicating whether the popup window is open.
	 * @return True if the file was deleted successfully, false otherwise.
	 */
	bool DrawDeleteFilePopup(const std::filesystem::path& filepath, bool* opened);

	/**
	 * Draws a popup window for deleting a file to rename it.
	 * 
	 * @param filepath The full path of the file to be deleted.
	 * @param opened A pointer to a boolean indicating whether the popup window is open.
	 * @return True if the file was deleted successfully, false otherwise.
	 */
	bool DrawDeleteFileToRenamePopup(const std::filesystem::path& filepath, bool* opened);

}
