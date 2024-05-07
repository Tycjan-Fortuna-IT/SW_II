#pragma once

#include <miniaudio.h>

namespace SW {

	class Sound;

	enum class AttenuationType : int
	{
		None = 0,
		Inverse,
		Linear,
		Exponential
	};

	struct SoundSpecification
	{
		Sound* Sound = nullptr;
		f32 Volume = 1.0f;
		f32 Pitch = 1.0f;
		bool Looping = false;
		
		bool Is3D = false;
		AttenuationType Attenuation = AttenuationType::None;
		
		f32 RollOff = 1.0f;
		f32 MinGain = 0.0f;
		f32 MaxGain = 1.0f;
		f32 MinDistance = 0.3f;
		f32 MaxDistance = 100.0f;
		f32 ConeInnerAngle = glm::radians(360.0f);
		f32 ConeOuterAngle = glm::radians(360.0f);
		f32 ConeOuterGain = 0.0f;
		
		f32 DopplerFactor = 1.0f;
	};

	class SoundInstance
	{
	public:
		SoundInstance() = delete;
		SoundInstance(const Sound* sound);
		SoundInstance(const SoundSpecification& spec);

		~SoundInstance();

		/**
		 * @brief Plays the sound.
		 * @warning Does not reset the sound and does nothing when the sound is already playing.
		 */
		void Play();

		/**
		 * @brief Pauses the sound.
		 * @warning Does nothing when the sound is not playing.
		 */
		void Pause();

		/**
		 * @brief Resets the sound.
		 * @warning Does nothing when the sound is not playing.
		 */
		void Reset();

		/**
		 * @brief Starts the sound.
		 * @warning Does nothing when the sound is already playing.
		 */
		void Start();

		/**
		 * @brief Stops and resets the sound.
		 * @warning Does nothing when the sound is not playing.
		 */
		void Stop();

		/**
		* @brief Checks if the sound is playing.
		 */
		bool IsPlaying() const;

		void SetPosition(const glm::vec3& position);

		void SetDirection(const glm::vec3& direction);

	private:
		ma_sound m_Instance;
	};

}
