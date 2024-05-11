#pragma once

namespace SW {

	class SoundListener
	{
	public:
		SoundListener(/*f32 coneInnerAngle, f32 coneOuterAngle, f32 coneOuterGain*/);
		~SoundListener() = default;

		void SetPosition(const glm::vec3& position) const;
		void SetDirection(const glm::vec3& forward) const;

	private:
		u32 m_ListenerIndex = 0u;
	};

}

