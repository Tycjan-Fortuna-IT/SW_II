#pragma once

#include <yaml-cpp/yaml.h>

namespace YAML
{

	Emitter& operator<<(Emitter& out, const glm::vec2& v);

	Emitter& operator<<(Emitter& out, const glm::vec3& v);

	Emitter& operator<<(Emitter& out, const glm::vec4& v);

	template <>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<f32>();
			rhs.y = node[1].as<f32>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<f32>();
			rhs.y = node[1].as<f32>();
			rhs.z = node[2].as<f32>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<f32>();
			rhs.y = node[1].as<f32>();
			rhs.z = node[2].as<f32>();
			rhs.w = node[3].as<f32>();
			return true;
		}
	};

} // namespace YAML

namespace SW
{

	template <typename T>
	static T TryDeserializeNode(const YAML::Node& node, const std::string& key, T defaultVal)
	{
		YAML::Node data = node[key];

		if (!data.IsDefined())
		{
			APP_WARN("Deserialization - Node: {} not found, using default value: {}", key, defaultVal);
			return defaultVal;
		}

		try
		{
			return data.as<T>();
		}
		catch (const YAML::Exception& e)
		{
			APP_WARN("Deserialization - Node: {} could not be deserialized, using default value: {}", key, defaultVal);
			APP_ERROR("Error: {}", e.what());

			return defaultVal;
		}
	}

} // namespace SW
