#include "AudioManagerPanel.hpp"

namespace SW {

	AudioManagerPanel::AudioManagerPanel()
		: Panel("Audio Manager", SW_ICON_VOLUME_HIGH, false) {}

	void AudioManagerPanel::OnUpdate(Timestep /*dt*/)
	{
		
	}

	void AudioManagerPanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
