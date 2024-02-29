Entities:
  - Entity:
      IDComponent:
        ID: 169886200146887300
      TagComponent:
        Tag: Box Collider
      TransformComponent:
        Transform: [-0.274217606, -2.26589489, 0]
        Rotation: [0, 0, 0]
        Scale: [15.3000002, 1, 1]
      SpriteComponent:
        Color: [0.802139044, 0.132974938, 0.132974938, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 0
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        AllowSleep: true
      BoxCollider2DComponent:
        Size: [0.5, 0.5]
        Offset: [0, 0]
        Density: 1
        IsSensor: false
  - Entity:
      IDComponent:
        ID: 4094335318453348632
      TagComponent:
        Tag: Wheel 1
      TransformComponent:
        Transform: [-2.4667778, -0.30221653, 0.300000012]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      CircleComponent:
        Color: [1, 1, 1, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        AllowSleep: true
      CircleCollider2DComponent:
        Radius: 0.5
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      WheelJoint2DComponent:
        ConnectedEntityID: 13565467309497251182
        OriginAnchor: [0, 0]
        LowerTranslation: -0.25
        UpperTranslation: 0.25
        MotorSpeed: 3.5
        MaxMotorTorque: 20
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        Frequency: 28.6000004
        DampingRatio: 0.699999988
        EnableLimit: true
        EnableMotor: true
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 6985190472811580254
      TagComponent:
        Tag: Circle Collider
      TransformComponent:
        Transform: [1.9567337, -0.28786087, 0.100000001]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      CircleComponent:
        Color: [1, 1, 1, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        AllowSleep: true
      CircleCollider2DComponent:
        Radius: 0.5
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      WheelJoint2DComponent:
        ConnectedEntityID: 13565467309497251182
        OriginAnchor: [0, 0]
        LowerTranslation: -0.25
        UpperTranslation: 0.25
        MotorSpeed: 3.5
        MaxMotorTorque: 20
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        Frequency: 26.6000004
        DampingRatio: 0.699999988
        EnableLimit: true
        EnableMotor: true
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 9518401258470154822
      TagComponent:
        Tag: Camera
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
      CameraComponent:
        Primary: true
        AspectRatio: 1.99825788
  - Entity:
      IDComponent:
        ID: 13565467309497251182
      TagComponent:
        Tag: Box Collider
      TransformComponent:
        Transform: [-0.259782791, 1.38551867, 0]
        Rotation: [0, 0, 0]
        Scale: [5.25, 2.54999995, 1]
      SpriteComponent:
        Color: [0.0535331368, 0.625668406, 0.598132491, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        AllowSleep: true
      BoxCollider2DComponent:
        Size: [0.5, 0.5]
        Offset: [0, 0]
        Density: 1
        IsSensor: false