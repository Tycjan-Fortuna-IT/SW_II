#pragma once

#include <miniaudio.h>
#include "Core/Timestep.hpp"

namespace SW {

	class Sound;
	class SoundInstance;

	class AudioEngine
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void OnUpdate(Timestep dt);

		static void PlaySound(const Sound* sound);

		static ma_engine* Get() { return &engine; }

	private:
		static ma_engine engine;
		static std::vector<SoundInstance*> m_ActiveInstances;
	};

}
