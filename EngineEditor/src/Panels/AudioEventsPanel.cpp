#include "AudioEventsPanel.hpp"

namespace SW {

	AudioEventsPanel::AudioEventsPanel()
		: Panel("Audio Events", SW_ICON_VOLUME_HIGH, false) {}

	void AudioEventsPanel::OnUpdate(Timestep /*dt*/)
	{

	}

	void AudioEventsPanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
