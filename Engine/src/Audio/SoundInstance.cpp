#include "SoundInstance.hpp"
#include "Sound.hpp"

namespace SW {

	SoundInstance::SoundInstance(const Sound* sound)
	{
		ma_result result = ma_sound_init_copy(AudioEngine::Get(), sound->GetAudioHandle(), 0, nullptr, &m_Instance);

		ASSERT(result == MA_SUCCESS, "Failed to initialize sound instance {}", sound->GetHandle());
	}

    SoundInstance::SoundInstance(const SoundSpecification& spec)
    {
		ma_result result = ma_sound_init_copy(AudioEngine::Get(), spec.Sound->GetAudioHandle(), 0, nullptr, &m_Instance);

		ASSERT(result == MA_SUCCESS, "Failed to initialize sound instance {}", spec.Sound->GetHandle());

		ma_sound_set_volume(&m_Instance, spec.Volume);
		ma_sound_set_pitch(&m_Instance, spec.Pitch);
		ma_sound_set_looping(&m_Instance, spec.Looping ? MA_TRUE : MA_FALSE);

		if (spec.Is3D) {
			ma_sound_set_spatialization_enabled(&m_Instance, MA_TRUE);
			ma_sound_set_attenuation_model(&m_Instance, (ma_attenuation_model)spec.Attenuation);
			ma_sound_set_rolloff(&m_Instance, spec.RollOff);
			ma_sound_set_min_gain(&m_Instance, spec.MinGain);
			ma_sound_set_max_gain(&m_Instance, spec.MaxGain);
			ma_sound_set_min_distance(&m_Instance, spec.MinDistance);
			ma_sound_set_max_distance(&m_Instance, spec.MaxDistance);
			//ma_sound_set_cone(&m_Instance, spec.ConeInnerAngle, spec.ConeOuterAngle, spec.ConeOuterGain);
			//ma_sound_set_doppler_factor(&m_Instance, spec.DopplerFactor);
		} else {
			ma_sound_set_spatialization_enabled(&m_Instance, MA_FALSE);
		}
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

	void SoundInstance::Reset()
	{
		ma_sound_seek_to_pcm_frame(&m_Instance, 0);
	}

	void SoundInstance::Start()
	{
		ma_sound_start(&m_Instance);
	}

	void SoundInstance::Stop()
	{
		ma_sound_stop(&m_Instance);
		ma_sound_seek_to_pcm_frame(&m_Instance, 0);
	}

	bool SoundInstance::IsPlaying() const
	{
		return (bool)ma_sound_is_playing(&m_Instance);
	}

	void SoundInstance::SetPosition(const glm::vec3& position)
	{
		ma_sound_set_position(&m_Instance, position.x, position.y, position.z);
	}

	void SoundInstance::SetDirection(const glm::vec3& direction)
	{
		ma_sound_set_direction(&m_Instance, direction.x, direction.y, direction.z);
	}

}
