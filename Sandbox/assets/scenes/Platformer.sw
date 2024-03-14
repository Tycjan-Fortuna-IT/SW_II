Entities:
  - Entity:
      IDComponent:
        ID: 3771982976849701410
      TagComponent:
        Tag: Floor 2
      TransformComponent:
        Transform: [7.43335533, -1.90887392, 0.000122070312]
        Rotation: [8.37404457e-09, -1.20068289e-09, 0.28482154]
        Scale: [8, 1, 1]
      SpriteComponent:
        Color: [0.541996062, 0.743315518, 0.254396766, 1]
      RelationshipComponent:
        ParentID: 10496884223937514442
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
  - Entity:
      IDComponent:
        ID: 7447106538690029325
      TagComponent:
        Tag: Camera
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 8745615158125158202
        ChildrenCount: 0
      CameraComponent:
        Primary: true
        AspectRatio: 2.09269166
        ProjectionType: 1
        OrthographicSize: 10
        OrthographicNearClip: -1
        OrthographicFarClip: 1
  - Entity:
      IDComponent:
        ID: 8745615158125158202
      TagComponent:
        Tag: Player
      TransformComponent:
        Transform: [0, 0.0147704929, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      SpriteComponent:
        Color: [1, 1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 1
        ChildrenIDs:
          0: 7447106538690029325
      ScriptComponent:
        ScriptID: 3044585441
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
  - Entity:
      IDComponent:
        ID: 10496884223937514442
      TagComponent:
        Tag: Platforms
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 2
        ChildrenIDs:
          0: 17234884401592776596
          1: 3771982976849701410
  - Entity:
      IDComponent:
        ID: 17234884401592776596
      TagComponent:
        Tag: Floor 1
      TransformComponent:
        Transform: [-0.251487792, -3.01226807, 0]
        Rotation: [0, 0, 0]
        Scale: [8, 1, 1]
      SpriteComponent:
        Color: [0.541996062, 0.743315518, 0.254396766, 1]
      RelationshipComponent:
        ParentID: 10496884223937514442
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