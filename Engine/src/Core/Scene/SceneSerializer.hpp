/**
 * @file SceneSerializer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-04-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/ECS/Entity.hpp"

namespace YAML {
	class Emitter;
	class Node;
}

namespace SW {

	class Scene;

	/**
	 * @brief Class responsible for serializing and deserializing scenes.
	 */
	class SceneSerializer final
	{
	public:
		/**
		 * @brief Serializes scene to file.
		 *
		 * @param scene Scene to serialize.
		 * @param path Path to file.
		 */
		static void Serialize(Scene* scene, const std::filesystem::path& path);

		static void SerializeEntity(YAML::Emitter& output, Entity entity, const Scene* scene);
		
		/**
		 * @brief Deserializes scene from file.
		 *
		 * @param scene Scene to deserialize.
		 * @param path Path to file.
		 */
		[[nodiscard]] static Scene* Deserialize(const std::filesystem::path& path);

		static void DeserializeEntitiesNode(YAML::Node& entitiesNode, Scene* scene);
	};

}
