Entities:
  - Entity:
      IDComponent:
        ID: 7289931465563712418
      TagComponent:
        Tag: Sprite
      TransformComponent:
        Transform: [0.990723193, 1.93443453, -0.000122071797]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      SpriteComponent:
        Color: [0.614973307, 0.161142737, 0.161142737, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0
  - Entity:
      IDComponent:
        ID: 13592262179755970519
      TagComponent:
        Tag: Camera
      TransformComponent:
        Transform: [0, 0, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      RelationshipComponent:
        ParentID: 14733869924994960009
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
        ID: 14733869924994960009
      TagComponent:
        Tag: Circle
      TransformComponent:
        Transform: [-0.699999928, -0.100000024, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      CircleComponent:
        Color: [1, 1, 1, 1]
        Thickness: 1
        Fade: 0.00499999989
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 1
        ChildrenIDs:
          0: 13592262179755970519
      ScriptComponent:
        ScriptID: 3044585441
  - Entity:
      IDComponent:
        ID: 16757084428091199902
      TagComponent:
        Tag: Sprite
      TransformComponent:
        Transform: [-3.33724976, 1.2297821, 0]
        Rotation: [0, 0, 0]
        Scale: [1, 1, 1]
      SpriteComponent:
        Color: [0.0772112608, 0.534759402, 0.248485923, 1]
      RelationshipComponent:
        ParentID: 0
        ChildrenCount: 0