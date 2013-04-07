var project = {
  types: [
    
      {
        name: "enable_if",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "is_same",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "is_base_of",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Serializable",
        decl: "struct",
        methods: [
          
            {
              name:   "Serialize",
              params: "Packet&",
              attrs:  68,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Packet&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Packet",
        decl: "class",
        methods: [
          
            {
              name:   "raw",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "char",
              return_attrs: 5
            },
          
            {
              name:   "size",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "size_t",
              return_attrs: 0
            },
          
            {
              name:   "PrintContent",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PrintRawContent",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "T& v",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SerializeArray",
              params: "T& tehList",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "canIHaz",
              params: "size_t sizeType,int howMany",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "checkType",
              params: "int assumedType",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "realloc",
              params: "int newsize",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "updateHeader",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "read",
              params: "T& tmp",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "typeCode",
            attrs: 1
          },
          
          {
            name: "copy",
            attrs: 1
          },
          
          {
            name: "typeCode",
            attrs: 1
          },
          
          {
            name: "copy",
            attrs: 1
          },
          
          {
            name: "size",
            attrs: 8
          },
          
          {
            name: "it",
            attrs: 0
          },
          
          {
            name: "reading",
            attrs: 0
          },
          
          {
            name: "size",
            attrs: 8
          },
          
          {
            name: "it",
            attrs: 0
          },
          
          {
            name: "reading",
            attrs: 0
          },
          
          {
            name: "isDuplicate",
            attrs: 0
          },
          
          {
            name: "buffer",
            attrs: 1
          },
          
          {
            name: "sizeBuffer",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Packet::SelectUnserializer",
        decl: "struct",
        methods: [
          
            {
              name:   "Func",
              params: "Utils::Packet& packet,T& v",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Packet::TypeToCode",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "FBoundingBox",
        decl: "struct",
        methods: [
          
            {
              name:   "Intersects",
              params: "float x,float y",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "left",
            attrs: 0
          },
          
          {
            name: "top",
            attrs: 0
          },
          
          {
            name: "width",
            attrs: 0
          },
          
          {
            name: "height",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Waypoint",
        decl: "struct",
        methods: [
          
            {
              name:   "WithdrawArc",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnwithdrawArc",
              params: "Waypoint* other,ArcObserver* observer",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWithdrawable",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "no description",
              return_type: "",
              return_attrs: 1
            },
          
            {
              name:   "ConnectUnsafe",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "no description",
              return_type: "Arcs::iterator",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "no description",
              return_type: "Arcs::iterator",
              return_attrs: 0
            },
          
            {
              name:   "Disconnect",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "no description",
              return_type: "Arcs::iterator",
              return_attrs: 0
            },
          
            {
              name:   "DisconnectAll",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetArcTo",
              params: "unsigned int id",
              attrs:  0,
              desc:   "no description",
              return_type: "Waypoint::Arc",
              return_attrs: 1
            },
          
            {
              name:   "PositionChanged",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateArcDirection",
              params: "Waypoint*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSelected",
              params: "bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsSelected",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetArcsVisible",
              params: "bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoalDistanceEstimate",
              params: "const Waypoint& goal",
              attrs:  4,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "GetDistanceEstimate",
              params: "const Waypoint& other",
              attrs:  4,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "GetCost",
              params: "Waypoint&",
              attrs:  0,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "LoadArcs",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnserializeLoadArcs",
              params: "World*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetMouseBox",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "observer",
            attrs: 1
          },
          
          {
            name: "id",
            attrs: 8
          },
          
          {
            name: "floor",
            attrs: 8
          },
          
          {
            name: "arcs",
            attrs: 0
          },
          
          {
            name: "arcs_withdrawed",
            attrs: 0
          },
          
          {
            name: "nodePath",
            attrs: 0
          },
          
          {
            name: "mouseBox",
            attrs: 0
          },
          
          {
            name: "World",
            attrs: 0
          },
          
          {
            name: "selected",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Waypoint::ArcObserver",
        decl: "struct",
        methods: [
          
            {
              name:   "CanGoThrough",
              params: "unsigned char type",
              attrs:  64,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GoingThrough",
              params: "void*",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Waypoint::Arc",
        decl: "struct",
        methods: [
          
            {
              name:   "UpdateDirection",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVisible",
              params: "bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "nodePath",
            attrs: 0
          },
          
          {
            name: "from",
            attrs: 0
          },
          
          {
            name: "to",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "MapObject",
        decl: "struct",
        methods: [
          
            {
              name:   "UnSerialize",
              params: "WindowFramework* window,Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "nodePath",
            attrs: 0
          },
          
          {
            name: "floor",
            attrs: 8
          },
          
          {
            name: "strModel",
            attrs: 0
          },
          
          {
            name: "strTexture",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "DynamicObject",
        decl: "struct",
        methods: [
          
            {
              name:   "UnSerialize",
              params: "World*,Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "waypoint",
            attrs: 1
          },
          
          {
            name: "type",
            attrs: 0
          },
          
          {
            name: "script",
            attrs: 0
          },
          
          {
            name: "locked",
            attrs: 0
          },
          
          {
            name: "key",
            attrs: 0
          },
          
          {
            name: "charsheet",
            attrs: 0
          },
          
          {
            name: "interactions",
            attrs: 0
          },
          
          {
            name: "dialog",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "MapObject",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Zone",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "name",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ExitZone",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "Zone",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "WorldLight",
        decl: "struct",
        methods: [
          
            {
              name:   "GetColor",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "LColor",
              return_attrs: 0
            },
          
            {
              name:   "SetColor",
              params: "float r,float g,float b,float a",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "color",
              params: "r,g,b,a",
              attrs:  0,
              desc:   "no description",
              return_type: "LColor",
              return_attrs: 0
            },
          
            {
              name:   "UnSerialize",
              params: "World*,Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReparentTo",
              params: "DynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReparentTo",
              params: "MapObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "name",
            attrs: 0
          },
          
          {
            name: "nodePath",
            attrs: 0
          },
          
          {
            name: "type",
            attrs: 0
          },
          
          {
            name: "parent_type",
            attrs: 0
          },
          
          {
            name: "lens",
            attrs: 1
          },
          
          {
            name: "zoneSize",
            attrs: 0
          },
          
          {
            name: "parent",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "World",
        decl: "struct",
        methods: [
          
            {
              name:   "FloorResize",
              params: "int",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddWayPoint",
              params: "float x,float y,float z",
              attrs:  0,
              desc:   "no description",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "DeleteWayPoint",
              params: "Waypoint*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointFromNodePath",
              params: "NodePath path",
              attrs:  0,
              desc:   "no description",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "GetWaypointFromId",
              params: "unsigned int id",
              attrs:  0,
              desc:   "no description",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "GetWaypointClosest",
              params: "LPoint3",
              attrs:  0,
              desc:   "no description",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "SetWaypointsVisible",
              params: "bool v",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointLimits",
              params: "short currentFloor,LPoint3& upperRight,LPoint3& upperLeft,LPoint3& bottomLeft",
              attrs:  4,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointPlane",
              params: "short currentFloor",
              attrs:  4,
              desc:   "no description",
              return_type: "LPlane",
              return_attrs: 0
            },
          
            {
              name:   "DeleteObject",
              params: "MapObject* ptr,std::list<OBJTYPE>& list",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetObjectFromName",
              params: "const std::string& name,std::list<OBJTYPE>& list",
              attrs:  0,
              desc:   "no description",
              return_type: "OBJTYPE",
              return_attrs: 1
            },
          
            {
              name:   "GetObjectFromNodePath",
              params: "NodePath path,std::list<OBJTYPE>& list",
              attrs:  0,
              desc:   "no description",
              return_type: "OBJTYPE",
              return_attrs: 1
            },
          
            {
              name:   "ObjectChangeFloor",
              params: "MapObject&,unsigned char floor,unsigned short type",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddMapObject",
              params: "const std::string& name,const std::string& model,const std::string& texture,float x,float y,float z",
              attrs:  0,
              desc:   "no description",
              return_type: "MapObject",
              return_attrs: 1
            },
          
            {
              name:   "DeleteMapObject",
              params: "MapObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetMapObjectFromName",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "MapObject",
              return_attrs: 1
            },
          
            {
              name:   "GetMapObjectFromNodePath",
              params: "NodePath path",
              attrs:  0,
              desc:   "no description",
              return_type: "MapObject",
              return_attrs: 1
            },
          
            {
              name:   "SetMapObjectsVisible",
              params: "bool v",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MapObjectChangeFloor",
              params: "MapObject&,unsigned char floor",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertDynamicObject",
              params: "DynamicObject&",
              attrs:  0,
              desc:   "no description",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "AddDynamicObject",
              params: "const std::string& name,DynamicObject::Type type,const std::string& model,const std::string& texture",
              attrs:  0,
              desc:   "no description",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "DeleteDynamicObject",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetDynamicObjectFromName",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetDynamicObjectFromNodePath",
              params: "NodePath path",
              attrs:  0,
              desc:   "no description",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "SetDynamicObjectsVisible",
              params: "bool v",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DynamicObjectSetWaypoint",
              params: "DynamicObject&,Waypoint&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DynamicObjectChangeFloor",
              params: "DynamicObject&,unsigned char floor",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddExitZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DeleteExitZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetExitZoneByName",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "ExitZone",
              return_attrs: 1
            },
          
            {
              name:   "AddEntryZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DeleteEntryZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetEntryZoneByName",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "EntryZone",
              return_attrs: 1
            },
          
            {
              name:   "AddLight",
              params: "WorldLight::Type,const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddLight",
              params: "WorldLight::Type,const std::string&,MapObject* parent",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddLight",
              params: "WorldLight::Type,const std::string&,DynamicObject* parent",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DeleteLight",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetLightByName",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "WorldLight",
              return_attrs: 1
            },
          
            {
              name:   "CompileLight",
              params: "WorldLight*,unsigned char = ColMask::Object | ColMask::DynObject",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointAt",
              params: "LPoint2f",
              attrs:  0,
              desc:   "no description",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "UnSerialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CompileWaypoints",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CompileDoors",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "window",
            attrs: 1
          },
          
          {
            name: "floors",
            attrs: 0
          },
          
          {
            name: "rootWaypoints",
            attrs: 0
          },
          
          {
            name: "waypoints",
            attrs: 0
          },
          
          {
            name: "rootMapObjects",
            attrs: 0
          },
          
          {
            name: "objects",
            attrs: 0
          },
          
          {
            name: "rootDynamicObjects",
            attrs: 0
          },
          
          {
            name: "dynamicObjects",
            attrs: 0
          },
          
          {
            name: "rootLights",
            attrs: 0
          },
          
          {
            name: "lights",
            attrs: 0
          },
          
          {
            name: "exitZones",
            attrs: 0
          },
          
          {
            name: "entryZones",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Party",
        decl: "class",
        methods: [
          
            {
              name:   "Join",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Leave",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ConstGetObjects",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "DynamicObjects",
              return_attrs: 6
            },
          
            {
              name:   "GetObjects",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "DynamicObjects",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "PlayerParty",
        decl: "class",
        methods: [
          
            {
              name:   "Create",
              params: "const std::string& savepath,const std::string& name,const std::string& model,const std::string& texture",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Save",
              params: "const std::string& savepath",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetHasLocalObjects",
              params: "bool val",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet&",
              attrs:  4,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnSerialize",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_local_objects",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "Party",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Semaphore",
        decl: "class",
        methods: [
          
            {
              name:   "TryWait",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Wait",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Post",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetDeadlockSafety",
              params: "bool to_set",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SelfLock",
              params: "void",
              attrs:  36,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetCurrentThreadId",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "ThreadId",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          {
            name: "Handle",
            attrs: 0
          },
          
          {
            name: "ThreadId",
            attrs: 0
          },
          
          {
            name: "Handle",
            attrs: 0
          },
          
          {
            name: "ThreadId",
            attrs: 0
          },
          
          {
            name: "_semaphore",
            attrs: 2
          },
          
          {
            name: "_handle",
            attrs: 0
          },
          
          {
            name: "_max_count",
            attrs: 8
          },
          
          {
            name: "_deadlock_safety",
            attrs: 0
          },
          
          {
            name: "_thread_lock",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Semaphore::Lock",
        decl: "class",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ISignal",
        decl: "class",
        methods: [
          
            {
              name:   "ExecuteRecordedCalls",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetDirect",
              params: "bool",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Signals",
        decl: "class",
        methods: [
          
            {
              name:   "ExecuteRecordedCalls",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "std::list<ISignal*>",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Signal",
        decl: "class",
        methods: [
          
            {
              name:   "SetDirect",
              params: "bool set",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsDirect",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Emit",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "ObserverClass& observerInstance,typename Observer<ObserverClass>::Method method",
              attrs:  0,
              desc:   "no description",
              return_type: "ObserverId",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "typename FunctionObserver::Function function",
              attrs:  0,
              desc:   "no description",
              return_type: "ObserverId",
              return_attrs: 0
            },
          
            {
              name:   "Disconnect",
              params: "ObserverId id",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisconnectAll",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ObserverCount",
              params: "void",
              attrs:  36,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteRecordedCalls",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "no description",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "FuncExecuteRecordedCalls",
              params: "RecordedCalls& calls",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushRecordCall",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "no description",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "FetchRecordCall",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "no description",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "RecordCallCount",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "no description",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "PushedCallCount",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "no description",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "BackupRecordedCalls",
              params: "bool on_off",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddObserver",
              params: "InterfaceObserver* observer",
              attrs:  0,
              desc:   "no description",
              return_type: "ObserverId",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "ObserverHandler",
            attrs: 0
          },
          
          {
            name: "id",
            attrs: 0
          },
          
          {
            name: "byte",
            attrs: 0
          },
          
          {
            name: "it",
            attrs: 1
          },
          
          {
            name: "observer",
            attrs: 0
          },
          
          {
            name: "params",
            attrs: 0
          },
          
          {
            name: "_iterator",
            attrs: 0
          },
          
          {
            name: "_iterator_updated_during_emit",
            attrs: 0
          },
          
          {
            name: "_observers",
            attrs: 0
          },
          
          {
            name: "_recordedCalls",
            attrs: 0
          },
          
          {
            name: "_backedupCalls",
            attrs: 0
          },
          
          {
            name: "_pushedCalls",
            attrs: 0
          },
          
          {
            name: "_direct",
            attrs: 0
          },
          
          {
            name: "_semaphore",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "public",
            visibility: "virtual"
          }
          
        ]
      },
    
      {
        name: "Signal::InterfaceObserver",
        decl: "struct",
        methods: [
          
            {
              name:   "operator",
              params: "",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Signal::FunctionObserver",
        decl: "class",
        methods: [
          
            {
              name:   "operator",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_function",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "InterfaceObserver",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Signal::Observer",
        decl: "class",
        methods: [
          
            {
              name:   "void ",
              params: "ObserverClass::*Method",
              attrs:  0,
              desc:   "no description",
              return_type: "typedef",
              return_attrs: 0
            },
          
            {
              name:   "operator",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_observer",
            attrs: 2
          },
          
          {
            name: "_method",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "InterfaceObserver",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Signal::RecordedCall",
        decl: "struct",
        methods: [
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ObserverHandler",
        decl: "class",
        methods: [
          
            {
              name:   "DisconnectAll",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisconnectAllFrom",
              params: "const ISignal& signal",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Add",
              params: "C& signal,ObserverId id",
              attrs:  32,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "C& signal,ObserverClass& observerInstance,typename C::template Observer<ObserverClass>::Method method",
              attrs:  32,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "C& signal,typename C::FunctionObserver::Function function",
              attrs:  32,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "observer",
            attrs: 0
          },
          
          {
            name: "observer",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ObserverHandler::IObserverPair",
        decl: "struct",
        methods: [
          
            {
              name:   "Disconnect",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ObserverHandler::ObserverPair",
        decl: "struct",
        methods: [
          
            {
              name:   "Disconnect",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "signal",
            attrs: 2
          },
          
          {
            name: "id",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "IObserverPair",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Timer",
        decl: "class",
        methods: [
          
            {
              name:   "GetElapsedTime",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "double",
              return_attrs: 0
            },
          
            {
              name:   "Profile",
              params: "std::string str",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Restart",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLastTime",
              params: "double v",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetLastTime",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "asConstructor",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "asDestructor",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          {
            name: "_lastTime",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "TimeManager",
        decl: "class",
        methods: [
          
            {
              name:   "GetDaysPerMonth",
              params: "unsigned short m,unsigned short year = 1",
              attrs:  16,
              desc:   "no description",
              return_type: "short",
              return_attrs: 24
            },
          
            {
              name:   "ClearTasks",
              params: "unsigned char level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTime",
              params: "unsigned short s,unsigned short m,unsigned short h,unsigned short d,unsigned short mo,unsigned short y",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetYear",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetMonth",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetDay",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetHour",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetMinute",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetSecond",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "AddElapsedSeconds",
              params: "float s",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddElapsedTime",
              params: "unsigned short s,unsigned short m = 0,unsigned short h = 0,unsigned short d = 0,unsigned short mo = 0,unsigned short y = 0",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddTask",
              params: "unsigned char level,bool loop,unsigned short s,unsigned short m = 0,unsigned short h = 0,unsigned short d = 0,unsigned short mo = 0,unsigned short y = 0",
              attrs:  0,
              desc:   "no description",
              return_type: "TimeManager::Task",
              return_attrs: 1
            },
          
            {
              name:   "DelTask",
              params: "Task* task",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteTasks",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CorrectValues",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "it",
            attrs: 0
          },
          
          {
            name: "end",
            attrs: 0
          },
          
          {
            name: "task",
            attrs: 0
          },
          
          {
            name: "fseconds",
            attrs: 0
          },
          
          {
            name: "year",
            attrs: 8
          },
          
          {
            name: "month",
            attrs: 0
          },
          
          {
            name: "day",
            attrs: 8
          },
          
          {
            name: "hour",
            attrs: 8
          },
          
          {
            name: "minute",
            attrs: 8
          },
          
          {
            name: "second",
            attrs: 8
          },
          
          {
            name: "tasks",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "TimeManager::Task",
        decl: "struct",
        methods: [
          
            {
              name:   "NextStep",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "loop",
            attrs: 0
          },
          
          {
            name: "it",
            attrs: 8
          },
          
          {
            name: "lastY",
            attrs: 8
          },
          
          {
            name: "lastMo",
            attrs: 0
          },
          
          {
            name: "lastD",
            attrs: 8
          },
          
          {
            name: "lastH",
            attrs: 8
          },
          
          {
            name: "lastM",
            attrs: 8
          },
          
          {
            name: "lastS",
            attrs: 8
          },
          
          {
            name: "timeY",
            attrs: 8
          },
          
          {
            name: "timeMo",
            attrs: 0
          },
          
          {
            name: "timeD",
            attrs: 8
          },
          
          {
            name: "timeH",
            attrs: 8
          },
          
          {
            name: "timeM",
            attrs: 8
          },
          
          {
            name: "timeS",
            attrs: 8
          },
          
          {
            name: "level",
            attrs: 8
          },
          
          {
            name: "TimeManager",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "DataBranch",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "key",
            attrs: 0
          },
          
          {
            name: "value",
            attrs: 0
          },
          
          {
            name: "children",
            attrs: 0
          },
          
          {
            name: "father",
            attrs: 1
          },
          
          {
            name: "nil",
            attrs: 0
          },
          
          {
            name: "root",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Data",
        decl: "class",
        methods: [
          
            {
              name:   "Key",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "Value",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "SetKey",
              params: "const std::string& newKey",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Duplicate",
              params: "Data var",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "T",
              params: "",
              attrs:  4,
              desc:   "no description",
              return_type: "operator",
              return_attrs: 0
            },
          
            {
              name:   "Remove",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CutBranch",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Nil",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "NotNil",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Parent",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "PushBack",
              params: "Data d",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Output",
              params: "unsigned char indent = 0",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Count",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "begin",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "Data::my_iterator",
              return_attrs: 0
            },
          
            {
              name:   "const_begin",
              params: "",
              attrs:  4,
              desc:   "no description",
              return_type: "Data::const_my_iterator",
              return_attrs: 0
            },
          
            {
              name:   "end",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "Data::my_iterator",
              return_attrs: 0
            },
          
            {
              name:   "const_end",
              params: "",
              attrs:  4,
              desc:   "no description",
              return_type: "Data::const_my_iterator",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "Children",
            attrs: 0
          },
          
          {
            name: "stream",
            attrs: 0
          },
          
          {
            name: "stream",
            attrs: 0
          },
          
          {
            name: "stream",
            attrs: 0
          },
          
          {
            name: "out",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Data::my_iterator",
        decl: "class",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "_it",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "std::iterator_traits<Children::iterator>",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Data::const_my_iterator",
        decl: "class",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "_it",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "std::iterator_traits<Children::const_iterator>",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "DataTree",
        decl: "class",
        methods: [
          
            {
              name:   "tmp",
              params: "this",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "const std::string& path",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetSourceFile",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            }
          
        ],
        attributes: [
          
          {
            name: "Data",
            attrs: 0
          },
          
          {
            name: "Factory",
            attrs: 0
          },
          
          {
            name: "source",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "DataBranch",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "DataTree::Factory",
        decl: "struct",
        methods: [
          
            {
              name:   "Yaml",
              params: "const std::string& filename",
              attrs:  16,
              desc:   "no description",
              return_type: "DataTree",
              return_attrs: 17
            },
          
            {
              name:   "ShinyLang",
              params: "const std::string& filename",
              attrs:  16,
              desc:   "no description",
              return_type: "DataTree",
              return_attrs: 17
            },
          
            {
              name:   "JSON",
              params: "const std::string& filename",
              attrs:  16,
              desc:   "no description",
              return_type: "DataTree",
              return_attrs: 17
            },
          
            {
              name:   "StringJSON",
              params: "const std::string& str",
              attrs:  16,
              desc:   "no description",
              return_type: "DataTree",
              return_attrs: 17
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "DataTree::Writers",
        decl: "struct",
        methods: [
          
            {
              name:   "JSON",
              params: "Data,const std::string& filename",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "StringJSON",
              params: "Data,std::string& str",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "DataEngine",
        decl: "class",
        methods: [
          
            {
              name:   "Load",
              params: "const std::string& filepath",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "const std::string& filepath",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetDiplomacy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "WorldDiplomacy",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          {
            name: "_dataTree",
            attrs: 1
          },
          
          {
            name: "_diplomacy",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "Data",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "AnimatedObject",
        decl: "class",
        methods: [
          
            {
              name:   "GetNodePath",
              params: "void",
              attrs:  68,
              desc:   "no description",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "ResetAnimation",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayAnimation",
              params: "const std::string& name,bool loop = false",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadAnimation",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TaskAnimation",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayIdleAnimation",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "pendingAnimationDone",
            attrs: 0
          },
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_modelName",
            attrs: 0
          },
          
          {
            name: "_anims",
            attrs: 0
          },
          
          {
            name: "_mapAnims",
            attrs: 0
          },
          
          {
            name: "_anim",
            attrs: 1
          },
          
          {
            name: "_animLoop",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "WaypointModifier",
        decl: "struct",
        methods: [
          
            {
              name:   "ProcessCollisions",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnprocessCollisions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "HasOccupiedWaypoint",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetOccupiedWaypointAsInt",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetOccupiedWaypoint",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "SetOccupiedWaypoint",
              params: "Waypoint* wp",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawAllArcs",
              params: "unsigned int id",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawAllArcs",
              params: "Waypoint* waypoint",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawArc",
              params: "unsigned int id1,unsigned int id2",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawArc",
              params: "Waypoint* first,Waypoint* second",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_collision_processed",
            attrs: 8
          },
          
          {
            name: "_level",
            attrs: 1
          },
          
          {
            name: "_waypointOccupied",
            attrs: 1
          },
          
          {
            name: "observer",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "WaypointModifier::WithdrawedArc",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "first",
            attrs: 1
          },
          
          {
            name: "second",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ObjectType2Code",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "InstanceDynamicObject",
        decl: "class",
        methods: [
          
            {
              name:   "Load",
              params: "Utils::Packet&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "float elapsedTime",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "GetNodePath",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "GetInteractions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "InteractionList",
              return_attrs: 2
            },
          
            {
              name:   "GetDialog",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetDynamicObject",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject* character",
              attrs:  64,
              desc:   "no description",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "C",
              return_attrs: 1
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ResetAnimation",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ResetInteractions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ThatDoesNothing",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackAnimationEnded",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "signal",
            attrs: 1
          },
          
          {
            name: "min_distance",
            attrs: 0
          },
          
          {
            name: "pendingActionOn",
            attrs: 1
          },
          
          {
            name: "pendingActionObject",
            attrs: 1
          },
          
          {
            name: "pendingActionObjectActionIt",
            attrs: 8
          },
          
          {
            name: "_type",
            attrs: 8
          },
          
          {
            name: "_object",
            attrs: 1
          },
          
          {
            name: "_interactions",
            attrs: 0
          },
          
          {
            name: "_idle_size",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "WaypointModifier",
            visibility: "public"
          },
          
          {
            name:       "AnimatedObject",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "InstanceDynamicObject::Interaction",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "name",
            attrs: 0
          },
          
          {
            name: "instance",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "InstanceDynamicObject::GoToData",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "nearest",
            attrs: 1
          },
          
          {
            name: "objective",
            attrs: 1
          },
          
          {
            name: "max_distance",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "LevelExitZone",
        decl: "class",
        methods: [
          
            {
              name:   "SetName",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "CanGoThrough",
              params: "unsigned char id",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GoingThrough",
              params: "void* character",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_name",
            attrs: 0
          },
          
          {
            name: "_level",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "Waypoint::ArcObserver",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Lockable",
        decl: "class",
        methods: [
          
            {
              name:   "GetKeyName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "string",
              return_attrs: 0
            },
          
            {
              name:   "IsLocked",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsOpen",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Unlock",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_closed",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ObjectDoor",
        decl: "class",
        methods: [
          
            {
              name:   "ProcessCollisions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetKeyName",
              params: "",
              attrs:  4,
              desc:   "no description",
              return_type: "string",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject* character",
              attrs:  0,
              desc:   "no description",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "ObserveWaypoints",
              params: "bool doObserver",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CanGoThrough",
              params: "unsigned char id",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GoingThrough",
              params: "void*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionOpen",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "InstanceDynamicObject",
            visibility: "public"
          },
          
          {
            name:       "Lockable",
            visibility: "public"
          },
          
          {
            name:       "Waypoint::ArcObserver",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Engine",
        decl: "class",
        methods: [
          
            {
              name:   "Get",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "asIScriptEngine",
              return_attrs: 17
            },
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "LoadModule",
              params: "const std::string& name,const std::string& filepath",
              attrs:  16,
              desc:   "no description",
              return_type: "asIScriptModule",
              return_attrs: 17
            },
          
            {
              name:   "MessageCallback",
              params: "const asSMessageInfo* msg,void* param",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ModuleManager",
        decl: "class",
        methods: [
          
            {
              name:   "Require",
              params: "const std::string& name,const std::string& filepath",
              attrs:  16,
              desc:   "no description",
              return_type: "asIScriptModule",
              return_attrs: 17
            },
          
            {
              name:   "Release",
              params: "asIScriptModule* module",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "OutputFunctionList",
              params: "asIScriptModule* module",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          {
            name: "users",
            attrs: 8
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ModuleManager::LoadedModule",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "filepath",
            attrs: 0
          },
          
          {
            name: "ptr",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "StdList",
        decl: "class",
        methods: [
          
            {
              name:   "Add",
              params: "T t",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Register",
              params: "asIScriptEngine* engine,const std::string& arrayName,const std::string& typeName",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Constructor",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Destructor ",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Size",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "Begin",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StdList::asIterator",
              return_attrs: 0
            },
          
            {
              name:   "RBegin",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StdList::asRIterator",
              return_attrs: 0
            },
          
            {
              name:   "End",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StdList::asIterator",
              return_attrs: 0
            },
          
            {
              name:   "REnd",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StdList::asRIterator",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "self",
            attrs: 1
          },
          
          {
            name: "self",
            attrs: 1
          },
          
          {
            name: "toRet",
            attrs: 0
          },
          
          {
            name: "toRet",
            attrs: 0
          },
          
          {
            name: "toRet",
            attrs: 0
          },
          
          {
            name: "toRet",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "std::list<T>",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "StdList::asIterator",
        decl: "struct",
        methods: [
          
            {
              name:   "Constructor",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Destructor ",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Assign",
              params: "asIterator it",
              attrs:  16,
              desc:   "no description",
              return_type: "T",
              return_attrs: 16
            },
          
            {
              name:   "Value",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "T",
              return_attrs: 0
            },
          
            {
              name:   "Equal",
              params: "asIterator comp",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Increment",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Decrement",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "it",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "StdList::asRIterator",
        decl: "struct",
        methods: [
          
            {
              name:   "Constructor",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Destructor ",
              params: "void* memory",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Assign",
              params: "asRIterator it",
              attrs:  16,
              desc:   "no description",
              return_type: "T",
              return_attrs: 16
            },
          
            {
              name:   "Value",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "T",
              return_attrs: 0
            },
          
            {
              name:   "Equal",
              params: "asRIterator comp",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Increment",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Decrement",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "it",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "i18n",
        decl: "class",
        methods: [
          
            {
              name:   "Load",
              params: "const std::string& language",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Unload",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "T",
              params: "const std::string& key",
              attrs:  16,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 16
            },
          
            {
              name:   "GetDialogs",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 16
            },
          
            {
              name:   "GetStatistics",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          {
            name: "_self",
            attrs: 17
          },
          
          {
            name: "_dialogs",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "MyRocket",
        decl: "struct",
        methods: [
          
            {
              name:   "SetVisibility",
              params: "Rocket::Core::Context* context,bool visibility",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "RocketListener",
        decl: "struct",
        methods: [
          
            {
              name:   "ProcessEvent",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "Rocket::Core::EventListener",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "UiBase",
        decl: "class",
        methods: [
          
            {
              name:   "FireShow",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FireHide",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Show",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsVisible",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetModal",
              params: "bool modal",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPropertyOnAll",
              params: "Rocket::Core::Element* elem,const std::string& property,const std::string& value",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "FadeOut",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FadeIn",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleEventListener",
              params: "bool toggle_on,const std::string& id,const std::string& event,RocketListener& listener",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Translate",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RecursiveTranslate",
              params: "Rocket::Core::Element*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "GameUi",
            attrs: 0
          },
          
          {
            name: "instance",
            attrs: 2
          },
          
          {
            name: "child",
            attrs: 1
          },
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_root",
            attrs: 1
          },
          
          {
            name: "_context",
            attrs: 1
          },
          
          {
            name: "_languageObs",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "UiBase::Listener",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "elem",
            attrs: 4
          },
          
          {
            name: "event",
            attrs: 4
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "GameMenu",
        decl: "class",
        methods: [
          
            {
              name:   "MenuEventContinue",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "GameUi",
            attrs: 0
          },
          
          {
            name: "_continueClicked",
            attrs: 0
          },
          
          {
            name: "_exitClicked",
            attrs: 0
          },
          
          {
            name: "_optionsClicked",
            attrs: 0
          },
          
          {
            name: "_saveClicked",
            attrs: 0
          },
          
          {
            name: "_loadClicked",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "GameMainBar",
        decl: "class",
        methods: [
          
            {
              name:   "AppendToConsole",
              params: "const std::string& str",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentAP",
              params: "unsigned short ap,unsigned short maxap",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetMaxAP",
              params: "unsigned short ap",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEnabledAP",
              params: "bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentHp",
              params: "short hp",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentAc",
              params: "short ac",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEquipedItem",
              params: "unsigned short it,InventoryObject* item",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEquipedItemAction",
              params: "unsigned short it,InventoryObject* item,unsigned char actionIt",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackEquipedItem1Clicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackEquipedItem2Clicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackPassTurnClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCombatEndClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "GameUi",
            attrs: 0
          },
          
          {
            name: "MenuButtonClicked",
            attrs: 0
          },
          
          {
            name: "InventoryButtonClicked",
            attrs: 0
          },
          
          {
            name: "PersButtonClicked",
            attrs: 0
          },
          
          {
            name: "PipbuckButtonClicked",
            attrs: 0
          },
          
          {
            name: "EquipedItem1Clicked",
            attrs: 0
          },
          
          {
            name: "EquipedItem2Clicked",
            attrs: 0
          },
          
          {
            name: "PassTurnClicked",
            attrs: 0
          },
          
          {
            name: "CombatEndClicked",
            attrs: 0
          },
          
          {
            name: "_apMax",
            attrs: 8
          },
          
          {
            name: "_apEnabled",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "InventoryObject",
        decl: "class",
        methods: [
          
            {
              name:   "UseAsWeapon",
              params: "ObjectCharacter* user,ObjectCharacter* target,unsigned char useType",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "UseOn",
              params: "ObjectCharacter* user,InstanceDynamicObject* target,unsigned char useType",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "Use",
              params: "ObjectCharacter* user,unsigned char useType",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetIcon",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "CreateDynamicObject",
              params: "World* world",
              attrs:  4,
              desc:   "no description",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "CreateEquipedModel",
              params: "World* world",
              attrs:  0,
              desc:   "no description",
              return_type: "InventoryObject::EquipedModel",
              return_attrs: 1
            },
          
            {
              name:   "CanWeild",
              params: "ObjectCharacter*,EquipedMode",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetEquiped",
              params: "bool set",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsEquiped",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsGroupableWith",
              params: "const InventoryObject*",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteHook",
              params: "asIScriptFunction* hook,ObjectCharacter* user,C* target,unsigned char actionIt",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            }
          
        ],
        attributes: [
          
          {
            name: "_dataTree",
            attrs: 0
          },
          
          {
            name: "_equiped",
            attrs: 0
          },
          
          {
            name: "_script_context",
            attrs: 1
          },
          
          {
            name: "_script_module",
            attrs: 1
          },
          
          {
            name: "UseAsWeapon",
            attrs: 1
          },
          
          {
            name: "_actionHooks",
            attrs: 0
          },
          
          {
            name: "_hookUseOnCharacter",
            attrs: 1
          },
          
          {
            name: "_hookUseOnDoor",
            attrs: 1
          },
          
          {
            name: "_hookUseOnOthers",
            attrs: 1
          },
          
          {
            name: "_hookUseAsWeapon",
            attrs: 1
          },
          
          {
            name: "_hookCanWeildMouth",
            attrs: 1
          },
          
          {
            name: "_hookCanWeildMagic",
            attrs: 1
          },
          
          {
            name: "_hookCanWeildBattleSaddle",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "Data",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "InventoryObject::EquipedModel",
        decl: "struct",
        methods: [
          
            {
              name:   "GetNodePath",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "ResetAnimation",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "np",
            attrs: 0
          },
          
          {
            name: "object",
            attrs: 2
          }
          
        ],
        ancestors: [
          
          {
            name:       "AnimatedObject",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "InventoryObject::ActionHooks",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "Use",
            attrs: 1
          },
          
          {
            name: "UseOnCharacter",
            attrs: 1
          },
          
          {
            name: "UseOnDoor",
            attrs: 1
          },
          
          {
            name: "UseOnOthers",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Inventory",
        decl: "class",
        methods: [
          
            {
              name:   "LoadInventory",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveInventory",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadInventory",
              params: "Data",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveInventory",
              params: "Data",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DelObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IncludesObject",
              params: "InventoryObject*",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetContent",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "Content",
              return_attrs: 6
            },
          
            {
              name:   "GetContent",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Content",
              return_attrs: 2
            },
          
            {
              name:   "GetObject",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "InventoryObject",
              return_attrs: 1
            },
          
            {
              name:   "ContainsHowMany",
              params: "const std::string& name",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetCurrentWeight",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetCapacity",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetCapacity",
              params: "unsigned short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CanCarry",
              params: "InventoryObject*,unsigned short quantity = 1",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_content",
            attrs: 0
          },
          
          {
            name: "_currentWeight",
            attrs: 8
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Scriptable",
        decl: "class",
        methods: [
          
            {
              name:   "LoadScript",
              params: "std::string module_name,std::string filepath",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReloadFunction",
              params: "asIScriptFunction** pointer",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_script_context",
            attrs: 1
          },
          
          {
            name: "_script_module",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "StatModel",
        decl: "class",
        methods: [
          
            {
              name:   "Backup",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RestoreBackup",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Action",
              params: "const std::string& action,const std::string& fmt,...",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetAll",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "SetName",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "SetRace",
              params: "const std::string& race",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetRace",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "SetAge",
              params: "unsigned short age",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetAge",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetGender",
              params: "const std::string& g",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGender",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "AddKill",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetKills",
              params: "const std::string& race",
              attrs:  4,
              desc:   "no description",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "AddPerk",
              params: "const std::string& perk",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "ToggleTrait",
              params: "const std::string& trait",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleSkillAffinity",
              params: "const std::string& skill",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "HasTrait",
              params: "const std::string& trait",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "HasSkillAffinity",
              params: "const std::string& skill",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetSkillAffinities",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "SetStatistic",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSpecial",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkill",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetLevel",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetStatistic",
              params: "const std::string& stat",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "GetSpecial",
              params: "const std::string& stat",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "GetSkill",
              params: "const std::string& stat",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "SelectRandomEncounter",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "GetSpecialPoints",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetSkillPoints",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetPerksPoints",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetXpNextLevel",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetExperience",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetExperience",
              params: "unsigned short e",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LevelUp",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentHp",
              params: "short hp",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetCurrentHp",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "GetMaxHp",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "IsReady",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "UpdateAllValues",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "LoadFunctions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_statsheet",
            attrs: 0
          },
          
          {
            name: "_statsheet_backup",
            attrs: 0
          },
          
          {
            name: "_scriptAddSpecialPoint",
            attrs: 1
          },
          
          {
            name: "_scriptActivateTraits",
            attrs: 1
          },
          
          {
            name: "_scriptAddExperience",
            attrs: 1
          },
          
          {
            name: "_scriptXpNextLevel",
            attrs: 1
          },
          
          {
            name: "_scriptLevelUp",
            attrs: 1
          },
          
          {
            name: "_scriptUpdateAllValues",
            attrs: 1
          },
          
          {
            name: "_scriptIsReady",
            attrs: 1
          },
          
          {
            name: "_scriptAvailableTraits",
            attrs: 1
          },
          
          {
            name: "_scriptAddPerk",
            attrs: 1
          },
          
          {
            name: "_selectRandomEncounter",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "Scriptable",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "StatView",
        decl: "class",
        methods: [
          
            {
              name:   "SetEditMode",
              params: "EditMode",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetEditMode",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "EditMode",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Show",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,const std::string& value",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,short value",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,const std::string& value",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,short value",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCategoryFields",
              params: "const std::string& category,const std::vector<std::string>& keys",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,const std::string& value",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,short value",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetExperience",
              params: "unsigned int xp,unsigned short lvl,unsigned int next_level",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraits",
              params: "std::list<std::string>",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraitActive",
              params: "const std::string&,bool",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPerks",
              params: "std::list<std::string>",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAvailablePerks",
              params: "std::list<std::string> perks",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkillAffinity",
              params: "const std::string& skill,bool",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetNumPerks",
              params: "unsigned short n_perks",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_editMode",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "StatController",
        decl: "class",
        methods: [
          
            {
              name:   "Model",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StatModel",
              return_attrs: 2
            },
          
            {
              name:   "Model",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "StatModel",
              return_attrs: 6
            },
          
            {
              name:   "GetData",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "SetView",
              params: "StatView*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpSpecial",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DownSpecial",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSpecial",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetStatistic",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpSkill",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DownSkill",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkill",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddExperience",
              params: "unsigned int experience",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentHp",
              params: "short hp",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TriggerSkillAffinity",
              params: "const std::string& stat,bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddKill",
              params: "const std::string& race",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMetabolism",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetMaxHp",
              params: "short hp",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SpecialChanged",
              params: "const std::string&,short",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SkillChanged",
              params: "const std::string&,short",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatisticChanged",
              params: "const std::string&,short",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PerksChanged",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TraitToggled",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SkillAffinityToggled",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LevelChanged",
              params: "unsigned short",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InformationChanged",
              params: "const std::string&,const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AgeChanged",
              params: "unsigned char",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PerkAdded",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ViewStatUpped",
              params: "const std::string&,const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ViewStatDowned",
              params: "const std::string&,const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AcceptChanges",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CancelChanges",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MakeBackup",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_model",
            attrs: 0
          },
          
          {
            name: "_view",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "StatViewRocket",
        decl: "class",
        methods: [
          
            {
              name:   "SetEditMode",
              params: "EditMode",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Show",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,const std::string& value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,const std::string& value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,const std::string& value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCategoryFields",
              params: "const std::string& category,const std::vector<std::string>& keys",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetExperience",
              params: "unsigned int,unsigned short,unsigned int",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraits",
              params: "std::list<std::string>",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraitActive",
              params: "const std::string&,bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkillAffinity",
              params: "const std::string& skill,bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPerks",
              params: "std::list<std::string>",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAvailablePerks",
              params: "std::list<std::string>",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Accept",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateName",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateGender",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateAge",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TraitClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SpecialClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SkillClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GeneralClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatMore",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatLess",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatUpdate",
              params: "Rocket::Core::Event& event,std::string& type,std::string& stat",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_i18n",
            attrs: 0
          },
          
          {
            name: "_perks_dialog",
            attrs: 0
          },
          
          {
            name: "CancelButton",
            attrs: 0
          },
          
          {
            name: "DoneButton",
            attrs: 0
          },
          
          {
            name: "EventSpecialClicked",
            attrs: 0
          },
          
          {
            name: "EventSkillClicked",
            attrs: 0
          },
          
          {
            name: "EventGeneralClicked",
            attrs: 0
          },
          
          {
            name: "EventTraitClicked",
            attrs: 0
          },
          
          {
            name: "ButtonUp",
            attrs: 0
          },
          
          {
            name: "ButtonDown",
            attrs: 0
          },
          
          {
            name: "EventNameChanged",
            attrs: 0
          },
          
          {
            name: "EventAgeChanged",
            attrs: 0
          },
          
          {
            name: "EventGenderChanged",
            attrs: 0
          },
          
          {
            name: "SpecialSelected",
            attrs: 0
          },
          
          {
            name: "SkillSelected",
            attrs: 0
          },
          
          {
            name: "TraitSelected",
            attrs: 0
          },
          
          {
            name: "_specialSelected",
            attrs: 1
          },
          
          {
            name: "_skillSelected",
            attrs: 1
          },
          
          {
            name: "_traitSelected",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          },
          
          {
            name:       "StatView",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "StatViewRocket::PerksDialog",
        decl: "struct",
        methods: [
          
            {
              name:   "SetAvailablePerks",
              params: "std::list<std::string> perks",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPerkDescription",
              params: "std::string description",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClearPerksButtons",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSelectedPerk",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackChoosePerk",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancel",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackDblClickPerk",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "Cancel",
            attrs: 0
          },
          
          {
            name: "SelectPerk",
            attrs: 0
          },
          
          {
            name: "ChoosePerk",
            attrs: 0
          },
          
          {
            name: "DblClickPerk",
            attrs: 0
          },
          
          {
            name: "_selected_perk",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "CharacterBuff",
        decl: "class",
        methods: [
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "Begin",
              params: "ObjectCharacter* from,TimeManager::Task* task = 0",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "End",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Load",
              params: "Level*,ObjectCharacter*,Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "Level*,ObjectCharacter*,Data buff",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_buff",
            attrs: 0
          },
          
          {
            name: "_timeManager",
            attrs: 2
          },
          
          {
            name: "_character",
            attrs: 1
          },
          
          {
            name: "_duration",
            attrs: 8
          },
          
          {
            name: "_task",
            attrs: 1
          },
          
          {
            name: "_name",
            attrs: 0
          },
          
          {
            name: "_context",
            attrs: 1
          },
          
          {
            name: "_module",
            attrs: 1
          },
          
          {
            name: "_begin",
            attrs: 1
          },
          
          {
            name: "_end",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ObjectCharacter",
        decl: "class",
        methods: [
          
            {
              name:   "Load",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetStatistics",
              params: "DataTree* stats,StatController* statsController",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "NullifyStatistics",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject* character",
              attrs:  64,
              desc:   "no description",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "SetInventory",
              params: "Inventory* inventory",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "statistics",
              params: "_statistics",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "ProcessCollisions",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LookAt",
              params: "LVecBase3",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LookAt",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoTo",
              params: "unsigned int id",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoTo",
              params: "Waypoint* waypoint",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoTo",
              params: "InstanceDynamicObject* object,int max_distance = 0",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToRandomWaypoint",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TruncatePath",
              params: "unsigned short max_length",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetPathDistance",
              params: "Waypoint* waypoint",
              attrs:  0,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetPathDistance",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetDistance",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "GetBestWaypoint",
              params: "InstanceDynamicObject* object,bool farthest",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetNearestWaypoint",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetFarthestWaypoint",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetPathSize",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "HasLineOfSight",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsMoving",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsAlive",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsInterrupted",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Inventory",
              return_attrs: 2
            },
          
            {
              name:   "GetStatistics",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "GetStatController",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StatController",
              return_attrs: 1
            },
          
            {
              name:   "GetFactionName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetFaction",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "GetActionPoints",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetActionPoints",
              params: "unsigned short ap",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RestartActionPoints",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetHitPoints",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "SetHitPoints",
              params: "short hp",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatHpUpdate",
              params: "short",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetArmorClass",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "SetArmorClass",
              params: "short ac",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RestartArmorClass",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetBonusAC",
              params: "short ac",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayEquipedItemAnimation",
              params: "unsigned short it,const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEquipedItem",
              params: "unsigned short it,InventoryObject* object,EquipedMode mode = EquipedMouth",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetEquipedItem",
              params: "unsigned short it",
              attrs:  0,
              desc:   "no description",
              return_type: "InventoryObject",
              return_attrs: 1
            },
          
            {
              name:   "GetequipedAction",
              params: "unsigned short it",
              attrs:  4,
              desc:   "no description",
              return_type: "char",
              return_attrs: 8
            },
          
            {
              name:   "UnequipItem",
              params: "unsigned short it",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ItemNextUseType",
              params: "unsigned short it",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushBuff",
              params: "Data,ObjectCharacter* caster",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DelBuff",
              params: "CharacterBuff* buff",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CheckFieldOfView",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFaction",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFaction",
              params: "unsigned int flag",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAsEnemy",
              params: "const ObjectCharacter*,bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsEnemy",
              params: "const ObjectCharacter*",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsAlly",
              params: "const ObjectCharacter*",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "RequestAttack",
              params: "ObjectCharacter* attack,ObjectCharacter* from",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestHeal",
              params: "ObjectCharacter* heal,ObjectCharacter* from",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestFollow",
              params: "ObjectCharacter* follow,ObjectCharacter* from",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestStopFollowing",
              params: "ObjectCharacter* follow,ObjectCharacter* from",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AskMorale",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "SendMessage",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMovement",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMovementNext",
              params: "float elaspedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunDeath",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartRunAnimation",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StopRunAnimation",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestCharacter",
              params: "ObjectCharacter*,ObjectCharacter*,asIScriptFunction*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DebugPathfinding",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "ttl",
            attrs: 8
          },
          
          {
            name: "ret",
            attrs: 0
          },
          
          {
            name: "_inventory",
            attrs: 0
          },
          
          {
            name: "_obs_handler",
            attrs: 0
          },
          
          {
            name: "_goToData",
            attrs: 0
          },
          
          {
            name: "_statistics",
            attrs: 1
          },
          
          {
            name: "_stats",
            attrs: 1
          },
          
          {
            name: "_faction",
            attrs: 5
          },
          
          {
            name: "_self_enemyMask",
            attrs: 8
          },
          
          {
            name: "_actionPoints",
            attrs: 8
          },
          
          {
            name: "_hitPoints",
            attrs: 0
          },
          
          {
            name: "_armorClass",
            attrs: 0
          },
          
          {
            name: "_tmpArmorClass",
            attrs: 0
          },
          
          {
            name: "jointHorn",
            attrs: 0
          },
          
          {
            name: "jointBattleSaddle",
            attrs: 0
          },
          
          {
            name: "jointMouth",
            attrs: 0
          },
          
          {
            name: "_inventory",
            attrs: 1
          },
          
          {
            name: "_losPath",
            attrs: 0
          },
          
          {
            name: "_losTraverser",
            attrs: 0
          },
          
          {
            name: "_fovTargetNp",
            attrs: 0
          },
          
          {
            name: "_fovNeedsUpdate",
            attrs: 0
          },
          
          {
            name: "_fovNp",
            attrs: 0
          },
          
          {
            name: "_fovTraverser",
            attrs: 0
          },
          
          {
            name: "_scriptMain",
            attrs: 1
          },
          
          {
            name: "_scriptFight",
            attrs: 1
          },
          
          {
            name: "_scriptRequestAttack",
            attrs: 1
          },
          
          {
            name: "_scriptRequestHeal",
            attrs: 1
          },
          
          {
            name: "_scriptRequestFollow",
            attrs: 1
          },
          
          {
            name: "_scriptRequestStopFollowing",
            attrs: 1
          },
          
          {
            name: "_scriptAskMorale",
            attrs: 1
          },
          
          {
            name: "_scriptSendMessage",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "InstanceDynamicObject",
            visibility: "public"
          },
          
          {
            name:       "Scriptable",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "ObjectCharacter::FovEnemy",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "enemy",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ObjectCharacter::ItemEquiped",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "actionIt",
            attrs: 8
          },
          
          {
            name: "mode",
            attrs: 0
          },
          
          {
            name: "equiped",
            attrs: 1
          },
          
          {
            name: "default_",
            attrs: 1
          },
          
          {
            name: "graphics",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "UiObjectQuantityPicker",
        decl: "class",
        methods: [
          
            {
              name:   "Accepted",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Increment",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetQuantity",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetQuantity",
              params: "unsigned short",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "Observer",
            attrs: 0
          },
          
          {
            name: "EventAccepted",
            attrs: 0
          },
          
          {
            name: "EventCanceled",
            attrs: 0
          },
          
          {
            name: "EventIncrement",
            attrs: 0
          },
          
          {
            name: "EventDecrement",
            attrs: 0
          },
          
          {
            name: "EventValueChanged",
            attrs: 0
          },
          
          {
            name: "_max_quantity",
            attrs: 8
          },
          
          {
            name: "_line_edit",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "InventoryView",
        decl: "class",
        methods: [
          
            {
              name:   "UpdateView",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetObjectFromId",
              params: "const std::string& id",
              attrs:  0,
              desc:   "no description",
              return_type: "InventoryObject",
              return_attrs: 1
            },
          
            {
              name:   "ProcessEvent",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Inventory",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          {
            name: "_element",
            attrs: 2
          },
          
          {
            name: "_inventory",
            attrs: 2
          }
          
        ],
        ancestors: [
          
          {
            name:       "Rocket::Core::EventListener",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "InventoryViewController",
        decl: "class",
        methods: [
          
            {
              name:   "AddView",
              params: "Rocket::Core::Element* element,Inventory& inventory",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DragObserver",
              params: "InventoryView* container,Rocket::Core::Element* element",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Update",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AllowDrop",
              params: "InventoryView& from,InventoryView& to",
              attrs:  64,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "UiUseObjectOn",
        decl: "class",
        methods: [
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RocketCancelClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "CancelClicked",
            attrs: 0
          },
          
          {
            name: "_viewController",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "UiBarter",
        decl: "class",
        methods: [
          
            {
              name:   "AllowDrop",
              params: "InventoryView& from,InventoryView& to",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SwapObjects",
              params: "InventoryObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MakeDeal",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "BarterEnd",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInterface",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInterfaceSide",
              params: "Rocket::Core::Element* e,Inventory::Content&,StatController*,StatController*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetStackValue",
              params: "Inventory::Content&,StatController*,StatController*",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "DropInventory",
              params: "Inventory& from,Inventory& to",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SwapFunctor",
              params: "InventoryObject* object,Inventory& from,Inventory& to",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "EventMakeDeal",
            attrs: 0
          },
          
          {
            name: "EventBarterEnd",
            attrs: 0
          },
          
          {
            name: "_stack_player",
            attrs: 0
          },
          
          {
            name: "_stack_other",
            attrs: 0
          },
          
          {
            name: "_stats_player",
            attrs: 1
          },
          
          {
            name: "_stats_other",
            attrs: 1
          },
          
          {
            name: "_inventory_player",
            attrs: 2
          },
          
          {
            name: "_inventory_other",
            attrs: 2
          },
          
          {
            name: "_quantity_picker",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          },
          
          {
            name:       "InventoryViewController",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "UiLoot",
        decl: "class",
        methods: [
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SwapObjects",
              params: "InventoryObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RocketDoneClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RocketTakeAllClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "DoneClicked",
            attrs: 0
          },
          
          {
            name: "TakeAllClicked",
            attrs: 0
          },
          
          {
            name: "_viewController",
            attrs: 0
          },
          
          {
            name: "_quantity_picker",
            attrs: 1
          },
          
          {
            name: "_looter",
            attrs: 2
          },
          
          {
            name: "_looted",
            attrs: 2
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "UiEquipMode",
        decl: "class",
        methods: [
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisableMode",
              params: "EquipedMode",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButton",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "MouthClicked",
            attrs: 0
          },
          
          {
            name: "MagicClicked",
            attrs: 0
          },
          
          {
            name: "BattleSaddleClicked",
            attrs: 0
          },
          
          {
            name: "CancelClicked",
            attrs: 0
          },
          
          {
            name: "_it",
            attrs: 8
          },
          
          {
            name: "_object",
            attrs: 2
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "UiNextZone",
        decl: "class",
        methods: [
          
            {
              name:   "CallbackLevelSelected",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "LevelSelected",
            attrs: 0
          },
          
          {
            name: "CancelSelected",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "GameInventory",
        decl: "class",
        methods: [
          
            {
              name:   "SetInventory",
              params: "Inventory&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInventory",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInventoryCapacity",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSelectedObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonUse",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonDrop",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonEquip1",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonEquip2",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonUnequip1",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonUnequip2",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "GameUi",
            attrs: 0
          },
          
          {
            name: "ButtonUseClicked",
            attrs: 0
          },
          
          {
            name: "ButtonDropClicked",
            attrs: 0
          },
          
          {
            name: "ButtonEquip1Clicked",
            attrs: 0
          },
          
          {
            name: "ButtonEquip2Clicked",
            attrs: 0
          },
          
          {
            name: "ButtonUnequip1",
            attrs: 0
          },
          
          {
            name: "ButtonUnequip2",
            attrs: 0
          },
          
          {
            name: "_inventory",
            attrs: 1
          },
          
          {
            name: "_inventoryView",
            attrs: 0
          },
          
          {
            name: "_selectedObject",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "GameConsole",
        decl: "class",
        methods: [
          
            {
              name:   "Show",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Get",
              params: "",
              attrs:  16,
              desc:   "no description",
              return_type: "GameConsole",
              return_attrs: 18
            },
          
            {
              name:   "WriteOn",
              params: "const std::string& string",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "ListFunctions",
              params: "",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "PrintScenegraph",
              params: "",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "PrintChildren",
              params: "const NodePath& n,int lvl",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Toggle",
              params: "const Event*,void* data",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "KeyUp",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Execute",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Output",
              params: "const std::string str",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "GameUi",
            attrs: 0
          },
          
          {
            name: "GConsole",
            attrs: 17
          },
          
          {
            name: "ConsoleKeyUp",
            attrs: 0
          },
          
          {
            name: "ExecuteEvent",
            attrs: 0
          },
          
          {
            name: "_histIter",
            attrs: 0
          },
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_root",
            attrs: 1
          },
          
          {
            name: "_input",
            attrs: 1
          },
          
          {
            name: "_currentLine",
            attrs: 0
          },
          
          {
            name: "_script_context",
            attrs: 1
          },
          
          {
            name: "_observerError",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "GameUi",
        decl: "class",
        methods: [
          
            {
              name:   "window, PT",
              params: "RocketRegion",
              attrs:  0,
              desc:   "no description",
              return_type: "WindowFramework",
              return_attrs: 1
            },
          
            {
              name:   "GetContext",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "Rocket::Core::Context",
              return_attrs: 1
            },
          
            {
              name:   "GetMenu",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "GameMenu",
              return_attrs: 2
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "GameInventory",
              return_attrs: 2
            },
          
            {
              name:   "GetPers",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StatViewRocket",
              return_attrs: 2
            },
          
            {
              name:   "OpenMenu",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenInventory",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenPers",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_console",
            attrs: 1
          },
          
          {
            name: "_menu",
            attrs: 1
          },
          
          {
            name: "_inventory",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "LoadingScreen",
        decl: "class",
        methods: [
          
            {
              name:   "AppendText",
              params: "const std::string& str",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "LevelUi",
        decl: "class",
        methods: [
          
            {
              name:   "GetContext",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "Rocket::Core::Context",
              return_attrs: 1
            },
          
            {
              name:   "GetMainBar",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "GameMainBar",
              return_attrs: 2
            },
          
            {
              name:   "GetMenu",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "GameMenu",
              return_attrs: 2
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "GameInventory",
              return_attrs: 2
            },
          
            {
              name:   "GetPers",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "StatViewRocket",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          {
            name: "_gameUi",
            attrs: 2
          },
          
          {
            name: "_mainBar",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "GameOptions",
        decl: "class",
        methods: [
          
            {
              name:   "SetLanguage",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetResolution",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetQuality",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleFullscreen",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "ExitClicked",
            attrs: 0
          },
          
          {
            name: "LanguageSelected",
            attrs: 0
          },
          
          {
            name: "QualitySelected",
            attrs: 0
          },
          
          {
            name: "ScreenSelected",
            attrs: 0
          },
          
          {
            name: "FullscreenToggled",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "AlertUi",
        decl: "class",
        methods: [
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "ButtonClicked",
            attrs: 0
          },
          
          {
            name: "_continue",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "GeneralUi",
        decl: "class",
        methods: [
          
            {
              name:   "GetOptions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "GameOptions",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_console",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "MusicManager",
        decl: "class",
        methods: [
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "MusicManager",
              return_attrs: 17
            },
          
            {
              name:   "Play",
              params: "const std::string& category",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Play",
              params: "const std::string& category,const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayNext",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVolume",
              params: "float volume",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FadeOut",
              params: "float elapsed_time",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FadeVolume",
              params: "float elapsed_time",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_global_ptr",
            attrs: 17
          },
          
          {
            name: "_global_ptr",
            attrs: 0
          },
          
          {
            name: "_data",
            attrs: 0
          },
          
          {
            name: "_data_tree",
            attrs: 1
          },
          
          {
            name: "_current_category",
            attrs: 0
          },
          
          {
            name: "_timer",
            attrs: 0
          },
          
          {
            name: "_fading_out",
            attrs: 0
          },
          
          {
            name: "_volume_goal",
            attrs: 0
          },
          
          {
            name: "_volume_ref",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ISampleInstance",
        decl: "struct",
        methods: [
          
            {
              name:   "Start",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Stop",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Pause",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVolume",
              params: "float",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetVolume",
              params: "void",
              attrs:  68,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "IsPlaying",
              params: "void",
              attrs:  68,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "AddReference",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DelReference",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetReferenceCount",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "short",
              return_attrs: 8
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "ISample",
        decl: "struct",
        methods: [
          
            {
              name:   "NewInstance",
              params: "void",
              attrs:  64,
              desc:   "no description",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "LoadFromFile",
              params: "const std::string& filename",
              attrs:  64,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetFilename",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            }
          
        ],
        attributes: [
          
          {
            name: "filename",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "SoundManager",
        decl: "class",
        methods: [
          
            {
              name:   "NewSoundManager",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "SoundManager",
              return_attrs: 17
            },
          
            {
              name:   "Release",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CreateInstance",
              params: "const std::string& key",
              attrs:  0,
              desc:   "no description",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "DeleteInstance",
              params: "ISampleInstance*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GarbageCollect",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GarbageCollectAll",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "SetVolume",
              params: "float",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetVolume",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "Require",
              params: "const std::string& key",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "RequireSound",
              params: "const std::string& key",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "data_audio",
              params: "_data_audio",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "if ",
              params: "(find(_sounds_required.begin(",
              attrs:  0,
              desc:   "no description",
              return_type: "else",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "ref_count",
            attrs: 8
          },
          
          {
            name: "this",
            attrs: 0
          },
          
          {
            name: "_sound_managers",
            attrs: 16
          },
          
          {
            name: "_sounds",
            attrs: 16
          },
          
          {
            name: "_data_audio",
            attrs: 17
          },
          
          {
            name: "_sounds_required",
            attrs: 0
          },
          
          {
            name: "_volume",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "SoundManager::Sound",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "key",
            attrs: 4
          },
          
          {
            name: "sample",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "sf_SampleInstance",
        decl: "class",
        methods: [
          
            {
              name:   "Start",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Stop",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Pause",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVolume",
              params: "float volume",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetVolume",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "IsPlaying",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "instance",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "ISampleInstance",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "sf_Sample",
        decl: "class",
        methods: [
          
            {
              name:   "NewInstance",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "LoadFromFile",
              params: "const std::string& filename",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "buffer",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "ISample",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Level",
        decl: "class",
        methods: [
          
            {
              name:   "InitSun",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InitPlayer",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetDataEngine",
              params: "DataEngine* de",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPlayerInventory",
              params: "Inventory*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveUpdateWorld",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Load",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertParty",
              params: "PlayerParty& party",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FetchParty",
              params: "PlayerParty& party",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StripParty",
              params: "PlayerParty& party",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "do_task",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "AsyncTask::DoneStatus",
              return_attrs: 0
            },
          
            {
              name:   "SetPersistent",
              params: "bool set",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsPersistent",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetState",
              params: "State",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetState",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "State",
              return_attrs: 0
            },
          
            {
              name:   "SetInterrupted",
              params: "bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisplayCombatPath",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DestroyCombatPath",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FindPath",
              params: "std::list<Waypoint>&,Waypoint&,Waypoint&",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetWorld",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "World",
              return_attrs: 1
            },
          
            {
              name:   "GetCamera",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "SceneCamera",
              return_attrs: 2
            },
          
            {
              name:   "GetCharacter",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "GetCharacter",
              params: "const DynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "GetPlayer",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "UnprocessAllCollisions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ProcessAllCollisions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FindObjectFromNode",
              params: "NodePath node",
              attrs:  0,
              desc:   "no description",
              return_type: "InstanceDynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetObject",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "InstanceDynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetTimeManager",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "TimeManager",
              return_attrs: 2
            },
          
            {
              name:   "GetDataEngine",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "GetItems",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "ConsoleWrite",
              params: "const std::string& str",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RemoveObject",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackExitZone",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackGoToZone",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackSelectNextZone",
              params: "const std::vector<std::string>& zones",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancelSelectZone",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetNextZone",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetExitZone",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "SetEntryZone",
              params: "Party&,const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionBarter",
              params: "ObjectCharacter*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionTalkTo",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUseObjectOn",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionTargetUse",
              params: "unsigned short it",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SelectedUseObjectOn",
              params: "InventoryObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUse",
              params: "ObjectCharacter* user,InstanceDynamicObject* target",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUseObject",
              params: "ObjectCharacter* user,InventoryObject* object,unsigned char actionIt",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUseObjectOn",
              params: "ObjectCharacter* user,InstanceDynamicObject* target,InventoryObject* object,unsigned char actionIt",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionDropObject",
              params: "ObjectCharacter* user,InventoryObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUseWeaponOn",
              params: "ObjectCharacter* user,ObjectCharacter* target,InventoryObject* object,unsigned char actionIt",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionTalkTo",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionUse",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionUseObjectOn",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionUseWeaponOn",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerDropObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerUseObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerLoot",
              params: "Inventory*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerEquipObject",
              params: "unsigned short it,InventoryObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartFight",
              params: "ObjectCharacter* starter",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StopFight",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "NextTurn",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UseActionPoints",
              params: "unsigned short ap",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetMouseState",
              params: "MouseState",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseLeftClicked",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseRightClicked",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartCombat",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SpawnEnemies",
              params: "const std::string& type,unsigned short quantity,unsigned short n_spawn",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsWaypointOccupied",
              params: "unsigned int id",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "PlaySound",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "SetupCamera",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunDaylight",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMetabolism",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseInit",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleCharacterOutline",
              params: "bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertDynamicObject",
              params: "DynamicObject&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertCharacter",
              params: "ObjectCharacter*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CloseRunningUi",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsInsideBuilding",
              params: "unsigned char& floor",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "CheckCurrentFloor",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentFloor",
              params: "unsigned char floor",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FloorFade",
              params: "bool in,NodePath floor",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "CurrentLevel",
            attrs: 17
          },
          
          {
            name: "_mouseState",
            attrs: 0
          },
          
          {
            name: "obs",
            attrs: 0
          },
          
          {
            name: "obs_player",
            attrs: 0
          },
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_graphicWindow",
            attrs: 1
          },
          
          {
            name: "_mouse",
            attrs: 0
          },
          
          {
            name: "_camera",
            attrs: 0
          },
          
          {
            name: "_timer",
            attrs: 0
          },
          
          {
            name: "_timeManager",
            attrs: 2
          },
          
          {
            name: "_state",
            attrs: 0
          },
          
          {
            name: "_persistent",
            attrs: 0
          },
          
          {
            name: "_world",
            attrs: 1
          },
          
          {
            name: "_sound_manager",
            attrs: 0
          },
          
          {
            name: "_objects",
            attrs: 0
          },
          
          {
            name: "_characters",
            attrs: 0
          },
          
          {
            name: "_itCharacter",
            attrs: 0
          },
          
          {
            name: "_currentCharacter",
            attrs: 0
          },
          
          {
            name: "_player_halo",
            attrs: 0
          },
          
          {
            name: "_exitZones",
            attrs: 0
          },
          
          {
            name: "_exitingZone",
            attrs: 0
          },
          
          {
            name: "_exitingZoneTo",
            attrs: 0
          },
          
          {
            name: "_exitingZoneName",
            attrs: 0
          },
          
          {
            name: "_sunLightNode",
            attrs: 0
          },
          
          {
            name: "_sunLightAmbientNode",
            attrs: 0
          },
          
          {
            name: "_task_daylight",
            attrs: 1
          },
          
          {
            name: "_task_metabolism",
            attrs: 1
          },
          
          {
            name: "_levelUi",
            attrs: 0
          },
          
          {
            name: "_currentRunningDialog",
            attrs: 1
          },
          
          {
            name: "_currentUseObjectOn",
            attrs: 1
          },
          
          {
            name: "_currentUiLoot",
            attrs: 1
          },
          
          {
            name: "_mouseActionBlocked",
            attrs: 0
          },
          
          {
            name: "_dataEngine",
            attrs: 1
          },
          
          {
            name: "_items",
            attrs: 1
          },
          
          {
            name: "_last_combat_path",
            attrs: 0
          },
          
          {
            name: "_currentFloor",
            attrs: 8
          },
          
          {
            name: "_floor_lastWp",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Level::HidingFloor",
        decl: "class",
        methods: [
          
            {
              name:   "Done",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Alpha",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "ForceAlpha",
              params: "float _alpha",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetNodePath",
              params: "NodePath np",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFadingIn",
              params: "bool set",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "floor",
            attrs: 0
          },
          
          {
            name: "fadingIn",
            attrs: 0
          },
          
          {
            name: "done",
            attrs: 0
          },
          
          {
            name: "alpha",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "MouseHovering",
        decl: "struct",
        methods: [
          
            {
              name:   "Reset",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetWaypoint",
              params: "NodePath np",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetDynObject",
              params: "NodePath np",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "hasWaypoint",
            attrs: 0
          },
          
          {
            name: "hasDynObject",
            attrs: 0
          },
          
          {
            name: "waypoint",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Mouse",
        decl: "class",
        methods: [
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hovering",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "MouseHovering",
              return_attrs: 6
            },
          
            {
              name:   "SetMouseState",
              params: "char",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClosestWaypoint",
              params: "World*,short currentFloor",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButton1",
              params: "const Event*,void* ptr",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "CallbackButton2",
              params: "const Event*,void* ptr",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "CallbackButton3",
              params: "const Event*,void* ptr",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_camera",
            attrs: 0
          },
          
          {
            name: "_lastMousePos",
            attrs: 0
          },
          
          {
            name: "_pickerPath",
            attrs: 0
          },
          
          {
            name: "_collisionTraverser",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "DialogAnswers",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "DialogModel",
        decl: "class",
        methods: [
          
            {
              name:   "SetCurrentNpcLine",
              params: "const std::string& id",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetHookAvailable",
              params: "const std::string& answerId",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetExecuteMethod",
              params: "const std::string& answerId",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetDefaultNextLine",
              params: "const std::string& answerId",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetNpcLine",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetDialogAnswers",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "DialogAnswers",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_tree",
            attrs: 0
          },
          
          {
            name: "_tree",
            attrs: 1
          },
          
          {
            name: "_data",
            attrs: 0
          },
          
          {
            name: "_l18n",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "DialogView",
        decl: "class",
        methods: [
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateView",
              params: "const std::string& npcLine,const DialogAnswers& answers",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CleanView",
              params: "const DialogAnswers& answers",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "AnswerSelected",
            attrs: 0
          },
          
          {
            name: "BarterOpened",
            attrs: 0
          },
          
          {
            name: "_containerNpcLine",
            attrs: 1
          },
          
          {
            name: "_containerAnswers",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "DialogController",
        decl: "class",
        methods: [
          
            {
              name:   "WithCharacter",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "ExecuteAnswer",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentNode",
              params: "const std::string& nodeName",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenBarter",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_context",
            attrs: 1
          },
          
          {
            name: "_module",
            attrs: 1
          },
          
          {
            name: "_model",
            attrs: 0
          },
          
          {
            name: "_character",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "DialogView",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "ObjectShelf",
        decl: "class",
        methods: [
          
            {
              name:   "LockWaypoints",
              params: "bool",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Inventory",
              return_attrs: 2
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject*",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_inventory",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "InstanceDynamicObject",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "ObjectLocker",
        decl: "class",
        methods: [
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject*",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetKeyName",
              params: "",
              attrs:  4,
              desc:   "no description",
              return_type: "string",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionOpen",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "ObjectShelf",
            visibility: "public"
          },
          
          {
            name:       "Lockable",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "ObjectStair",
        decl: "class",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "InstanceDynamicObject",
            visibility: "public"
          },
          
          {
            name:       "Waypoint::ArcObserver",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "ObjectItem",
        decl: "class",
        methods: [
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ProcessCollisions",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_item",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "InstanceDynamicObject",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "SceneCamera",
        decl: "class",
        methods: [
          
            {
              name:   "Run",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEnabledScroll",
              params: "bool set",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SwapCameraView",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLimits",
              params: "unsigned int minX,unsigned int minY,unsigned int maxX,unsigned maxY",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SlideToHeight",
              params: "float",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CenterCameraInstant",
              params: "LPoint3f",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CenterCameraOn",
              params: "NodePath np",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FollowNodePath",
              params: "NodePath np",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StopFollowingNodePath",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CenterOnObject",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FollowObject",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetNodePath",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "RunScroll",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunFollow",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunSlideHeight",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_graphicWindow",
            attrs: 1
          },
          
          {
            name: "_camera",
            attrs: 0
          },
          
          {
            name: "_cameraPos",
            attrs: 0
          },
          
          {
            name: "_cameraMovementSpeed",
            attrs: 0
          },
          
          {
            name: "_scrollEnabled",
            attrs: 0
          },
          
          {
            name: "_currentCameraAngle",
            attrs: 8
          },
          
          {
            name: "_currentHpr",
            attrs: 0
          },
          
          {
            name: "_objectiveHpr",
            attrs: 0
          },
          
          {
            name: "_minPosX",
            attrs: 0
          },
          
          {
            name: "_minPosY",
            attrs: 0
          },
          
          {
            name: "_maxPosX",
            attrs: 0
          },
          
          {
            name: "_maxPosY",
            attrs: 0
          },
          
          {
            name: "_destHeight",
            attrs: 0
          },
          
          {
            name: "_centeringCamera",
            attrs: 0
          },
          
          {
            name: "_followingNodePath",
            attrs: 0
          },
          
          {
            name: "_toFollow",
            attrs: 0
          },
          
          {
            name: "_currentPos",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "InteractMenu",
        decl: "class",
        methods: [
          
            {
              name:   "ButtonClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ButtonHovered",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseButton",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteForButtonId",
              params: "Rocket::Core::Event& event,std::function<bool (Rocket::Core::Event&,const std::string&,InstanceDynamicObject::Interaction*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_buttons",
            attrs: 0
          },
          
          {
            name: "_listeners",
            attrs: 0
          },
          
          {
            name: "_buttonListener",
            attrs: 0
          },
          
          {
            name: "_buttonHover",
            attrs: 0
          },
          
          {
            name: "_buttonClick",
            attrs: 0
          },
          
          {
            name: "_obs",
            attrs: 0
          },
          
          {
            name: "_done",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "WorldDiplomacy",
        decl: "class",
        methods: [
          
            {
              name:   "AddFaction",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetFaction",
              params: "const std::string& name",
              attrs:  0,
              desc:   "no description",
              return_type: "WorldDiplomacy::Faction",
              return_attrs: 1
            },
          
            {
              name:   "GetFaction",
              params: "unsigned int flag",
              attrs:  0,
              desc:   "no description",
              return_type: "WorldDiplomacy::Faction",
              return_attrs: 1
            },
          
            {
              name:   "SetAsEnemy",
              params: "bool set,const std::string& name1,const std::string& name2",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAsEnemy",
              params: "bool set,unsigned int flag1,unsigned int flag2",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAsEnemy",
              params: "bool set,Faction& first,Faction& second",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_data_engine",
            attrs: 2
          },
          
          {
            name: "_factions",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "WorldDiplomacy::Faction",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "name",
            attrs: 0
          },
          
          {
            name: "flag",
            attrs: 8
          },
          
          {
            name: "enemyMask",
            attrs: 8
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Parser",
        decl: "class",
        methods: [
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "DataTree",
              return_attrs: 1
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "DataTree",
              return_attrs: 1
            },
          
            {
              name:   "ParseValue",
              params: "DataBranch*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseString",
              params: "DataBranch*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseOther",
              params: "DataBranch*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseObject",
              params: "DataBranch*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseArray",
              params: "DataBranch*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_fileExist",
            attrs: 0
          },
          
          {
            name: "_source",
            attrs: 0
          },
          
          {
            name: "_str",
            attrs: 0
          },
          
          {
            name: "_it",
            attrs: 8
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "WorldMap",
        decl: "class",
        methods: [
          
            {
              name:   "Show",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCityVisible",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddCity",
              params: "const std::string&,float pos_x,float pos_y,float radius",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "HasCity",
              params: "const std::string&",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "MapClicked",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PartyClicked",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CityClicked",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInterrupted",
              params: "bool set",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MoveTowardsCoordinates",
              params: "float x,float y",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MoveTowardsPlace",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetDataEngine",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "DataEngine",
              return_attrs: 2
            },
          
            {
              name:   "GetCaseData",
              params: "int x,int y",
              attrs:  4,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "GetCurrentPosition",
              params: "float& x,float& y",
              attrs:  4,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MapTileGenerator",
              params: "Data map",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdatePartyCursor",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateClock",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsPartyInCity",
              params: "std::string& ret",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetCaseAt",
              params: "int x,int y",
              attrs:  4,
              desc:   "no description",
              return_type: "Rocket::Core::Element",
              return_attrs: 1
            },
          
            {
              name:   "GetCurrentCase",
              params: "int&,int&",
              attrs:  4,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCaseVisibility",
              params: "int x,int y,char visibility",
              attrs:  4,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddCityToList",
              params: "Data",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveMapStatus",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CheckRandomEncounter",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "name",
            attrs: 0
          },
          
          {
            name: "CurrentWorldMap",
            attrs: 17
          },
          
          {
            name: "MapClickedEvent",
            attrs: 0
          },
          
          {
            name: "PartyCursorClicked",
            attrs: 0
          },
          
          {
            name: "CityButtonClicked",
            attrs: 0
          },
          
          {
            name: "ButtonInventory",
            attrs: 0
          },
          
          {
            name: "ButtonCharacter",
            attrs: 0
          },
          
          {
            name: "ButtonPipbuck",
            attrs: 0
          },
          
          {
            name: "ButtonMenu",
            attrs: 0
          },
          
          {
            name: "_mapTree",
            attrs: 1
          },
          
          {
            name: "_dataEngine",
            attrs: 2
          },
          
          {
            name: "_timeManager",
            attrs: 2
          },
          
          {
            name: "_gameUi",
            attrs: 2
          },
          
          {
            name: "_timer",
            attrs: 0
          },
          
          {
            name: "_interrupted",
            attrs: 0
          },
          
          {
            name: "_size_x",
            attrs: 0
          },
          
          {
            name: "_size_y",
            attrs: 0
          },
          
          {
            name: "_tsize_x",
            attrs: 0
          },
          
          {
            name: "_tsize_y",
            attrs: 0
          },
          
          {
            name: "_current_pos_x",
            attrs: 0
          },
          
          {
            name: "_current_pos_y",
            attrs: 0
          },
          
          {
            name: "_goal_x",
            attrs: 0
          },
          
          {
            name: "_goal_y",
            attrs: 0
          },
          
          {
            name: "_cursor",
            attrs: 1
          },
          
          {
            name: "_cities",
            attrs: 0
          },
          
          {
            name: "_cases",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "WorldMap::City",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "pos_x",
            attrs: 0
          },
          
          {
            name: "pos_y",
            attrs: 0
          },
          
          {
            name: "radius",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Filesystem",
        decl: "class",
        methods: [
          
            {
              name:   "FileContent",
              params: "const std::string& filepath,std::string& out",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "FileCopy",
              params: "const std::string& from,const std::string& dest",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Directory",
        decl: "class",
        methods: [
          
            {
              name:   "MakeDir",
              params: "const std::string& str",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "RemoveDir",
              params: "const std::string& str",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "OpenDir",
              params: "const std::string& str",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Path",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetEntries",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Entries",
              return_attrs: 2
            },
          
            {
              name:   "GetEntries",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "Entries",
              return_attrs: 6
            }
          
        ],
        attributes: [
          
          {
            name: "_dirEntries",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "UiLoad",
        decl: "class",
        methods: [
          
            {
              name:   "LoadSlotRml",
              params: "const std::string& prefix,unsigned short slot",
              attrs:  16,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 16
            },
          
            {
              name:   "LoadSlots",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClickSlot",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Erase",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "EventLoadGame",
            attrs: 0
          },
          
          {
            name: "EventClickSlot",
            attrs: 0
          },
          
          {
            name: "EventCancel",
            attrs: 0
          },
          
          {
            name: "EventErase",
            attrs: 0
          },
          
          {
            name: "_selectedSlot",
            attrs: 1
          },
          
          {
            name: "_savePath",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "UiSave",
        decl: "class",
        methods: [
          
            {
              name:   "LoadSlots",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClickSlot",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Erase",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "EventSaveGame",
            attrs: 0
          },
          
          {
            name: "EventClickSlot",
            attrs: 0
          },
          
          {
            name: "EventCancel",
            attrs: 0
          },
          
          {
            name: "EventErase",
            attrs: 0
          },
          
          {
            name: "_selectedSlot",
            attrs: 1
          },
          
          {
            name: "_savePath",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Pipbuck",
        decl: "class",
        methods: [
          
            {
              name:   "Restart",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReloadApps",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartApp",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "QuitApp",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoHome",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DoQuitApp",
              params: "App*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AppAskedFocus",
              params: "App*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AppAskedUnfocus",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AppAskedExit",
              params: "App*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_data_engine",
            attrs: 2
          },
          
          {
            name: "_context",
            attrs: 1
          },
          
          {
            name: "_module",
            attrs: 1
          },
          
          {
            name: "_running_apps",
            attrs: 0
          },
          
          {
            name: "_running_app",
            attrs: 1
          },
          
          {
            name: "_asked_unfocus",
            attrs: 0
          },
          
          {
            name: "_asked_focus",
            attrs: 0
          },
          
          {
            name: "_asked_exit",
            attrs: 0
          },
          
          {
            name: "EventStartApp",
            attrs: 0
          },
          
          {
            name: "EventQuitApp",
            attrs: 0
          },
          
          {
            name: "EventHome",
            attrs: 0
          },
          
          {
            name: "EventHidePipbuck",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Pipbuck::App",
        decl: "struct",
        methods: [
          
            {
              name:   "GetAppId",
              params: "void",
              attrs:  68,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  64,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  64,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "PipbuckAppScript",
        decl: "class",
        methods: [
          
            {
              name:   "GetAppId",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_context",
            attrs: 1
          },
          
          {
            name: "_module",
            attrs: 1
          },
          
          {
            name: "_data",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "Pipbuck::App",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "PipbuckClockApp",
        decl: "class",
        methods: [
          
            {
              name:   "GetAppId",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WaitChange",
              params: "int chance",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Wait",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SelectTimeUnit",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetValue",
              params: "Rocket::Core::Element* element,unsigned short value",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_appid",
            attrs: 4
          },
          
          {
            name: "_time_manager",
            attrs: 2
          },
          
          {
            name: "_inner_rml",
            attrs: 0
          },
          
          {
            name: "EventWaitMore",
            attrs: 0
          },
          
          {
            name: "EventWaitLess",
            attrs: 0
          },
          
          {
            name: "EventTimeUnitSelected",
            attrs: 0
          },
          
          {
            name: "EventWait",
            attrs: 0
          },
          
          {
            name: "_year",
            attrs: 1
          },
          
          {
            name: "_month",
            attrs: 1
          },
          
          {
            name: "_day",
            attrs: 1
          },
          
          {
            name: "_hour",
            attrs: 1
          },
          
          {
            name: "_minute",
            attrs: 1
          },
          
          {
            name: "_second",
            attrs: 1
          },
          
          {
            name: "_wait_days",
            attrs: 1
          },
          
          {
            name: "_wait_hours",
            attrs: 1
          },
          
          {
            name: "_wait_minutes",
            attrs: 1
          },
          
          {
            name: "_selected_wait",
            attrs: 1
          },
          
          {
            name: "_wait_more",
            attrs: 1
          },
          
          {
            name: "_wait_less",
            attrs: 1
          },
          
          {
            name: "_do_wait",
            attrs: 1
          },
          
          {
            name: "_span_error",
            attrs: 1
          },
          
          {
            name: "_minutes_to_spend",
            attrs: 8
          }
          
        ],
        ancestors: [
          
          {
            name:       "Pipbuck::App",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "PipbuckQuestApp",
        decl: "class",
        methods: [
          
            {
              name:   "GetAppId",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadQuestList",
              params: "Rocket::Core::Element*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadQuestView",
              params: "Rocket::Core::Element*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ListQuestHovered",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ListQuestClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "EventQuestHovered",
            attrs: 0
          },
          
          {
            name: "EventQuestClicked",
            attrs: 0
          },
          
          {
            name: "EventBackClicked",
            attrs: 0
          },
          
          {
            name: "_appid",
            attrs: 4
          },
          
          {
            name: "_rml_index",
            attrs: 0
          },
          
          {
            name: "_rml_view",
            attrs: 0
          },
          
          {
            name: "_data_engine",
            attrs: 1
          },
          
          {
            name: "_current_view",
            attrs: 0
          },
          
          {
            name: "_current_quest",
            attrs: 0
          },
          
          {
            name: "_last_hovered",
            attrs: 1
          },
          
          {
            name: "_root",
            attrs: 1
          },
          
          {
            name: "_observer",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "Pipbuck::App",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "Buff",
        decl: "class",
        methods: [
          
            {
              name:   "GetTargetName",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetStatistics",
              params: "void",
              attrs:  4,
              desc:   "no description",
              return_type: "StatController",
              return_attrs: 1
            },
          
            {
              name:   "SetStatistics",
              params: "StatController* v",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Refresh",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InitScripts",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_context",
            attrs: 1
          },
          
          {
            name: "_module",
            attrs: 1
          },
          
          {
            name: "_refresh",
            attrs: 1
          },
          
          {
            name: "_buff",
            attrs: 0
          },
          
          {
            name: "_target_name",
            attrs: 0
          },
          
          {
            name: "_target_stats",
            attrs: 1
          },
          
          {
            name: "_tm",
            attrs: 2
          },
          
          {
            name: "_task",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "BuffManager",
        decl: "struct",
        methods: [
          
            {
              name:   "Save",
              params: "Utils::Packet&,std::function<bool            (const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Load",
              params: "Utils::Packet&,std::function<StatController* (const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CollectGarbage",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cleanup",
              params: "Buff*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "buffs",
            attrs: 0
          },
          
          {
            name: "tm",
            attrs: 2
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "GameTask",
        decl: "class",
        methods: [
          
            {
              name:   "MapOpenLevel",
              params: "std::string name",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLevel",
              params: "Level* level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "do_task",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "AsyncTask::DoneStatus",
              return_attrs: 0
            },
          
            {
              name:   "SaveGame",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "LoadGame",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "OpenLevel",
              params: "const std::string& savepath,const std::string& level,const std::string& entry_zone = ##",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ExitLevel",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CopySave",
              params: "const std::string& savepath,const std::string& slotPath",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SaveToSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadLastState",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Exit",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLevelSpecialEncounter",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLevelEncounter",
              params: "const std::string& type,short n_creeps",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushBuff",
              params: "ObjectCharacter* character,Data buff",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushBuff",
              params: "const std::string& name,Data buff",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveLevelBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SavePartyBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadLevelBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadPartyBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetTimeManager",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "TimeManager",
              return_attrs: 2
            },
          
            {
              name:   "GetDiplomacy",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "WorldDiplomacy",
              return_attrs: 2
            },
          
            {
              name:   "GetQuestManager",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "QuestManager",
              return_attrs: 2
            },
          
            {
              name:   "PlaySound",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "FinishLoad",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveLevel",
              params: "Level* level,const std::string& name",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "LoadLevel",
              params: "WindowFramework* window,GameUi& gameUi,const std::string& path,const std::string& name,const std::string& entry_zone = ##,bool isSaveFile = false",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DoLoadLevel",
              params: "LoadLevelParams",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GameOver",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DoCheckRandomEncounter",
              params: "int x,int y",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPlayerInventory",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "EraseSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "entry_zone",
            attrs: 0
          },
          
          {
            name: "CurrentGameTask",
            attrs: 17
          },
          
          {
            name: "_signals",
            attrs: 0
          },
          
          {
            name: "_continue",
            attrs: 0
          },
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_sound_manager",
            attrs: 0
          },
          
          {
            name: "_gameUi",
            attrs: 0
          },
          
          {
            name: "_dataEngine",
            attrs: 0
          },
          
          {
            name: "_timeManager",
            attrs: 0
          },
          
          {
            name: "_buff_manager",
            attrs: 0
          },
          
          {
            name: "_pipbuck",
            attrs: 0
          },
          
          {
            name: "_charSheet",
            attrs: 1
          },
          
          {
            name: "_playerParty",
            attrs: 1
          },
          
          {
            name: "_playerStats",
            attrs: 1
          },
          
          {
            name: "_playerInventory",
            attrs: 1
          },
          
          {
            name: "_quest_manager",
            attrs: 1
          },
          
          {
            name: "_worldMap",
            attrs: 1
          },
          
          {
            name: "_levelName",
            attrs: 0
          },
          
          {
            name: "_level",
            attrs: 1
          },
          
          {
            name: "_savePath",
            attrs: 0
          },
          
          {
            name: "_uiSaveGame",
            attrs: 1
          },
          
          {
            name: "_uiLoadGame",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "GameTask::LoadLevelParams",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "name",
            attrs: 0
          },
          
          {
            name: "path",
            attrs: 0
          },
          
          {
            name: "isSaveFile",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "MouseCursor",
        decl: "class",
        methods: [
          
            {
              name:   "SetCursorTexture",
              params: "const std::string& texture",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Update",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PullToFront",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "MouseCursor",
              return_attrs: 17
            }
          
        ],
        attributes: [
          
          {
            name: "_cursor",
            attrs: 1
          },
          
          {
            name: "_static",
            attrs: 17
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "OptionsManager",
        decl: "class",
        methods: [
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 16
            },
          
            {
              name:   "Refresh",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "UiNewGame",
        decl: "class",
        methods: [
          
            {
              name:   "SelectedProfile",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClickedStartFromScratch",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToPreviousProfile",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToNextProfile",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToProfile",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoCancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "PreviousProfile",
            attrs: 0
          },
          
          {
            name: "NextProfile",
            attrs: 0
          },
          
          {
            name: "EventSelectProfile",
            attrs: 0
          },
          
          {
            name: "EventStartFromScratch",
            attrs: 0
          },
          
          {
            name: "EventCancel",
            attrs: 0
          },
          
          {
            name: "_data_profiles",
            attrs: 1
          },
          
          {
            name: "_current_profile",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "NewGameTask",
        decl: "class",
        methods: [
          
            {
              name:   "SelectProfile",
              params: "const std::string&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartFromScratch",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Done",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_rocket",
            attrs: 1
          },
          
          {
            name: "_ui_new_game",
            attrs: 0
          },
          
          {
            name: "_stat_view",
            attrs: 1
          },
          
          {
            name: "_stat_controller",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "MainMenu",
        decl: "class",
        methods: [
          
            {
              name:   "Continue",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "EndGame",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "NewGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartGame",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CancelNewGame",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "do_task",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "DoneStatus",
              return_attrs: 0
            },
          
            {
              name:   "AsyncCreateLevel",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "QuitGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenUiLoad",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisplayAlerts",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "_window",
            attrs: 1
          },
          
          {
            name: "_generalUi",
            attrs: 0
          },
          
          {
            name: "_mouseCursor",
            attrs: 0
          },
          
          {
            name: "_new_game_task",
            attrs: 1
          },
          
          {
            name: "_levelTask",
            attrs: 1
          },
          
          {
            name: "_view",
            attrs: 0
          },
          
          {
            name: "_alerts",
            attrs: 0
          },
          
          {
            name: "_uiLoad",
            attrs: 1
          },
          
          {
            name: "_need_garbage_collect",
            attrs: 0
          },
          
          {
            name: "quitGamePlz",
            attrs: 0
          },
          
          {
            name: "createLevelPlz",
            attrs: 0
          },
          
          {
            name: "slotToLoadPlz",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "AsyncTask",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "MainMenu::View",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "NewGameClicked",
            attrs: 0
          },
          
          {
            name: "QuitClicked",
            attrs: 0
          },
          
          {
            name: "LoadGameClicked",
            attrs: 0
          },
          
          {
            name: "OptionsClicked",
            attrs: 0
          },
          
          {
            name: "ContinueClicked",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "AstarPathfinding",
        decl: "class",
        methods: [
          
            {
              name:   "CancelSearch",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetStartAndGoalStates",
              params: "UserState &Start,UserState &Goal",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SearchSuccedded",
              params: "Node* node",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SearchStep",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "State",
              return_attrs: 0
            },
          
            {
              name:   "if",
              params: "_state != Succeeded && _state != Failed",
              attrs:  0,
              desc:   "no description",
              return_type: "else",
              return_attrs: 0
            },
          
            {
              name:   "FreeSolutionNodes",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetStepCount",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "FreeAllNodes",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FreeUnusedNodes",
              params: "bool onlyUnused = true",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PedirNode",
              params: "void",
              attrs:  32,
              desc:   "no description",
              return_type: "AstarPathfinding::Node",
              return_attrs: 1
            },
          
            {
              name:   "Node",
              params: "",
              attrs:  0,
              desc:   "no description",
              return_type: "new",
              return_attrs: 0
            },
          
            {
              name:   "ReleaseNode",
              params: "Node* node",
              attrs:  32,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "userNode",
            attrs: 0
          },
          
          {
            name: "NodeListIterator",
            attrs: 0
          },
          
          {
            name: "Node",
            attrs: 0
          },
          
          {
            name: "Node",
            attrs: 0
          },
          
          {
            name: "node",
            attrs: 0
          },
          
          {
            name: "closedListSearch",
            attrs: 0
          },
          
          {
            name: "openListSearch",
            attrs: 0
          },
          
          {
            name: "listEnd",
            attrs: 0
          },
          
          {
            name: "successor",
            attrs: 0
          },
          
          {
            name: "successor",
            attrs: 0
          },
          
          {
            name: "_state",
            attrs: 0
          },
          
          {
            name: "del",
            attrs: 0
          },
          
          {
            name: "node",
            attrs: 0
          },
          
          {
            name: "_start",
            attrs: 0
          },
          
          {
            name: "_goal",
            attrs: 0
          },
          
          {
            name: "_nSteps",
            attrs: 0
          },
          
          {
            name: "_goal",
            attrs: 0
          },
          
          {
            name: "it",
            attrs: 0
          },
          
          {
            name: "end",
            attrs: 0
          },
          
          {
            name: "node",
            attrs: 0
          },
          
          {
            name: "node",
            attrs: 0
          },
          
          {
            name: "node",
            attrs: 0
          },
          
          {
            name: "_openList",
            attrs: 0
          },
          
          {
            name: "_closedList",
            attrs: 0
          },
          
          {
            name: "_state",
            attrs: 0
          },
          
          {
            name: "_cancelRequest",
            attrs: 0
          },
          
          {
            name: "_nSteps",
            attrs: 0
          },
          
          {
            name: "_start",
            attrs: 1
          },
          
          {
            name: "_goal",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "AstarPathfinding::Node",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "parent",
            attrs: 1
          },
          
          {
            name: "child",
            attrs: 1
          },
          
          {
            name: "g",
            attrs: 0
          },
          
          {
            name: "h",
            attrs: 0
          },
          
          {
            name: "f",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "AstarPathfinding::FunctorCompareNode",
        decl: "struct",
        methods: [
          
            {
              name:   "operator",
              params: "",
              attrs:  4,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Quest",
        decl: "class",
        methods: [
          
            {
              name:   "Initialize",
              params: "Level* level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetData",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "CompleteCondition",
              params: "const std::string& objective,const std::string& condition",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsConditionCompleted",
              params: "const std::string& objective,const std::string& condition",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsOver",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "void ",
              params: "Quest::*WatcherInitializerMethod",
              attrs:  0,
              desc:   "no description",
              return_type: "typedef",
              return_attrs: 0
            },
          
            {
              name:   "CheckIfCompleted",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "InitializeCondition",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WatcherCharacterInventory",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WatcherCharacterKill",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WatcherTime",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "data",
            attrs: 0
          },
          
          {
            name: "method",
            attrs: 0
          },
          
          {
            name: "_observers",
            attrs: 0
          },
          
          {
            name: "_update_hook",
            attrs: 1
          }
          
        ],
        ancestors: [
          
          {
            name:       "Scriptable",
            visibility: "private"
          }
          
        ]
      },
    
      {
        name: "Quest::WatcherInitializer",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "key",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "QuestManager",
        decl: "class",
        methods: [
          
            {
              name:   "AddQuest",
              params: "Data",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "Level* level",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "QuestCompleted",
              params: "Quest*",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "quest",
            attrs: 0
          },
          
          {
            name: "it",
            attrs: 0
          },
          
          {
            name: "end",
            attrs: 0
          },
          
          {
            name: "_data_engine",
            attrs: 2
          },
          
          {
            name: "_quests",
            attrs: 0
          },
          
          {
            name: "_stats_controller",
            attrs: 1
          }
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Dices",
        decl: "struct",
        methods: [
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Throw",
              params: "unsigned int max",
              attrs:  16,
              desc:   "no description",
              return_type: "int",
              return_attrs: 16
            },
          
            {
              name:   "Test",
              params: "int successPercentage",
              attrs:  16,
              desc:   "no description",
              return_type: "bool",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Executor",
        decl: "class",
        methods: [
          
            {
              name:   "ExecuteLater",
              params: "std::function<void (void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  16,
              desc:   "no description",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "Parser",
        decl: "class",
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
    
      {
        name: "UiDialog",
        decl: "class",
        methods: [
          
            {
              name:   "SetMessage",
              params: "const std::string& message",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddChoice",
              params: "const std::string& name,std::function<void (Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PickedChoice",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "no description",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          {
            name: "listener",
            attrs: 0
          },
          
          {
            name: "_button_container",
            attrs: 1
          },
          
          {
            name: "_buttons",
            attrs: 0
          }
          
        ],
        ancestors: [
          
          {
            name:       "UiBase",
            visibility: "public"
          }
          
        ]
      },
    
      {
        name: "UiDialog::Button",
        decl: "struct",
        methods: [
          
        ],
        attributes: [
          
          {
            name: "id",
            attrs: 0
          }
          
        ],
        ancestors: [
          
        ]
      }
    
  ]
};

$(document).ready(function()
{
  $("#sexy-search").keyup(function()
  {
    var value  = $(this).val();
    if (value == '')
      return ($("#search-results").slideUp());
    var regexp = new RegExp(value, 'i');
    var matches_classes = [];

    for (var i = 0 ; i < project.types.length ; i++)
    {
      var klass  = project.types[i];
      var match  = false;
      var struct = { name: klass.name, decl: klass.decl };

      if (klass.name.match(regexp))
        match = true;
      for (var ii = 0 ; ii < klass.methods.length ; ii++)
      {
        var method = klass.methods[ii];
        
        if (method.name.match(regexp) != null)
        {
          if (typeof struct['methods'] == 'undefined')
            struct['methods'] = [];
          struct['methods'].push(method);
          match = true;
        }
      }
      for (var ii = 0 ; ii < klass.attributes.length ; ii++)
      {
        var attribute = klass.attributes[ii];

       if (attribute.name.match(regexp) != null)
       {
         if (typeof struct['attributes'] == 'undefined')
           struct['attributes'] = [];
         struct['attributes'].push(attribute);
         match = true;
       }
      }
      if (match)
        matches_classes.push(struct);
    }
    
    var elem = $("#search-results");
    elem.html('');
    for (var i = 0 ; i < matches_classes.length ; i++)
    {
      var match = matches_classes[i];
      var html  = '';
      
      html += "<div class='panel' style='cursor:pointer;' onclick='document.location=\"./class_" + match.name + ".html\"'>";
      html += "<h5>" + match.decl + ' ' + match.name + "</h5><br />";
      if (typeof match.methods != 'undefined')
      {
        html += "<div class='match-bundle'>";
        for (var ii = 0 ; ii < match.methods.length ; ii++)
        {
          html += "<span class='radius label'>method</span> " + match.methods[ii].name + "<br />";
        }
        html += "</div>";
      }
      if (typeof match.attributes != 'undefined')
      {
        html += "<div class='match-bundle'>";
        for (var ii = 0 ; ii < match.attributes.length ; ii++)
        {
          html += "<span class='radius label success'>attribute</span> " + match.attributes[ii].name + "<br />";
        }
        html += "</div>";
      }
      html += "</div>";
      elem.append(html);
    }
    if (matches_classes.length > 0)
      elem.slideDown();
    else
      elem.slideUp();
  });
});

$(document).ready(function()
{
  $(".show-code").click(function()
  {
    var self = $(this);
    var line = self.parent().parent().parent().parent();

    if (line.next().hasClass('showed-code'))
    {
      if (line.next().is(':visible'))
        line.next().hide();
      else
        line.next().show();
    }
    else
    {
      var html = "<tr class='showed-code'><td></td>";
      html += "<td colspan='3'>";
      html += "<pre class='sh_cpp'>" + self.attr('data-code') + "</pre>";
      html += "</td>";
      html += "</tr>";
      $(html).insertAfter(line);
      sh_highlightDocument();
    }
    console.log(line);
  });
});
