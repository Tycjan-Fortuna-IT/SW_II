#include "AudioEngine.hpp"

#include "SoundInstance.hpp"

namespace SW {

	ma_engine AudioEngine::engine;

	std::vector<SoundInstance*> AudioEngine::m_ActiveInstances;

	void AudioEngine::Initialize()
	{
		ma_engine_config config = ma_engine_config_init();
		config.listenerCount = 1;

		ma_result result = ma_engine_init(&config, &engine);
		
		ASSERT(result == MA_SUCCESS, "Failed to initialize the audio engine");
	}

	void AudioEngine::Shutdown()
	{
		ma_engine_uninit(&engine);
	}

	void AudioEngine::OnUpdate(Timestep /*dt*/)
	{
		for (int i = 0; i < m_ActiveInstances.size(); i++)
		{
			SoundInstance** instance = &m_ActiveInstances[i];

			ASSERT(instance);

			if (!(*instance)->IsPlaying()) {
				(*instance)->Stop();
				delete *instance;
				*instance = nullptr;
				m_ActiveInstances.erase(m_ActiveInstances.begin() + i);
			}
		}
	}

	void AudioEngine::PlaySoundOnce(const Sound* sound)
	{
		SoundInstance* soundInstance = new SoundInstance(sound);
		soundInstance->Play();

		m_ActiveInstances.push_back(soundInstance);
	}

	SoundInstance** AudioEngine::PlaySound(const Sound* sound)
	{
		SoundInstance* soundInstance = new SoundInstance(sound);
		soundInstance->Play();

		m_ActiveInstances.push_back(soundInstance);
		return &m_ActiveInstances.back();
	}

	SoundInstance** AudioEngine::PlaySound(const SoundSpecification& spec)
	{
		SoundInstance* soundInstance = new SoundInstance(spec);
		soundInstance->Play();

		m_ActiveInstances.push_back(soundInstance);
		return &m_ActiveInstances.back();
	}

	SoundInstance** AudioEngine::PlaySound3D(const SoundSpecification& spec, const glm::vec3& position, const glm::vec3& direction)
	{
		SoundInstance* soundInstance = new SoundInstance(spec);
		soundInstance->SetPosition(position);
		soundInstance->SetDirection(direction);
		soundInstance->Play();

		m_ActiveInstances.push_back(soundInstance);
		return &m_ActiveInstances.back();
	}

}
