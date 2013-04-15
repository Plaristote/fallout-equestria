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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Packet&",
              attrs:  64,
              desc:   "",
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
              desc:   "",
              return_type: "char",
              return_attrs: 5
            },
          
            {
              name:   "size",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "size_t",
              return_attrs: 0
            },
          
            {
              name:   "PrintContent",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PrintRawContent",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "T& v",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SerializeArray",
              params: "T& tehList",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "canIHaz",
              params: "size_t sizeType,int howMany",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "checkType",
              params: "int assumedType",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "realloc",
              params: "int newsize",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "updateHeader",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "read",
              params: "T& tmp",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "typeCode",
            type: "char",
            
            attrs: 1
          },
          
          
          {
            name: "copy",
            type: "T",
            
            attrs: 1
          },
          
          
          {
            name: "typeCode",
            type: "char",
            
            attrs: 1
          },
          
          
          {
            name: "copy",
            type: "T",
            
            attrs: 1
          },
          
          
          {
            name: "size",
            type: "int",
            
            attrs: 8
          },
          
          
          {
            name: "it",
            type: "unsigned int	",
            
            attrs: 0
          },
          
          
          {
            name: "reading",
            type: "T",
            
            attrs: 0
          },
          
          
          {
            name: "size",
            type: "int",
            
            attrs: 8
          },
          
          
          {
            name: "it",
            type: "unsigned int	",
            
            attrs: 0
          },
          
          
          {
            name: "reading",
            type: "T",
            
            attrs: 0
          },
          
          
          {
            name: "isDuplicate",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "buffer",
            type: "void",
            
            attrs: 1
          },
          
          
          {
            name: "sizeBuffer",
            type: "size_t",
            
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
              desc:   "",
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
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "left",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "top",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "width",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "height",
            type: "float",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnwithdrawArc",
              params: "Waypoint* other,ArcObserver* observer",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWithdrawable",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "",
              return_type: "",
              return_attrs: 1
            },
          
            {
              name:   "ConnectUnsafe",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "",
              return_type: "Arcs::iterator",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "",
              return_type: "Arcs::iterator",
              return_attrs: 0
            },
          
            {
              name:   "Disconnect",
              params: "Waypoint* other",
              attrs:  0,
              desc:   "",
              return_type: "Arcs::iterator",
              return_attrs: 0
            },
          
            {
              name:   "DisconnectAll",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetArcTo",
              params: "unsigned int id",
              attrs:  0,
              desc:   "",
              return_type: "Waypoint::Arc",
              return_attrs: 1
            },
          
            {
              name:   "PositionChanged",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateArcDirection",
              params: "Waypoint*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSelected",
              params: "bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsSelected",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetArcsVisible",
              params: "bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoalDistanceEstimate",
              params: "const Waypoint& goal",
              attrs:  4,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "GetDistanceEstimate",
              params: "const Waypoint& other",
              attrs:  4,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "GetCost",
              params: "Waypoint&",
              attrs:  0,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "LoadArcs",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnserializeLoadArcs",
              params: "World*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetMouseBox",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "observer",
            type: "ArcObserver",
            
            obj_type: "Waypoint::ArcObserver",
            
            attrs: 1
          },
          
          
          {
            name: "id",
            type: "int",
            
            attrs: 8
          },
          
          
          {
            name: "floor",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "arcs",
            type: "Arcs",
            
            attrs: 0
          },
          
          
          {
            name: "arcs_withdrawed",
            type: "ArcsWithdrawed",
            
            attrs: 0
          },
          
          
          {
            name: "nodePath",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "mouseBox",
            type: "FBoundingBox",
            
            obj_type: "FBoundingBox",
            
            attrs: 0
          },
          
          
          {
            name: "World",
            type: "struct",
            
            attrs: 0
          },
          
          
          {
            name: "selected",
            type: "bool",
            
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
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GoingThrough",
              params: "void*",
              attrs:  64,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVisible",
              params: "bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "nodePath",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "from",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "to",
            type: "Waypoint",
            
            obj_type: "Waypoint",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "nodePath",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "floor",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "strModel",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "strTexture",
            type: "std::string",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "waypoint",
            type: "Waypoint",
            
            obj_type: "Waypoint",
            
            attrs: 1
          },
          
          
          {
            name: "type",
            type: "Type",
            
            attrs: 0
          },
          
          
          {
            name: "script",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "locked",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "key",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "charsheet",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "interactions",
            type: "char",
            
            attrs: 0
          },
          
          
          {
            name: "dialog",
            type: "std::string",
            
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
            type: "std::string",
            
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
              desc:   "",
              return_type: "LColor",
              return_attrs: 0
            },
          
            {
              name:   "SetColor",
              params: "float r,float g,float b,float a",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "color",
              params: "r,g,b,a",
              attrs:  0,
              desc:   "",
              return_type: "LColor",
              return_attrs: 0
            },
          
            {
              name:   "UnSerialize",
              params: "World*,Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReparentTo",
              params: "DynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReparentTo",
              params: "MapObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "name",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "nodePath",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "type",
            type: "Type",
            
            attrs: 0
          },
          
          
          {
            name: "parent_type",
            type: "ParentType",
            
            attrs: 0
          },
          
          
          {
            name: "lens",
            type: "Lens",
            
            attrs: 1
          },
          
          
          {
            name: "zoneSize",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "parent",
            type: "NodePath",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddWayPoint",
              params: "float x,float y,float z",
              attrs:  0,
              desc:   "",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "DeleteWayPoint",
              params: "Waypoint*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointFromNodePath",
              params: "NodePath path",
              attrs:  0,
              desc:   "",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "GetWaypointFromId",
              params: "unsigned int id",
              attrs:  0,
              desc:   "",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "GetWaypointClosest",
              params: "LPoint3",
              attrs:  0,
              desc:   "",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "SetWaypointsVisible",
              params: "bool v",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointLimits",
              params: "short currentFloor,LPoint3& upperRight,LPoint3& upperLeft,LPoint3& bottomLeft",
              attrs:  4,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointPlane",
              params: "short currentFloor",
              attrs:  4,
              desc:   "",
              return_type: "LPlane",
              return_attrs: 0
            },
          
            {
              name:   "DeleteObject",
              params: "MapObject* ptr,std::list<OBJTYPE>& list",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetObjectFromName",
              params: "const std::string& name,std::list<OBJTYPE>& list",
              attrs:  0,
              desc:   "",
              return_type: "OBJTYPE",
              return_attrs: 1
            },
          
            {
              name:   "GetObjectFromNodePath",
              params: "NodePath path,std::list<OBJTYPE>& list",
              attrs:  0,
              desc:   "",
              return_type: "OBJTYPE",
              return_attrs: 1
            },
          
            {
              name:   "ObjectChangeFloor",
              params: "MapObject&,unsigned char floor,unsigned short type",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddMapObject",
              params: "const std::string& name,const std::string& model,const std::string& texture,float x,float y,float z",
              attrs:  0,
              desc:   "",
              return_type: "MapObject",
              return_attrs: 1
            },
          
            {
              name:   "DeleteMapObject",
              params: "MapObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetMapObjectFromName",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "MapObject",
              return_attrs: 1
            },
          
            {
              name:   "GetMapObjectFromNodePath",
              params: "NodePath path",
              attrs:  0,
              desc:   "",
              return_type: "MapObject",
              return_attrs: 1
            },
          
            {
              name:   "SetMapObjectsVisible",
              params: "bool v",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MapObjectChangeFloor",
              params: "MapObject&,unsigned char floor",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertDynamicObject",
              params: "DynamicObject&",
              attrs:  0,
              desc:   "",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "AddDynamicObject",
              params: "const std::string& name,DynamicObject::Type type,const std::string& model,const std::string& texture",
              attrs:  0,
              desc:   "",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "DeleteDynamicObject",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetDynamicObjectFromName",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetDynamicObjectFromNodePath",
              params: "NodePath path",
              attrs:  0,
              desc:   "",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "SetDynamicObjectsVisible",
              params: "bool v",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DynamicObjectSetWaypoint",
              params: "DynamicObject&,Waypoint&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DynamicObjectChangeFloor",
              params: "DynamicObject&,unsigned char floor",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddExitZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DeleteExitZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetExitZoneByName",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "ExitZone",
              return_attrs: 1
            },
          
            {
              name:   "AddEntryZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DeleteEntryZone",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetEntryZoneByName",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "EntryZone",
              return_attrs: 1
            },
          
            {
              name:   "AddLight",
              params: "WorldLight::Type,const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddLight",
              params: "WorldLight::Type,const std::string&,MapObject* parent",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddLight",
              params: "WorldLight::Type,const std::string&,DynamicObject* parent",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DeleteLight",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetLightByName",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "WorldLight",
              return_attrs: 1
            },
          
            {
              name:   "CompileLight",
              params: "WorldLight*,unsigned char = ColMask::Object | ColMask::DynObject",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetWaypointAt",
              params: "LPoint2f",
              attrs:  0,
              desc:   "",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "UnSerialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CompileWaypoints",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CompileDoors",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "floors",
            type: "Floors",
            
            attrs: 0
          },
          
          
          {
            name: "rootWaypoints",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "waypoints",
            type: "Waypoints",
            
            attrs: 0
          },
          
          
          {
            name: "rootMapObjects",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "objects",
            type: "MapObjects",
            
            attrs: 0
          },
          
          
          {
            name: "rootDynamicObjects",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "dynamicObjects",
            type: "DynamicObjects",
            
            attrs: 0
          },
          
          
          {
            name: "rootLights",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "lights",
            type: "WorldLights",
            
            attrs: 0
          },
          
          
          {
            name: "exitZones",
            type: "ExitZones",
            
            attrs: 0
          },
          
          
          {
            name: "entryZones",
            type: "EntryZones",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Leave",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ConstGetObjects",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "DynamicObjects",
              return_attrs: 6
            },
          
            {
              name:   "GetObjects",
              params: "void",
              attrs:  0,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Save",
              params: "const std::string& savepath",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetHasLocalObjects",
              params: "bool val",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet&",
              attrs:  4,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnSerialize",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_local_objects",
            type: "bool",
            
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
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Wait",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Post",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetDeadlockSafety",
              params: "bool to_set",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SelfLock",
              params: "void",
              attrs:  36,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetCurrentThreadId",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "ThreadId",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          
          {
            name: "Handle",
            type: "sem_t",
            
            attrs: 0
          },
          
          
          {
            name: "ThreadId",
            type: "std::thread::id",
            
            attrs: 0
          },
          
          
          {
            name: "Handle",
            type: "HANDLE",
            
            attrs: 0
          },
          
          
          {
            name: "ThreadId",
            type: "DWORD",
            
            attrs: 0
          },
          
          
          {
            name: "_semaphore",
            type: "Semaphore",
            
            obj_type: "Semaphore",
            
            attrs: 2
          },
          
          
          {
            name: "_handle",
            type: "Handle",
            
            attrs: 0
          },
          
          
          {
            name: "_max_count",
            type: "int",
            
            attrs: 8
          },
          
          
          {
            name: "_deadlock_safety",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_thread_lock",
            type: "bool",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetDirect",
              params: "bool",
              attrs:  64,
              desc:   "",
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
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsDirect",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Emit",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "ObserverClass& observerInstance,typename Observer<ObserverClass>::Method method",
              attrs:  0,
              desc:   "",
              return_type: "ObserverId",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "typename FunctionObserver::Function function",
              attrs:  0,
              desc:   "",
              return_type: "ObserverId",
              return_attrs: 0
            },
          
            {
              name:   "Disconnect",
              params: "ObserverId id",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisconnectAll",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ObserverCount",
              params: "void",
              attrs:  36,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteRecordedCalls",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "FuncExecuteRecordedCalls",
              params: "RecordedCalls& calls",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushRecordCall",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "FetchRecordCall",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "RecordCallCount",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "PushedCallCount",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "lock",
              params: "_semaphore",
              attrs:  0,
              desc:   "",
              return_type: "Semaphore::Lock",
              return_attrs: 0
            },
          
            {
              name:   "BackupRecordedCalls",
              params: "bool on_off",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddObserver",
              params: "InterfaceObserver* observer",
              attrs:  0,
              desc:   "",
              return_type: "ObserverId",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "ObserverHandler",
            type: "class",
            
            attrs: 0
          },
          
          
          {
            name: "id",
            type: "ObserverId",
            
            attrs: 0
          },
          
          
          {
            name: "byte",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "it",
            type: "delete",
            
            attrs: 1
          },
          
          
          {
            name: "observer",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "params",
            type: "RecordedCall",
            
            obj_type: "Signal::RecordedCall",
            
            attrs: 0
          },
          
          
          {
            name: "_iterator",
            type: "Observers::iterator",
            
            attrs: 0
          },
          
          
          {
            name: "_iterator_updated_during_emit",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_observers",
            type: "Observers",
            
            attrs: 0
          },
          
          
          {
            name: "_recordedCalls",
            type: "RecordedCalls",
            
            attrs: 0
          },
          
          
          {
            name: "_backedupCalls",
            type: "RecordedCalls",
            
            attrs: 0
          },
          
          
          {
            name: "_pushedCalls",
            type: "RecordedCalls",
            
            attrs: 0
          },
          
          
          {
            name: "_direct",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_semaphore",
            type: "Semaphore",
            
            obj_type: "Semaphore",
            
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
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_function",
            type: "Function",
            
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
              desc:   "",
              return_type: "typedef",
              return_attrs: 0
            },
          
            {
              name:   "operator",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_observer",
            type: "ObserverClass",
            
            attrs: 2
          },
          
          
          {
            name: "_method",
            type: "Method",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Utils::Packet& packet",
              attrs:  0,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisconnectAllFrom",
              params: "const ISignal& signal",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Add",
              params: "C& signal,ObserverId id",
              attrs:  32,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "C& signal,ObserverClass& observerInstance,typename C::template Observer<ObserverClass>::Method method",
              attrs:  32,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Connect",
              params: "C& signal,typename C::FunctionObserver::Function function",
              attrs:  32,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "observer",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "observer",
            type: "delete",
            
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
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "signal",
            type: "C",
            
            attrs: 2
          },
          
          
          {
            name: "id",
            type: "ObserverId",
            
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
              desc:   "",
              return_type: "double",
              return_attrs: 0
            },
          
            {
              name:   "Profile",
              params: "std::string str",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Restart",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLastTime",
              params: "double v",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetLastTime",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "asConstructor",
              params: "void* memory",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "asDestructor",
              params: "void* memory",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          
          {
            name: "_lastTime",
            type: "double",
            
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
              desc:   "",
              return_type: "short",
              return_attrs: 24
            },
          
            {
              name:   "ClearTasks",
              params: "unsigned char level",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTime",
              params: "unsigned short s,unsigned short m,unsigned short h,unsigned short d,unsigned short mo,unsigned short y",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetYear",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetMonth",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetDay",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetHour",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetMinute",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetSecond",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "AddElapsedSeconds",
              params: "float s",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddElapsedTime",
              params: "unsigned short s,unsigned short m = 0,unsigned short h = 0,unsigned short d = 0,unsigned short mo = 0,unsigned short y = 0",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddTask",
              params: "unsigned char level,bool loop,unsigned short s,unsigned short m = 0,unsigned short h = 0,unsigned short d = 0,unsigned short mo = 0,unsigned short y = 0",
              attrs:  0,
              desc:   "",
              return_type: "TimeManager::Task",
              return_attrs: 1
            },
          
            {
              name:   "DelTask",
              params: "Task* task",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteTasks",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CorrectValues",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "it",
            type: "Tasks::iterator",
            
            attrs: 0
          },
          
          
          {
            name: "end",
            type: "Tasks::iterator ",
            
            attrs: 0
          },
          
          
          {
            name: "task",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "fseconds",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "year",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "month",
            type: "unsigned short  ",
            
            attrs: 0
          },
          
          
          {
            name: "day",
            type: "short  ",
            
            attrs: 8
          },
          
          
          {
            name: "hour",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "minute",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "second",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "tasks",
            type: "Tasks",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Serialize",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unserialize",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "loop",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "it",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "lastY",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "lastMo",
            type: "unsigned short ",
            
            attrs: 0
          },
          
          
          {
            name: "lastD",
            type: "short ",
            
            attrs: 8
          },
          
          
          {
            name: "lastH",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "lastM",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "lastS",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "timeY",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "timeMo",
            type: "unsigned short ",
            
            attrs: 0
          },
          
          
          {
            name: "timeD",
            type: "short ",
            
            attrs: 8
          },
          
          
          {
            name: "timeH",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "timeM",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "timeS",
            type: "",
            
            attrs: 8
          },
          
          
          {
            name: "level",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "TimeManager",
            type: "class",
            
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
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "value",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "children",
            type: "Children",
            
            attrs: 0
          },
          
          
          {
            name: "father",
            type: "DataBranch",
            
            obj_type: "DataBranch",
            
            attrs: 1
          },
          
          
          {
            name: "nil",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "root",
            type: "bool         ",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "Value",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "SetKey",
              params: "const std::string& newKey",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Duplicate",
              params: "Data var",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "T",
              params: "",
              attrs:  4,
              desc:   "",
              return_type: "operator",
              return_attrs: 0
            },
          
            {
              name:   "Remove",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CutBranch",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Nil",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "NotNil",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Parent",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "PushBack",
              params: "Data d",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Output",
              params: "unsigned char indent = 0",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Count",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "begin",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "Data::my_iterator",
              return_attrs: 0
            },
          
            {
              name:   "const_begin",
              params: "",
              attrs:  4,
              desc:   "",
              return_type: "Data::const_my_iterator",
              return_attrs: 0
            },
          
            {
              name:   "end",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "Data::my_iterator",
              return_attrs: 0
            },
          
            {
              name:   "const_end",
              params: "",
              attrs:  4,
              desc:   "",
              return_type: "Data::const_my_iterator",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "Children",
            type: "DataBranch::Children",
            
            attrs: 0
          },
          
          
          {
            name: "stream",
            type: "std::stringstream",
            
            attrs: 0
          },
          
          
          {
            name: "stream",
            type: "std::stringstream",
            
            attrs: 0
          },
          
          
          {
            name: "stream",
            type: "std::stringstream",
            
            attrs: 0
          },
          
          
          {
            name: "out",
            type: "T",
            
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
            type: "Children::iterator",
            
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
            type: "Children::const_iterator",
            
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
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "const std::string& path",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetSourceFile",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            }
          
        ],
        attributes: [
          
          
          {
            name: "Data",
            type: "class",
            
            attrs: 0
          },
          
          
          {
            name: "Factory",
            type: "struct",
            
            attrs: 0
          },
          
          
          {
            name: "source",
            type: "std::string",
            
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
              desc:   "",
              return_type: "DataTree",
              return_attrs: 17
            },
          
            {
              name:   "ShinyLang",
              params: "const std::string& filename",
              attrs:  16,
              desc:   "",
              return_type: "DataTree",
              return_attrs: 17
            },
          
            {
              name:   "JSON",
              params: "const std::string& filename",
              attrs:  16,
              desc:   "",
              return_type: "DataTree",
              return_attrs: 17
            },
          
            {
              name:   "StringJSON",
              params: "const std::string& str",
              attrs:  16,
              desc:   "",
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
              desc:   "",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "StringJSON",
              params: "Data,std::string& str",
              attrs:  16,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "const std::string& filepath",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetDiplomacy",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "WorldDiplomacy",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          
          {
            name: "_dataTree",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_diplomacy",
            type: "WorldDiplomacy",
            
            obj_type: "WorldDiplomacy",
            
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
              desc:   "",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "ResetAnimation",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayAnimation",
              params: "const std::string& name,bool loop = false",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadAnimation",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TaskAnimation",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayIdleAnimation",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "pendingAnimationDone",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_modelName",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_anims",
            type: "AnimControlCollection",
            
            attrs: 0
          },
          
          
          {
            name: "_mapAnims",
            type: "MapAnims",
            
            attrs: 0
          },
          
          
          {
            name: "_anim",
            type: "AnimControl",
            
            attrs: 1
          },
          
          
          {
            name: "_animLoop",
            type: "bool",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UnprocessCollisions",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "HasOccupiedWaypoint",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetOccupiedWaypointAsInt",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetOccupiedWaypoint",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "Waypoint",
              return_attrs: 1
            },
          
            {
              name:   "SetOccupiedWaypoint",
              params: "Waypoint* wp",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawAllArcs",
              params: "unsigned int id",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawAllArcs",
              params: "Waypoint* waypoint",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawArc",
              params: "unsigned int id1,unsigned int id2",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WithdrawArc",
              params: "Waypoint* first,Waypoint* second",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_collision_processed",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "_level",
            type: "Level",
            
            obj_type: "Level",
            
            attrs: 1
          },
          
          
          {
            name: "_waypointOccupied",
            type: "Waypoint",
            
            obj_type: "Waypoint",
            
            attrs: 1
          },
          
          
          {
            name: "observer",
            type: "Waypoint::ArcObserver",
            
            obj_type: "Waypoint::ArcObserver",
            
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
            type: "Waypoint",
            
            obj_type: "Waypoint",
            
            attrs: 1
          },
          
          
          {
            name: "second",
            type: "Waypoint              ",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "float elapsedTime",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "GetNodePath",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "GetInteractions",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "InteractionList",
              return_attrs: 2
            },
          
            {
              name:   "GetDialog",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetDynamicObject",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject* character",
              attrs:  64,
              desc:   "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "C",
              return_attrs: 1
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ResetAnimation",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ResetInteractions",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ThatDoesNothing",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackAnimationEnded",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "signal",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "min_distance",
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "pendingActionOn",
            type: "InstanceDynamicObject",
            
            obj_type: "InstanceDynamicObject",
            
            attrs: 1
          },
          
          
          {
            name: "pendingActionObject",
            type: "InventoryObject",
            
            obj_type: "InventoryObject",
            
            attrs: 1
          },
          
          
          {
            name: "pendingActionObjectActionIt",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "_type",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "_object",
            type: "DynamicObject",
            
            obj_type: "DynamicObject",
            
            attrs: 1
          },
          
          
          {
            name: "_interactions",
            type: "InteractionList",
            
            attrs: 0
          },
          
          
          {
            name: "_idle_size",
            type: "LPoint3",
            
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
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "instance",
            type: "InstanceDynamicObject",
            
            obj_type: "InstanceDynamicObject",
            
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
            type: "Waypoint",
            
            obj_type: "Waypoint",
            
            attrs: 1
          },
          
          
          {
            name: "objective",
            type: "InstanceDynamicObject",
            
            obj_type: "InstanceDynamicObject",
            
            attrs: 1
          },
          
          
          {
            name: "max_distance",
            type: "int",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "CanGoThrough",
              params: "unsigned char id",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GoingThrough",
              params: "void* character",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_name",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_level",
            type: "Level",
            
            obj_type: "Level",
            
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
              desc:   "",
              return_type: "string",
              return_attrs: 0
            },
          
            {
              name:   "IsLocked",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsOpen",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Unlock",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_closed",
            type: "bool",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetKeyName",
              params: "",
              attrs:  4,
              desc:   "",
              return_type: "string",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject* character",
              attrs:  0,
              desc:   "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "ObserveWaypoints",
              params: "bool doObserver",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CanGoThrough",
              params: "unsigned char id",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GoingThrough",
              params: "void*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionOpen",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "",
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
              desc:   "",
              return_type: "asIScriptEngine",
              return_attrs: 17
            },
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "LoadModule",
              params: "const std::string& name,const std::string& filepath",
              attrs:  16,
              desc:   "",
              return_type: "asIScriptModule",
              return_attrs: 17
            },
          
            {
              name:   "MessageCallback",
              params: "const asSMessageInfo* msg,void* param",
              attrs:  16,
              desc:   "",
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
              desc:   "",
              return_type: "asIScriptModule",
              return_attrs: 17
            },
          
            {
              name:   "Release",
              params: "asIScriptModule* module",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "OutputFunctionList",
              params: "asIScriptModule* module",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          
          {
            name: "users",
            type: "short",
            
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
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "ptr",
            type: "asIScriptModule",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Register",
              params: "asIScriptEngine* engine,const std::string& arrayName,const std::string& typeName",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Constructor",
              params: "void* memory",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Destructor ",
              params: "void* memory",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Size",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "Begin",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "StdList::asIterator",
              return_attrs: 0
            },
          
            {
              name:   "RBegin",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "StdList::asRIterator",
              return_attrs: 0
            },
          
            {
              name:   "End",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "StdList::asIterator",
              return_attrs: 0
            },
          
            {
              name:   "REnd",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "StdList::asRIterator",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "self",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "self",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "toRet",
            type: "asIterator",
            
            obj_type: "StdList::asIterator",
            
            attrs: 0
          },
          
          
          {
            name: "toRet",
            type: "asRIterator",
            
            obj_type: "StdList::asRIterator",
            
            attrs: 0
          },
          
          
          {
            name: "toRet",
            type: "asIterator",
            
            obj_type: "StdList::asIterator",
            
            attrs: 0
          },
          
          
          {
            name: "toRet",
            type: "asRIterator",
            
            obj_type: "StdList::asRIterator",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Destructor ",
              params: "void* memory",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Assign",
              params: "asIterator it",
              attrs:  16,
              desc:   "",
              return_type: "T",
              return_attrs: 16
            },
          
            {
              name:   "Value",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "T",
              return_attrs: 0
            },
          
            {
              name:   "Equal",
              params: "asIterator comp",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Increment",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Decrement",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "it",
            type: "::iterator",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Destructor ",
              params: "void* memory",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Assign",
              params: "asRIterator it",
              attrs:  16,
              desc:   "",
              return_type: "T",
              return_attrs: 16
            },
          
            {
              name:   "Value",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "T",
              return_attrs: 0
            },
          
            {
              name:   "Equal",
              params: "asRIterator comp",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Increment",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Decrement",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "it",
            type: "::reverse_iterator",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Unload",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "T",
              params: "const std::string& key",
              attrs:  16,
              desc:   "",
              return_type: "std::string",
              return_attrs: 16
            },
          
            {
              name:   "GetDialogs",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "Data",
              return_attrs: 16
            },
          
            {
              name:   "GetStatistics",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "Data",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          
          {
            name: "_self",
            type: "i18n",
            
            obj_type: "i18n",
            
            attrs: 17
          },
          
          
          {
            name: "_dialogs",
            type: "DataTree",
            
            obj_type: "DataTree",
            
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
              desc:   "",
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
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FireHide",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Show",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsVisible",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetModal",
              params: "bool modal",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPropertyOnAll",
              params: "Rocket::Core::Element* elem,const std::string& property,const std::string& value",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "FadeOut",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FadeIn",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleEventListener",
              params: "bool toggle_on,const std::string& id,const std::string& event,RocketListener& listener",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Translate",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RecursiveTranslate",
              params: "Rocket::Core::Element*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "GameUi",
            type: "class",
            
            attrs: 0
          },
          
          
          {
            name: "instance",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 2
          },
          
          
          {
            name: "child",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_root",
            type: "Rocket::Core::ElementDocument",
            
            attrs: 1
          },
          
          
          {
            name: "_context",
            type: "Rocket::Core::Context",
            
            attrs: 1
          },
          
          
          {
            name: "_languageObs",
            type: "Sync::ObserverId",
            
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
            type: "std::string",
            
            attrs: 4
          },
          
          
          {
            name: "event",
            type: "std::string",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "GameUi",
            type: "class",
            
            attrs: 0
          },
          
          
          {
            name: "_continueClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_exitClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_optionsClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_saveClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_loadClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentAP",
              params: "unsigned short ap,unsigned short maxap",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetMaxAP",
              params: "unsigned short ap",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEnabledAP",
              params: "bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentHp",
              params: "short hp",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentAc",
              params: "short ac",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEquipedItem",
              params: "unsigned short it,InventoryObject* item",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEquipedItemAction",
              params: "unsigned short it,InventoryObject* item,unsigned char actionIt",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackEquipedItem1Clicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackEquipedItem2Clicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackPassTurnClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCombatEndClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "GameUi",
            type: "class",
            
            attrs: 0
          },
          
          
          {
            name: "MenuButtonClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "InventoryButtonClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "PersButtonClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "PipbuckButtonClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EquipedItem1Clicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EquipedItem2Clicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "PassTurnClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "CombatEndClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_apMax",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "_apEnabled",
            type: "bool",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "UseOn",
              params: "ObjectCharacter* user,InstanceDynamicObject* target,unsigned char useType",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "Use",
              params: "ObjectCharacter* user,unsigned char useType",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetIcon",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "CreateDynamicObject",
              params: "World* world",
              attrs:  4,
              desc:   "",
              return_type: "DynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "CreateEquipedModel",
              params: "World* world",
              attrs:  0,
              desc:   "",
              return_type: "InventoryObject::EquipedModel",
              return_attrs: 1
            },
          
            {
              name:   "CanWeild",
              params: "ObjectCharacter*,EquipedMode",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetEquiped",
              params: "bool set",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsEquiped",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsGroupableWith",
              params: "const InventoryObject*",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteHook",
              params: "asIScriptFunction* hook,ObjectCharacter* user,C* target,unsigned char actionIt",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            }
          
        ],
        attributes: [
          
          
          {
            name: "_dataTree",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 0
          },
          
          
          {
            name: "_equiped",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_script_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_script_module",
            type: "asIScriptModule",
            
            attrs: 1
          },
          
          
          {
            name: "UseAsWeapon",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_actionHooks",
            type: "ActionsHooks",
            
            attrs: 0
          },
          
          
          {
            name: "_hookUseOnCharacter",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_hookUseOnDoor",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_hookUseOnOthers",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_hookUseAsWeapon",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_hookCanWeildMouth",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_hookCanWeildMagic",
            type: "asIScriptFunction* ",
            
            attrs: 1
          },
          
          
          {
            name: "_hookCanWeildBattleSaddle",
            type: "",
            
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
              desc:   "",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "ResetAnimation",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "np",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "object",
            type: "InventoryObject",
            
            obj_type: "InventoryObject",
            
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
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "UseOnCharacter",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "UseOnDoor",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "UseOnOthers",
            type: "asIScriptFunction",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveInventory",
              params: "DynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadInventory",
              params: "Data",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveInventory",
              params: "Data",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DelObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IncludesObject",
              params: "InventoryObject*",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetContent",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "Content",
              return_attrs: 6
            },
          
            {
              name:   "GetContent",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Content",
              return_attrs: 2
            },
          
            {
              name:   "GetObject",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "InventoryObject",
              return_attrs: 1
            },
          
            {
              name:   "ContainsHowMany",
              params: "const std::string& name",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetCurrentWeight",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetCapacity",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetCapacity",
              params: "unsigned short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CanCarry",
              params: "InventoryObject*,unsigned short quantity = 1",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_content",
            type: "Content",
            
            attrs: 0
          },
          
          
          {
            name: "_currentWeight",
            type: "short",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReloadFunction",
              params: "asIScriptFunction** pointer",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_script_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_script_module",
            type: "asIScriptModule",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RestoreBackup",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Action",
              params: "const std::string& action,const std::string& fmt,...",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetAll",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "SetName",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetName",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "SetRace",
              params: "const std::string& race",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetRace",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "SetAge",
              params: "unsigned short age",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetAge",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetGender",
              params: "const std::string& g",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGender",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "AddKill",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetKills",
              params: "const std::string& race",
              attrs:  4,
              desc:   "",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "AddPerk",
              params: "const std::string& perk",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "ToggleTrait",
              params: "const std::string& trait",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleSkillAffinity",
              params: "const std::string& skill",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "HasTrait",
              params: "const std::string& trait",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "HasSkillAffinity",
              params: "const std::string& skill",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetSkillAffinities",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "SetStatistic",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSpecial",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkill",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetLevel",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetStatistic",
              params: "const std::string& stat",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "GetSpecial",
              params: "const std::string& stat",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "GetSkill",
              params: "const std::string& stat",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "SelectRandomEncounter",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 0
            },
          
            {
              name:   "GetSpecialPoints",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetSkillPoints",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetPerksPoints",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetXpNextLevel",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetExperience",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetExperience",
              params: "unsigned short e",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LevelUp",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentHp",
              params: "short hp",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetCurrentHp",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "GetMaxHp",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "IsReady",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "UpdateAllValues",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "LoadFunctions",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_statsheet",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "_statsheet_backup",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "_scriptAddSpecialPoint",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptActivateTraits",
            type: "asIScriptFunction ",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptAddExperience",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptXpNextLevel",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptLevelUp",
            type: "asIScriptFunction ",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptUpdateAllValues",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptIsReady",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptAvailableTraits",
            type: "asIScriptFunction ",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptAddPerk",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_selectRandomEncounter",
            type: "asIScriptFunction",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetEditMode",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "EditMode",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Show",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,const std::string& value",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,short value",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,const std::string& value",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,short value",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCategoryFields",
              params: "const std::string& category,const std::vector<std::string>& keys",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,const std::string& value",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,short value",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetExperience",
              params: "unsigned int xp,unsigned short lvl,unsigned int next_level",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraits",
              params: "std::list<std::string>",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraitActive",
              params: "const std::string&,bool",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPerks",
              params: "std::list<std::string>",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAvailablePerks",
              params: "std::list<std::string> perks",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkillAffinity",
              params: "const std::string& skill,bool",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetNumPerks",
              params: "unsigned short n_perks",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_editMode",
            type: "EditMode",
            
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
              desc:   "",
              return_type: "StatModel",
              return_attrs: 2
            },
          
            {
              name:   "Model",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "StatModel",
              return_attrs: 6
            },
          
            {
              name:   "GetData",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "SetView",
              params: "StatView*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpSpecial",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DownSpecial",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSpecial",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetStatistic",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpSkill",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DownSkill",
              params: "const std::string& stat",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkill",
              params: "const std::string& stat,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddExperience",
              params: "unsigned int experience",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentHp",
              params: "short hp",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TriggerSkillAffinity",
              params: "const std::string& stat,bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddKill",
              params: "const std::string& race",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMetabolism",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetMaxHp",
              params: "short hp",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SpecialChanged",
              params: "const std::string&,short",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SkillChanged",
              params: "const std::string&,short",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatisticChanged",
              params: "const std::string&,short",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PerksChanged",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TraitToggled",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SkillAffinityToggled",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LevelChanged",
              params: "unsigned short",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InformationChanged",
              params: "const std::string&,const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AgeChanged",
              params: "unsigned char",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PerkAdded",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ViewStatUpped",
              params: "const std::string&,const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ViewStatDowned",
              params: "const std::string&,const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AcceptChanges",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CancelChanges",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MakeBackup",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_model",
            type: "StatModel",
            
            obj_type: "StatModel",
            
            attrs: 0
          },
          
          
          {
            name: "_view",
            type: "StatView",
            
            obj_type: "StatView",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Show",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,const std::string& value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInformation",
              params: "const std::string& name,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,const std::string& value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFieldValue",
              params: "const std::string& category,const std::string& key,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,const std::string& value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetIdValue",
              params: "const std::string& id,short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCategoryFields",
              params: "const std::string& category,const std::vector<std::string>& keys",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetExperience",
              params: "unsigned int,unsigned short,unsigned int",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraits",
              params: "std::list<std::string>",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetTraitActive",
              params: "const std::string&,bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSkillAffinity",
              params: "const std::string& skill,bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPerks",
              params: "std::list<std::string>",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAvailablePerks",
              params: "std::list<std::string>",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Accept",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateName",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateGender",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateAge",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TraitClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SpecialClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SkillClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GeneralClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatMore",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatLess",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatUpdate",
              params: "Rocket::Core::Event& event,std::string& type,std::string& stat",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_i18n",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "_perks_dialog",
            type: "PerksDialog",
            
            obj_type: "StatViewRocket::PerksDialog",
            
            attrs: 0
          },
          
          
          {
            name: "CancelButton",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "DoneButton",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventSpecialClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventSkillClicked",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventGeneralClicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "EventTraitClicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonUp",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonDown",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventNameChanged",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventAgeChanged",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventGenderChanged",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "SpecialSelected",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "SkillSelected",
            type: "RocketListener         ",
            
            attrs: 0
          },
          
          
          {
            name: "TraitSelected",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_specialSelected",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_skillSelected",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_traitSelected",
            type: "Rocket::Core::Element",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPerkDescription",
              params: "std::string description",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClearPerksButtons",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSelectedPerk",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackChoosePerk",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancel",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackDblClickPerk",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "Cancel",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "SelectPerk",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "ChoosePerk",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "DblClickPerk",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_selected_perk",
            type: "string",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "Begin",
              params: "ObjectCharacter* from,TimeManager::Task* task = 0",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "End",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Load",
              params: "Level*,ObjectCharacter*,Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "Level*,ObjectCharacter*,Data buff",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_buff",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "_timeManager",
            type: "TimeManager",
            
            obj_type: "TimeManager",
            
            attrs: 2
          },
          
          
          {
            name: "_character",
            type: "ObjectCharacter",
            
            obj_type: "ObjectCharacter",
            
            attrs: 1
          },
          
          
          {
            name: "_duration",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "_task",
            type: "TimeManager::Task",
            
            obj_type: "TimeManager::Task",
            
            attrs: 1
          },
          
          
          {
            name: "_name",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_module",
            type: "asIScriptModule",
            
            attrs: 1
          },
          
          
          {
            name: "_begin",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_end",
            type: "asIScriptFunction",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetStatistics",
              params: "DataTree* stats,StatController* statsController",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "NullifyStatistics",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject* character",
              attrs:  64,
              desc:   "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "SetInventory",
              params: "Inventory* inventory",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "statistics",
              params: "_statistics",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "ProcessCollisions",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LookAt",
              params: "LVecBase3",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LookAt",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoTo",
              params: "unsigned int id",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoTo",
              params: "Waypoint* waypoint",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoTo",
              params: "InstanceDynamicObject* object,int max_distance = 0",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToRandomWaypoint",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "TruncatePath",
              params: "unsigned short max_length",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetPathDistance",
              params: "Waypoint* waypoint",
              attrs:  0,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetPathDistance",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "GetDistance",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "GetBestWaypoint",
              params: "InstanceDynamicObject* object,bool farthest",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetNearestWaypoint",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetFarthestWaypoint",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "GetPathSize",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "HasLineOfSight",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsMoving",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsAlive",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsInterrupted",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Inventory",
              return_attrs: 2
            },
          
            {
              name:   "GetStatistics",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "GetStatController",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "StatController",
              return_attrs: 1
            },
          
            {
              name:   "GetFactionName",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetFaction",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "int",
              return_attrs: 8
            },
          
            {
              name:   "GetActionPoints",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetActionPoints",
              params: "unsigned short ap",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RestartActionPoints",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetHitPoints",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "SetHitPoints",
              params: "short hp",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StatHpUpdate",
              params: "short",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetArmorClass",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 0
            },
          
            {
              name:   "SetArmorClass",
              params: "short ac",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RestartArmorClass",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetBonusAC",
              params: "short ac",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayEquipedItemAnimation",
              params: "unsigned short it,const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEquipedItem",
              params: "unsigned short it,InventoryObject* object,EquipedMode mode = EquipedMouth",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetEquipedItem",
              params: "unsigned short it",
              attrs:  0,
              desc:   "",
              return_type: "InventoryObject",
              return_attrs: 1
            },
          
            {
              name:   "GetequipedAction",
              params: "unsigned short it",
              attrs:  4,
              desc:   "",
              return_type: "char",
              return_attrs: 8
            },
          
            {
              name:   "UnequipItem",
              params: "unsigned short it",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ItemNextUseType",
              params: "unsigned short it",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushBuff",
              params: "Data,ObjectCharacter* caster",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DelBuff",
              params: "CharacterBuff* buff",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CheckFieldOfView",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFaction",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFaction",
              params: "unsigned int flag",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAsEnemy",
              params: "const ObjectCharacter*,bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsEnemy",
              params: "const ObjectCharacter*",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsAlly",
              params: "const ObjectCharacter*",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "RequestAttack",
              params: "ObjectCharacter* attack,ObjectCharacter* from",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestHeal",
              params: "ObjectCharacter* heal,ObjectCharacter* from",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestFollow",
              params: "ObjectCharacter* follow,ObjectCharacter* from",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestStopFollowing",
              params: "ObjectCharacter* follow,ObjectCharacter* from",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AskMorale",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "SendMessage",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMovement",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMovementNext",
              params: "float elaspedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunDeath",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartRunAnimation",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StopRunAnimation",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RequestCharacter",
              params: "ObjectCharacter*,ObjectCharacter*,asIScriptFunction*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DebugPathfinding",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "ttl",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "ret",
            type: "GoToData",
            
            obj_type: "InstanceDynamicObject::GoToData",
            
            attrs: 0
          },
          
          
          {
            name: "_inventory",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_obs_handler",
            type: "Sync::ObserverHandler",
            
            attrs: 0
          },
          
          
          {
            name: "_goToData",
            type: "GoToData",
            
            obj_type: "InstanceDynamicObject::GoToData",
            
            attrs: 0
          },
          
          
          {
            name: "_statistics",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_stats",
            type: "StatController",
            
            obj_type: "StatController",
            
            attrs: 1
          },
          
          
          {
            name: "_faction",
            type: "WorldDiplomacy::Faction",
            
            obj_type: "WorldDiplomacy::Faction",
            
            attrs: 5
          },
          
          
          {
            name: "_self_enemyMask",
            type: "int",
            
            attrs: 8
          },
          
          
          {
            name: "_actionPoints",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "_hitPoints",
            type: "short",
            
            attrs: 0
          },
          
          
          {
            name: "_armorClass",
            type: "short                          ",
            
            attrs: 0
          },
          
          
          {
            name: "_tmpArmorClass",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "jointHorn",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "jointBattleSaddle",
            type: "NodePath                       ",
            
            attrs: 0
          },
          
          
          {
            name: "jointMouth",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_inventory",
            type: "Inventory",
            
            obj_type: "Inventory",
            
            attrs: 1
          },
          
          
          {
            name: "_losPath",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_losTraverser",
            type: "CollisionTraverser",
            
            attrs: 0
          },
          
          
          {
            name: "_fovTargetNp",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_fovNeedsUpdate",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_fovNp",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_fovTraverser",
            type: "CollisionTraverser",
            
            attrs: 0
          },
          
          
          {
            name: "_scriptMain",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptFight",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptRequestAttack",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptRequestHeal",
            type: "asIScriptFunction ",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptRequestFollow",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptRequestStopFollowing",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptAskMorale",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_scriptSendMessage",
            type: "asIScriptFunction",
            
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
            type: "ObjectCharacter",
            
            obj_type: "ObjectCharacter",
            
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
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "mode",
            type: "EquipedMode",
            
            attrs: 0
          },
          
          
          {
            name: "equiped",
            type: "InventoryObject",
            
            obj_type: "InventoryObject",
            
            attrs: 1
          },
          
          
          {
            name: "default_",
            type: "InventoryObject",
            
            obj_type: "InventoryObject",
            
            attrs: 1
          },
          
          
          {
            name: "graphics",
            type: "InventoryObject::EquipedModel",
            
            obj_type: "InventoryObject::EquipedModel",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Increment",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetQuantity",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "short",
              return_attrs: 8
            },
          
            {
              name:   "SetQuantity",
              params: "unsigned short",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "Observer",
            type: "Sync::ObserverHandler",
            
            attrs: 0
          },
          
          
          {
            name: "EventAccepted",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventCanceled",
            type: "RocketListener          ",
            
            attrs: 0
          },
          
          
          {
            name: "EventIncrement",
            type: "RocketListener          ",
            
            attrs: 0
          },
          
          
          {
            name: "EventDecrement",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "EventValueChanged",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_max_quantity",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "_line_edit",
            type: "Rocket::Core::Element",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetObjectFromId",
              params: "const std::string& id",
              attrs:  0,
              desc:   "",
              return_type: "InventoryObject",
              return_attrs: 1
            },
          
            {
              name:   "ProcessEvent",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Inventory",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          
          {
            name: "_element",
            type: "Rocket::Core::Element",
            
            attrs: 2
          },
          
          
          {
            name: "_inventory",
            type: "Inventory",
            
            obj_type: "Inventory",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DragObserver",
              params: "InventoryView* container,Rocket::Core::Element* element",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Destroy",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Update",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AllowDrop",
              params: "InventoryView& from,InventoryView& to",
              attrs:  64,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RocketCancelClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "CancelClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_viewController",
            type: "InventoryViewController",
            
            obj_type: "InventoryViewController",
            
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
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SwapObjects",
              params: "InventoryObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MakeDeal",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "BarterEnd",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInterface",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInterfaceSide",
              params: "Rocket::Core::Element* e,Inventory::Content&,StatController*,StatController*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetStackValue",
              params: "Inventory::Content&,StatController*,StatController*",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "DropInventory",
              params: "Inventory& from,Inventory& to",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SwapFunctor",
              params: "InventoryObject* object,Inventory& from,Inventory& to",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "EventMakeDeal",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventBarterEnd",
            type: "RocketListener          ",
            
            attrs: 0
          },
          
          
          {
            name: "_stack_player",
            type: "Inventory",
            
            obj_type: "Inventory",
            
            attrs: 0
          },
          
          
          {
            name: "_stack_other",
            type: "Inventory               ",
            
            attrs: 0
          },
          
          
          {
            name: "_stats_player",
            type: "StatController",
            
            obj_type: "StatController",
            
            attrs: 1
          },
          
          
          {
            name: "_stats_other",
            type: "StatController         ",
            
            attrs: 1
          },
          
          
          {
            name: "_inventory_player",
            type: "Inventory",
            
            obj_type: "Inventory",
            
            attrs: 2
          },
          
          
          {
            name: "_inventory_other",
            type: "Inventory",
            
            obj_type: "Inventory",
            
            attrs: 2
          },
          
          
          {
            name: "_quantity_picker",
            type: "UiObjectQuantityPicker",
            
            obj_type: "UiObjectQuantityPicker",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SwapObjects",
              params: "InventoryObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RocketDoneClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RocketTakeAllClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "DoneClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "TakeAllClicked",
            type: "RocketListener                               ",
            
            attrs: 0
          },
          
          
          {
            name: "_viewController",
            type: "InventoryViewController",
            
            obj_type: "InventoryViewController",
            
            attrs: 0
          },
          
          
          {
            name: "_quantity_picker",
            type: "UiObjectQuantityPicker",
            
            obj_type: "UiObjectQuantityPicker",
            
            attrs: 1
          },
          
          
          {
            name: "_looter",
            type: "Inventory",
            
            obj_type: "Inventory",
            
            attrs: 2
          },
          
          
          {
            name: "_looted",
            type: "Inventory",
            
            obj_type: "Inventory",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisableMode",
              params: "EquipedMode",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButton",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "MouthClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "MagicClicked",
            type: "RocketListener   ",
            
            attrs: 0
          },
          
          
          {
            name: "BattleSaddleClicked",
            type: "RocketListener   ",
            
            attrs: 0
          },
          
          
          {
            name: "CancelClicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_it",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "_object",
            type: "InventoryObject",
            
            obj_type: "InventoryObject",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "LevelSelected",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "CancelSelected",
            type: "RocketListener ",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInventory",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateInventoryCapacity",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetSelectedObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonUse",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonDrop",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonEquip1",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonEquip2",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonUnequip1",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButtonUnequip2",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "GameUi",
            type: "class",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonUseClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonDropClicked",
            type: "RocketListener           ",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonEquip1Clicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonEquip2Clicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonUnequip1",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonUnequip2",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_inventory",
            type: "Inventory",
            
            obj_type: "Inventory",
            
            attrs: 1
          },
          
          
          {
            name: "_inventoryView",
            type: "InventoryViewController",
            
            obj_type: "InventoryViewController",
            
            attrs: 0
          },
          
          
          {
            name: "_selectedObject",
            type: "InventoryObject",
            
            obj_type: "InventoryObject",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hide",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Get",
              params: "",
              attrs:  16,
              desc:   "",
              return_type: "GameConsole",
              return_attrs: 18
            },
          
            {
              name:   "WriteOn",
              params: "const std::string& string",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "ListFunctions",
              params: "",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "PrintScenegraph",
              params: "",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "PrintChildren",
              params: "const NodePath& n,int lvl",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Toggle",
              params: "const Event*,void* data",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "KeyUp",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Execute",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Output",
              params: "const std::string str",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "GameUi",
            type: "class",
            
            attrs: 0
          },
          
          
          {
            name: "GConsole",
            type: "GameConsole",
            
            obj_type: "GameConsole",
            
            attrs: 17
          },
          
          
          {
            name: "ConsoleKeyUp",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "ExecuteEvent",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_histIter",
            type: "::iterator",
            
            attrs: 0
          },
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_root",
            type: "Rocket::Core::ElementDocument",
            
            attrs: 1
          },
          
          
          {
            name: "_input",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_currentLine",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_script_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_observerError",
            type: "Sync::ObserverId",
            
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
              desc:   "",
              return_type: "WindowFramework",
              return_attrs: 1
            },
          
            {
              name:   "GetContext",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "Rocket::Core::Context",
              return_attrs: 1
            },
          
            {
              name:   "GetMenu",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "GameMenu",
              return_attrs: 2
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "GameInventory",
              return_attrs: 2
            },
          
            {
              name:   "GetPers",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "StatViewRocket",
              return_attrs: 2
            },
          
            {
              name:   "OpenMenu",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenInventory",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenPers",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_console",
            type: "GameConsole",
            
            obj_type: "GameConsole",
            
            attrs: 1
          },
          
          
          {
            name: "_menu",
            type: "GameMenu",
            
            obj_type: "GameMenu",
            
            attrs: 1
          },
          
          
          {
            name: "_inventory",
            type: "GameInventory",
            
            obj_type: "GameInventory",
            
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
              desc:   "",
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
              desc:   "",
              return_type: "Rocket::Core::Context",
              return_attrs: 1
            },
          
            {
              name:   "GetMainBar",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "GameMainBar",
              return_attrs: 2
            },
          
            {
              name:   "GetMenu",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "GameMenu",
              return_attrs: 2
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "GameInventory",
              return_attrs: 2
            },
          
            {
              name:   "GetPers",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "StatViewRocket",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          
          {
            name: "_gameUi",
            type: "GameUi",
            
            obj_type: "GameUi",
            
            attrs: 2
          },
          
          
          {
            name: "_mainBar",
            type: "GameMainBar",
            
            obj_type: "GameMainBar",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetResolution",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetQuality",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleFullscreen",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "ExitClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "LanguageSelected",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "QualitySelected",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "ScreenSelected",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "FullscreenToggled",
            type: "",
            
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
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "ButtonClicked",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_continue",
            type: "bool",
            
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
              desc:   "",
              return_type: "GameOptions",
              return_attrs: 2
            }
          
        ],
        attributes: [
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_console",
            type: "GameConsole",
            
            obj_type: "GameConsole",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "MusicManager",
              return_attrs: 17
            },
          
            {
              name:   "Play",
              params: "const std::string& category",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Play",
              params: "const std::string& category,const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayNext",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVolume",
              params: "float volume",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FadeOut",
              params: "float elapsed_time",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FadeVolume",
              params: "float elapsed_time",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_global_ptr",
            type: "MusicManager",
            
            obj_type: "MusicManager",
            
            attrs: 17
          },
          
          
          {
            name: "_global_ptr",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_data",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "_data_tree",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_current_category",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_timer",
            type: "Timer",
            
            obj_type: "Timer",
            
            attrs: 0
          },
          
          
          {
            name: "_fading_out",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_volume_goal",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "_volume_ref",
            type: "float",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Stop",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Pause",
              params: "void",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVolume",
              params: "float",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetVolume",
              params: "void",
              attrs:  68,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "IsPlaying",
              params: "void",
              attrs:  68,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "AddReference",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DelReference",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetReferenceCount",
              params: "void",
              attrs:  4,
              desc:   "",
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
              desc:   "",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "LoadFromFile",
              params: "const std::string& filename",
              attrs:  64,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetFilename",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            }
          
        ],
        attributes: [
          
          
          {
            name: "filename",
            type: "std::string",
            
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
              desc:   "",
              return_type: "SoundManager",
              return_attrs: 17
            },
          
            {
              name:   "Release",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CreateInstance",
              params: "const std::string& key",
              attrs:  0,
              desc:   "",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "DeleteInstance",
              params: "ISampleInstance*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GarbageCollect",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GarbageCollectAll",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "SetVolume",
              params: "float",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetVolume",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "Require",
              params: "const std::string& key",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "RequireSound",
              params: "const std::string& key",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "data_audio",
              params: "_data_audio",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "if ",
              params: "(find(_sounds_required.begin(",
              attrs:  0,
              desc:   "",
              return_type: "else",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "ref_count",
            type: "short",
            
            attrs: 8
          },
          
          
          {
            name: "this",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_sound_managers",
            type: "SoundManagers",
            
            attrs: 16
          },
          
          
          {
            name: "_sounds",
            type: "Sounds",
            
            attrs: 16
          },
          
          
          {
            name: "_data_audio",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 17
          },
          
          
          {
            name: "_sounds_required",
            type: "SoundsIterators",
            
            attrs: 0
          },
          
          
          {
            name: "_volume",
            type: "float",
            
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
            type: "std::string",
            
            attrs: 4
          },
          
          
          {
            name: "sample",
            type: "ISample",
            
            obj_type: "ISample",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Stop",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Pause",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetVolume",
              params: "float volume",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetVolume",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "IsPlaying",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "instance",
            type: "sf::Sound",
            
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
              desc:   "",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "LoadFromFile",
              params: "const std::string& filename",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "buffer",
            type: "sf::SoundBuffer",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InitPlayer",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetDataEngine",
              params: "DataEngine* de",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPlayerInventory",
              params: "Inventory*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveUpdateWorld",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Load",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertParty",
              params: "PlayerParty& party",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FetchParty",
              params: "PlayerParty& party",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StripParty",
              params: "PlayerParty& party",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "do_task",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "AsyncTask::DoneStatus",
              return_attrs: 0
            },
          
            {
              name:   "SetPersistent",
              params: "bool set",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsPersistent",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetState",
              params: "State",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetState",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "State",
              return_attrs: 0
            },
          
            {
              name:   "SetInterrupted",
              params: "bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisplayCombatPath",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DestroyCombatPath",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FindPath",
              params: "std::list<Waypoint>&,Waypoint&,Waypoint&",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetWorld",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "World",
              return_attrs: 1
            },
          
            {
              name:   "GetCamera",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "SceneCamera",
              return_attrs: 2
            },
          
            {
              name:   "GetCharacter",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "GetCharacter",
              params: "const DynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "GetPlayer",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "UnprocessAllCollisions",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ProcessAllCollisions",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FindObjectFromNode",
              params: "NodePath node",
              attrs:  0,
              desc:   "",
              return_type: "InstanceDynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetObject",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "InstanceDynamicObject",
              return_attrs: 1
            },
          
            {
              name:   "GetTimeManager",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "TimeManager",
              return_attrs: 2
            },
          
            {
              name:   "GetDataEngine",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "GetItems",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "ConsoleWrite",
              params: "const std::string& str",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RemoveObject",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackExitZone",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackGoToZone",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackSelectNextZone",
              params: "const std::vector<std::string>& zones",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackCancelSelectZone",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetNextZone",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetExitZone",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "SetEntryZone",
              params: "Party&,const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionBarter",
              params: "ObjectCharacter*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionTalkTo",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionUseObjectOn",
              params: "InstanceDynamicObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackActionTargetUse",
              params: "unsigned short it",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SelectedUseObjectOn",
              params: "InventoryObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUse",
              params: "ObjectCharacter* user,InstanceDynamicObject* target",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUseObject",
              params: "ObjectCharacter* user,InventoryObject* object,unsigned char actionIt",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUseObjectOn",
              params: "ObjectCharacter* user,InstanceDynamicObject* target,InventoryObject* object,unsigned char actionIt",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionDropObject",
              params: "ObjectCharacter* user,InventoryObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ActionUseWeaponOn",
              params: "ObjectCharacter* user,ObjectCharacter* target,InventoryObject* object,unsigned char actionIt",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionTalkTo",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionUse",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionUseObjectOn",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionUseWeaponOn",
              params: "InstanceDynamicObject* fromObject",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerDropObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerUseObject",
              params: "InventoryObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerLoot",
              params: "Inventory*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PlayerEquipObject",
              params: "unsigned short it,InventoryObject* object",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartFight",
              params: "ObjectCharacter* starter",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StopFight",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "NextTurn",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UseActionPoints",
              params: "unsigned short ap",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SetMouseState",
              params: "MouseState",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseLeftClicked",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseRightClicked",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartCombat",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SpawnEnemies",
              params: "const std::string& type,unsigned short quantity,unsigned short n_spawn",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsWaypointOccupied",
              params: "unsigned int id",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "PlaySound",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "SetupCamera",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunDaylight",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunMetabolism",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseInit",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ToggleCharacterOutline",
              params: "bool",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertDynamicObject",
              params: "DynamicObject&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InsertCharacter",
              params: "ObjectCharacter*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CloseRunningUi",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsInsideBuilding",
              params: "unsigned char& floor",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "CheckCurrentFloor",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentFloor",
              params: "unsigned char floor",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FloorFade",
              params: "bool in,NodePath floor",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "CurrentLevel",
            type: "Level",
            
            obj_type: "Level",
            
            attrs: 17
          },
          
          
          {
            name: "_mouseState",
            type: "MouseState",
            
            attrs: 0
          },
          
          
          {
            name: "obs",
            type: "Sync::ObserverHandler",
            
            attrs: 0
          },
          
          
          {
            name: "obs_player",
            type: "Sync::ObserverHandler",
            
            attrs: 0
          },
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_graphicWindow",
            type: "GraphicsWindow",
            
            attrs: 1
          },
          
          
          {
            name: "_mouse",
            type: "Mouse",
            
            obj_type: "Mouse",
            
            attrs: 0
          },
          
          
          {
            name: "_camera",
            type: "SceneCamera",
            
            obj_type: "SceneCamera",
            
            attrs: 0
          },
          
          
          {
            name: "_timer",
            type: "Timer",
            
            obj_type: "Timer",
            
            attrs: 0
          },
          
          
          {
            name: "_timeManager",
            type: "TimeManager",
            
            obj_type: "TimeManager",
            
            attrs: 2
          },
          
          
          {
            name: "_state",
            type: "State",
            
            attrs: 0
          },
          
          
          {
            name: "_persistent",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_world",
            type: "World",
            
            obj_type: "World",
            
            attrs: 1
          },
          
          
          {
            name: "_sound_manager",
            type: "SoundManager",
            
            obj_type: "SoundManager",
            
            attrs: 0
          },
          
          
          {
            name: "_objects",
            type: "InstanceObjects",
            
            attrs: 0
          },
          
          
          {
            name: "_characters",
            type: "Characters",
            
            attrs: 0
          },
          
          
          {
            name: "_itCharacter",
            type: "Characters::iterator",
            
            attrs: 0
          },
          
          
          {
            name: "_currentCharacter",
            type: "Characters::iterator",
            
            attrs: 0
          },
          
          
          {
            name: "_player_halo",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_exitZones",
            type: "ExitZones",
            
            attrs: 0
          },
          
          
          {
            name: "_exitingZone",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_exitingZoneTo",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_exitingZoneName",
            type: "std::string          ",
            
            attrs: 0
          },
          
          
          {
            name: "_sunLightNode",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_sunLightAmbientNode",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_task_daylight",
            type: "TimeManager::Task",
            
            obj_type: "TimeManager::Task",
            
            attrs: 1
          },
          
          
          {
            name: "_task_metabolism",
            type: "TimeManager::Task",
            
            obj_type: "TimeManager::Task",
            
            attrs: 1
          },
          
          
          {
            name: "_levelUi",
            type: "LevelUi",
            
            obj_type: "LevelUi",
            
            attrs: 0
          },
          
          
          {
            name: "_currentRunningDialog",
            type: "DialogController",
            
            obj_type: "DialogController",
            
            attrs: 1
          },
          
          
          {
            name: "_currentUseObjectOn",
            type: "UiUseObjectOn",
            
            obj_type: "UiUseObjectOn",
            
            attrs: 1
          },
          
          
          {
            name: "_currentUiLoot",
            type: "UiLoot",
            
            obj_type: "UiLoot",
            
            attrs: 1
          },
          
          
          {
            name: "_mouseActionBlocked",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_dataEngine",
            type: "DataEngine",
            
            obj_type: "DataEngine",
            
            attrs: 1
          },
          
          
          {
            name: "_items",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_last_combat_path",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_currentFloor",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "_floor_lastWp",
            type: "Waypoint",
            
            obj_type: "Waypoint",
            
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
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Alpha",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "float",
              return_attrs: 0
            },
          
            {
              name:   "ForceAlpha",
              params: "float _alpha",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetNodePath",
              params: "NodePath np",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetFadingIn",
              params: "bool set",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "floor",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "fadingIn",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "done",
            type: "bool     ",
            
            attrs: 0
          },
          
          
          {
            name: "alpha",
            type: "float",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetWaypoint",
              params: "NodePath np",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetDynObject",
              params: "NodePath np",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "hasWaypoint",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "hasDynObject",
            type: "bool     ",
            
            attrs: 0
          },
          
          
          {
            name: "waypoint",
            type: "NodePath",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Hovering",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "MouseHovering",
              return_attrs: 6
            },
          
            {
              name:   "SetMouseState",
              params: "char",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClosestWaypoint",
              params: "World*,short currentFloor",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CallbackButton1",
              params: "const Event*,void* ptr",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "CallbackButton2",
              params: "const Event*,void* ptr",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "CallbackButton3",
              params: "const Event*,void* ptr",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            }
          
        ],
        attributes: [
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_camera",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_lastMousePos",
            type: "LPoint2f",
            
            attrs: 0
          },
          
          
          {
            name: "_pickerPath",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_collisionTraverser",
            type: "CollisionTraverser",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetHookAvailable",
              params: "const std::string& answerId",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetExecuteMethod",
              params: "const std::string& answerId",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetDefaultNextLine",
              params: "const std::string& answerId",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetNpcLine",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "GetDialogAnswers",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "DialogAnswers",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_tree",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_tree",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_data",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "_l18n",
            type: "Data",
            
            obj_type: "Data",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateView",
              params: "const std::string& npcLine,const DialogAnswers& answers",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CleanView",
              params: "const DialogAnswers& answers",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "AnswerSelected",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "BarterOpened",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_containerNpcLine",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_containerAnswers",
            type: "Rocket::Core::Element",
            
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
              desc:   "",
              return_type: "ObjectCharacter",
              return_attrs: 1
            },
          
            {
              name:   "ExecuteAnswer",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCurrentNode",
              params: "const std::string& nodeName",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenBarter",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_module",
            type: "asIScriptModule",
            
            attrs: 1
          },
          
          
          {
            name: "_model",
            type: "DialogModel",
            
            obj_type: "DialogModel",
            
            attrs: 0
          },
          
          
          {
            name: "_character",
            type: "ObjectCharacter",
            
            obj_type: "ObjectCharacter",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetGoToData",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0
            },
          
            {
              name:   "GetInventory",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Inventory",
              return_attrs: 2
            },
          
            {
              name:   "CallbackActionUse",
              params: "InstanceDynamicObject*",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_inventory",
            type: "Inventory",
            
            obj_type: "Inventory",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetKeyName",
              params: "",
              attrs:  4,
              desc:   "",
              return_type: "string",
              return_attrs: 0
            },
          
            {
              name:   "PendingActionOpen",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ProcessCollisions",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_item",
            type: "InventoryObject",
            
            obj_type: "InventoryObject",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetEnabledScroll",
              params: "bool set",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SwapCameraView",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLimits",
              params: "unsigned int minX,unsigned int minY,unsigned int maxX,unsigned maxY",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SlideToHeight",
              params: "float",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CenterCameraInstant",
              params: "LPoint3f",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CenterCameraOn",
              params: "NodePath np",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FollowNodePath",
              params: "NodePath np",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StopFollowingNodePath",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CenterOnObject",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FollowObject",
              params: "InstanceDynamicObject*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetNodePath",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "NodePath",
              return_attrs: 0
            },
          
            {
              name:   "RunScroll",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunFollow",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunSlideHeight",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_graphicWindow",
            type: "GraphicsWindow",
            
            attrs: 1
          },
          
          
          {
            name: "_camera",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_cameraPos",
            type: "LPoint3f",
            
            attrs: 0
          },
          
          
          {
            name: "_cameraMovementSpeed",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "_scrollEnabled",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_currentCameraAngle",
            type: "char",
            
            attrs: 8
          },
          
          
          {
            name: "_currentHpr",
            type: "LPoint3f",
            
            attrs: 0
          },
          
          
          {
            name: "_objectiveHpr",
            type: "LPoint3f",
            
            attrs: 0
          },
          
          
          {
            name: "_minPosX",
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "_minPosY",
            type: "int              ",
            
            attrs: 0
          },
          
          
          {
            name: "_maxPosX",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_maxPosY",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_destHeight",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "_centeringCamera",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_followingNodePath",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_toFollow",
            type: "NodePath",
            
            attrs: 0
          },
          
          
          {
            name: "_currentPos",
            type: "LPoint3f",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ButtonHovered",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MouseButton",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ExecuteForButtonId",
              params: "Rocket::Core::Event& event,std::function<bool (Rocket::Core::Event&,const std::string&,InstanceDynamicObject::Interaction*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_buttons",
            type: "Buttons",
            
            attrs: 0
          },
          
          
          {
            name: "_listeners",
            type: "Listeners",
            
            attrs: 0
          },
          
          
          {
            name: "_buttonListener",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_buttonHover",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_buttonClick",
            type: "RocketListener               ",
            
            attrs: 0
          },
          
          
          {
            name: "_obs",
            type: "Sync::ObserverHandler",
            
            attrs: 0
          },
          
          
          {
            name: "_done",
            type: "bool",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetFaction",
              params: "const std::string& name",
              attrs:  0,
              desc:   "",
              return_type: "WorldDiplomacy::Faction",
              return_attrs: 1
            },
          
            {
              name:   "GetFaction",
              params: "unsigned int flag",
              attrs:  0,
              desc:   "",
              return_type: "WorldDiplomacy::Faction",
              return_attrs: 1
            },
          
            {
              name:   "SetAsEnemy",
              params: "bool set,const std::string& name1,const std::string& name2",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAsEnemy",
              params: "bool set,unsigned int flag1,unsigned int flag2",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetAsEnemy",
              params: "bool set,Faction& first,Faction& second",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_data_engine",
            type: "DataEngine",
            
            obj_type: "DataEngine",
            
            attrs: 2
          },
          
          
          {
            name: "_factions",
            type: "Factions",
            
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
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "flag",
            type: "int",
            
            attrs: 8
          },
          
          
          {
            name: "enemyMask",
            type: "int",
            
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
              desc:   "",
              return_type: "DataTree",
              return_attrs: 1
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "DataTree",
              return_attrs: 1
            },
          
            {
              name:   "ParseValue",
              params: "DataBranch*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseString",
              params: "DataBranch*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseOther",
              params: "DataBranch*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseObject",
              params: "DataBranch*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ParseArray",
              params: "DataBranch*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_fileExist",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_source",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_str",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_it",
            type: "int",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCityVisible",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddCity",
              params: "const std::string&,float pos_x,float pos_y,float radius",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "HasCity",
              params: "const std::string&",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "MapClicked",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PartyClicked",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CityClicked",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetInterrupted",
              params: "bool set",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MoveTowardsCoordinates",
              params: "float x,float y",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MoveTowardsPlace",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetDataEngine",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "DataEngine",
              return_attrs: 2
            },
          
            {
              name:   "GetCaseData",
              params: "int x,int y",
              attrs:  4,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "GetCurrentPosition",
              params: "float& x,float& y",
              attrs:  4,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "MapTileGenerator",
              params: "Data map",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdatePartyCursor",
              params: "float elapsedTime",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "UpdateClock",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsPartyInCity",
              params: "std::string& ret",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "GetCaseAt",
              params: "int x,int y",
              attrs:  4,
              desc:   "",
              return_type: "Rocket::Core::Element",
              return_attrs: 1
            },
          
            {
              name:   "GetCurrentCase",
              params: "int&,int&",
              attrs:  4,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetCaseVisibility",
              params: "int x,int y,char visibility",
              attrs:  4,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddCityToList",
              params: "Data",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveMapStatus",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CheckRandomEncounter",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "name",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "CurrentWorldMap",
            type: "WorldMap",
            
            obj_type: "WorldMap",
            
            attrs: 17
          },
          
          
          {
            name: "MapClickedEvent",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "PartyCursorClicked",
            type: "RocketListener         ",
            
            attrs: 0
          },
          
          
          {
            name: "CityButtonClicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonInventory",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonCharacter",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonPipbuck",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ButtonMenu",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_mapTree",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_dataEngine",
            type: "DataEngine",
            
            obj_type: "DataEngine",
            
            attrs: 2
          },
          
          
          {
            name: "_timeManager",
            type: "TimeManager",
            
            obj_type: "TimeManager",
            
            attrs: 2
          },
          
          
          {
            name: "_gameUi",
            type: "GameUi",
            
            obj_type: "GameUi",
            
            attrs: 2
          },
          
          
          {
            name: "_timer",
            type: "Timer",
            
            obj_type: "Timer",
            
            attrs: 0
          },
          
          
          {
            name: "_interrupted",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_size_x",
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "_size_y",
            type: "int                    ",
            
            attrs: 0
          },
          
          
          {
            name: "_tsize_x",
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "_tsize_y",
            type: "int                    ",
            
            attrs: 0
          },
          
          
          {
            name: "_current_pos_x",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "_current_pos_y",
            type: "float                  ",
            
            attrs: 0
          },
          
          
          {
            name: "_goal_x",
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "_goal_y",
            type: "int                    ",
            
            attrs: 0
          },
          
          
          {
            name: "_cursor",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_cities",
            type: "Cities",
            
            attrs: 0
          },
          
          
          {
            name: "_cases",
            type: "Cases",
            
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
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "pos_y",
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "radius",
            type: "int",
            
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
              desc:   "",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "FileCopy",
              params: "const std::string& from,const std::string& dest",
              attrs:  16,
              desc:   "",
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
              desc:   "",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "RemoveDir",
              params: "const std::string& str",
              attrs:  16,
              desc:   "",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "OpenDir",
              params: "const std::string& str",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Path",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetEntries",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Entries",
              return_attrs: 2
            },
          
            {
              name:   "GetEntries",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "Entries",
              return_attrs: 6
            }
          
        ],
        attributes: [
          
          
          {
            name: "_dirEntries",
            type: "Entries",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 16
            },
          
            {
              name:   "LoadSlots",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClickSlot",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Erase",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "EventLoadGame",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventClickSlot",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventCancel",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "EventErase",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_selectedSlot",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_savePath",
            type: "std::string",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClickSlot",
              params: "Rocket::Core::Event& event",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Erase",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "EventSaveGame",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventClickSlot",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventCancel",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "EventErase",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_selectedSlot",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_savePath",
            type: "std::string",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ReloadApps",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartApp",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "QuitApp",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoHome",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DoQuitApp",
              params: "App*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AppAskedFocus",
              params: "App*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AppAskedUnfocus",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AppAskedExit",
              params: "App*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_data_engine",
            type: "DataEngine",
            
            obj_type: "DataEngine",
            
            attrs: 2
          },
          
          
          {
            name: "_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_module",
            type: "asIScriptModule",
            
            attrs: 1
          },
          
          
          {
            name: "_running_apps",
            type: "AppList",
            
            attrs: 0
          },
          
          
          {
            name: "_running_app",
            type: "App",
            
            obj_type: "Pipbuck::App",
            
            attrs: 1
          },
          
          
          {
            name: "_asked_unfocus",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_asked_focus",
            type: "AppList",
            
            attrs: 0
          },
          
          
          {
            name: "_asked_exit",
            type: "AppList",
            
            attrs: 0
          },
          
          
          {
            name: "EventStartApp",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventQuitApp",
            type: "RocketListener    ",
            
            attrs: 0
          },
          
          
          {
            name: "EventHome",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "EventHidePipbuck",
            type: "",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  64,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  64,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  64,
              desc:   "",
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_module",
            type: "asIScriptModule",
            
            attrs: 1
          },
          
          
          {
            name: "_data",
            type: "Data",
            
            obj_type: "Data",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WaitChange",
              params: "int chance",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Wait",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SelectTimeUnit",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetValue",
              params: "Rocket::Core::Element* element,unsigned short value",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_appid",
            type: "std::string",
            
            attrs: 4
          },
          
          
          {
            name: "_time_manager",
            type: "TimeManager",
            
            obj_type: "TimeManager",
            
            attrs: 2
          },
          
          
          {
            name: "_inner_rml",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "EventWaitMore",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventWaitLess",
            type: "RocketListener        ",
            
            attrs: 0
          },
          
          
          {
            name: "EventTimeUnitSelected",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "EventWait",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_year",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_month",
            type: "Rocket::Core::Element ",
            
            attrs: 1
          },
          
          
          {
            name: "_day",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_hour",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_minute",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_second",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_wait_days",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_wait_hours",
            type: "Rocket::Core::Element ",
            
            attrs: 1
          },
          
          
          {
            name: "_wait_minutes",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_selected_wait",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_wait_more",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_wait_less",
            type: "Rocket::Core::Element ",
            
            attrs: 1
          },
          
          
          {
            name: "_do_wait",
            type: "",
            
            attrs: 1
          },
          
          
          {
            name: "_span_error",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_minutes_to_spend",
            type: "int",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 4
            },
          
            {
              name:   "RunAsMainTask",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "RunAsBackgroundTask",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Started",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "Exited",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Unfocused",
              params: "DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Focused",
              params: "Rocket::Core::Element*,DataEngine&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadQuestList",
              params: "Rocket::Core::Element*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadQuestView",
              params: "Rocket::Core::Element*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ListQuestHovered",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ListQuestClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "EventQuestHovered",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "EventQuestClicked",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventBackClicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_appid",
            type: "std::string",
            
            attrs: 4
          },
          
          
          {
            name: "_rml_index",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_rml_view",
            type: "std::string       ",
            
            attrs: 0
          },
          
          
          {
            name: "_data_engine",
            type: "DataEngine",
            
            obj_type: "DataEngine",
            
            attrs: 1
          },
          
          
          {
            name: "_current_view",
            type: "Interfaces",
            
            attrs: 0
          },
          
          
          {
            name: "_current_quest",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_last_hovered",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_root",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_observer",
            type: "Sync::ObserverHandler",
            
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
              desc:   "",
              return_type: "std::string",
              return_attrs: 6
            },
          
            {
              name:   "GetStatistics",
              params: "void",
              attrs:  4,
              desc:   "",
              return_type: "StatController",
              return_attrs: 1
            },
          
            {
              name:   "SetStatistics",
              params: "StatController* v",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Refresh",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Save",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "InitScripts",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_context",
            type: "asIScriptContext",
            
            attrs: 1
          },
          
          
          {
            name: "_module",
            type: "asIScriptModule",
            
            attrs: 1
          },
          
          
          {
            name: "_refresh",
            type: "asIScriptFunction",
            
            attrs: 1
          },
          
          
          {
            name: "_buff",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "_target_name",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_target_stats",
            type: "StatController",
            
            obj_type: "StatController",
            
            attrs: 1
          },
          
          
          {
            name: "_tm",
            type: "TimeManager",
            
            obj_type: "TimeManager",
            
            attrs: 2
          },
          
          
          {
            name: "_task",
            type: "TimeManager::Task",
            
            obj_type: "TimeManager::Task",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Load",
              params: "Utils::Packet&,std::function<StatController* (const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CollectGarbage",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Cleanup",
              params: "Buff*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "buffs",
            type: "Buffs",
            
            attrs: 0
          },
          
          
          {
            name: "tm",
            type: "TimeManager",
            
            obj_type: "TimeManager",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLevel",
              params: "Level* level",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "do_task",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "AsyncTask::DoneStatus",
              return_attrs: 0
            },
          
            {
              name:   "SaveGame",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "LoadGame",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "OpenLevel",
              params: "const std::string& savepath,const std::string& level,const std::string& entry_zone = ##",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ExitLevel",
              params: "const std::string& savepath",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CopySave",
              params: "const std::string& savepath,const std::string& slotPath",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "SaveToSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadLastState",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Exit",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLevelSpecialEncounter",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetLevelEncounter",
              params: "const std::string& type,short n_creeps",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushBuff",
              params: "ObjectCharacter* character,Data buff",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PushBuff",
              params: "const std::string& name,Data buff",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveLevelBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SavePartyBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadLevelBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadPartyBuffs",
              params: "Utils::Packet&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetTimeManager",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "TimeManager",
              return_attrs: 2
            },
          
            {
              name:   "GetDiplomacy",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "WorldDiplomacy",
              return_attrs: 2
            },
          
            {
              name:   "GetQuestManager",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "QuestManager",
              return_attrs: 2
            },
          
            {
              name:   "GetPlayerParty",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Party",
              return_attrs: 1
            },
          
            {
              name:   "PlaySound",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "ISampleInstance",
              return_attrs: 1
            },
          
            {
              name:   "FinishLoad",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveClicked",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SaveLevel",
              params: "Level* level,const std::string& name",
              attrs:  16,
              desc:   "",
              return_type: "bool",
              return_attrs: 16
            },
          
            {
              name:   "LoadLevel",
              params: "WindowFramework* window,GameUi& gameUi,const std::string& path,const std::string& name,const std::string& entry_zone = ##,bool isSaveFile = false",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DoLoadLevel",
              params: "LoadLevelParams",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GameOver",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DoCheckRandomEncounter",
              params: "int x,int y",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetPlayerInventory",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "EraseSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "entry_zone",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "CurrentGameTask",
            type: "GameTask",
            
            obj_type: "GameTask",
            
            attrs: 17
          },
          
          
          {
            name: "_signals",
            type: "Sync::Signals",
            
            attrs: 0
          },
          
          
          {
            name: "_continue",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_sound_manager",
            type: "SoundManager",
            
            obj_type: "SoundManager",
            
            attrs: 0
          },
          
          
          {
            name: "_gameUi",
            type: "GameUi",
            
            obj_type: "GameUi",
            
            attrs: 0
          },
          
          
          {
            name: "_dataEngine",
            type: "DataEngine",
            
            obj_type: "DataEngine",
            
            attrs: 0
          },
          
          
          {
            name: "_timeManager",
            type: "TimeManager",
            
            obj_type: "TimeManager",
            
            attrs: 0
          },
          
          
          {
            name: "_buff_manager",
            type: "BuffManager",
            
            obj_type: "BuffManager",
            
            attrs: 0
          },
          
          
          {
            name: "_pipbuck",
            type: "Pipbuck",
            
            obj_type: "Pipbuck",
            
            attrs: 0
          },
          
          
          {
            name: "_charSheet",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_playerParty",
            type: "PlayerParty",
            
            obj_type: "PlayerParty",
            
            attrs: 1
          },
          
          
          {
            name: "_playerStats",
            type: "StatController",
            
            obj_type: "StatController",
            
            attrs: 1
          },
          
          
          {
            name: "_playerInventory",
            type: "Inventory",
            
            obj_type: "Inventory",
            
            attrs: 1
          },
          
          
          {
            name: "_quest_manager",
            type: "QuestManager",
            
            obj_type: "QuestManager",
            
            attrs: 1
          },
          
          
          {
            name: "_worldMap",
            type: "WorldMap",
            
            obj_type: "WorldMap",
            
            attrs: 1
          },
          
          
          {
            name: "_levelName",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_level",
            type: "Level",
            
            obj_type: "Level",
            
            attrs: 1
          },
          
          
          {
            name: "_savePath",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "_uiSaveGame",
            type: "UiSave",
            
            obj_type: "UiSave",
            
            attrs: 1
          },
          
          
          {
            name: "_uiLoadGame",
            type: "UiLoad",
            
            obj_type: "UiLoad",
            
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
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "path",
            type: "std::string",
            
            attrs: 0
          },
          
          
          {
            name: "isSaveFile",
            type: "bool",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Update",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PullToFront",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "MouseCursor",
              return_attrs: 17
            }
          
        ],
        attributes: [
          
          
          {
            name: "_cursor",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_static",
            type: "MouseCursor",
            
            obj_type: "MouseCursor",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Get",
              params: "void",
              attrs:  16,
              desc:   "",
              return_type: "Data",
              return_attrs: 16
            },
          
            {
              name:   "Refresh",
              params: "void",
              attrs:  16,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "ClickedStartFromScratch",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToPreviousProfile",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToNextProfile",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoToProfile",
              params: "const std::string&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GoCancel",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "PreviousProfile",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "NextProfile",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventSelectProfile",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "EventStartFromScratch",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "EventCancel",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "_data_profiles",
            type: "DataTree",
            
            obj_type: "DataTree",
            
            attrs: 1
          },
          
          
          {
            name: "_current_profile",
            type: "::const_iterator",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartFromScratch",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Done",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_rocket",
            type: "Rocket::Core::Context",
            
            attrs: 1
          },
          
          
          {
            name: "_ui_new_game",
            type: "UiNewGame",
            
            obj_type: "UiNewGame",
            
            attrs: 0
          },
          
          
          {
            name: "_stat_view",
            type: "StatViewRocket",
            
            obj_type: "StatViewRocket",
            
            attrs: 1
          },
          
          
          {
            name: "_stat_controller",
            type: "StatController",
            
            obj_type: "StatController",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "EndGame",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "NewGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "StartGame",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "CancelNewGame",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "do_task",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "DoneStatus",
              return_attrs: 0
            },
          
            {
              name:   "AsyncCreateLevel",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "QuitGame",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "OpenUiLoad",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "LoadSlot",
              params: "unsigned char slot",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "DisplayAlerts",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "_window",
            type: "WindowFramework",
            
            attrs: 1
          },
          
          
          {
            name: "_generalUi",
            type: "GeneralUi",
            
            obj_type: "GeneralUi",
            
            attrs: 0
          },
          
          
          {
            name: "_mouseCursor",
            type: "MouseCursor",
            
            obj_type: "MouseCursor",
            
            attrs: 0
          },
          
          
          {
            name: "_new_game_task",
            type: "NewGameTask",
            
            obj_type: "NewGameTask",
            
            attrs: 1
          },
          
          
          {
            name: "_levelTask",
            type: "GameTask",
            
            obj_type: "GameTask",
            
            attrs: 1
          },
          
          
          {
            name: "_view",
            type: "View",
            
            obj_type: "MainMenu::View",
            
            attrs: 0
          },
          
          
          {
            name: "_alerts",
            type: "Alerts",
            
            attrs: 0
          },
          
          
          {
            name: "_uiLoad",
            type: "UiLoad",
            
            obj_type: "UiLoad",
            
            attrs: 1
          },
          
          
          {
            name: "_need_garbage_collect",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "quitGamePlz",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "createLevelPlz",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "slotToLoadPlz",
            type: "short",
            
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
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "QuitClicked",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "LoadGameClicked",
            type: "RocketListener ",
            
            attrs: 0
          },
          
          
          {
            name: "OptionsClicked",
            type: "",
            
            attrs: 0
          },
          
          
          {
            name: "ContinueClicked",
            type: "",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SetStartAndGoalStates",
              params: "UserState &Start,UserState &Goal",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SearchSuccedded",
              params: "Node* node",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "SearchStep",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "State",
              return_attrs: 0
            },
          
            {
              name:   "if",
              params: "_state != Succeeded && _state != Failed",
              attrs:  0,
              desc:   "",
              return_type: "else",
              return_attrs: 0
            },
          
            {
              name:   "FreeSolutionNodes",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetStepCount",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "int",
              return_attrs: 0
            },
          
            {
              name:   "FreeAllNodes",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "FreeUnusedNodes",
              params: "bool onlyUnused = true",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PedirNode",
              params: "void",
              attrs:  32,
              desc:   "",
              return_type: "AstarPathfinding::Node",
              return_attrs: 1
            },
          
            {
              name:   "Node",
              params: "",
              attrs:  0,
              desc:   "",
              return_type: "new",
              return_attrs: 0
            },
          
            {
              name:   "ReleaseNode",
              params: "Node* node",
              attrs:  32,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "userNode",
            type: "UserState",
            
            attrs: 0
          },
          
          
          {
            name: "NodeListIterator",
            type: "::iterator",
            
            attrs: 0
          },
          
          
          {
            name: "Node",
            type: "new",
            
            attrs: 0
          },
          
          
          {
            name: "Node",
            type: "new",
            
            attrs: 0
          },
          
          
          {
            name: "node",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "closedListSearch",
            type: "NodeListIterator",
            
            attrs: 0
          },
          
          
          {
            name: "openListSearch",
            type: "NodeListIterator",
            
            attrs: 0
          },
          
          
          {
            name: "listEnd",
            type: "NodeListIterator",
            
            attrs: 0
          },
          
          
          {
            name: "successor",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "successor",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_state",
            type: "return",
            
            attrs: 0
          },
          
          
          {
            name: "del",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "node",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_start",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_goal",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_nSteps",
            type: "return",
            
            attrs: 0
          },
          
          
          {
            name: "_goal",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "it",
            type: "NodeListIterator",
            
            attrs: 0
          },
          
          
          {
            name: "end",
            type: "NodeListIterator",
            
            attrs: 0
          },
          
          
          {
            name: "node",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "node",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "node",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "_openList",
            type: "NodeList",
            
            attrs: 0
          },
          
          
          {
            name: "_closedList",
            type: "NodeList",
            
            attrs: 0
          },
          
          
          {
            name: "_state",
            type: "State",
            
            attrs: 0
          },
          
          
          {
            name: "_cancelRequest",
            type: "bool",
            
            attrs: 0
          },
          
          
          {
            name: "_nSteps",
            type: "int",
            
            attrs: 0
          },
          
          
          {
            name: "_start",
            type: "Node",
            
            obj_type: "AstarPathfinding::Node",
            
            attrs: 1
          },
          
          
          {
            name: "_goal",
            type: "Node",
            
            obj_type: "AstarPathfinding::Node",
            
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
            type: "Node",
            
            obj_type: "AstarPathfinding::Node",
            
            attrs: 1
          },
          
          
          {
            name: "child",
            type: "Node",
            
            obj_type: "AstarPathfinding::Node",
            
            attrs: 1
          },
          
          
          {
            name: "g",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "h",
            type: "float",
            
            attrs: 0
          },
          
          
          {
            name: "f",
            type: "float",
            
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
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "GetData",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "Data",
              return_attrs: 0
            },
          
            {
              name:   "CompleteCondition",
              params: "const std::string& objective,const std::string& condition",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "IsConditionCompleted",
              params: "const std::string& objective,const std::string& condition",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "IsOver",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "void ",
              params: "Quest::*WatcherInitializerMethod",
              attrs:  0,
              desc:   "",
              return_type: "typedef",
              return_attrs: 0
            },
          
            {
              name:   "CheckIfCompleted",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "bool",
              return_attrs: 0
            },
          
            {
              name:   "InitializeCondition",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WatcherCharacterInventory",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WatcherCharacterKill",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "WatcherTime",
              params: "Data condition,Level* level",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "data",
            type: "Data",
            
            obj_type: "Data",
            
            attrs: 0
          },
          
          
          {
            name: "method",
            type: "WatcherInitializerMethod",
            
            attrs: 0
          },
          
          
          {
            name: "_observers",
            type: "Sync::ObserverHandler",
            
            attrs: 0
          },
          
          
          {
            name: "_update_hook",
            type: "asIScriptFunction",
            
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
            type: "std::string",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Initialize",
              params: "Level* level",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "Finalize",
              params: "void",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "QuestCompleted",
              params: "Quest*",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "quest",
            type: "delete",
            
            attrs: 0
          },
          
          
          {
            name: "it",
            type: "Quests::const_iterator",
            
            attrs: 0
          },
          
          
          {
            name: "end",
            type: "Quests::const_iterator",
            
            attrs: 0
          },
          
          
          {
            name: "_data_engine",
            type: "DataEngine",
            
            obj_type: "DataEngine",
            
            attrs: 2
          },
          
          
          {
            name: "_quests",
            type: "Quests",
            
            attrs: 0
          },
          
          
          {
            name: "_stats_controller",
            type: "StatController",
            
            obj_type: "StatController",
            
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Throw",
              params: "unsigned int max",
              attrs:  16,
              desc:   "",
              return_type: "int",
              return_attrs: 16
            },
          
            {
              name:   "Test",
              params: "int successPercentage",
              attrs:  16,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 16
            },
          
            {
              name:   "Run",
              params: "void",
              attrs:  16,
              desc:   "",
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
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "AddChoice",
              params: "const std::string& name,std::function<void (Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            },
          
            {
              name:   "PickedChoice",
              params: "Rocket::Core::Event&",
              attrs:  0,
              desc:   "",
              return_type: "void",
              return_attrs: 0
            }
          
        ],
        attributes: [
          
          
          {
            name: "listener",
            type: "RocketListener",
            
            obj_type: "RocketListener",
            
            attrs: 0
          },
          
          
          {
            name: "_button_container",
            type: "Rocket::Core::Element",
            
            attrs: 1
          },
          
          
          {
            name: "_buttons",
            type: "Buttons",
            
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
            type: "std::string",
            
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
