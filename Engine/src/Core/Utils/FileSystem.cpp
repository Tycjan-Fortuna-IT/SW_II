#include "FileSystem.hpp"

#include <nfd.hpp>

#ifdef SW_WINDOWS
	#include <ShlObj_core.h>
	#include <comdef.h>
	#include <commdlg.h>
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

    bool FileSystem::CreateFileWithContent(const std::filesystem::path& path, const std::string& content)
    {
		std::ofstream outputFile(path);

		if (!outputFile.is_open())
			return false;

		outputFile << content;
		outputFile.close();

		return true;
    }

    bool FileSystem::CreateEmptyDirectory(const std::filesystem::path& path)
    {
		return std::filesystem::create_directory(path);
    }

    bool FileSystem::CreateEmptyDirectoryIfNotExists(const std::filesystem::path& path)
    {
		if (Exists(path))
			return false;

		return std::filesystem::create_directory(path);
	}

    u64 FileSystem::GetLastWriteTime(const std::filesystem::directory_entry& entry)
	{
		return GetLastWriteTime(entry.path());
	}

	u64 FileSystem::GetLastWriteTime(const std::filesystem::path& path)
	{
		auto temp = std::filesystem::last_write_time(path);

		return temp.time_since_epoch().count();
	}

	bool FileSystem::RenameFile(const std::filesystem::path& path, const std::string& newName)
	{
		std::filesystem::path oldFilename = path.filename();
		std::filesystem::path oldPath = path.parent_path();

		std::filesystem::rename(path, oldPath / newName);
		
		return true;
	}

	bool FileSystem::Exists(const std::filesystem::path& path)
	{
		return std::filesystem::exists(path);
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

    std::filesystem::path FileSystem::OpenFolderDialog(const char* initialFolder)
    {
		NFD::UniquePath filePath;
		nfdresult_t result = NFD::PickFolder(filePath, initialFolder);

		switch (result) {
			case NFD_OKAY: return filePath.get();
			case NFD_CANCEL: return "";
			case NFD_ERROR:
			{
				ASSERT(false, "NFD-Extended threw an error: {}", NFD::GetError());
				return "";
			}
		}

		return "";
    }

    void FileSystem::RevealFolderInFileExplorer(const std::filesystem::path& path)
    {
		RevealFolderInFileExplorer(path.string().c_str());
    }

	void FileSystem::RevealFolderInFileExplorer(const char* path)
	{
		ShellExecuteA(nullptr, "open", path, nullptr, nullptr, SW_SHOWDEFAULT);
	}

    void FileSystem::OpenFolderAndSelectItem(const std::filesystem::path& path)
    {
		const _bstr_t widePath(path.c_str());

		if (const LPITEMIDLIST pidl = ILCreateFromPath(widePath)) {
			SHOpenFolderAndSelectItems(pidl, 0, nullptr, 0);
			ILFree(pidl);
		}
    }

	bool FileSystem::OpenExternally(const std::filesystem::path& path)
	{
		const std::filesystem::path absolutePath = std::filesystem::canonical(path);
		
		if (!std::filesystem::exists(absolutePath))
			return false;

		ShellExecute(NULL, L"open", absolutePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		
		return true;
	}

}
