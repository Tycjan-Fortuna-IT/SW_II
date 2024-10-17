#pragma once

#include "Asset/Asset.hpp"
#include "SoundInstance.hpp"

namespace SW {

	enum class AudioEventTriggerType : u8
	{
		//OnSceneStart,
		OnScriptTrigger,
	};

	struct AudioEvent
	{
		AssetHandle Handle = 0;
		f32 Volume = 1.0f;
		f32 Pitch = 1.0f;
		AudioEventTriggerType TriggerType = AudioEventTriggerType::OnScriptTrigger;
		bool Looping = false;
		SoundInstance** Instance = nullptr;
	};

}

