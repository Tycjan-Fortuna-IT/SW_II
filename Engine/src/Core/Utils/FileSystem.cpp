#include "FileSystem.hpp"

#include <nfd.hpp>

#ifdef SW_WINDOWS
	#include <shellapi.h>
#endif


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

    void FileSystem::RevealFolderInFileExplorer(const std::filesystem::path& path)
    {
		RevealFolderInFileExplorer(path.string().c_str());
    }

	void FileSystem::RevealFolderInFileExplorer(const char* path)
	{
#ifdef SW_WINDOWS
		ShellExecuteA(nullptr, "open", path, nullptr, nullptr, SW_SHOWDEFAULT);
#endif
	}

}
