Entities:
  - Entity:
      IDComponent:
        ID: 1804136898320342281
      TagComponent:
        Tag: Right Line
      TransformComponent:
        Transform: [3, 3, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 9189886445870706617
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
      SpringJoint2DComponent:
        ConnectedEntityID: 11261900507726111826
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [0, 0]
        ConnectedAnchor: [3, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1
        BreakingForce: 3.40282347e+38
        Frequency: 2
        DampingRatio: 0.5
  - Entity:
      IDComponent:
        ID: 1823444031938407901
      TagComponent:
        Tag: Floor
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 3
        ChildrenIDs:
          0: 18331807443577825100
          1: 10022383356269630104
          2: 5458427869381750040
  - Entity:
      IDComponent:
        ID: 2131269139217275618
      TagComponent:
        Tag: Right Wheel
      TransformComponent:
        Transform: [-6.5, 0.199939013, -0.000122070312]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 5.07999945]
      CircleComponent:
        Color: [0.392156869, 0.392156869, 0.392156869, 1]
        Thickness: 1
        Fade: 0
      RelationshipComponent:
        ParentID: 15633082648636036500
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
      CircleCollider2DComponent:
        Radius: 0.5
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      WheelJoint2DComponent:
        ConnectedEntityID: 3643749894792680226
        OriginAnchor: [0, 0]
        LowerTranslation: 0
        UpperTranslation: 0
        MotorSpeed: 10
        MaxMotorTorque: 20
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        Frequency: 10
        DampingRatio: 0.699999988
        EnableLimit: false
        EnableMotor: false
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 2535477167252287573
      TagComponent:
        Tag: Left Wheel
      TransformComponent:
        Transform: [-9.50000191, 0.219395161, -0.00012205659]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1.04558396]
      CircleComponent:
        Color: [0.392156869, 0.392156869, 0.392156869, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 18156684293390743624
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
      CircleCollider2DComponent:
        Radius: 0.5
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      WheelJoint2DComponent:
        ConnectedEntityID: 4175566080875530561
        OriginAnchor: [0, 0]
        LowerTranslation: 0
        UpperTranslation: 0
        MotorSpeed: 5
        MaxMotorTorque: 12.5
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        Frequency: 10
        DampingRatio: 0
        EnableLimit: false
        EnableMotor: true
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 2720753100678130970
      TagComponent:
        Tag: Sprite
      TransformComponent:
        Transform: [-0.903770447, 4.77804565, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
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
  - Entity:
      IDComponent:
        ID: 3225848602235102333
      TagComponent:
        Tag: Left Wheel
      TransformComponent:
        Transform: [-9.50000191, 0.219395161, -0.00012205659]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1.04558396]
      CircleComponent:
        Color: [0.392156869, 0.392156869, 0.392156869, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 15633082648636036500
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
      CircleCollider2DComponent:
        Radius: 0.5
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      WheelJoint2DComponent:
        ConnectedEntityID: 3643749894792680226
        OriginAnchor: [0, 0]
        LowerTranslation: 0
        UpperTranslation: 0
        MotorSpeed: 5
        MaxMotorTorque: 12.5
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        Frequency: 10
        DampingRatio: 0
        EnableLimit: false
        EnableMotor: true
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 3643749894792680226
      TagComponent:
        Tag: Body
      TransformComponent:
        Transform: [-8, 1.03081608, -0.00999999978]
        Rotation: [0, 0, 0]
        Scale: [4.47966337, 1.49409747, 1]
      SpriteComponent:
        Color: [0.786096275, 0.176556394, 0.176556394, 1]
      RelationshipComponent:
        ParentID: 15633082648636036500
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        AutoMass: true
        Mass: 1
        GravityScale: 1
        Friction: 0.5
        Restitution: 0.574000001
        RestitutionThreshold: 0
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
        ID: 4175566080875530561
      TagComponent:
        Tag: Body
      TransformComponent:
        Transform: [-8, 1.03081608, -0.00999999978]
        Rotation: [0, 0, 0]
        Scale: [4.47966337, 1.49409747, 1]
      SpriteComponent:
        Color: [0.786096275, 0.176556394, 0.176556394, 1]
      RelationshipComponent:
        ParentID: 18156684293390743624
        ChildrenCount: 0
      ScriptComponent:
        ScriptID: 983274863
      RigidBody2DComponent:
        Type: 2
        AutoMass: true
        Mass: 1
        GravityScale: 1
        Friction: 0.5
        Restitution: 0.574000001
        RestitutionThreshold: 0
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
        ID: 4238985469750613728
      TagComponent:
        Tag: Platform
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [7.55000019, 1, 1]
      SpriteComponent:
        Color: [0.807486653, 0.700111151, 0.159770086, 1]
      RelationshipComponent:
        ParentID: 15895639110335943461
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
  - Entity:
      IDComponent:
        ID: 4640612343552914116
      TagComponent:
        Tag: Left Line
      TransformComponent:
        Transform: [0, 3, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 9189886445870706617
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
      SpringJoint2DComponent:
        ConnectedEntityID: 11261900507726111826
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [-3, 0]
        ConnectedAnchor: [-3, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1
        BreakingForce: 3.40282347e+38
        Frequency: 2
        DampingRatio: 0.5
  - Entity:
      IDComponent:
        ID: 5458427869381750040
      TagComponent:
        Tag: 3
      TransformComponent:
        Transform: [12.7799997, 0.389999986, 0]
        Rotation: [0, 0, 0]
        Scale: [9.05000019, 1, 1]
      SpriteComponent:
        Color: [0.411764681, 0.411764681, 0.411764681, 1]
      RelationshipComponent:
        ParentID: 1823444031938407901
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
        ID: 5701364074816771619
      TagComponent:
        Tag: Left Line
      TransformComponent:
        Transform: [0, 3, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 15895639110335943461
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
      SpringJoint2DComponent:
        ConnectedEntityID: 4238985469750613728
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [-3, 0]
        ConnectedAnchor: [-3, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1
        BreakingForce: 3.40282347e+38
        Frequency: 2
        DampingRatio: 0.5
  - Entity:
      IDComponent:
        ID: 6071787040322477704
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
        AspectRatio: 2.07308388
        ProjectionType: 1
        OrthographicSize: 10
        OrthographicNearClip: -1
        OrthographicFarClip: 1
  - Entity:
      IDComponent:
        ID: 6419327897363398420
      TagComponent:
        Tag: Right Wheel
      TransformComponent:
        Transform: [-6.5, 0.199939013, -0.000122070312]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 5.07999945]
      CircleComponent:
        Color: [0.392156869, 0.392156869, 0.392156869, 1]
        Thickness: 1
        Fade: 0
      RelationshipComponent:
        ParentID: 18156684293390743624
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
      CircleCollider2DComponent:
        Radius: 0.5
        Offset: [0, 0]
        Density: 1
        IsSensor: false
      WheelJoint2DComponent:
        ConnectedEntityID: 4175566080875530561
        OriginAnchor: [0, 0]
        LowerTranslation: 0
        UpperTranslation: 0
        MotorSpeed: 10
        MaxMotorTorque: 20
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        Frequency: 10
        DampingRatio: 0.699999988
        EnableLimit: false
        EnableMotor: false
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 6898766488449789902
      TagComponent:
        Tag: Box Collider
      TransformComponent:
        Transform: [6.82264614, 5.4365387, 0]
        Rotation: [0, 0, 0]
        Scale: [4.69999981, 1, 1]
      SpriteComponent:
        Color: [0.184391871, 0.240056157, 0.556149721, 1]
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
      RevolutionJoint2DComponent:
        ConnectedEntityID: 2720753100678130970
        OriginAnchor: [0, 0]
        LowerAngle: 0
        UpperAngle: 6.26573181
        MotorSpeed: 5
        MaxMotorTorque: 10000
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        EnableLimit: false
        EnableMotor: true
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 9189886445870706617
      TagComponent:
        Tag: HingePlatform
      TransformComponent:
        Transform: [-11.3779449, -0.300000012, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 3
        ChildrenIDs:
          0: 4640612343552914116
          1: 11261900507726111826
          2: 1804136898320342281
  - Entity:
      IDComponent:
        ID: 10022383356269630104
      TagComponent:
        Tag: 2
      TransformComponent:
        Transform: [4.11902332, -0.624212444, 0]
        Rotation: [0, 0, 0.238070816]
        Scale: [8.74999809, 1, 1]
      SpriteComponent:
        Color: [0.411764711, 0.411764711, 0.411764711, 1]
      RelationshipComponent:
        ParentID: 1823444031938407901
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
        ID: 11261900507726111826
      TagComponent:
        Tag: Platform
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [7.55000019, 1, 1]
      SpriteComponent:
        Color: [0.807486653, 0.700111151, 0.159770086, 1]
      RelationshipComponent:
        ParentID: 9189886445870706617
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
  - Entity:
      IDComponent:
        ID: 15633082648636036500
      TagComponent:
        Tag: Car
      TransformComponent:
        Transform: [11.1499996, 2.1500001, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 3
        ChildrenIDs:
          0: 3225848602235102333
          1: 2131269139217275618
          2: 3643749894792680226
  - Entity:
      IDComponent:
        ID: 15895639110335943461
      TagComponent:
        Tag: HingePlatform
      TransformComponent:
        Transform: [-3.56999993, -0.300000012, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 3
        ChildrenIDs:
          0: 5701364074816771619
          1: 4238985469750613728
          2: 18383681532785808073
  - Entity:
      IDComponent:
        ID: 18156684293390743624
      TagComponent:
        Tag: Car
      TransformComponent:
        Transform: [3.6500001, 2.1500001, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 3
        ChildrenIDs:
          0: 2535477167252287573
          1: 6419327897363398420
          2: 4175566080875530561
  - Entity:
      IDComponent:
        ID: 18331807443577825100
      TagComponent:
        Tag: 1
      TransformComponent:
        Transform: [9.59017849, -5.32030487, 0]
        Rotation: [0, 0, 0]
        Scale: [10.8000002, 1, 1]
      SpriteComponent:
        Color: [0.85561496, 0.352312028, 0.352312028, 1]
      RelationshipComponent:
        ParentID: 1823444031938407901
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
        ID: 18383681532785808073
      TagComponent:
        Tag: Right Line
      TransformComponent:
        Transform: [3, 3, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 15895639110335943461
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
      SpringJoint2DComponent:
        ConnectedEntityID: 4238985469750613728
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [0, 0]
        ConnectedAnchor: [3, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1
        BreakingForce: 3.40282347e+38
        Frequency: 2
        DampingRatio: 0.5