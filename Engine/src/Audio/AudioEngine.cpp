#include "AudioEngine.hpp"

#include "Sound.hpp"
#include "SoundInstance.hpp"

namespace SW {

	ma_engine AudioEngine::engine;

	std::vector<SoundInstance*> AudioEngine::m_ActiveInstances;

	void AudioEngine::Initialize()
	{
		ma_engine_config config = ma_engine_config_init();

		ma_result result = ma_engine_init(NULL, &engine);
		
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
			SoundInstance* instance = m_ActiveInstances[i];

			if (!instance->IsPlaying()) {
				instance->Stop();
				m_ActiveInstances.erase(m_ActiveInstances.begin() + i);
				delete instance;
			}
		}
	}

	void AudioEngine::PlaySound(const Sound* sound)
	{
		SoundInstance* soundInstance = new SoundInstance(sound);
		soundInstance->Play();

		m_ActiveInstances.push_back(soundInstance);
	}

}
