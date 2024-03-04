Entities:
  - Entity:
      IDComponent:
        ID: 1189312223116170767
      TagComponent:
        Tag: Circle
      TransformComponent:
        Transform: [-0.0758367181, -2.0771749, 0]
        Rotation: [0, -0, 1.16435242]
        Scale: [0.999993205, 0.999993205, 1]
      CircleComponent:
        Color: [0.844919801, 0.207841247, 0.207841247, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 17795518619633248643
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
      CircleCollider2DComponent:
        Radius: 0.5
        Offset: [0, 0]
        Density: 1
        IsSensor: false
  - Entity:
      IDComponent:
        ID: 1707892069376765505
      TagComponent:
        Tag: Entity
      TransformComponent:
        Transform: [0.0500000007, -1.68202209, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 1
        ChildrenIDs:
          0: 17795518619633248643
  - Entity:
      IDComponent:
        ID: 3128652375786225553
      TagComponent:
        Tag: Falling
      TransformComponent:
        Transform: [-1.65367436, -0.147777051, -4.32427107e-08]
        Rotation: [-4.36827214e-08, -4.21281605e-08, -1.55023468]
        Scale: [3.00000024, 0.999995172, 1]
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
        LinearDamping: 0
        AngularDamping: 0
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
        Transform: [-0.211443484, -3.08987308, 0]
        Rotation: [0, -0, 0.0200046934]
        Scale: [7.05000162, 0.999980748, 1]
      SpriteComponent:
        Color: [0.172953203, 0.598930478, 0.195732683, 1]
      RelationshipComponent:
        ParentID: 17795518619633248643
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
      BoxCollider2DComponent:
        Size: [0.5, 0.5]
        Offset: [0, 0]
        Density: 1
        IsSensor: false
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
        Transform: [2.72695279, 1.5, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      SpriteComponent:
        Color: [1, 1, 1, 1]
      RelationshipComponent:
        ParentID: 17795518619633248643
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 0
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
      BoxCollider2DComponent:
        Size: [0.5, 0.5]
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      SpringJoint2DComponent:
        ConnectedEntityID: 3180699473944458832
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [-3, 0]
        ConnectedAnchor: [2.9000001, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1
        BreakingForce: 3.40282347e+38
        Frequency: 0.5
        DampingRatio: 0
  - Entity:
      IDComponent:
        ID: 17259964157603751307
      TagComponent:
        Tag: Joint
      TransformComponent:
        Transform: [-3.41117549, 1.5, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      SpriteComponent:
        Color: [1, 1, 1, 1]
      RelationshipComponent:
        ParentID: 17795518619633248643
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 0
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
      SpringJoint2DComponent:
        ConnectedEntityID: 3180699473944458832
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [3.0999999, 0]
        ConnectedAnchor: [-3.20000005, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1
        BreakingForce: 3.40282347e+38
        Frequency: 0.5
        DampingRatio: 0
  - Entity:
      IDComponent:
        ID: 17795518619633248643
      TagComponent:
        Tag: Couldbeprefab
      TransformComponent:
        Transform: [-0.550000012, 0.949999988, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 1707892069376765505
        ChildrenCount: 4
        ChildrenIDs:
          0: 17259964157603751307
          1: 14924479237669594077
          2: 3180699473944458832
          3: 1189312223116170767