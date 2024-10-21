#pragma once

#include "AudioEvent.hpp"

namespace SW
{

	class AudioEventSerializer
	{
	public:
		static void Serialize(const std::filesystem::path& path,
		                      const std::unordered_map<std::string, AudioEvent>& audioEvents);

		static void Deserialize(const std::filesystem::path& path,
		                        std::unordered_map<std::string, AudioEvent>& outAudioEvents);
	};

} // namespace SW
