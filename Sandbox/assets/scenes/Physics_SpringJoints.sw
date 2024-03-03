Entities:
  - Entity:
      IDComponent:
        ID: 3128652375786225553
      TagComponent:
        Tag: Falling
      TransformComponent:
        Transform: [-0.215145126, 1.43342018, 0]
        Rotation: [-4.36827214e-08, -4.21281747e-08, -1.53456891]
        Scale: [3.00000024, 0.999999762, 1]
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
        Transform: [-0.229820192, -1.9483614, 0]
        Rotation: [0, 0, 0]
        Scale: [7.05000019, 1, 1]
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
        Transform: [1.1615231, 1.5, 0]
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
        OriginAnchor: [0, 0]
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
        Transform: [-1.69863582, 1.5, 0]
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
        OriginAnchor: [0, 0]
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
        Transform: [0, -1.39999998, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 3
        ChildrenIDs:
          0: 17259964157603751307
          1: 14924479237669594077
          2: 3180699473944458832