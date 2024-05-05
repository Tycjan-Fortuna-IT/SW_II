#include "Sound.hpp"

namespace SW {

	Sound::Sound(const std::filesystem::path& path)
	{
		ma_result result = ma_sound_init_from_file(AudioEngine::Get(), path.string().c_str(), 0, nullptr, nullptr, &m_Handle);

		ASSERT(result == MA_SUCCESS, "Failed to load sound file: {}", path);
	}

	Sound::~Sound()
	{
		ma_sound_stop(&m_Handle);
		ma_sound_uninit(&m_Handle);
	}

}
