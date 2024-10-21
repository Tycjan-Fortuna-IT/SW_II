#include "AudioEngine.hpp"

#include "Asset/AssetManager.hpp"
#include "AudioEventSerializer.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "SoundInstance.hpp"

namespace SW
{

	ma_engine AudioEngine::engine;

	std::vector<SoundInstance*> AudioEngine::s_ActiveInstances;
	std::unordered_map<std::string, AudioEvent> AudioEngine::s_AudioEvents;

	void AudioEngine::Initialize()
	{
		ma_engine_config config = ma_engine_config_init();
		config.listenerCount    = 1;

		ma_result result = ma_engine_init(&config, &engine);

		ASSERT(result == MA_SUCCESS, "Failed to initialize the audio engine");

		EventSystem::Register(EVENT_CODE_PROJECT_LOADED, [](Event /*event*/) -> bool {
			const std::filesystem::path& regPath = ProjectContext::Get()->GetAudioRegistryPath();

			if (!FileSystem::Exists(regPath))
				FileSystem::CreateFileWithContent(regPath, "AudioEvents:\n");

			ClearActiveInstances();
			s_AudioEvents.clear();

			AudioEventSerializer::Deserialize(regPath, s_AudioEvents);

			return false;
		});
	}

	void AudioEngine::Shutdown()
	{
		ClearActiveInstances();

		ma_engine_uninit(&engine);

		if (ProjectContext::Get())
			AudioEventSerializer::Serialize(ProjectContext::Get()->GetAudioRegistryPath(), s_AudioEvents);
	}

	void AudioEngine::OnUpdate(Timestep /*dt*/)
	{
		for (int i = 0; i < s_ActiveInstances.size(); i++)
		{
			SoundInstance** instance = &s_ActiveInstances[i];

			ASSERT(instance);

			if (!(*instance)->IsPlaying())
			{
				(*instance)->Stop();
				delete *instance;
				*instance = nullptr;
				s_ActiveInstances.erase(s_ActiveInstances.begin() + i);
			}
		}
	}

	void AudioEngine::PlaySoundOnce(const Sound* sound)
	{
		SoundInstance* soundInstance = new SoundInstance(sound);
		soundInstance->Play();

		s_ActiveInstances.push_back(soundInstance);
	}

	SoundInstance** AudioEngine::PlaySound(const Sound* sound)
	{
		SoundInstance* soundInstance = new SoundInstance(sound);
		soundInstance->Play();

		s_ActiveInstances.push_back(soundInstance);
		return &s_ActiveInstances.back();
	}

	SoundInstance** AudioEngine::PlaySound(const SoundSpecification& spec)
	{
		SoundInstance* soundInstance = new SoundInstance(spec);
		soundInstance->Play();

		s_ActiveInstances.push_back(soundInstance);
		return &s_ActiveInstances.back();
	}

	SoundInstance** AudioEngine::PlaySound3D(const SoundSpecification& spec, const glm::vec3& position,
	                                         const glm::vec3& direction)
	{
		SoundInstance* soundInstance = new SoundInstance(spec);
		soundInstance->SetPosition(position);
		soundInstance->SetDirection(direction);
		soundInstance->Play();

		s_ActiveInstances.push_back(soundInstance);
		return &s_ActiveInstances.back();
	}

	SoundInstance** AudioEngine::DispatchAudioEvent(const std::string& name)
	{
		if (s_AudioEvents.find(name) == s_AudioEvents.end())
		{
			APP_ERROR("Audio event with name '{0}' not found", name);

			return nullptr;
		}

		AudioEvent& event = s_AudioEvents[name];

		if (event.Handle == 0)
		{
			APP_ERROR("Audio event '{0}' has no sound assigned", name);
			return nullptr;
		}

		Sound* sound = *AssetManager::GetAssetRaw<Sound>(event.Handle);

		SoundSpecification spec;
		spec.Sound   = sound;
		spec.Volume  = event.Volume;
		spec.Pitch   = event.Pitch;
		spec.Looping = event.Looping;

		return PlaySound(spec);
	}

	void AudioEngine::ClearActiveInstances()
	{
		for (int i = 0; i < s_ActiveInstances.size(); i++)
		{
			SoundInstance** instance = &s_ActiveInstances[i];

			ASSERT(instance);

			if (*instance)
			{
				(*instance)->Stop();
				delete *instance;
				*instance = nullptr;
			}
		}
	}

} // namespace SW
