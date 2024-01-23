#pragma once

namespace SW {

	class Scene;

	class SceneSerializer final
	{
		void Serialize(Scene* const scene, const std::string& path);
		void Deserialize(Scene** scene, const std::string& path);
	};

}
