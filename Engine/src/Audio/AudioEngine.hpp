#pragma once

#include <miniaudio.h>

#include "AudioEvent.hpp"
#include "Core/Timestep.hpp"
#include "SoundInstance.hpp"

namespace SW
{

	class Sound;

	class AudioEngine
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void OnUpdate(Timestep dt);

		static void PlaySoundOnce(const Sound* sound);

		static SoundInstance** PlaySound(const Sound* sound);

		static SoundInstance** PlaySound(const SoundSpecification& spec);

		static SoundInstance** PlaySound3D(const SoundSpecification& spec, const glm::vec3& position,
		                                   const glm::vec3& direction);

		static SoundInstance** DispatchAudioEvent(const std::string& name);

		static ma_engine* Get() { return &engine; }

		static std::unordered_map<std::string, AudioEvent>& GetAudioEvents() { return s_AudioEvents; }

		static void ClearActiveInstances();

	private:
		static ma_engine engine;
		static std::vector<SoundInstance*> s_ActiveInstances;
		static std::unordered_map<std::string, AudioEvent> s_AudioEvents;
	};

} // namespace SW
