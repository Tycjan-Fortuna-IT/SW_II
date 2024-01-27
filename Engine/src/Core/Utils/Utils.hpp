#pragma once

namespace SW {

    class Utils final
    {
    public:
        static std::string ReadFile(const std::string& filepath);
    };

	namespace String {

		///**
		// * @brief Converts a pointer to a char8_t string to a pointer to a char string.
		// *
		// * @warning Note that the returned string will only be valid if the input string is encoded in ASCII or UTF-8.
		// *			This function does not check the encoding of the input string!
		// *
		// * @param text Pointer to the char8_t string to convert.
		// * @return Pointer to the converted char string.
		// */
		//[[nodiscard]] static const char* FromChar8T(const char8_t* text)
		//{
		//	return reinterpret_cast<const char*>(text);
		//}

		/**
		 * @brief Returns a string representation of the given bytes.
		 * 
		 * @param bytes The bytes to convert to a string.
		 * @return std::string The string representation of the given bytes.
		 */
		static std::string BytesToString(u32 bytes)
		{
			constexpr f32 GB = 1024 * 1024 * 1024;
			constexpr f32 MB = 1024 * 1024;
			constexpr f32 KB = 1024;

			char buffer[32 + 1]{};

			if (bytes >= GB) {
				snprintf(buffer, 32, "%.2f GB", (f32)bytes / GB);
			} else if (bytes >= MB) {
				snprintf(buffer, 32, "%.2f MB", (f32)bytes / MB);
			} else if (bytes >= KB) {
				snprintf(buffer, 32, "%.2f KB", (f32)bytes / KB);
			} else {
				snprintf(buffer, 32, "%.2f bytes", (f32)bytes);
			}

			return std::string(buffer);
		}

		/**
		 * @brief Returns a string representation of the sum of allocated memory 
		 *		  in the program.
		 *
		 * @return std::string The string representation of the sum of allocated memory.
		 */
		std::string GetAllocatedMemoryString();

		/**
		 * @brief Returns a string representation of the sum of allocated memory
		 *		  in the program.
		 *
		 * @return std::string The string representation of the sum of allocated memory. (bytes)
		 */
		std::string GetAllocatedMemoryStringRaw();
	}

}
