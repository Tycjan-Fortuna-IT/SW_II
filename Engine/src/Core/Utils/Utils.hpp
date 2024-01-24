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

	}

}
