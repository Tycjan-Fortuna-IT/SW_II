#include "AudioEventSerializer.hpp"

#include "Core/Utils/SerializationUtils.hpp"

namespace SW {

	void AudioEventSerializer::Serialize(const std::filesystem::path& path, const std::unordered_map<std::string, AudioEvent>& audioEvents)
	{
		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "AudioEvents" << YAML::Value << YAML::BeginSeq;

		for (auto&&[key, event] : audioEvents) {
			output << YAML::BeginMap;

			output << YAML::Key << "Handle" << YAML::Value << event.Handle;
			output << YAML::Key << "Key" << YAML::Value << key;
			output << YAML::Key << "Volume" << YAML::Value << event.Volume;
			output << YAML::Key << "Pitch" << YAML::Value << event.Pitch;
			output << YAML::Key << "TriggerType" << YAML::Value << (int)event.TriggerType;
			output << YAML::Key << "Looping" << YAML::Value << event.Looping;

			output << YAML::EndMap;
		}

		output << YAML::EndMap;

		output << YAML::EndSeq;

		std::ofstream fout(path);
		fout << output.c_str();
	}

	void AudioEventSerializer::Deserialize(const std::filesystem::path& path, std::unordered_map<std::string, AudioEvent>& outAudioEvents)
	{
		const YAML::Node registry = YAML::LoadFile(path.string());

		const YAML::Node audioEvents = registry["AudioEvents"];

		for (const YAML::Node& event : audioEvents) {
			AudioEvent audioEvent;
			audioEvent.Handle = TryDeserializeNode<AssetHandle>(event, "Handle", 0);
			audioEvent.Volume = TryDeserializeNode<f32>(event, "Volume", 1.0f);
			audioEvent.Pitch = TryDeserializeNode<f32>(event, "Pitch", 1.0f);
			audioEvent.TriggerType = (AudioEventTriggerType)TryDeserializeNode<int>(event, "TriggerType", 0);
			audioEvent.Looping = TryDeserializeNode<bool>(event, "Looping", false);

			outAudioEvents[TryDeserializeNode<std::string>(event, "Key", "Unnamed")] = audioEvent;
		}
	}

}
