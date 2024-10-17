#include "AudioEventsPanel.hpp"
#include "Audio/Sound.hpp"

namespace SW {

	AudioEventsPanel::AudioEventsPanel()
		: Panel("Audio Events", SW_ICON_VOLUME_HIGH, false) {}

	void AudioEventsPanel::OnUpdate(Timestep /*dt*/)
	{

	}

	void AudioEventsPanel::OnRender()
	{
		if (OnBegin()) {
			constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

			auto& audioEventsMap = AudioEngine::GetAudioEvents();

			if (ImGui::Button(SW_ICON_PLUS " Add")) {
				audioEventsMap[std::format("New Audio Event - {}", Random::CreateTag(5))] = {};
			}

			if (ImGui::BeginTable("AudioEventsPanel_MainViewTable", 2, flags | ImGuiTableFlags_ScrollY, ImGui::GetContentRegionAvail())) {
				ImGui::TableSetupColumn("Audio Events");
				ImGui::TableSetupColumn("Event");

				ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				if (ImGui::BeginTable("AudioEventsPanel_EventsTable", 1, flags, ImGui::GetContentRegionAvail())) {
					for (auto& [name, event] : audioEventsMap) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();

						GUI::ScopedID id(name.c_str());
						const bool isSelected = m_Selected == name;
						if (ImGui::Selectable(name.c_str(), isSelected)) {
							m_Selected = name;
							m_SelectedEvent = &event;
						}
					}

					ImGui::EndTable();
				}

				ImGui::TableNextColumn();

				if (m_Selected.empty()) {
					ImGui::TextUnformatted("Select an event to view its properties");
				} else {
					GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));
					GUI::ScopedStyle IndentSpacing(ImGuiStyleVar_IndentSpacing, 0.0f);
					GUI::ScopedStyle WindowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));

					std::string name = m_Selected;
					if (GUI::Components::SingleLineTextInputDeffered<128>(&name)) {
						AudioEvent event = audioEventsMap[m_Selected];
						audioEventsMap.erase(m_Selected);
						audioEventsMap[name] = event;
						m_Selected = name;
						m_SelectedEvent = &audioEventsMap[name];
					}

					GUI::Properties::BeginProperties("audio_event_properties");
					GUI::Properties::AssetSearchProperty<Sound>(&m_SelectedEvent->Handle, "Clip", "The audio sound to be played");
					GUI::Properties::SelectableProperty<AudioEventTriggerType>(&m_SelectedEvent->TriggerType, {
						{ "On Script Trigger", AudioEventTriggerType::OnScriptTrigger }
					}, "Trigger Type", "The type of trigger for the audio event");
					GUI::Properties::ScalarSliderProperty<f32>(&m_SelectedEvent->Volume, "Volume", "The volume of the audio source", 0.f, 2.f);
					GUI::Properties::ScalarSliderProperty<f32>(&m_SelectedEvent->Pitch, "Pitch", "The pitch of the audio source", 0.f, 2.f);
					GUI::Properties::CheckboxProperty(&m_SelectedEvent->Looping, "Looping", "Whether the audio source should loop");
					GUI::Properties::EndProperties();

					if (m_SelectedEvent->Handle) {

						if (ImGui::Button(SW_ICON_PLAY " Play")) {
							if (m_SelectedEvent->Instance && *m_SelectedEvent->Instance) {
								(*m_SelectedEvent->Instance)->Stop();
							}

							Sound* sound = *AssetManager::GetAssetRaw<Sound>(m_SelectedEvent->Handle);

							SoundSpecification spec;
							spec.Sound = sound;
							spec.Pitch = m_SelectedEvent->Pitch;
							spec.Volume = m_SelectedEvent->Volume;
							spec.Looping = m_SelectedEvent->Looping;

							m_SelectedEvent->Instance = AudioEngine::PlaySound(spec);
						}

						if (m_SelectedEvent->Instance && *m_SelectedEvent->Instance) {
							ImGui::SameLine();

							if (ImGui::Button(SW_ICON_STOP " Stop")) {
								(*m_SelectedEvent->Instance)->Stop();
							}
						}
					}

					if (ImGui::Button(SW_ICON_DELETE " Delete")) {
						audioEventsMap.erase(m_Selected);
						m_Selected.clear();
						m_SelectedEvent = nullptr;
					}
				}

				ImGui::EndTable();
			}

			OnEnd();
		}
	}

}
