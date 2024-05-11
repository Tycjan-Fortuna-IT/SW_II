#include "SoundListener.hpp"

#include "Audio/AudioEngine.hpp"

namespace SW {

	SoundListener::SoundListener(/*f32 coneInnerAngle, f32 coneOuterAngle, f32 coneOuterGain*/)
	{
		//ma_engine_listener_set_cone(AudioEngine::Get(), m_ListenerIndex, coneInnerAngle, coneOuterAngle, coneOuterGain);
		ma_engine_listener_set_position(AudioEngine::Get(), m_ListenerIndex, 0.0f, 0.0f, 0.0f);
		ma_engine_listener_set_direction(AudioEngine::Get(), m_ListenerIndex, 0.0f, 0.0f, -1.0f);
		ma_engine_listener_set_world_up(AudioEngine::Get(), m_ListenerIndex, 0.0f, 1.0f, 0.0f);
	}

	void SoundListener::SetPosition(const glm::vec3& position) const
	{
		ma_engine_listener_set_position(AudioEngine::Get(), m_ListenerIndex, position.x, position.y, position.z);
	}

	void SoundListener::SetDirection(const glm::vec3& forward) const
	{
		ma_engine_listener_set_direction(AudioEngine::Get(), m_ListenerIndex, forward.x, forward.y, forward.z);
	}

}
