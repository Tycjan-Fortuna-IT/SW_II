#include "SoundInstance.hpp"

#include "Sound.hpp"
#include "Asset/AssetManager.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW {

	SoundInstance::SoundInstance(const Sound* sound)
	{
		ma_result result = ma_sound_init_copy(AudioEngine::Get(), sound->GetAudioHandle(), 0, nullptr, &m_Instance);

		ASSERT(result == MA_SUCCESS, "Failed to initialize sound instance {}", sound->GetHandle());
	}

	SoundInstance::~SoundInstance()
	{
		ma_sound_uninit(&m_Instance);
	}

	void SoundInstance::Play()
	{
		ma_sound_seek_to_pcm_frame(&m_Instance, 0);
		ma_sound_start(&m_Instance);
	}

	void SoundInstance::Pause()
	{
		ma_sound_stop(&m_Instance);
	}

	void SoundInstance::Stop()
	{
		ma_sound_stop(&m_Instance);
		ma_sound_seek_to_pcm_frame(&m_Instance, 0);
	}

	void SoundInstance::Reset()
	{
		ma_sound_seek_to_pcm_frame(&m_Instance, 0);
	}

	bool SoundInstance::IsPlaying() const
	{
		return (bool)ma_sound_is_playing(&m_Instance);
	}

}
