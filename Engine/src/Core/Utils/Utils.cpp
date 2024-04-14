#include "Utils.hpp"

namespace SW {
    std::string Utils::ReadFile(const std::string& filepath)
	{
        std::string result;

        std::ifstream stream(filepath, std::ios::in | std::ios::binary);

        ASSERT(stream.is_open(), "Could not open file {}", filepath);

        stream.seekg(0, std::ios::end);
        result.resize(stream.tellg());

        stream.seekg(0, std::ios::beg);
        stream.read(result.data(), result.size());
        stream.close();

        return result;
    }

	namespace String {

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

	}

}
