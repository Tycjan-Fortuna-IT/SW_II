#pragma once

#include "miniaudio.h"

namespace SW {

	class Sound;

	class SoundInstance
	{
	public:
		SoundInstance() = delete;
		SoundInstance(const Sound* sound);
		
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
		 * @brief Stops and resets the sound.
		 * @warning Does nothing when the sound is not playing.
		 */
		void Stop();

		/**
		 * @brief Resets the sound.
		 * @warning Does nothing when the sound is not playing.
		 */
		void Reset();

		/**
		* @brief Checks if the sound is playing.
		 */
		bool IsPlaying() const;

	private:
		ma_sound m_Instance;
	};

}
