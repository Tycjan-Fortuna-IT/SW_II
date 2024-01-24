#include "FileSystem.hpp"

#include <nfd.hpp>

namespace SW {

	void FileSystem::Initialize()
	{
		NFD::Init();
	}

	void FileSystem::Shutdown()
	{
		NFD::Quit();
	}

	std::filesystem::path FileSystem::OpenFileDialog(const std::initializer_list<FileDialogFilterItem> filters)
	{
		NFD::UniquePath filePath;
		nfdresult_t result = NFD::OpenDialog(filePath, (const nfdfilteritem_t*)filters.begin(), (nfdfiltersize_t)filters.size());

		if (result == NFD_OKAY) {
			return filePath.get();
		} else if (result == NFD_CANCEL) {
			return "";
		}

		SW_ERROR("NFD-Extended threw an error: {}", NFD::GetError());

		return "";
    }

	std::filesystem::path FileSystem::SaveFileDialog(const std::initializer_list<FileDialogFilterItem> filters)
	{
		NFD::UniquePath filePath;
		nfdresult_t result = NFD::SaveDialog(filePath, (const nfdfilteritem_t*)filters.begin(), (nfdfiltersize_t)filters.size());

		if (result == NFD_OKAY) {
			return filePath.get();
		}
		else if (result == NFD_CANCEL) {
			return "";
		}

		SW_ERROR("NFD-Extended threw an error: {}", NFD::GetError());

		return "";
	}

}
