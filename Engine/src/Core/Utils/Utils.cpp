#include "Utils.hpp"

namespace SW
{
	std::string Utils::ReadFile(const std::string& filepath)
	{
		std::string result;

		std::ifstream stream(filepath, std::ios::in | std::ios::binary);

		ASSERT(stream.is_open(), "Could not open file {}", filepath);

		stream.seekg(0, std::ios::end);
		result.resize(static_cast<size_t>(stream.tellg()));

		stream.seekg(0, std::ios::beg);
		stream.read(result.data(), static_cast<std::streamsize>(result.size()));
		stream.close();

		return result;
	}

	namespace String
	{

		std::string BytesToString(u32 bytes)
		{
			constexpr u32 GB = 1024 * 1024 * 1024;
			constexpr u32 MB = 1024 * 1024;
			constexpr u32 KB = 1024;

			const std::string result = [bytes]() {
				if (bytes >= GB)
					return std::format("{:.2f} GB", static_cast<float>(bytes) / GB);
				else if (bytes >= MB)
					return std::format("{:.2f} MB", static_cast<float>(bytes) / MB);
				else if (bytes >= KB)
					return std::format("{:.2f} KB", static_cast<float>(bytes) / KB);

				return std::format("{} bytes", bytes);
			}();

			return result;
		}

		std::string GetAllocatedMemoryString()
		{
			return BytesToString((u32)GetAllocationSizeSum());
		}

		std::string GetAllocatedMemoryStringRaw()
		{
			return std::to_string(GetAllocationSizeSum());
		}

		std::string RemoveExtension(const std::string& filename)
		{
			return filename.substr(0, filename.find_last_of('.'));
		}

	} // namespace String

} // namespace SW
