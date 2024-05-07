#pragma once

#include <miniaudio.h>

#include "Core/Timestep.hpp"
#include "SoundInstance.hpp"

namespace SW {

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

		static SoundInstance** PlaySound3D(const SoundSpecification& spec, const glm::vec3& position, const glm::vec3& direction);

		static ma_engine* Get() { return &engine; }

	private:
		static ma_engine engine;
		static std::vector<SoundInstance*> m_ActiveInstances;
	};

}
