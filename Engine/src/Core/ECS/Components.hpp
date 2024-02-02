/**
 * @file Components.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Vector2.hpp"
#include "Core/Math/Vector3.hpp"
#include "Core/Math/Vector4.hpp"
#include "Core/Utils/Random.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/Math/Math.hpp"
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
        Vector3<f32> Position = { 0.0f, 0.0f , 0.0f };
        Vector3<f32> Rotation = { 0.0f, 0.0f , 0.0f };		/** @brief Rotation angles are in radians! */
        Vector3<f32> Scale =    { 1.0f, 1.0f , 1.0f };

        TransformComponent() = default;
        TransformComponent(const Vector3<f32>& position, const Vector3<f32>& rotation, const Vector3<f32>& scale)
            : Position(position), Rotation(rotation), Scale(scale) {}
        TransformComponent(const Vector3<f32>& position)
            : Position(position) {}

        TransformComponent(const TransformComponent& other) = default;
        TransformComponent(TransformComponent&& other) = default;
        TransformComponent& operator=(const TransformComponent& other) = default;
        TransformComponent& operator=(TransformComponent&& other) = default;

        ~TransformComponent() = default;

		Matrix4<f32> GetTransform() const
		{
			return Math::Translate(Matrix4<f32>::Identity(), Position) *
				Math::RotateZ(Matrix4<f32>::Identity(), Rotation.z) *
				Math::Scale(Matrix4<f32>::Identity(), Scale);
		}
    };

    /**
     * @struct SpriteComponent
     * @brief This component is used to store the color of an entity. This component is used for rendering.
     * @note This component is optional.
     */
    struct SpriteComponent final
    {
        Vector4<f32> Color = { 1.0f };
		Texture2D* Texture = nullptr;	/** @brief Texture of the entity, can be nullptr! */
		f32 TilingFactor = 1.0f;

        SpriteComponent() = default;
        SpriteComponent(const Vector4<f32>& color)
            : Color(color) {}
		SpriteComponent(Texture2D* texture, const Vector4<f32>& tint = { 1.f }, f32 tilingFactor = 1.f)
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

		Vector2<f32> Size = { 0.5f, 0.5f };		/**< Size of the collider */
		Vector2<f32> Offset = { 0.0f, 0.0f };	/**< Center of the box in local coordinates */
	};

}
