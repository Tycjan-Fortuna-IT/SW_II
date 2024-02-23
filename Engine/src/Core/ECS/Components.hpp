/**
 * @file Components.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Utils/Random.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "../Math/Math.hpp"

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

		void SetTransform(const glm::mat4& transform)
		{
			Math::DecomposeTransform(transform, Position, Rotation, Scale);
		}
    };

    /**
     * @struct SpriteComponent
     * @brief This component is used to store the color of an entity. This component is used for rendering of squares and rectangles.
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
	 * @struct CircleComponent
	 * @brief This component is used to store the color of an entity. This component is used for rendering of circles and rings.
	 * @note This component is optional.
	 */
	struct CircleComponent final
	{
		glm::vec4 Color = glm::vec4(1.0f);
		f32 Thickness = 1.0f;
		f32 Fade = 0.005f;

		CircleComponent() = default;
		CircleComponent(const glm::vec4& color)
			: Color(color) {}
		
		CircleComponent(const CircleComponent& other) = default;
		CircleComponent(CircleComponent&& other) = default;
		CircleComponent& operator=(const CircleComponent& other) = default;
		CircleComponent& operator=(CircleComponent&& other) = default;

		~CircleComponent() = default;
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

	/**
	 * @struct RelationshipComponent
	 * @brief This component is used to store the parent-child relationship between entities.
	 */
	struct RelationshipComponent final
	{
		u64 ParentID = 0u;
		std::vector<u64> ChildrenIDs;

		RelationshipComponent() = default;
		RelationshipComponent(u64 parentID)
			: ParentID(parentID) {}

		RelationshipComponent(const RelationshipComponent& other) = default;
		RelationshipComponent(RelationshipComponent&& other) = default;
		RelationshipComponent& operator=(const RelationshipComponent& other) = default;
		RelationshipComponent& operator=(RelationshipComponent&& other) = default;
	};

	/**
	 * @brief Enum class for the type of the physics body. 
	 */
	enum class PhysicBodyType : u8
	{
		Static = 0,		/**< zero mass, zero velocity, may be manually moved */
		Kinematic,		/**< zero mass, non-zero velocity set by user, moved by the physics system */
		Dynamic			/**< positive mass, non-zero velocity determined by forces, moved by the physics system */
	};

	/**
	 * @struct RigidBody2DComponent
	 * @brief This component is used to store the physics body data.
	 * @note This component is optional.
	 */
	struct RigidBody2DComponent final
	{
		PhysicBodyType Type = PhysicBodyType::Static;	/**< By default the body is static */

		void* Handle = nullptr;		/**< Internal box2D physics body handle */

		f32 GravityScale = 1.0f;	/**< Scale the gravity applied to this body. */

		f32 Friction = 0.5f;		/**< The friction coefficient [0 - 1] range. */
		f32 Restitution = 0.f;				/**< The restitution (elasticity) [0 - 1] range. */
		f32 RestitutionThreshold = 0.5f;	/**< Restitution velocity threshold, usually in [m/s] (above this speed will bounce). */

		bool AllowSleep = true;		/**< Set this flag to false if this body should never fall asleep. */
	};

	/**
	 * @struct BoxCollider2DComponent
	 * @brief This component is used to store the box collider data.
	 * @note This component is optional.
	 */
	struct BoxCollider2DComponent final
	{
		void* Handle = nullptr;		/**< Internal box2D physics box collider handle */

		glm::vec2 Size = { 0.5f, 0.5f };	/**< Size of the collider */
		glm::vec2 Offset = { 0.0f, 0.0f };	/**< Center of the box in local coordinates */

		f32 Density = 1.f;			/**< The density of the rigid body [kg/m^2]. */

		bool IsSensor = false;      /**< A sensor shape collects contact information but never generates a collision */
	};

	/**
	 * @struct CircleCollider2DComponent
	 * @brief This component is used to store the circle collider data.
	 * @note This component is optional.
	 */
	struct CircleCollider2DComponent final
	{
		void* Handle = nullptr;		/**< Internal box2D physics circle collider handle */

		f32 Radius = 0.5f;					/**< Radius of the circle collider */
		glm::vec2 Offset = { 0.0f, 0.0f };	/**< Offset of the collider (from center) */

		f32 Density = 1.f;			/**< The density of the rigid body [kg/m^2]. */

		bool IsSensor = false;      /**< A sensor shape collects contact information but never generates a collision */
	};

	/**
	 * @struct BuoyancyEffector2DComponent
	 * @brief This component is used to store the buoyancy effector data.
	 * @note This component is optional.
	 */
	struct BuoyancyEffector2DComponent final
	{
		f32 Density = 2.f;			/**< The density of the fluid [kg/m^2]. */
		f32 DragMultiplier = 1.0f;			/**< The fluid drag multiplier. */
		f32 FlowMagnitude = 0.0f;			/**< The magnitude of the flow. */
		f32 FlowAngle = glm::radians(0.0f);	/**< The angle of the flow. */
	};

}
