#include "Utils.hpp"

namespace SW {
    std::string Utils::ReadFile(const std::string& filepath)
	{
        std::string result;

        std::ifstream stream(filepath, std::ios::in | std::ios::binary);

        if (!stream.is_open())
            SW_ERROR("Could not open file {}", filepath);

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
	}

}
