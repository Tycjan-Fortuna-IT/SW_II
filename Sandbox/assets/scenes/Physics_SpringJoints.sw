Entities:
  - Entity:
      IDComponent:
        ID: 3128652375786225553
      TagComponent:
        Tag: Falling
      TransformComponent:
        Transform: [-0.274211884, -0.0432968102, 0]
        Rotation: [0, 0, 0]
        Scale: [3, 1, 1]
      SpriteComponent:
        Color: [0.0817867219, 0.588235259, 0.547610998, 1]
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
  - Entity:
      IDComponent:
        ID: 3180699473944458832
      TagComponent:
        Tag: Box Collider
      TransformComponent:
        Transform: [-0.245353639, -1.9483614, 0]
        Rotation: [0, 0, 0]
        Scale: [7.05000019, 1, 1]
      SpriteComponent:
        Color: [0.172953203, 0.598930478, 0.195732683, 1]
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
      SpringJoint2DComponent:
        ConnectedEntityID: 14924479237669594077
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [0, 0]
        ConnectedAnchor: [0, 0]
        Length: 0
        MinLength: 0
        MaxLength: 8.10000038
        BreakingForce: 3.40282347e+38
        Frequency: 0.699999988
        DampingRatio: 0
  - Entity:
      IDComponent:
        ID: 6107928204056234211
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
        ID: 14924479237669594077
      TagComponent:
        Tag: Joint
      TransformComponent:
        Transform: [-0.30308044, 1.48653698, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      SpriteComponent:
        Color: [1, 1, 1, 1]
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