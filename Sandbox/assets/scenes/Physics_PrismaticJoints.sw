Entities:
  - Entity:
      IDComponent:
        ID: 3181020117429068377
      TagComponent:
        Tag: Box Collider
      TransformComponent:
        Transform: [0.0432991907, 0.278119802, 0]
        Rotation: [0, 0, 0]
        Scale: [5.6500001, 1, 1]
      SpriteComponent:
        Color: [0.324133754, 0.284938097, 0.56684494, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        AutoMass: true
        Mass: 1
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
        InitiallyAwake: true
        IsBullet: false
      BoxCollider2DComponent:
        Size: [0.5, 0.5]
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      PrismaticJoint2DComponent:
        ConnectedEntityID: 11381743344894021134
        OriginAnchor: [0, 0]
        Angle: 0
        LowerTranslation: -1.60000002
        UpperTranslation: 2.4000001
        MotorSpeed: 5
        MaxMotorForce: 20
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        EnableLimit: true
        EnableMotor: true
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 7537412498907422467
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
        ProjectionType: 1
        OrthographicSize: 10
        OrthographicNearClip: -1
        OrthographicFarClip: 1
  - Entity:
      IDComponent:
        ID: 11381743344894021134
      TagComponent:
        Tag: Joint
      TransformComponent:
        Transform: [0.0865944624, 0.606155396, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      SpriteComponent:
        Color: [0.786096275, 0.0210186336, 0.0210186336, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 0
        AutoMass: true
        Mass: 1
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
        InitiallyAwake: true
        IsBullet: false
      BoxCollider2DComponent:
        Size: [0.5, 0.5]
        Offset: [0, 0]
        Density: 1
        IsSensor: false