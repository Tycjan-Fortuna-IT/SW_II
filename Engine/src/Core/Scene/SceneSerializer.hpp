/**
 * @file SceneSerializer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-26
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

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
		static void Serialize(Scene* scene, const std::string& path);

		/**
		 * @brief Deserializes scene from file.
		 *
		 * @param scene Scene to deserialize.
		 * @param path Path to file.
		 */
		[[nodiscard]] static Scene* Deserialize(const std::string& path);
	};

}
