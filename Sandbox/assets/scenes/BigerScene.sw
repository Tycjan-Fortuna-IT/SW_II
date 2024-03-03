Entities:
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
        ID: 2535477167252287573
      TagComponent:
        Tag: Left Wheel
      TransformComponent:
        Transform: [-9.5, 0.200000003, 1.49011612e-08]
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
        Density: 25
        IsSensor: false
      WheelJoint2DComponent:
        ConnectedEntityID: 4175566080875530561
        OriginAnchor: [0, 0]
        LowerTranslation: 0
        UpperTranslation: 0
        MotorSpeed: 50
        MaxMotorTorque: 45
        BreakingForce: 3.40282347e+38
        BreakingTorque: 3.40282347e+38
        Frequency: 0
        DampingRatio: 0.699999988
        EnableLimit: true
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
        GravityScale: 1
        Friction: 0.5
        Restitution: 0
        RestitutionThreshold: 0.5
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
  - Entity:
      IDComponent:
        ID: 4175566080875530561
      TagComponent:
        Tag: Body
      TransformComponent:
        Transform: [-8, 0.818944097, -0.00999999978]
        Rotation: [0, 0, 0]
        Scale: [4.47966337, 1.49409747, 1]
      SpriteComponent:
        Color: [0.786096275, 0.176556394, 0.176556394, 1]
      RelationshipComponent:
        ParentID: 18156684293390743624
        ChildrenCount: 0
      RigidBody2DComponent:
        Type: 2
        GravityScale: 1
        Friction: 0.5
        Restitution: 0.574000001
        RestitutionThreshold: 0
        LinearDamping: 0
        AngularDamping: 0
        AllowSleep: true
      BoxCollider2DComponent:
        Size: [0.5, 0.5]
        Offset: [0, 0]
        Density: 4
        IsSensor: false
  - Entity:
      IDComponent:
        ID: 5458427869381750040
      TagComponent:
        Tag: 3
      TransformComponent:
        Transform: [12.5867004, 0.366147041, 0]
        Rotation: [0, 0, 0]
        Scale: [9.05000019, 1, 1]
      SpriteComponent:
        Color: [0.411764681, 0.411764681, 0.411764681, 1]
      RelationshipComponent:
        ParentID: 1823444031938407901
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
  - Entity:
      IDComponent:
        ID: 6149229169700030249
      TagComponent:
        Tag: Platform
      TransformComponent:
        Transform: [-7.59000015, -2.06542158, 0]
        Rotation: [0, 0, 0]
        Scale: [10.8500004, 1, 1]
      SpriteComponent:
        Color: [0.919786096, 0.546390116, 0.15247792, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 2
        ChildrenIDs:
          0: 13361696574532116246
          1: 14436223244464092395
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
        ID: 6419327897363398420
      TagComponent:
        Tag: Right Wheel
      TransformComponent:
        Transform: [-6.5, 0.199938953, -0.000122070312]
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
        Density: 25
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
        Frequency: 0
        DampingRatio: 0.699999988
        EnableLimit: true
        EnableMotor: false
        EnableCollision: false
  - Entity:
      IDComponent:
        ID: 6898766488449789902
      TagComponent:
        Tag: Box Collider
      TransformComponent:
        Transform: [6.82264614, 4.31764221, 0]
        Rotation: [0, 0, 0]
        Scale: [4.69999981, 1, 1]
      SpriteComponent:
        Color: [0.184391871, 0.240056157, 0.556149721, 1]
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
        ID: 13322905985239132909
      TagComponent:
        Tag: Platform
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
  - Entity:
      IDComponent:
        ID: 13361696574532116246
      TagComponent:
        Tag: Hinge 1
      TransformComponent:
        Transform: [0, 4, 0]
        Rotation: [0, 0, 0]
        Scale: [0.300000012, 0.300000012, 1]
      CircleComponent:
        Color: [1, 0, 0, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 6149229169700030249
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
        ConnectedEntityID: 6149229169700030249
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [-3.79999995, 0]
        ConnectedAnchor: [-3.79999995, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1.5
        BreakingForce: 3.40282347e+38
        Frequency: 2
        DampingRatio: 0.150000006
  - Entity:
      IDComponent:
        ID: 14436223244464092395
      TagComponent:
        Tag: Hinge 2
      TransformComponent:
        Transform: [0, 4, 0]
        Rotation: [0, 0, 0]
        Scale: [0.300000012, 0.300000012, 1]
      CircleComponent:
        Color: [1, 0, 0, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 6149229169700030249
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
        ConnectedEntityID: 6149229169700030249
        EnableCollision: false
        AutoLength: true
        OriginAnchor: [3.70000005, 0]
        ConnectedAnchor: [3.75, 0]
        Length: 0
        MinLength: 0
        MaxLength: 1.5
        BreakingForce: 3.40282347e+38
        Frequency: 2
        DampingRatio: 0.150000006
  - Entity:
      IDComponent:
        ID: 18156684293390743624
      TagComponent:
        Tag: Car
      TransformComponent:
        Transform: [0, 0, 0]
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