/**
 * @file Components.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Core/Utils/Random.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/OpenGL/Texture2D.hpp"

namespace SW {

    /**
     * @struct IDComponent
     * @brief This component is used to identify an entity. Each entity has a unique ID.
     * @note This component is required for each entity. ID = 0 is reserved for null or invalid entity.
     */
    struct IDComponent final
    {
        u64 ID = 0;

        IDComponent()
            : ID(CreateID()) {}
        IDComponent(u64 id)
            : ID(id) {}

        IDComponent(const IDComponent& other) = default;
        IDComponent(IDComponent&& other) = default;
        IDComponent& operator=(const IDComponent& other) = default;
        IDComponent& operator=(IDComponent&& other) = default;

        ~IDComponent() = default;
    };

    /**
     * @struct TagComponent
     * @brief This component is used to identify an entity by a tag.
     * @note This component is optional.
     */
    struct TagComponent final
    {
        std::string Tag = "No tag";

        TagComponent() = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}

        TagComponent(const TagComponent& other) = default;
        TagComponent(TagComponent&& other) = default;
        TagComponent& operator=(const TagComponent& other) = default;
        TagComponent& operator=(TagComponent&& other) = default;

        ~TagComponent() = default;
    };

    /**
     * @struct TransformComponent
     * @brief This component is used to store the position, rotation and scale of an entity.
     * @note This component is required for each entity.
     */
    struct TransformComponent final
    {
        glm::vec3 Position = { 0.0f, 0.0f , 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f , 0.0f };		/** @brief Rotation angles are in radians! */
        glm::vec3 Scale =    { 1.0f, 1.0f , 1.0f };

        TransformComponent() = default;
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
            : Position(position), Rotation(rotation), Scale(scale) {}
        TransformComponent(const glm::vec3& position)
            : Position(position) {}

        TransformComponent(const TransformComponent& other) = default;
        TransformComponent(TransformComponent&& other) = default;
        TransformComponent& operator=(const TransformComponent& other) = default;
        TransformComponent& operator=(TransformComponent&& other) = default;

        ~TransformComponent() = default;

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Position)
				* glm::toMat4(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);
		}
    };

    /**
     * @struct SpriteComponent
     * @brief This component is used to store the color of an entity. This component is used for rendering.
     * @note This component is optional.
     */
    struct SpriteComponent final
    {
        glm::vec4 Color = glm::vec4(1.0f);
		Texture2D* Texture = nullptr;	/** @brief Texture of the entity, can be nullptr! */
		f32 TilingFactor = 1.0f;

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color)
            : Color(color) {}
		SpriteComponent(Texture2D* texture, const glm::vec4& tint = glm::vec4(1.f), f32 tilingFactor = 1.f)
			: Texture(texture), Color(tint), TilingFactor(tilingFactor) {}

        SpriteComponent(const SpriteComponent& other) = default;
        SpriteComponent(SpriteComponent&& other) = default;
        SpriteComponent& operator=(const SpriteComponent& other) = default;
        SpriteComponent& operator=(SpriteComponent&& other) = default;

        ~SpriteComponent() = default;
    };

	/**
	 * @struct CameraComponent
	 * @brief This component is used to store the camera's data with position's attached to the component.
	 * @note This component is optional. However without even single camera the scene will not be visible!
	 */
	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true;	/** @brief In case of many cameras only the first primary one will be used. */

		CameraComponent() = default;
		CameraComponent(const SceneCamera& camera)
			: Camera(camera) {}

		CameraComponent(const CameraComponent& other) = default;
		CameraComponent(CameraComponent&& other) = default;
		CameraComponent& operator=(const CameraComponent& other) = default;
		CameraComponent& operator=(CameraComponent&& other) = default;
	};

	enum class PhysicBodyType : u8
	{
		Static = 0,		/**< zero mass, zero velocity, may be manually moved */
		Kinematic,		/**< zero mass, non-zero velocity set by user, moved by the physics system */
		Dynamic			/**< positive mass, non-zero velocity determined by forces, moved by the physics system */
	};

	struct RigidBody2DComponent final
	{
		PhysicBodyType Type = PhysicBodyType::Static;	/**< By default the body is static */

		void* Handle = nullptr;		/**< Internal box2D physics body handle */

		f32 GravityScale = 1.0f;	/**< Scale the gravity applied to this body. */

		bool AllowSleep = true;		/**< Set this flag to false if this body should never fall asleep. */
	};

	struct BoxCollider2DComponent final
	{
		void* Handle = nullptr;		/**< Internal box2D physics box collider handle */

		glm::vec2 Size = { 0.5f, 0.5f };		/**< Size of the collider */
		glm::vec2 Offset = { 0.0f, 0.0f };	/**< Center of the box in local coordinates */
	};

}
