/**
 * @file Components.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.6
 * @date 2024-03-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Utils/Random.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/Math/Math.hpp"
#include "Core/Scripting/CSharpObject.hpp"
#include "Asset/Asset.hpp"
#include "Asset/Animation2D.hpp"
#include "Audio/SoundInstance.hpp"
#include "Audio/SoundListener.hpp"

namespace SW {

    /**
     * @struct IDComponent
     * @brief This component is used to identify an entity. Each entity has a unique ID.
     * @note This component is required for each entity. ID = 0 is reserved for null or invalid entity.
     */
    struct IDComponent
    {
        u64 ID = 0;

        IDComponent()
            : ID(Random::CreateID()) {}
        IDComponent(u64 id)
            : ID(id) {}
    };

    /**
     * @struct TagComponent
     * @brief This component is used to identify an entity by a tag.
     * @note This component is required for each entity.
     */
    struct TagComponent
    {
        std::string Tag = "No tag";

        TagComponent() = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}
    };

    /**
     * @struct TransformComponent
     * @brief This component is used to store the position, rotation and scale of an entity.
     * @note This component is required for each entity.
     */
    struct TransformComponent
    {
        glm::vec3 Position = { 0.0f, 0.0f , 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f , 0.0f };		/** @brief Rotation angles are in radians! */
        glm::vec3 Scale =    { 1.0f, 1.0f , 1.0f };

        TransformComponent() = default;
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
            : Position(position), Rotation(rotation), Scale(scale) {}
        TransformComponent(const glm::vec3& position)
            : Position(position) {}

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
     */
    struct SpriteComponent
    {
        glm::vec4 Color = glm::vec4(1.0f);
		AssetHandle Handle = 0u; // sprite handle
		f32 TilingFactor = 1.0f;
		int ZIndex = 0;

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color)
            : Color(color) {}
		SpriteComponent(AssetHandle handle, const glm::vec4& tint = glm::vec4(1.f), f32 tilingFactor = 1.f)
			: Color(tint), Handle(handle), TilingFactor(tilingFactor) {}
    };

	/**
	 * @struct AnimatedSpriteComponent
	 * @brief This component is used to store the animation data of an entity.
	 */
	struct AnimatedSpriteComponent
	{
		int CurrentFrame = 0u;

		Animation2D** DefaultAnimation = nullptr;

		Animation2D** CurrentAnimation = nullptr;

		std::unordered_map<std::string, Animation2D**> Animations;
	};

	/**
	 * @struct CircleComponent
	 * @brief This component is used to store the color of an entity. This component is used for rendering of circles and rings.
	 */
	struct CircleComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);
		f32 Thickness = 1.0f;
		f32 Fade = 0.005f;

		CircleComponent() = default;
		CircleComponent(const glm::vec4& color)
			: Color(color) {}
	};

	/**
	 * @struct TextComponent
	 * @brief This component is used to store the text of an entity. This component is used for rendering of text.
	 */
	struct TextComponent
	{
		std::string TextString = "Empty";	/**< Text to be rendered. */

		AssetHandle Handle = 0u; // font handle

		glm::vec4 Color = glm::vec4(1.0f);	/**< Color of the text. */

		f32 Kerning = 0.0f;			/**< Kerning of the text. */
		f32 LineSpacing = 0.0f;		/**< Line spacing of the text. */
	};

	/**
	 * @struct CameraComponent
	 * @brief This component is used to store the camera's data with position's attached to the component.
	 * @note However without even single camera the scene will not be visible!
	 */
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;	/** @brief In case of many cameras only the first primary one will be used. */

		CameraComponent() = default;
		CameraComponent(const SceneCamera& camera)
			: Camera(camera) {}
	};

	/**
	 * @struct RelationshipComponent
	 * @brief This component is used to store the parent-child relationship between entities.
	 * @note This component is used to create a hierarchy of entities. It is required.
	 */
	struct RelationshipComponent
	{
		u64 ParentID = 0u;
		std::vector<u64> ChildrenIDs;

		RelationshipComponent() = default;
		RelationshipComponent(u64 parentID)
			: ParentID(parentID) {}
	};

	/**
	 * @struct NativeScriptComponent
	 * @brief This component is used to store the native script data.
	 */
	struct ScriptComponent
	{
		u64 ScriptID = 0;			/** @brief Unique ID of the script. */
		
		CSharpObject Instance;		/** @brief Instance of the script. */

		std::vector<u32> FieldIDs;	/** @brief IDs of the fields. */
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
	 */
	struct RigidBody2DComponent
	{
		PhysicBodyType Type = PhysicBodyType::Static;	/**< By default the body is static */

		void* Handle = nullptr;				/**< Internal box2D physics body handle */

		f32 GravityScale = 1.0f;			/**< Scale the gravity applied to this body. */

		f32 Mass = 1.0f;					/**< The mass of the body. [0-oo] */
		f32 Friction = 0.5f;				/**< The friction coefficient [0 - 1] range. */
		f32 Restitution = 0.f;				/**< The restitution (elasticity) [0 - 1] range. */
		f32 RestitutionThreshold = 0.5f;	/**< Restitution velocity threshold, usually in [m/s] (above this speed will bounce). */
		f32 LinearDamping = 0.0f;			/**< The linear damping of the rigid body. */
		f32 AngularDamping = 0.0f;			/**< The angular damping of the rigid body. */

		bool AutoMass = true;				/**< Whether the body's mass should be determined automatically or based on set mass */
		bool AllowSleep = true;				/**< Set this flag to false if this body should never fall asleep. */
		bool InitiallyAwake = true;			/**< Is this body initially awake or sleeping? */
		bool FixedRotation = false;			/**< Should this body be prevented from rotating? Useful for characters. */
		bool IsBullet = false;				/**< Is this a fast moving body that should be prevented from tunneling through other moving bodies? */
	};

	/**
	 * @struct BoxCollider2DComponent
	 * @brief This component is used to store the box collider data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct BoxCollider2DComponent
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
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct CircleCollider2DComponent
	{
		void* Handle = nullptr;		/**< Internal box2D physics circle collider handle */

		f32 Radius = 0.5f;					/**< Radius of the circle collider */
		glm::vec2 Offset = { 0.0f, 0.0f };	/**< Offset of the collider (from center) */

		f32 Density = 1.f;			/**< The density of the rigid body [kg/m^2]. */

		bool IsSensor = false;      /**< A sensor shape collects contact information but never generates a collision */
	};

	/**
	 * @struct PolygonCollider2DComponent
	 * @brief This component is used to store the polygon collider data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct PolygonCollider2DComponent
	{
		void* Handle = nullptr;		/**< Internal box2D physics polygon collider handle */

		std::vector<glm::vec2> Vertices = { { 0.0f, 0.0f }, { 1.0, 0.0f }, { 0.0f, 1.0f } };	/**< Vertices of the polygon collider */

		glm::vec2 Offset = { 0.0f, 0.0f };	/**< Offset of the collider (from center) */

		f32 Density = 1.f;			/**< The density of the rigid body [kg/m^2]. */

		bool IsSensor = false;      /**< A sensor shape collects contact information but never generates a collision */
	};

	/**
	 * @struct BuoyancyEffector2DComponent
	 * @brief This component is used to store the buoyancy effector data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct BuoyancyEffector2DComponent
	{
		f32 Density = 2.f;			/**< The density of the fluid [kg/m^2]. */
		f32 DragMultiplier = 1.0f;			/**< The fluid drag multiplier. */
		f32 FlowMagnitude = 0.0f;			/**< The magnitude of the flow. */
		f32 FlowAngle = glm::radians(0.0f);	/**< The angle of the flow. */
	};

	/**
	 * @struct DistanceJoint2DComponent
	 * @brief This component is used to store the joint data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct DistanceJoint2DComponent 
	{
		void* RuntimeJoint = nullptr;

		u64 ConnectedEntityID = 0;	/** The entity to which the joint is connected. */

		bool EnableCollision = false; 	/**< Enable collision between connected bodies. */
		bool AutoLength = true;			/**< Automatically calculate the length of the joint. */

		glm::vec2 OriginAnchor = glm::vec2(0.f);		/**< The anchor point on the first body. */
		glm::vec2 ConnectedAnchor = glm::vec2(0.f);		/**< The anchor point on the second body. */

		f32 Length = 0.f;				/**< The length of the joint. */
		f32 MinLength = 0.f;			/**< The minimum length of the joint. */
		f32 MaxLength = 1.f;			/**< The maximum length of the joint. */
		f32 BreakingForce = FLT_MAX;	/**< The breaking force of the joint. */
	};

	/**
	 * @struct RevolutionJoint2DComponent
	 * @brief This component is used to store the joint data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct RevolutionJoint2DComponent
	{
		void* RuntimeJoint = nullptr;

		u64 ConnectedEntityID = 0;		/** The entity to which the joint is connected. */
		
		glm::vec2 OriginAnchor = glm::vec2(0.0f);	/**< The anchor point on the first body. */

		f32 LowerAngle = glm::radians(0.0f);		/**< The lower angle limit. */
		f32 UpperAngle = glm::radians(359.0f);		/**< The upper angle limit. */
		f32 MotorSpeed = 5.0f;			/**< The motor speed. (in radians per second) */
		f32 MaxMotorTorque = 10000.0f;	/**< The maximum motor torque. */
		f32 BreakingForce = FLT_MAX;	/**< The breaking force of the joint. */
		f32 BreakingTorque = FLT_MAX;	/**< The breaking torque of the joint. */

		bool EnableLimit = false;		/**< Enable the joint limit. */
		bool EnableMotor = false;		/**< Enable the joint motor. */
		bool EnableCollision = false;	/**< Enable collision between connected bodies. */
	};

	/**
	 * @struct PrismaticJoint2DComponent
	 * @brief This component is used to store the joint data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct PrismaticJoint2DComponent
	{
		void* RuntimeJoint = nullptr;

		u64 ConnectedEntityID = 0;		/** The entity to which the joint is connected. */

		glm::vec2 OriginAnchor = glm::vec2(0.0f);	/**< The anchor point on the first body. */

		f32 Angle = 0.0f;				/**< The angle of the joint. */
		f32 LowerTranslation = 0.0f;	/**< The lower translation limit. */
		f32 UpperTranslation = 0.0f;	/**< The upper translation limit. */
		f32 MotorSpeed = 5.0f;			/**< The motor speed. */
		f32 MaxMotorForce = 20.0f;		/**< The maximum motor force. */
		f32 BreakingForce = FLT_MAX;	/**< The breaking force of the joint. */
		f32 BreakingTorque = FLT_MAX;	/**< The breaking torque of the joint. */

		bool EnableLimit = false;		/**< Enable the joint limit. */
		bool EnableMotor = false;		/**< Enable the joint motor. */
		bool EnableCollision = false;	/**< Enable collision between connected bodies. */
	};

	/**
	 * @struct SpringJoint2DComponent
	 * @brief This component is used to store the joint data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct SpringJoint2DComponent
	{
		void* RuntimeJoint = nullptr;

		u64 ConnectedEntityID = 0;	/** The entity to which the joint is connected. */

		bool EnableCollision = false; 	/**< Enable collision between connected bodies. */
		bool AutoLength = true;			/**< Automatically calculate the length of the joint. */

		glm::vec2 OriginAnchor = glm::vec2(0.f);		/**< The anchor point on the first body. */
		glm::vec2 ConnectedAnchor = glm::vec2(0.f);		/**< The anchor point on the second body. */

		f32 Length = 0.f;				/**< The length of the joint. */
		f32 MinLength = 0.f;			/**< The minimum length of the joint. */
		f32 MaxLength = 1.f;			/**< The maximum length of the joint. */
		f32 BreakingForce = FLT_MAX;	/**< The breaking force of the joint. */
		f32 Frequency = 4.f;			/**< The frequency of the spring. */
		f32 DampingRatio = 0.5f;		/**< The damping ratio of the spring. */
	};

	/**
	 * @struct WheelJoint2DComponent
	 * @brief This component is used to store the joint data.
	 * @warning Rigidbody2DComponent is required for this component to work properly.
	 */
	struct WheelJoint2DComponent
	{
		void* RuntimeJoint = nullptr;

		u64 ConnectedEntityID = 0;		/** The entity to which the joint is connected. */

		glm::vec2 OriginAnchor = glm::vec2(0.0f);	/**< The anchor point on the first body. */

		f32 Frequency = 4.0f;				/**< The frequency of the spring. */
		f32 DampingRatio = 0.7f;			/**< The damping ratio of the spring. */
		f32 LowerTranslation = -0.25f;		/**< The lower translation limit. */	
		f32 UpperTranslation = 0.25f;		/**< The upper translation limit. */
		f32 MotorSpeed = 10.0f;				/**< The motor speed. */
		f32 MaxMotorTorque = 20.0f;			/**< The maximum motor torque. */
		f32 BreakingForce = FLT_MAX;		/**< The breaking force of the joint. */
		f32 BreakingTorque = FLT_MAX;		/**< The breaking torque of the joint. */

		bool EnableLimit = true;		/**< Enable the joint limit. */
		bool EnableMotor = true;		/**< Enable the joint motor. */
		bool EnableCollision = false;	/**< Enable collision between connected bodies. */
	};

	/**
	 * @struct AudioSourceComponent
	 * @brief This component is used to store the audio source data.
	 */
	struct AudioSourceComponent
	{
		AssetHandle Handle = 0u;			/**< Audio handle */
		SoundInstance** Instance = nullptr;	/**< Audio instance Can be nullptr if the audio is not playing!!! */

		f32 Volume = 1.0f;			/**< Volume of the audio source. */
		f32 Pitch = 1.0f;			/**< Pitch of the audio source. */

		bool Looping = false;		/**< Is the audio source looping. */
		bool PlayOnCreate = false;	/**< Should the audio source play on creation. */
		
		bool Is3D = false;
		AttenuationType Attenuation = AttenuationType::None;
		
		f32 RollOff = 1.0f;
		f32 MinGain = 0.0f;
		f32 MaxGain = 1.0f;
		f32 MinDistance = 0.3f;
		f32 MaxDistance = 100.0f;
		f32 ConeInnerAngle = glm::radians(360.0f);
		f32 ConeOuterAngle = glm::radians(360.0f);
		f32 ConeOuterGain = 0.0f;
		
		f32 DopplerFactor = 1.0f;
	};

	/**
	 * @struct AudioListenerComponent
	 * @brief This component is used to store the audio listener data.
	 */
	struct AudioListenerComponent
	{
		SoundListener* Listener = nullptr;
		f32 ConeInnerAngle = glm::radians(360.0f);
		f32 ConeOuterAngle = glm::radians(360.0f);
		f32 ConeOuterGain = 0.0f;
	};
}
