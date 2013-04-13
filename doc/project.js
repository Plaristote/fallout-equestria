var project = {
  name: "Fallout: Equestria RPG",
  desc: {"homepage":"<h5>Overview</h5>\nI'm far too stoned to elaborate.\n"},
  types: [
    
      {
        name:       "Serializable",
        file:       "include/serializer.hpp",
        decl:       "struct",
        namespaces: ["Serializable"],
        methods: [
          {
              name:        "Serialize",
              params:      "Packet&",
              attrs:       68,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unserialize",
              params:      "Packet&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Packet",
        file:       "include/serializer.hpp",
        decl:       "class",
        namespaces: ["Packet"],
        methods: [
          {
              name:        "raw",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "char",
              return_attrs: 5,
              visibility:  "public"
            },{
              name:        "size",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "size_t",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PrintContent",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PrintRawContent",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unserialize",
              params:      "T& v",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SerializeArray",
              params:      "T& tehList",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "canIHaz",
              params:      "size_t sizeType,int howMany",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "checkType",
              params:      "int assumedType",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "realloc",
              params:      "int newsize",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "updateHeader",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "read",
              params:      "T& tmp",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "typeCode",
            type:       "char",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "copy",
            type:       "T",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "typeCode",
            type:       "char",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "copy",
            type:       "T",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "size",
            type:       "int",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "it",
            type:       "unsigned int	",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "reading",
            type:       "T",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "size",
            type:       "int",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "it",
            type:       "unsigned int	",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "reading",
            type:       "T",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "isDuplicate",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "buffer",
            type:       "void",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "sizeBuffer",
            type:       "size_t",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "reading",
            type:       "void",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Packet::SelectUnserializer",
        file:       "include/serializer.hpp",
        decl:       "struct",
        namespaces: ["Packet","Packet::SelectUnserializer"],
        methods: [
          {
              name:        "Func",
              params:      "Utils::Packet& packet,T& v",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Packet::TypeToCode",
        file:       "include/serializer.hpp",
        decl:       "struct",
        namespaces: ["Packet","Packet::TypeToCode"],
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "FBoundingBox",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["FBoundingBox"],
        methods: [
          {
              name:        "Intersects",
              params:      "float x,float y",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "left",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "top",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "width",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "height",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Waypoint",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["Waypoint"],
        methods: [
          {
              name:        "WithdrawArc",
              params:      "Waypoint* other",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UnwithdrawArc",
              params:      "Waypoint* other,ArcObserver* observer",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetWithdrawable",
              params:      "Waypoint* other",
              attrs:       0,
              desc:        "",
              return_type: "",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "ConnectUnsafe",
              params:      "Waypoint* other",
              attrs:       0,
              desc:        "",
              return_type: "Arcs::iterator",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Connect",
              params:      "Waypoint* other",
              attrs:       0,
              desc:        "",
              return_type: "Arcs::iterator",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Disconnect",
              params:      "Waypoint* other",
              attrs:       0,
              desc:        "",
              return_type: "Arcs::iterator",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DisconnectAll",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetArcTo",
              params:      "unsigned int id",
              attrs:       0,
              desc:        "",
              return_type: "Waypoint::Arc",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "PositionChanged",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UpdateArcDirection",
              params:      "Waypoint*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetSelected",
              params:      "bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsSelected",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetArcsVisible",
              params:      "bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoalDistanceEstimate",
              params:      "const Waypoint& goal",
              attrs:       4,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetDistanceEstimate",
              params:      "const Waypoint& other",
              attrs:       4,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetCost",
              params:      "Waypoint&",
              attrs:       0,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadArcs",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unserialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UnserializeLoadArcs",
              params:      "World*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Serialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetMouseBox",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "id",
            type:       "int",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "floor",
            type:       "char",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "arcs",
            type:       "Arcs",
            
            obj_type:   "Waypoint::Arcs",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "arcs_withdrawed",
            type:       "ArcsWithdrawed",
            
            obj_type:   "Waypoint::ArcsWithdrawed",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "nodePath",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "mouseBox",
            type:       "FBoundingBox",
            
            obj_type:   "FBoundingBox",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "selected",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Waypoint::ArcObserver",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["Waypoint","Waypoint::ArcObserver"],
        methods: [
          {
              name:        "CanGoThrough",
              params:      "unsigned char type",
              attrs:       64,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoingThrough",
              params:      "void*",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Waypoint::Arc",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["Waypoint","Waypoint::Arc"],
        methods: [
          {
              name:        "UpdateDirection",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Destroy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetVisible",
              params:      "bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "nodePath",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "from",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "to",
            type:       "Waypoint",
            
            obj_type:   "Waypoint",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "observer",
            type:       "ArcObserver",
            
            obj_type:   "Waypoint::ArcObserver",
            
            attrs:      1,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "MapObject",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["MapObject"],
        methods: [
          {
              name:        "UnSerialize",
              params:      "WindowFramework* window,Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Serialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "nodePath",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "floor",
            type:       "char",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "strModel",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "strTexture",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "DynamicObject",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["DynamicObject","","MapObject"],
        methods: [
          {
              name:        "UnSerialize",
              params:      "World*,Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Serialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "waypoint",
            type:       "Waypoint",
            
            obj_type:   "Waypoint",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "type",
            type:       "Type",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "script",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "locked",
            type:       "bool",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "key",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "charsheet",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "interactions",
            type:       "char",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "dialog",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
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
        name:       "Zone",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["Zone"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ExitZone",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["ExitZone","","Zone"],
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
        name:       "WorldLight",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["WorldLight"],
        methods: [
          {
              name:        "GetColor",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "LColor",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetColor",
              params:      "float r,float g,float b,float a",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "color",
              params:      "r,g,b,a",
              attrs:       0,
              desc:        "",
              return_type: "LColor",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UnSerialize",
              params:      "World*,Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Serialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ReparentTo",
              params:      "DynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ReparentTo",
              params:      "MapObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Initialize",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "nodePath",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "type",
            type:       "Type",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "parent_type",
            type:       "ParentType",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "lens",
            type:       "Lens",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "zoneSize",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "parent",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "parent_i",
            type:       "MapObject",
            
            obj_type:   "MapObject",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "World",
        file:       "include/level/world.h",
        decl:       "struct",
        namespaces: ["World"],
        methods: [
          {
              name:        "FloorResize",
              params:      "int",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddWayPoint",
              params:      "float x,float y,float z",
              attrs:       0,
              desc:        "",
              return_type: "Waypoint",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "DeleteWayPoint",
              params:      "Waypoint*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetWaypointFromNodePath",
              params:      "NodePath path",
              attrs:       0,
              desc:        "",
              return_type: "Waypoint",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetWaypointFromId",
              params:      "unsigned int id",
              attrs:       0,
              desc:        "",
              return_type: "Waypoint",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetWaypointClosest",
              params:      "LPoint3",
              attrs:       0,
              desc:        "",
              return_type: "Waypoint",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "SetWaypointsVisible",
              params:      "bool v",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetWaypointLimits",
              params:      "short currentFloor,LPoint3& upperRight,LPoint3& upperLeft,LPoint3& bottomLeft",
              attrs:       4,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetWaypointPlane",
              params:      "short currentFloor",
              attrs:       4,
              desc:        "",
              return_type: "LPlane",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DeleteObject",
              params:      "MapObject* ptr,std::list<OBJTYPE>& list",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetObjectFromName",
              params:      "const std::string& name,std::list<OBJTYPE>& list",
              attrs:       0,
              desc:        "",
              return_type: "OBJTYPE",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetObjectFromNodePath",
              params:      "NodePath path,std::list<OBJTYPE>& list",
              attrs:       0,
              desc:        "",
              return_type: "OBJTYPE",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "ObjectChangeFloor",
              params:      "MapObject&,unsigned char floor,unsigned short type",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddMapObject",
              params:      "const std::string& name,const std::string& model,const std::string& texture,float x,float y,float z",
              attrs:       0,
              desc:        "",
              return_type: "MapObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "DeleteMapObject",
              params:      "MapObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetMapObjectFromName",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "MapObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetMapObjectFromNodePath",
              params:      "NodePath path",
              attrs:       0,
              desc:        "",
              return_type: "MapObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "SetMapObjectsVisible",
              params:      "bool v",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "MapObjectChangeFloor",
              params:      "MapObject&,unsigned char floor",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "InsertDynamicObject",
              params:      "DynamicObject&",
              attrs:       0,
              desc:        "",
              return_type: "DynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "AddDynamicObject",
              params:      "const std::string& name,DynamicObject::Type type,const std::string& model,const std::string& texture",
              attrs:       0,
              desc:        "",
              return_type: "DynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "DeleteDynamicObject",
              params:      "DynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetDynamicObjectFromName",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "DynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetDynamicObjectFromNodePath",
              params:      "NodePath path",
              attrs:       0,
              desc:        "",
              return_type: "DynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "SetDynamicObjectsVisible",
              params:      "bool v",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DynamicObjectSetWaypoint",
              params:      "DynamicObject&,Waypoint&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DynamicObjectChangeFloor",
              params:      "DynamicObject&,unsigned char floor",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddExitZone",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DeleteExitZone",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetExitZoneByName",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "ExitZone",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "AddEntryZone",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DeleteEntryZone",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetEntryZoneByName",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "EntryZone",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "AddLight",
              params:      "WorldLight::Type,const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddLight",
              params:      "WorldLight::Type,const std::string&,MapObject* parent",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddLight",
              params:      "WorldLight::Type,const std::string&,DynamicObject* parent",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DeleteLight",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetLightByName",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "WorldLight",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "CompileLight",
              params:      "WorldLight*,unsigned char = ColMask::Object | ColMask::DynObject",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetWaypointAt",
              params:      "LPoint2f",
              attrs:       0,
              desc:        "",
              return_type: "Waypoint",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "UnSerialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Serialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CompileWaypoints",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CompileDoors",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "floors",
            type:       "Floors",
            
            obj_type:   "World::Floors",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "rootWaypoints",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "waypoints",
            type:       "Waypoints",
            
            obj_type:   "World::Waypoints",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "rootMapObjects",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "objects",
            type:       "MapObjects",
            
            obj_type:   "World::MapObjects",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "rootDynamicObjects",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "dynamicObjects",
            type:       "DynamicObjects",
            
            obj_type:   "World::DynamicObjects",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "rootLights",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "lights",
            type:       "WorldLights",
            
            obj_type:   "World::WorldLights",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "exitZones",
            type:       "ExitZones",
            
            obj_type:   "World::ExitZones",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "entryZones",
            type:       "EntryZones",
            
            obj_type:   "World::EntryZones",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "model_sphere",
            type:       "NodePath",
            
            attrs:      16,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Party",
        file:       "include/playerparty.hpp",
        decl:       "class",
        namespaces: ["Party"],
        methods: [
          {
              name:        "Join",
              params:      "DynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Leave",
              params:      "DynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ConstGetObjects",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "Party::DynamicObjects",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "GetObjects",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Party::DynamicObjects",
              return_attrs: 2,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_objects",
            type:       "DynamicObjects",
            
            obj_type:   "Party::DynamicObjects",
            
            attrs:      0,
            visibility: "protected"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "PlayerParty",
        file:       "include/playerparty.hpp",
        decl:       "class",
        namespaces: ["PlayerParty","","Party"],
        methods: [
          {
              name:        "Create",
              params:      "const std::string& savepath,const std::string& name,const std::string& model,const std::string& texture",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "const std::string& savepath",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetHasLocalObjects",
              params:      "bool val",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Serialize",
              params:      "Utils::Packet&",
              attrs:       4,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UnSerialize",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_local_objects",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Semaphore",
        file:       "include/semaphore.hpp",
        decl:       "class",
        namespaces: ["Semaphore"],
        methods: [
          {
              name:        "TryWait",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Wait",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Post",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetDeadlockSafety",
              params:      "bool to_set",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SelfLock",
              params:      "void",
              attrs:       36,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GetCurrentThreadId",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "Semaphore::ThreadId",
              return_attrs: 16,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_handle",
            type:       "Handle",
            
            obj_type:   "Semaphore::Handle",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_max_count",
            type:       "int",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_deadlock_safety",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_thread_lock",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_thread_id",
            type:       "ThreadId",
            
            obj_type:   "Semaphore::ThreadId",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Semaphore::Lock",
        file:       "include/semaphore.hpp",
        decl:       "class",
        namespaces: ["Semaphore","Semaphore::Lock"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "_semaphore",
            type:       "Semaphore",
            
            obj_type:   "Semaphore",
            
            attrs:      2,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ISignal",
        file:       "include/observatory.hpp",
        decl:       "class",
        namespaces: ["ISignal"],
        methods: [
          {
              name:        "ExecuteRecordedCalls",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetDirect",
              params:      "bool",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Signals",
        file:       "include/observatory.hpp",
        decl:       "class",
        namespaces: ["Signals",""],
        methods: [
          {
              name:        "ExecuteRecordedCalls",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
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
        name:       "Signal",
        file:       "include/observatory.hpp",
        decl:       "class",
        namespaces: ["Signal",""],
        methods: [
          {
              name:        "SetDirect",
              params:      "bool set",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsDirect",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Emit",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Connect",
              params:      "ObserverClass& observerInstance,typename Observer<ObserverClass>::Method method",
              attrs:       0,
              desc:        "",
              return_type: "ObserverId",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Connect",
              params:      "typename FunctionObserver::Function function",
              attrs:       0,
              desc:        "",
              return_type: "ObserverId",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Disconnect",
              params:      "ObserverId id",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DisconnectAll",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ObserverCount",
              params:      "void",
              attrs:       36,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ExecuteRecordedCalls",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "lock",
              params:      "_semaphore",
              attrs:       0,
              desc:        "",
              return_type: "Semaphore::Lock",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FuncExecuteRecordedCalls",
              params:      "RecordedCalls& calls",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PushRecordCall",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "lock",
              params:      "_semaphore",
              attrs:       0,
              desc:        "",
              return_type: "Semaphore::Lock",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FetchRecordCall",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "lock",
              params:      "_semaphore",
              attrs:       0,
              desc:        "",
              return_type: "Semaphore::Lock",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RecordCallCount",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "lock",
              params:      "_semaphore",
              attrs:       0,
              desc:        "",
              return_type: "Semaphore::Lock",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PushedCallCount",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "lock",
              params:      "_semaphore",
              attrs:       0,
              desc:        "",
              return_type: "Semaphore::Lock",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "BackupRecordedCalls",
              params:      "bool on_off",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddObserver",
              params:      "InterfaceObserver* observer",
              attrs:       0,
              desc:        "",
              return_type: "ObserverId",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_iterator",
            type:       "Observers::iterator",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_iterator_updated_during_emit",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_observers",
            type:       "Observers",
            
            obj_type:   "Signal::Observers",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_recordedCalls",
            type:       "RecordedCalls",
            
            obj_type:   "Signal::RecordedCalls",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_backedupCalls",
            type:       "RecordedCalls",
            
            obj_type:   "Signal::RecordedCalls",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_pushedCalls",
            type:       "RecordedCalls",
            
            obj_type:   "Signal::RecordedCalls",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_direct",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_semaphore",
            type:       "Semaphore",
            
            obj_type:   "Semaphore",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Signal::InterfaceObserver",
        file:       "include/observatory.hpp",
        decl:       "struct",
        namespaces: ["Signal","Signal::InterfaceObserver"],
        methods: [
          {
              name:        "operator",
              params:      "",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "id",
            type:       "ObserverId",
            
            obj_type:   "ObserverId",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Signal::FunctionObserver",
        file:       "include/observatory.hpp",
        decl:       "class",
        namespaces: ["Signal","Signal::FunctionObserver","","Signal","Signal::InterfaceObserver"],
        methods: [
          {
              name:        "operator",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_function",
            type:       "Function",
            
            obj_type:   "Signal::FunctionObserver::Function",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Signal::Observer",
        file:       "include/observatory.hpp",
        decl:       "class",
        namespaces: ["Signal","Signal::Observer","","Signal","Signal::InterfaceObserver"],
        methods: [
          {
              name:        "operator",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_observer",
            type:       "ObserverClass",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_method",
            type:       "Method",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Signal::RecordedCall",
        file:       "include/observatory.hpp",
        decl:       "struct",
        namespaces: ["Signal","Signal::RecordedCall"],
        methods: [
          {
              name:        "Serialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unserialize",
              params:      "Utils::Packet& packet",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "byte",
            type:       "bool",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ObserverHandler",
        file:       "include/observatory.hpp",
        decl:       "class",
        namespaces: ["ObserverHandler"],
        methods: [
          {
              name:        "DisconnectAll",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DisconnectAllFrom",
              params:      "const ISignal& signal",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Add",
              params:      "C& signal,ObserverId id",
              attrs:       32,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Connect",
              params:      "C& signal,ObserverClass& observerInstance,typename C::template Observer<ObserverClass>::Method method",
              attrs:       32,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Connect",
              params:      "C& signal,typename C::FunctionObserver::Function function",
              attrs:       32,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_observers",
            type:       "Observers",
            
            obj_type:   "ObserverHandler::Observers",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ObserverHandler::IObserverPair",
        file:       "include/observatory.hpp",
        decl:       "struct",
        namespaces: ["ObserverHandler","ObserverHandler::IObserverPair"],
        methods: [
          {
              name:        "Disconnect",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ObserverHandler::ObserverPair",
        file:       "include/observatory.hpp",
        decl:       "struct",
        namespaces: ["ObserverHandler","ObserverHandler::ObserverPair","","ObserverHandler","ObserverHandler::IObserverPair"],
        methods: [
          {
              name:        "Disconnect",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "signal",
            type:       "C",
            
            attrs:      2,
            visibility: "public"
          },
          {
            name:       "id",
            type:       "ObserverId",
            
            obj_type:   "ObserverId",
            
            attrs:      0,
            visibility: "public"
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
        name:       "Timer",
        file:       "include/timer.hpp",
        decl:       "class",
        namespaces: ["Timer"],
        methods: [
          {
              name:        "GetElapsedTime",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "double",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Profile",
              params:      "std::string str",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Restart",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetLastTime",
              params:      "double v",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetLastTime",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "asConstructor",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "asDestructor",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_lastTime",
            type:       "double",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "TimeManager",
        file:       "include/timer.hpp",
        decl:       "class",
        namespaces: ["TimeManager"],
        methods: [
          {
              name:        "GetDaysPerMonth",
              params:      "unsigned short m,unsigned short year = 1",
              attrs:       16,
              desc:        "",
              return_type: "short",
              return_attrs: 24,
              visibility:  "private"
            },{
              name:        "ClearTasks",
              params:      "unsigned char level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetTime",
              params:      "unsigned short s,unsigned short m,unsigned short h,unsigned short d,unsigned short mo,unsigned short y",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GetYear",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "private"
            },{
              name:        "GetMonth",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "private"
            },{
              name:        "GetDay",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "private"
            },{
              name:        "GetHour",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "private"
            },{
              name:        "GetMinute",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "private"
            },{
              name:        "GetSecond",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "private"
            },{
              name:        "AddElapsedSeconds",
              params:      "float s",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AddElapsedTime",
              params:      "unsigned short s,unsigned short m = 0,unsigned short h = 0,unsigned short d = 0,unsigned short mo = 0,unsigned short y = 0",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AddTask",
              params:      "unsigned char level,bool loop,unsigned short s,unsigned short m = 0,unsigned short h = 0,unsigned short d = 0,unsigned short mo = 0,unsigned short y = 0",
              attrs:       0,
              desc:        "",
              return_type: "TimeManager::Task",
              return_attrs: 1,
              visibility:  "private"
            },{
              name:        "DelTask",
              params:      "Task* task",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ExecuteTasks",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CorrectValues",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "fseconds",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "year",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "month",
            type:       "unsigned short  ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "day",
            type:       "short  ",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "hour",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "minute",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "second",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "tasks",
            type:       "Tasks",
            
            obj_type:   "TimeManager::Tasks",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "safeIt",
            type:       "Tasks::iterator",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "TimeManager::Task",
        file:       "include/timer.hpp",
        decl:       "struct",
        namespaces: ["TimeManager","TimeManager::Task"],
        methods: [
          {
              name:        "NextStep",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Serialize",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unserialize",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "loop",
            type:       "bool",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "it",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "lastY",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "lastMo",
            type:       "unsigned short ",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "lastD",
            type:       "short ",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "lastH",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "lastM",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "lastS",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "timeY",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "timeMo",
            type:       "unsigned short ",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "timeD",
            type:       "short ",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "timeH",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "timeM",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "timeS",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "level",
            type:       "char",
            
            attrs:      8,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "DataBranch",
        file:       "include/datatree.hpp",
        decl:       "struct",
        namespaces: ["DataBranch"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "key",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "value",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "children",
            type:       "Children",
            
            obj_type:   "DataBranch::Children",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "father",
            type:       "DataBranch",
            
            obj_type:   "DataBranch",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "nil",
            type:       "bool",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "root",
            type:       "bool         ",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "pointers",
            type:       "int",
            
            attrs:      8,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Data",
        file:       "include/datatree.hpp",
        decl:       "class",
        namespaces: ["Data"],
        methods: [
          {
              name:        "Key",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Value",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetKey",
              params:      "const std::string& newKey",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Duplicate",
              params:      "Data var",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "T",
              params:      "",
              attrs:       4,
              desc:        "",
              return_type: "operator",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Remove",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CutBranch",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Nil",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "NotNil",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Parent",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PushBack",
              params:      "Data d",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Output",
              params:      "unsigned char indent = 0",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Count",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "int",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "begin",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "Data::my_iterator",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "const_begin",
              params:      "",
              attrs:       4,
              desc:        "",
              return_type: "Data::const_my_iterator",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "end",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "Data::my_iterator",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "const_end",
              params:      "",
              attrs:       4,
              desc:        "",
              return_type: "Data::const_my_iterator",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "stream",
            type:       "std::stringstream",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "stream",
            type:       "std::stringstream",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "_data",
            type:       "DataBranch",
            
            obj_type:   "DataBranch",
            
            attrs:      1,
            visibility: "protected"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Data::my_iterator",
        file:       "include/datatree.hpp",
        decl:       "class",
        namespaces: ["Data","Data::my_iterator",""],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "_it",
            type:       "Children::iterator",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Data::const_my_iterator",
        file:       "include/datatree.hpp",
        decl:       "class",
        namespaces: ["Data","Data::const_my_iterator",""],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "_it",
            type:       "Children::const_iterator",
            
            attrs:      0,
            visibility: "private"
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
        name:       "DataTree",
        file:       "include/datatree.hpp",
        decl:       "class",
        namespaces: ["DataTree","","DataBranch"],
        methods: [
          {
              name:        "tmp",
              params:      "this",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "const std::string& path",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetSourceFile",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "source",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
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
        name:       "DataTree::Factory",
        file:       "include/datatree.hpp",
        decl:       "struct",
        namespaces: ["DataTree","DataTree::Factory"],
        methods: [
          {
              name:        "Yaml",
              params:      "const std::string& filename",
              attrs:       16,
              desc:        "",
              return_type: "DataTree",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "ShinyLang",
              params:      "const std::string& filename",
              attrs:       16,
              desc:        "",
              return_type: "DataTree",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "JSON",
              params:      "const std::string& filename",
              attrs:       16,
              desc:        "",
              return_type: "DataTree",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "StringJSON",
              params:      "const std::string& str",
              attrs:       16,
              desc:        "",
              return_type: "DataTree",
              return_attrs: 17,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "DataTree::Writers",
        file:       "include/datatree.hpp",
        decl:       "struct",
        namespaces: ["DataTree","DataTree::Writers"],
        methods: [
          {
              name:        "JSON",
              params:      "Data,const std::string& filename",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "StringJSON",
              params:      "Data,std::string& str",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "DataEngine",
        file:       "include/dataengine.hpp",
        decl:       "class",
        namespaces: ["DataEngine","","Data"],
        methods: [
          {
              name:        "Load",
              params:      "const std::string& filepath",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "const std::string& filepath",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetDiplomacy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "WorldDiplomacy",
              return_attrs: 2,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_dataTree",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_diplomacy",
            type:       "WorldDiplomacy",
            
            obj_type:   "WorldDiplomacy",
            
            attrs:      1,
            visibility: "private"
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
        name:       "AnimatedObject",
        file:       "include/animatedobject.hpp",
        decl:       "class",
        namespaces: ["AnimatedObject"],
        methods: [
          {
              name:        "GetNodePath",
              params:      "void",
              attrs:       68,
              desc:        "",
              return_type: "NodePath",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ResetAnimation",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlayAnimation",
              params:      "const std::string& name,bool loop = false",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadAnimation",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "TaskAnimation",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "PlayIdleAnimation",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            }
        ],
        attributes: [
          
          {
            name:       "pendingAnimationDone",
            type:       "bool",
            
            attrs:      0,
            visibility: "public",
            doc:         {"name":"pendingAnimationDone","short":"","desc":""}
          },
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "protected",
            doc:         {"name":"_window","short":"","desc":""}
          },
          {
            name:       "_modelName",
            type:       "std::string",
            
            attrs:      0,
            visibility: "protected",
            doc:         {"name":"_modelName","short":"The name of the model, used to find the corresponding animations files","desc":"The value must be the name of the model file with no extension or path given."}
          },
          {
            name:       "_anims",
            type:       "AnimControlCollection",
            
            attrs:      0,
            visibility: "protected",
            doc:         {"name":"_anims","short":""}
          },
          {
            name:       "_mapAnims",
            type:       "MapAnims",
            
            obj_type:   "AnimatedObject::MapAnims",
            
            attrs:      0,
            visibility: "protected",
            doc:         {"name":"_mapAnims","short":"key/value map of Panda3D animation controllers"}
          },
          {
            name:       "_anim",
            type:       "AnimControl",
            
            attrs:      1,
            visibility: "protected",
            doc:         {"name":"anim","short":"pointer to the currently played animation"}
          },
          {
            name:       "_animLoop",
            type:       "bool",
            
            attrs:      0,
            visibility: "protected",
            doc:         {"name":"_animLoop","short":"Set to true if the currently played animation must be looping"}
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "WaypointModifier",
        file:       "include/level/objectnode.hpp",
        decl:       "struct",
        namespaces: ["WaypointModifier"],
        methods: [
          {
              name:        "ProcessCollisions",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UnprocessCollisions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "HasOccupiedWaypoint",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetOccupiedWaypointAsInt",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetOccupiedWaypoint",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "Waypoint",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "SetOccupiedWaypoint",
              params:      "Waypoint* wp",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "WithdrawAllArcs",
              params:      "unsigned int id",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "WithdrawAllArcs",
              params:      "Waypoint* waypoint",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "WithdrawArc",
              params:      "unsigned int id1,unsigned int id2",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "WithdrawArc",
              params:      "Waypoint* first,Waypoint* second",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            }
        ],
        attributes: [
          
          {
            name:       "_collision_processed",
            type:       "short",
            
            attrs:      8,
            visibility: "protected"
          },
          {
            name:       "_level",
            type:       "Level",
            
            obj_type:   "Level",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_waypointOccupied",
            type:       "Waypoint",
            
            obj_type:   "Waypoint",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_withdrawedArcs",
            type:       "WithdrawedArcs",
            
            obj_type:   "WaypointModifier::WithdrawedArcs",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "WaypointModifier::WithdrawedArc",
        file:       "include/level/objectnode.hpp",
        decl:       "struct",
        namespaces: ["WaypointModifier","WaypointModifier::WithdrawedArc"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "first",
            type:       "Waypoint",
            
            obj_type:   "Waypoint",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "second",
            type:       "Waypoint              ",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "observer",
            type:       "Waypoint::ArcObserver",
            
            obj_type:   "Waypoint::ArcObserver",
            
            attrs:      1,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ObjectType2Code",
        file:       "include/level/objectnode.hpp",
        decl:       "struct",
        namespaces: ["ObjectType2Code"],
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "InstanceDynamicObject",
        file:       "include/level/objectnode.hpp",
        decl:       "class",
        namespaces: ["InstanceDynamicObject","","WaypointModifier","","AnimatedObject"],
        methods: [
          {
              name:        "Load",
              params:      "Utils::Packet&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "Utils::Packet&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "float elapsedTime",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetNodePath",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "NodePath",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetInteractions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "InstanceDynamicObject::InteractionList",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetDialog",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "GetDynamicObject",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "DynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetGoToData",
              params:      "InstanceDynamicObject* character",
              attrs:       64,
              desc:        "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Get",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "C",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "CallbackActionUse",
              params:      "InstanceDynamicObject* object",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ResetAnimation",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ResetInteractions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "ThatDoesNothing",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "CallbackAnimationEnded",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "pendingActionOn",
            type:       "InstanceDynamicObject",
            
            obj_type:   "InstanceDynamicObject",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "pendingActionObject",
            type:       "InventoryObject",
            
            obj_type:   "InventoryObject",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "pendingActionObjectActionIt",
            type:       "char",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "_type",
            type:       "char",
            
            attrs:      8,
            visibility: "protected"
          },
          {
            name:       "_object",
            type:       "DynamicObject",
            
            obj_type:   "DynamicObject",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_interactions",
            type:       "InteractionList",
            
            obj_type:   "InstanceDynamicObject::InteractionList",
            
            attrs:      0,
            visibility: "protected"
          },
          {
            name:       "_idle_size",
            type:       "LPoint3",
            
            attrs:      0,
            visibility: "protected"
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
        name:       "InstanceDynamicObject::Interaction",
        file:       "include/level/objectnode.hpp",
        decl:       "struct",
        namespaces: ["InstanceDynamicObject","InstanceDynamicObject::Interaction"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "instance",
            type:       "InstanceDynamicObject",
            
            obj_type:   "InstanceDynamicObject",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "signal",
            type:       "",
            
            attrs:      1,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "InstanceDynamicObject::GoToData",
        file:       "include/level/objectnode.hpp",
        decl:       "struct",
        namespaces: ["InstanceDynamicObject","InstanceDynamicObject::GoToData"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "nearest",
            type:       "Waypoint",
            
            obj_type:   "Waypoint",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "objective",
            type:       "InstanceDynamicObject",
            
            obj_type:   "InstanceDynamicObject",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "max_distance",
            type:       "int",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "min_distance",
            type:       "int",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "LevelExitZone",
        file:       "include/level/objects/door.hpp",
        decl:       "class",
        namespaces: ["LevelExitZone","","Waypoint","Waypoint::ArcObserver"],
        methods: [
          {
              name:        "SetName",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "CanGoThrough",
              params:      "unsigned char id",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoingThrough",
              params:      "void* character",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_level",
            type:       "Level",
            
            obj_type:   "Level",
            
            attrs:      1,
            visibility: "private"
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
        name:       "Lockable",
        file:       "include/level/objects/door.hpp",
        decl:       "class",
        namespaces: ["Lockable"],
        methods: [
          {
              name:        "GetKeyName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsLocked",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsOpen",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unlock",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_closed",
            type:       "bool",
            
            attrs:      0,
            visibility: "protected"
          },
          {
            name:       "__object",
            type:       "DynamicObject",
            
            obj_type:   "DynamicObject",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ObjectDoor",
        file:       "include/level/objects/door.hpp",
        decl:       "class",
        namespaces: ["ObjectDoor","","InstanceDynamicObject","","WaypointModifier","","AnimatedObject","","Lockable","","Waypoint","Waypoint::ArcObserver"],
        methods: [
          {
              name:        "ProcessCollisions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetKeyName",
              params:      "",
              attrs:       4,
              desc:        "",
              return_type: "string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackActionUse",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetGoToData",
              params:      "InstanceDynamicObject* character",
              attrs:       0,
              desc:        "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ObserveWaypoints",
              params:      "bool doObserver",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CanGoThrough",
              params:      "unsigned char id",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoingThrough",
              params:      "void*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PendingActionOpen",
              params:      "InstanceDynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
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
        name:       "Engine",
        file:       "include/scriptengine.hpp",
        decl:       "class",
        namespaces: ["Engine"],
        methods: [
          {
              name:        "Get",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "asIScriptEngine",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "Initialize",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Finalize",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "LoadModule",
              params:      "const std::string& name,const std::string& filepath",
              attrs:       16,
              desc:        "",
              return_type: "asIScriptModule",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "MessageCallback",
              params:      "const asSMessageInfo* msg,void* param",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_engine",
            type:       "asIScriptEngine",
            
            attrs:      17,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ModuleManager",
        file:       "include/scriptengine.hpp",
        decl:       "class",
        namespaces: ["ModuleManager"],
        methods: [
          {
              name:        "Require",
              params:      "const std::string& name,const std::string& filepath",
              attrs:       16,
              desc:        "",
              return_type: "asIScriptModule",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "Release",
              params:      "asIScriptModule* module",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "OutputFunctionList",
              params:      "asIScriptModule* module",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_modules",
            type:       "Modules",
            
            obj_type:   "ModuleManager::Modules",
            
            attrs:      16,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ModuleManager::LoadedModule",
        file:       "include/scriptengine.hpp",
        decl:       "struct",
        namespaces: ["ModuleManager","ModuleManager::LoadedModule"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "filepath",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "ptr",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "users",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "StdList",
        file:       "include/scriptengine.hpp",
        decl:       "class",
        namespaces: ["StdList",""],
        methods: [
          {
              name:        "Add",
              params:      "T t",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Register",
              params:      "asIScriptEngine* engine,const std::string& arrayName,const std::string& typeName",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Constructor",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Destructor ",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Size",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "Begin",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StdList::asIterator",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RBegin",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StdList::asRIterator",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "End",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StdList::asIterator",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "REnd",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StdList::asRIterator",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
          {
            name:       "std::list<T>",
            visibility: "public"
          }
        ]
      },
      {
        name:       "StdList::asIterator",
        file:       "include/scriptengine.hpp",
        decl:       "struct",
        namespaces: ["StdList","StdList::asIterator"],
        methods: [
          {
              name:        "Constructor",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Destructor ",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Assign",
              params:      "asIterator it",
              attrs:       16,
              desc:        "",
              return_type: "T",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Value",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "T",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Equal",
              params:      "asIterator comp",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Increment",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Decrement",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "it",
            type:       "iterator",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "self",
            type:       "",
            
            attrs:      1,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "StdList::asRIterator",
        file:       "include/scriptengine.hpp",
        decl:       "struct",
        namespaces: ["StdList","StdList::asRIterator"],
        methods: [
          {
              name:        "Constructor",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Destructor ",
              params:      "void* memory",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Assign",
              params:      "asRIterator it",
              attrs:       16,
              desc:        "",
              return_type: "T",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Value",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "T",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Equal",
              params:      "asRIterator comp",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Increment",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Decrement",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "it",
            type:       "reverse_iterator",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "self",
            type:       "",
            
            attrs:      1,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "i18n",
        file:       "include/i18n.hpp",
        decl:       "class",
        namespaces: ["i18n"],
        methods: [
          {
              name:        "Load",
              params:      "const std::string& language",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Unload",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "T",
              params:      "const std::string& key",
              attrs:       16,
              desc:        "",
              return_type: "std::string",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "GetDialogs",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "Data",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "GetStatistics",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "Data",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_self",
            type:       "i18n",
            
            obj_type:   "i18n",
            
            attrs:      17,
            visibility: "private"
          },
          {
            name:       "_dialogs",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_statistics",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "MyRocket",
        file:       "include/rocket_extension.hpp",
        decl:       "struct",
        namespaces: ["MyRocket"],
        methods: [
          {
              name:        "SetVisibility",
              params:      "Rocket::Core::Context* context,bool visibility",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "RocketListener",
        file:       "include/rocket_extension.hpp",
        decl:       "struct",
        namespaces: ["RocketListener",""],
        methods: [
          {
              name:        "ProcessEvent",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
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
        name:       "UiBase",
        file:       "include/rocket_extension.hpp",
        decl:       "class",
        namespaces: ["UiBase"],
        methods: [
          {
              name:        "FireShow",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FireHide",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Show",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Hide",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsVisible",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetModal",
              params:      "bool modal",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Destroy",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetPropertyOnAll",
              params:      "Rocket::Core::Element* elem,const std::string& property,const std::string& value",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "FadeOut",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FadeIn",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ToggleEventListener",
              params:      "bool toggle_on,const std::string& id,const std::string& event,RocketListener& listener",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Translate",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RecursiveTranslate",
              params:      "Rocket::Core::Element*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_root",
            type:       "Rocket::Core::ElementDocument",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_context",
            type:       "Rocket::Core::Context",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_languageObs",
            type:       "Sync::ObserverId",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_listeners",
            type:       "Listeners",
            
            obj_type:   "UiBase::Listeners",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "UiBase::Listener",
        file:       "include/rocket_extension.hpp",
        decl:       "struct",
        namespaces: ["UiBase","UiBase::Listener"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "elem",
            type:       "std::string",
            
            attrs:      4,
            visibility: "public"
          },
          {
            name:       "event",
            type:       "std::string",
            
            attrs:      4,
            visibility: "public"
          },
          {
            name:       "instance",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      2,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "GameMenu",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["GameMenu","","UiBase"],
        methods: [
          {
              name:        "MenuEventContinue",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_continueClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_exitClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_optionsClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_saveClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_loadClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
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
        name:       "GameMainBar",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["GameMainBar","","UiBase"],
        methods: [
          {
              name:        "AppendToConsole",
              params:      "const std::string& str",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCurrentAP",
              params:      "unsigned short ap,unsigned short maxap",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetMaxAP",
              params:      "unsigned short ap",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetEnabledAP",
              params:      "bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCurrentHp",
              params:      "short hp",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCurrentAc",
              params:      "short ac",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetEquipedItem",
              params:      "unsigned short it,InventoryObject* item",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetEquipedItemAction",
              params:      "unsigned short it,InventoryObject* item,unsigned char actionIt",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackEquipedItem1Clicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackEquipedItem2Clicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackPassTurnClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackCombatEndClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "MenuButtonClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "InventoryButtonClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "PersButtonClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "PipbuckButtonClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "EquipedItem1Clicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EquipedItem2Clicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "PassTurnClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "CombatEndClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_apMax",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_apEnabled",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
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
        name:       "InventoryObject",
        file:       "include/level/inventory.hpp",
        decl:       "class",
        namespaces: ["InventoryObject","","Data"],
        methods: [
          {
              name:        "UseAsWeapon",
              params:      "ObjectCharacter* user,ObjectCharacter* target,unsigned char useType",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "UseOn",
              params:      "ObjectCharacter* user,InstanceDynamicObject* target,unsigned char useType",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "Use",
              params:      "ObjectCharacter* user,unsigned char useType",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "GetName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "GetIcon",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "CreateDynamicObject",
              params:      "World* world",
              attrs:       4,
              desc:        "",
              return_type: "DynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "CreateEquipedModel",
              params:      "World* world",
              attrs:       0,
              desc:        "",
              return_type: "InventoryObject::EquipedModel",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "CanWeild",
              params:      "ObjectCharacter*,EquipedMode",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetEquiped",
              params:      "bool set",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsEquiped",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsGroupableWith",
              params:      "const InventoryObject*",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ExecuteHook",
              params:      "asIScriptFunction* hook,ObjectCharacter* user,C* target,unsigned char actionIt",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_dataTree",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_equiped",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_script_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_script_module",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_actionHooks",
            type:       "ActionsHooks",
            
            obj_type:   "InventoryObject::ActionsHooks",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_hookUseOnCharacter",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_hookUseOnDoor",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_hookUseOnOthers",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_hookUseAsWeapon",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_hookCanWeildMouth",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_hookCanWeildMagic",
            type:       "asIScriptFunction* ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_hookCanWeildBattleSaddle",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
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
        name:       "InventoryObject::EquipedModel",
        file:       "include/level/inventory.hpp",
        decl:       "struct",
        namespaces: ["InventoryObject","InventoryObject::EquipedModel","","AnimatedObject"],
        methods: [
          {
              name:        "GetNodePath",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "NodePath",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ResetAnimation",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "np",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "object",
            type:       "InventoryObject",
            
            obj_type:   "InventoryObject",
            
            attrs:      2,
            visibility: "public"
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
        name:       "InventoryObject::ActionHooks",
        file:       "include/level/inventory.hpp",
        decl:       "struct",
        namespaces: ["InventoryObject","InventoryObject::ActionHooks"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "Use",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "UseOnCharacter",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "UseOnDoor",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "UseOnOthers",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "UseAsWeapon",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Inventory",
        file:       "include/level/inventory.hpp",
        decl:       "class",
        namespaces: ["Inventory"],
        methods: [
          {
              name:        "LoadInventory",
              params:      "DynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SaveInventory",
              params:      "DynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadInventory",
              params:      "Data",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SaveInventory",
              params:      "Data",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddObject",
              params:      "InventoryObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DelObject",
              params:      "InventoryObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IncludesObject",
              params:      "InventoryObject*",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetContent",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "Inventory::Content",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "GetContent",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Inventory::Content",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetObject",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "InventoryObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "ContainsHowMany",
              params:      "const std::string& name",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetCurrentWeight",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetCapacity",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "SetCapacity",
              params:      "unsigned short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CanCarry",
              params:      "InventoryObject*,unsigned short quantity = 1",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_content",
            type:       "Content",
            
            obj_type:   "Inventory::Content",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_currentWeight",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_capacity",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Scriptable",
        file:       "include/scriptable.hpp",
        decl:       "class",
        namespaces: ["Scriptable"],
        methods: [
          {
              name:        "LoadScript",
              params:      "std::string module_name,std::string filepath",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ReloadFunction",
              params:      "asIScriptFunction** pointer",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_script_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_script_module",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "protected"
          },
          {
            name:       "_script_func_ptrs",
            type:       "ScriptFuncPtrs",
            
            obj_type:   "Scriptable::ScriptFuncPtrs",
            
            attrs:      0,
            visibility: "protected"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "StatModel",
        file:       "include/statsheet.hpp",
        decl:       "class",
        namespaces: ["StatModel","","Scriptable"],
        methods: [
          {
              name:        "Backup",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RestoreBackup",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Action",
              params:      "const std::string& action,const std::string& fmt,...",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetAll",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetName",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetRace",
              params:      "const std::string& race",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetRace",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetAge",
              params:      "unsigned short age",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetAge",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "SetGender",
              params:      "const std::string& g",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetGender",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddKill",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetKills",
              params:      "const std::string& race",
              attrs:       4,
              desc:        "",
              return_type: "int",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "AddPerk",
              params:      "const std::string& perk",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ToggleTrait",
              params:      "const std::string& trait",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ToggleSkillAffinity",
              params:      "const std::string& skill",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "HasTrait",
              params:      "const std::string& trait",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "HasSkillAffinity",
              params:      "const std::string& skill",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetSkillAffinities",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetStatistic",
              params:      "const std::string& stat,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetSpecial",
              params:      "const std::string& stat,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetSkill",
              params:      "const std::string& stat,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetLevel",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetStatistic",
              params:      "const std::string& stat",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetSpecial",
              params:      "const std::string& stat",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetSkill",
              params:      "const std::string& stat",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SelectRandomEncounter",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetSpecialPoints",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetSkillPoints",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetPerksPoints",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetXpNextLevel",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetExperience",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "SetExperience",
              params:      "unsigned short e",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LevelUp",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCurrentHp",
              params:      "short hp",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetCurrentHp",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetMaxHp",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsReady",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UpdateAllValues",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadFunctions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_statsheet",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_statsheet_backup",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_scriptAddSpecialPoint",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptActivateTraits",
            type:       "asIScriptFunction ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptAddExperience",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptXpNextLevel",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptLevelUp",
            type:       "asIScriptFunction ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptUpdateAllValues",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptIsReady",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptAvailableTraits",
            type:       "asIScriptFunction ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptAddPerk",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_selectRandomEncounter",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
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
        name:       "StatView",
        file:       "include/statsheet.hpp",
        decl:       "class",
        namespaces: ["StatView"],
        methods: [
          {
              name:        "SetEditMode",
              params:      "EditMode",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetEditMode",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "EditMode",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Hide",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Show",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetInformation",
              params:      "const std::string& name,const std::string& value",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetInformation",
              params:      "const std::string& name,short value",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetFieldValue",
              params:      "const std::string& category,const std::string& key,const std::string& value",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetFieldValue",
              params:      "const std::string& category,const std::string& key,short value",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCategoryFields",
              params:      "const std::string& category,const std::vector<std::string>& keys",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetIdValue",
              params:      "const std::string& id,const std::string& value",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetIdValue",
              params:      "const std::string& id,short value",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetExperience",
              params:      "unsigned int xp,unsigned short lvl,unsigned int next_level",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetTraits",
              params:      "std::list<std::string>",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetTraitActive",
              params:      "const std::string&,bool",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetPerks",
              params:      "std::list<std::string>",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetAvailablePerks",
              params:      "std::list<std::string> perks",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetSkillAffinity",
              params:      "const std::string& skill,bool",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetNumPerks",
              params:      "unsigned short n_perks",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_editMode",
            type:       "EditMode",
            
            attrs:      0,
            visibility: "protected"
          },
          {
            name:       "_n_perks",
            type:       "short",
            
            attrs:      8,
            visibility: "protected"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "StatController",
        file:       "include/statsheet.hpp",
        decl:       "class",
        namespaces: ["StatController"],
        methods: [
          {
              name:        "Model",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StatModel",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "Model",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "StatModel",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "GetData",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetView",
              params:      "StatView*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UpSpecial",
              params:      "const std::string& stat",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DownSpecial",
              params:      "const std::string& stat",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetSpecial",
              params:      "const std::string& stat,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetStatistic",
              params:      "const std::string& stat,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UpSkill",
              params:      "const std::string& stat",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DownSkill",
              params:      "const std::string& stat",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetSkill",
              params:      "const std::string& stat,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddExperience",
              params:      "unsigned int experience",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCurrentHp",
              params:      "short hp",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "TriggerSkillAffinity",
              params:      "const std::string& stat,bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddKill",
              params:      "const std::string& race",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RunMetabolism",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetMaxHp",
              params:      "short hp",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SpecialChanged",
              params:      "const std::string&,short",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SkillChanged",
              params:      "const std::string&,short",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StatisticChanged",
              params:      "const std::string&,short",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "PerksChanged",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "TraitToggled",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SkillAffinityToggled",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "LevelChanged",
              params:      "unsigned short",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "InformationChanged",
              params:      "const std::string&,const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AgeChanged",
              params:      "unsigned char",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "PerkAdded",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ViewStatUpped",
              params:      "const std::string&,const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ViewStatDowned",
              params:      "const std::string&,const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AcceptChanges",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CancelChanges",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "MakeBackup",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_model",
            type:       "StatModel",
            
            obj_type:   "StatModel",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_view",
            type:       "StatView",
            
            obj_type:   "StatView",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_viewObservers",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "StatViewRocket",
        file:       "include/statsheet.hpp",
        decl:       "class",
        namespaces: ["StatViewRocket","","UiBase","","StatView"],
        methods: [
          {
              name:        "SetEditMode",
              params:      "EditMode",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Hide",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Show",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetInformation",
              params:      "const std::string& name,const std::string& value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetInformation",
              params:      "const std::string& name,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetFieldValue",
              params:      "const std::string& category,const std::string& key,const std::string& value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetFieldValue",
              params:      "const std::string& category,const std::string& key,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetIdValue",
              params:      "const std::string& id,const std::string& value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetIdValue",
              params:      "const std::string& id,short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCategoryFields",
              params:      "const std::string& category,const std::vector<std::string>& keys",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetExperience",
              params:      "unsigned int,unsigned short,unsigned int",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetTraits",
              params:      "std::list<std::string>",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetTraitActive",
              params:      "const std::string&,bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetSkillAffinity",
              params:      "const std::string& skill,bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetPerks",
              params:      "std::list<std::string>",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetAvailablePerks",
              params:      "std::list<std::string>",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Cancel",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Accept",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UpdateName",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UpdateGender",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UpdateAge",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "TraitClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SpecialClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SkillClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GeneralClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StatMore",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StatLess",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StatUpdate",
              params:      "Rocket::Core::Event& event,std::string& type,std::string& stat",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_i18n",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_perks_dialog",
            type:       "PerksDialog",
            
            obj_type:   "StatViewRocket::PerksDialog",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "CancelButton",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "DoneButton",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventSpecialClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventSkillClicked",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventGeneralClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventTraitClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonUp",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonDown",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventNameChanged",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventAgeChanged",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventGenderChanged",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "SpecialSelected",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "SkillSelected",
            type:       "RocketListener         ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "TraitSelected",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_specialSelected",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_skillSelected",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_traitSelected",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
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
        name:       "StatViewRocket::PerksDialog",
        file:       "include/statsheet.hpp",
        decl:       "struct",
        namespaces: ["StatViewRocket","StatViewRocket::PerksDialog","","UiBase"],
        methods: [
          {
              name:        "SetAvailablePerks",
              params:      "std::list<std::string> perks",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetPerkDescription",
              params:      "std::string description",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ClearPerksButtons",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetSelectedPerk",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackChoosePerk",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackCancel",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackDblClickPerk",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "Cancel",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "SelectPerk",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ChoosePerk",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "DblClickPerk",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_selected_perk",
            type:       "string",
            
            attrs:      0,
            visibility: "private"
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
        name:       "CharacterBuff",
        file:       "include/level/character.hpp",
        decl:       "class",
        namespaces: ["CharacterBuff"],
        methods: [
          {
              name:        "GetName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "Begin",
              params:      "ObjectCharacter* from,TimeManager::Task* task = 0",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "End",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Load",
              params:      "Level*,ObjectCharacter*,Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Initialize",
              params:      "Level*,ObjectCharacter*,Data buff",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_buff",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_timeManager",
            type:       "TimeManager",
            
            obj_type:   "TimeManager",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_character",
            type:       "ObjectCharacter",
            
            obj_type:   "ObjectCharacter",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_duration",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_task",
            type:       "TimeManager::Task",
            
            obj_type:   "TimeManager::Task",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_module",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_begin",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_end",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_graphicalEffect",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ObjectCharacter",
        file:       "include/level/character.hpp",
        decl:       "class",
        namespaces: ["ObjectCharacter","","InstanceDynamicObject","","WaypointModifier","","AnimatedObject","","Scriptable"],
        methods: [
          {
              name:        "Load",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetStatistics",
              params:      "DataTree* stats,StatController* statsController",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "NullifyStatistics",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetGoToData",
              params:      "InstanceDynamicObject* character",
              attrs:       64,
              desc:        "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetInventory",
              params:      "Inventory* inventory",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "statistics",
              params:      "_statistics",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ProcessCollisions",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LookAt",
              params:      "LVecBase3",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LookAt",
              params:      "InstanceDynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoTo",
              params:      "unsigned int id",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoTo",
              params:      "Waypoint* waypoint",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoTo",
              params:      "InstanceDynamicObject* object,int max_distance = 0",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GoToRandomWaypoint",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "TruncatePath",
              params:      "unsigned short max_length",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetPathDistance",
              params:      "Waypoint* waypoint",
              attrs:       0,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetPathDistance",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetDistance",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetBestWaypoint",
              params:      "InstanceDynamicObject* object,bool farthest",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetNearestWaypoint",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetFarthestWaypoint",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetPathSize",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "int",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "HasLineOfSight",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsMoving",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsAlive",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsInterrupted",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetInventory",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Inventory",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetStatistics",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetStatController",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StatController",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetFactionName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "GetFaction",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "int",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "GetActionPoints",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "SetActionPoints",
              params:      "unsigned short ap",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RestartActionPoints",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetHitPoints",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetHitPoints",
              params:      "short hp",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "StatHpUpdate",
              params:      "short",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetArmorClass",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetArmorClass",
              params:      "short ac",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RestartArmorClass",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetBonusAC",
              params:      "short ac",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlayEquipedItemAnimation",
              params:      "unsigned short it,const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetEquipedItem",
              params:      "unsigned short it,InventoryObject* object,EquipedMode mode = EquipedMouth",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetEquipedItem",
              params:      "unsigned short it",
              attrs:       0,
              desc:        "",
              return_type: "InventoryObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetequipedAction",
              params:      "unsigned short it",
              attrs:       4,
              desc:        "",
              return_type: "char",
              return_attrs: 8,
              visibility:  "public"
            },{
              name:        "UnequipItem",
              params:      "unsigned short it",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ItemNextUseType",
              params:      "unsigned short it",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PushBuff",
              params:      "Data,ObjectCharacter* caster",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DelBuff",
              params:      "CharacterBuff* buff",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CheckFieldOfView",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetFaction",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetFaction",
              params:      "unsigned int flag",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetAsEnemy",
              params:      "const ObjectCharacter*,bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsEnemy",
              params:      "const ObjectCharacter*",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsAlly",
              params:      "const ObjectCharacter*",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RequestAttack",
              params:      "ObjectCharacter* attack,ObjectCharacter* from",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RequestHeal",
              params:      "ObjectCharacter* heal,ObjectCharacter* from",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RequestFollow",
              params:      "ObjectCharacter* follow,ObjectCharacter* from",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RequestStopFollowing",
              params:      "ObjectCharacter* follow,ObjectCharacter* from",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AskMorale",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SendMessage",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RunMovement",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RunMovementNext",
              params:      "float elaspedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RunDeath",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StartRunAnimation",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StopRunAnimation",
              params:      "InstanceDynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RequestCharacter",
              params:      "ObjectCharacter*,ObjectCharacter*,asIScriptFunction*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackActionUse",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "DebugPathfinding",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_obs_handler",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_goToData",
            type:       "GoToData",
            
            obj_type:   "InstanceDynamicObject::GoToData",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_statistics",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_stats",
            type:       "StatController",
            
            obj_type:   "StatController",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_faction",
            type:       "WorldDiplomacy::Faction",
            
            obj_type:   "WorldDiplomacy::Faction",
            
            attrs:      5,
            visibility: "private"
          },
          {
            name:       "_self_enemyMask",
            type:       "int",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_actionPoints",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_hitPoints",
            type:       "short",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_armorClass",
            type:       "short                          ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_tmpArmorClass",
            type:       "short",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_inventory",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_losPath",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_losTraverser",
            type:       "CollisionTraverser",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_fovTargetNp",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_fovNeedsUpdate",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_fovNp",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_fovTraverser",
            type:       "CollisionTraverser",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_scriptMain",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptFight",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptRequestAttack",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptRequestHeal",
            type:       "asIScriptFunction ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptRequestFollow",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptRequestStopFollowing",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptAskMorale",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_scriptSendMessage",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
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
        name:       "ObjectCharacter::FovEnemy",
        file:       "include/level/character.hpp",
        decl:       "struct",
        namespaces: ["ObjectCharacter","ObjectCharacter::FovEnemy"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "enemy",
            type:       "ObjectCharacter",
            
            obj_type:   "ObjectCharacter",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "ttl",
            type:       "char",
            
            attrs:      8,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ObjectCharacter::ItemEquiped",
        file:       "include/level/character.hpp",
        decl:       "struct",
        namespaces: ["ObjectCharacter","ObjectCharacter::ItemEquiped"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "actionIt",
            type:       "char",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "mode",
            type:       "EquipedMode",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "equiped",
            type:       "InventoryObject",
            
            obj_type:   "InventoryObject",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "default_",
            type:       "InventoryObject",
            
            obj_type:   "InventoryObject",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "graphics",
            type:       "InventoryObject::EquipedModel",
            
            obj_type:   "InventoryObject::EquipedModel",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "jointHorn",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "jointBattleSaddle",
            type:       "NodePath                       ",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "jointMouth",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "UiObjectQuantityPicker",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["UiObjectQuantityPicker","","UiBase"],
        methods: [
          {
              name:        "Accepted",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Increment",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GetQuantity",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "private"
            },{
              name:        "SetQuantity",
              params:      "unsigned short",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "Observer",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "EventAccepted",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventCanceled",
            type:       "RocketListener          ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventIncrement",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventDecrement",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventValueChanged",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_max_quantity",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_line_edit",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
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
        name:       "InventoryView",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["InventoryView",""],
        methods: [
          {
              name:        "UpdateView",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Destroy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetObjectFromId",
              params:      "const std::string& id",
              attrs:       0,
              desc:        "",
              return_type: "InventoryObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "ProcessEvent",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetInventory",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Inventory",
              return_attrs: 2,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_element",
            type:       "Rocket::Core::Element",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_inventory",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      2,
            visibility: "private"
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
        name:       "InventoryViewController",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["InventoryViewController"],
        methods: [
          {
              name:        "AddView",
              params:      "Rocket::Core::Element* element,Inventory& inventory",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DragObserver",
              params:      "InventoryView* container,Rocket::Core::Element* element",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Destroy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Update",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AllowDrop",
              params:      "InventoryView& from,InventoryView& to",
              attrs:       64,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "UiUseObjectOn",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["UiUseObjectOn","","UiBase"],
        methods: [
          {
              name:        "Destroy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RocketCancelClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "CancelClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_viewController",
            type:       "InventoryViewController",
            
            obj_type:   "InventoryViewController",
            
            attrs:      0,
            visibility: "private"
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
        name:       "UiBarter",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["UiBarter","","UiBase","","InventoryViewController"],
        methods: [
          {
              name:        "AllowDrop",
              params:      "InventoryView& from,InventoryView& to",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SwapObjects",
              params:      "InventoryObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "MakeDeal",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "BarterEnd",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UpdateInterface",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UpdateInterfaceSide",
              params:      "Rocket::Core::Element* e,Inventory::Content&,StatController*,StatController*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GetStackValue",
              params:      "Inventory::Content&,StatController*,StatController*",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "DropInventory",
              params:      "Inventory& from,Inventory& to",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SwapFunctor",
              params:      "InventoryObject* object,Inventory& from,Inventory& to",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "EventMakeDeal",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventBarterEnd",
            type:       "RocketListener          ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_stack_player",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_stack_other",
            type:       "Inventory               ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_stats_player",
            type:       "StatController",
            
            obj_type:   "StatController",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_stats_other",
            type:       "StatController         ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_inventory_player",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_inventory_other",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_quantity_picker",
            type:       "UiObjectQuantityPicker",
            
            obj_type:   "UiObjectQuantityPicker",
            
            attrs:      1,
            visibility: "private"
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
        name:       "UiLoot",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["UiLoot","","UiBase"],
        methods: [
          {
              name:        "Destroy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SwapObjects",
              params:      "InventoryObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RocketDoneClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RocketTakeAllClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "DoneClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "TakeAllClicked",
            type:       "RocketListener                               ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_viewController",
            type:       "InventoryViewController",
            
            obj_type:   "InventoryViewController",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_quantity_picker",
            type:       "UiObjectQuantityPicker",
            
            obj_type:   "UiObjectQuantityPicker",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_looter",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_looted",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      2,
            visibility: "private"
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
        name:       "UiEquipMode",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["UiEquipMode","","UiBase"],
        methods: [
          {
              name:        "Destroy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DisableMode",
              params:      "EquipedMode",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackButton",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackCancel",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "MouthClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "MagicClicked",
            type:       "RocketListener   ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "BattleSaddleClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "CancelClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_it",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_object",
            type:       "InventoryObject",
            
            obj_type:   "InventoryObject",
            
            attrs:      2,
            visibility: "private"
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
        name:       "UiNextZone",
        file:       "include/inventory_ui.hpp",
        decl:       "class",
        namespaces: ["UiNextZone","","UiBase"],
        methods: [
          {
              name:        "CallbackLevelSelected",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackCancel",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "LevelSelected",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "CancelSelected",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
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
        name:       "GameInventory",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["GameInventory","","UiBase"],
        methods: [
          {
              name:        "SetInventory",
              params:      "Inventory&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UpdateInventory",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UpdateInventoryCapacity",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetSelectedObject",
              params:      "InventoryObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackButtonUse",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackButtonDrop",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackButtonEquip1",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackButtonEquip2",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackButtonUnequip1",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CallbackButtonUnequip2",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "ButtonUseClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonDropClicked",
            type:       "RocketListener           ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonEquip1Clicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonEquip2Clicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonUnequip1",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonUnequip2",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_inventory",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_inventoryView",
            type:       "InventoryViewController",
            
            obj_type:   "InventoryViewController",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_selectedObject",
            type:       "InventoryObject",
            
            obj_type:   "InventoryObject",
            
            attrs:      1,
            visibility: "private"
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
        name:       "GameConsole",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["GameConsole","","UiBase"],
        methods: [
          {
              name:        "Show",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Hide",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Get",
              params:      "",
              attrs:       16,
              desc:        "",
              return_type: "GameConsole",
              return_attrs: 18,
              visibility:  "public"
            },{
              name:        "WriteOn",
              params:      "const std::string& string",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "ListFunctions",
              params:      "",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "PrintScenegraph",
              params:      "",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "PrintChildren",
              params:      "const NodePath& n,int lvl",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Toggle",
              params:      "const Event*,void* data",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "KeyUp",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Execute",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Output",
              params:      "const std::string str",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "GConsole",
            type:       "GameConsole",
            
            obj_type:   "GameConsole",
            
            attrs:      17,
            visibility: "public"
          },
          {
            name:       "ConsoleKeyUp",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "ExecuteEvent",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "_histIter",
            type:       "iterator",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_root",
            type:       "Rocket::Core::ElementDocument",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_input",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_currentLine",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_script_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_observerError",
            type:       "Sync::ObserverId",
            
            attrs:      0,
            visibility: "private"
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
        name:       "GameUi",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["GameUi"],
        methods: [
          {
              name:        "window, PT",
              params:      "RocketRegion",
              attrs:       0,
              desc:        "",
              return_type: "WindowFramework",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetContext",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "Rocket::Core::Context",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetMenu",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "GameMenu",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetInventory",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "GameInventory",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetPers",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StatViewRocket",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "OpenMenu",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "OpenInventory",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "OpenPers",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_console",
            type:       "GameConsole",
            
            obj_type:   "GameConsole",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_menu",
            type:       "GameMenu",
            
            obj_type:   "GameMenu",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_inventory",
            type:       "GameInventory",
            
            obj_type:   "GameInventory",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_pers",
            type:       "StatViewRocket",
            
            obj_type:   "StatViewRocket",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "LoadingScreen",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["LoadingScreen","","UiBase"],
        methods: [
          {
              name:        "AppendText",
              params:      "const std::string& str",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
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
        name:       "LevelUi",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["LevelUi"],
        methods: [
          {
              name:        "GetContext",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "Rocket::Core::Context",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetMainBar",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "GameMainBar",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetMenu",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "GameMenu",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetInventory",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "GameInventory",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetPers",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "StatViewRocket",
              return_attrs: 2,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_gameUi",
            type:       "GameUi",
            
            obj_type:   "GameUi",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_mainBar",
            type:       "GameMainBar",
            
            obj_type:   "GameMainBar",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_obs",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "GameOptions",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["GameOptions","","UiBase"],
        methods: [
          {
              name:        "SetLanguage",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetResolution",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetQuality",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ToggleFullscreen",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "ExitClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "LanguageSelected",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "QualitySelected",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ScreenSelected",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "FullscreenToggled",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
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
        name:       "AlertUi",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["AlertUi","","UiBase"],
        methods: [
          {
              name:        "Run",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "ButtonClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_continue",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
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
        name:       "GeneralUi",
        file:       "include/gameui.hpp",
        decl:       "class",
        namespaces: ["GeneralUi"],
        methods: [
          {
              name:        "GetOptions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "GameOptions",
              return_attrs: 2,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_console",
            type:       "GameConsole",
            
            obj_type:   "GameConsole",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_options",
            type:       "GameOptions",
            
            obj_type:   "GameOptions",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "MusicManager",
        file:       "include/musicmanager.hpp",
        decl:       "class",
        namespaces: ["MusicManager"],
        methods: [
          {
              name:        "Initialize",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Finalize",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Get",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "MusicManager",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "Play",
              params:      "const std::string& category",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Play",
              params:      "const std::string& category,const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlayNext",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetVolume",
              params:      "float volume",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FadeOut",
              params:      "float elapsed_time",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "FadeVolume",
              params:      "float elapsed_time",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_global_ptr",
            type:       "MusicManager",
            
            obj_type:   "MusicManager",
            
            attrs:      17,
            visibility: "private"
          },
          {
            name:       "_data",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_data_tree",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_current_category",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_timer",
            type:       "Timer",
            
            obj_type:   "Timer",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_fading_out",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_volume_goal",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_volume_ref",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_current_music",
            type:       "sf::Music",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_next_music",
            type:       "sf::Music            ",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ISampleInstance",
        file:       "include/soundmanager.hpp",
        decl:       "struct",
        namespaces: ["ISampleInstance"],
        methods: [
          {
              name:        "Start",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Stop",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Pause",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetVolume",
              params:      "float",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetVolume",
              params:      "void",
              attrs:       68,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsPlaying",
              params:      "void",
              attrs:       68,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddReference",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DelReference",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetReferenceCount",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "short",
              return_attrs: 8,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "ref_count",
            type:       "short",
            
            attrs:      8,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "ISample",
        file:       "include/soundmanager.hpp",
        decl:       "struct",
        namespaces: ["ISample"],
        methods: [
          {
              name:        "NewInstance",
              params:      "void",
              attrs:       64,
              desc:        "",
              return_type: "ISampleInstance",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "LoadFromFile",
              params:      "const std::string& filename",
              attrs:       64,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetFilename",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "filename",
            type:       "std::string",
            
            attrs:      0,
            visibility: "protected"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "SoundManager",
        file:       "include/soundmanager.hpp",
        decl:       "class",
        namespaces: ["SoundManager"],
        methods: [
          {
              name:        "NewSoundManager",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "SoundManager",
              return_attrs: 17,
              visibility:  "public"
            },{
              name:        "Release",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CreateInstance",
              params:      "const std::string& key",
              attrs:       0,
              desc:        "",
              return_type: "ISampleInstance",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "DeleteInstance",
              params:      "ISampleInstance*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GarbageCollect",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GarbageCollectAll",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "SetVolume",
              params:      "float",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetVolume",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Require",
              params:      "const std::string& key",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RequireSound",
              params:      "const std::string& key",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "data_audio",
              params:      "_data_audio",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_sound_managers",
            type:       "SoundManagers",
            
            obj_type:   "SoundManager::SoundManagers",
            
            attrs:      16,
            visibility: "private"
          },
          {
            name:       "_sounds",
            type:       "Sounds",
            
            obj_type:   "SoundManager::Sounds",
            
            attrs:      16,
            visibility: "private"
          },
          {
            name:       "_data_audio",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      17,
            visibility: "private"
          },
          {
            name:       "_sounds_required",
            type:       "SoundsIterators",
            
            obj_type:   "SoundManager::SoundsIterators",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_volume",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_sounds_playing",
            type:       "SoundInstances",
            
            obj_type:   "SoundManager::SoundInstances",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "SoundManager::Sound",
        file:       "include/soundmanager.hpp",
        decl:       "struct",
        namespaces: ["SoundManager","SoundManager::Sound"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "key",
            type:       "std::string",
            
            attrs:      4,
            visibility: "public"
          },
          {
            name:       "sample",
            type:       "ISample",
            
            obj_type:   "ISample",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "ref_count",
            type:       "short",
            
            attrs:      8,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "sf_SampleInstance",
        file:       "include/soundmanager.hpp",
        decl:       "class",
        namespaces: ["sf_SampleInstance","","ISampleInstance"],
        methods: [
          {
              name:        "Start",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Stop",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Pause",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetVolume",
              params:      "float volume",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetVolume",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsPlaying",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "instance",
            type:       "sf::Sound",
            
            attrs:      0,
            visibility: "private"
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
        name:       "sf_Sample",
        file:       "include/soundmanager.hpp",
        decl:       "class",
        namespaces: ["sf_Sample","","ISample"],
        methods: [
          {
              name:        "NewInstance",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "ISampleInstance",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "LoadFromFile",
              params:      "const std::string& filename",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "buffer",
            type:       "sf::SoundBuffer",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Level",
        file:       "include/level/level.hpp",
        decl:       "class",
        namespaces: ["Level"],
        methods: [
          {
              name:        "InitSun",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "InitPlayer",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetDataEngine",
              params:      "DataEngine* de",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetPlayerInventory",
              params:      "Inventory*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SaveUpdateWorld",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Load",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "InsertParty",
              params:      "PlayerParty& party",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FetchParty",
              params:      "PlayerParty& party",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "StripParty",
              params:      "PlayerParty& party",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "do_task",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "AsyncTask::DoneStatus",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetPersistent",
              params:      "bool set",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsPersistent",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetState",
              params:      "State",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetState",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "State",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetInterrupted",
              params:      "bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DisplayCombatPath",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "DestroyCombatPath",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FindPath",
              params:      "std::list<Waypoint>&,Waypoint&,Waypoint&",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetWorld",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "World",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetCamera",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "SceneCamera",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetCharacter",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "ObjectCharacter",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetCharacter",
              params:      "const DynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "ObjectCharacter",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetPlayer",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "ObjectCharacter",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "UnprocessAllCollisions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ProcessAllCollisions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FindObjectFromNode",
              params:      "NodePath node",
              attrs:       0,
              desc:        "",
              return_type: "InstanceDynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetObject",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "InstanceDynamicObject",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetTimeManager",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "TimeManager",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetDataEngine",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetItems",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ConsoleWrite",
              params:      "const std::string& str",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RemoveObject",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackExitZone",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackGoToZone",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackSelectNextZone",
              params:      "const std::vector<std::string>& zones",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackCancelSelectZone",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetNextZone",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "GetExitZone",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "SetEntryZone",
              params:      "Party&,const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackActionBarter",
              params:      "ObjectCharacter*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackActionUse",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackActionTalkTo",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackActionUseObjectOn",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackActionTargetUse",
              params:      "unsigned short it",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SelectedUseObjectOn",
              params:      "InventoryObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ActionUse",
              params:      "ObjectCharacter* user,InstanceDynamicObject* target",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ActionUseObject",
              params:      "ObjectCharacter* user,InventoryObject* object,unsigned char actionIt",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ActionUseObjectOn",
              params:      "ObjectCharacter* user,InstanceDynamicObject* target,InventoryObject* object,unsigned char actionIt",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ActionDropObject",
              params:      "ObjectCharacter* user,InventoryObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ActionUseWeaponOn",
              params:      "ObjectCharacter* user,ObjectCharacter* target,InventoryObject* object,unsigned char actionIt",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PendingActionTalkTo",
              params:      "InstanceDynamicObject* fromObject",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PendingActionUse",
              params:      "InstanceDynamicObject* fromObject",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PendingActionUseObjectOn",
              params:      "InstanceDynamicObject* fromObject",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PendingActionUseWeaponOn",
              params:      "InstanceDynamicObject* fromObject",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlayerDropObject",
              params:      "InventoryObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlayerUseObject",
              params:      "InventoryObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlayerLoot",
              params:      "Inventory*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlayerEquipObject",
              params:      "unsigned short it,InventoryObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "StartFight",
              params:      "ObjectCharacter* starter",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "StopFight",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "NextTurn",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UseActionPoints",
              params:      "unsigned short ap",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetMouseState",
              params:      "MouseState",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "MouseLeftClicked",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "MouseRightClicked",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "StartCombat",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SpawnEnemies",
              params:      "const std::string& type,unsigned short quantity,unsigned short n_spawn",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsWaypointOccupied",
              params:      "unsigned int id",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PlaySound",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "ISampleInstance",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "SetupCamera",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RunDaylight",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RunMetabolism",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "MouseInit",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ToggleCharacterOutline",
              params:      "bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "InsertDynamicObject",
              params:      "DynamicObject&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "InsertCharacter",
              params:      "ObjectCharacter*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CloseRunningUi",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "IsInsideBuilding",
              params:      "unsigned char& floor",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CheckCurrentFloor",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCurrentFloor",
              params:      "unsigned char floor",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FloorFade",
              params:      "bool in,NodePath floor",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "CurrentLevel",
            type:       "Level",
            
            obj_type:   "Level",
            
            attrs:      17,
            visibility: "public"
          },
          {
            name:       "_mouseState",
            type:       "MouseState",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "obs",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "obs_player",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_graphicWindow",
            type:       "GraphicsWindow",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_mouse",
            type:       "Mouse",
            
            obj_type:   "Mouse",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_camera",
            type:       "SceneCamera",
            
            obj_type:   "SceneCamera",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_timer",
            type:       "Timer",
            
            obj_type:   "Timer",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_timeManager",
            type:       "TimeManager",
            
            obj_type:   "TimeManager",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_state",
            type:       "State",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_persistent",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_world",
            type:       "World",
            
            obj_type:   "World",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_sound_manager",
            type:       "SoundManager",
            
            obj_type:   "SoundManager",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_objects",
            type:       "InstanceObjects",
            
            obj_type:   "Level::InstanceObjects",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_characters",
            type:       "Characters",
            
            obj_type:   "Level::Characters",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_itCharacter",
            type:       "Characters::iterator",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_currentCharacter",
            type:       "Characters::iterator",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_player_halo",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_exitZones",
            type:       "ExitZones",
            
            obj_type:   "Level::ExitZones",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_exitingZone",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_exitingZoneTo",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_exitingZoneName",
            type:       "std::string          ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_sunLightNode",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_sunLightAmbientNode",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_task_daylight",
            type:       "TimeManager::Task",
            
            obj_type:   "TimeManager::Task",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_task_metabolism",
            type:       "TimeManager::Task",
            
            obj_type:   "TimeManager::Task",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_levelUi",
            type:       "LevelUi",
            
            obj_type:   "LevelUi",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_currentRunningDialog",
            type:       "DialogController",
            
            obj_type:   "DialogController",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_currentUseObjectOn",
            type:       "UiUseObjectOn",
            
            obj_type:   "UiUseObjectOn",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_currentUiLoot",
            type:       "UiLoot",
            
            obj_type:   "UiLoot",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_mouseActionBlocked",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_dataEngine",
            type:       "DataEngine",
            
            obj_type:   "DataEngine",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_items",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_last_combat_path",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_currentFloor",
            type:       "char",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "_floor_lastWp",
            type:       "Waypoint",
            
            obj_type:   "Waypoint",
            
            attrs:      1,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Level::HidingFloor",
        file:       "include/level/level.hpp",
        decl:       "class",
        namespaces: ["Level","Level::HidingFloor"],
        methods: [
          {
              name:        "Done",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Alpha",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "float",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ForceAlpha",
              params:      "float _alpha",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetNodePath",
              params:      "NodePath np",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetFadingIn",
              params:      "bool set",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "floor",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "fadingIn",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "done",
            type:       "bool     ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "alpha",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "MouseHovering",
        file:       "include/level/mouse.hpp",
        decl:       "struct",
        namespaces: ["MouseHovering"],
        methods: [
          {
              name:        "Reset",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetWaypoint",
              params:      "NodePath np",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetDynObject",
              params:      "NodePath np",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "hasWaypoint",
            type:       "bool",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "hasDynObject",
            type:       "bool     ",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "waypoint",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "dynObject",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Mouse",
        file:       "include/level/mouse.hpp",
        decl:       "class",
        namespaces: ["Mouse"],
        methods: [
          {
              name:        "Run",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Hovering",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "MouseHovering",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "SetMouseState",
              params:      "char",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ClosestWaypoint",
              params:      "World*,short currentFloor",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CallbackButton1",
              params:      "const Event*,void* ptr",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "CallbackButton2",
              params:      "const Event*,void* ptr",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "CallbackButton3",
              params:      "const Event*,void* ptr",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_camera",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_lastMousePos",
            type:       "LPoint2f",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_pickerPath",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_collisionTraverser",
            type:       "CollisionTraverser",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_hovering",
            type:       "MouseHovering",
            
            obj_type:   "MouseHovering",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "DialogAnswers",
        file:       "include/level/dialog.hpp",
        decl:       "struct",
        namespaces: ["DialogAnswers"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "answers",
            type:       "AnswerList",
            
            obj_type:   "DialogAnswers::AnswerList",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "DialogModel",
        file:       "include/level/dialog.hpp",
        decl:       "class",
        namespaces: ["DialogModel"],
        methods: [
          {
              name:        "SetCurrentNpcLine",
              params:      "const std::string& id",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetHookAvailable",
              params:      "const std::string& answerId",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "GetExecuteMethod",
              params:      "const std::string& answerId",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "GetDefaultNextLine",
              params:      "const std::string& answerId",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "GetNpcLine",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "GetDialogAnswers",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "DialogAnswers",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_tree",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_data",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_l18n",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_currentNpcLine",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "DialogView",
        file:       "include/level/dialog.hpp",
        decl:       "class",
        namespaces: ["DialogView","","UiBase"],
        methods: [
          {
              name:        "Destroy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "UpdateView",
              params:      "const std::string& npcLine,const DialogAnswers& answers",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            },{
              name:        "CleanView",
              params:      "const DialogAnswers& answers",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "protected"
            }
        ],
        attributes: [
          
          {
            name:       "AnswerSelected",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "protected"
          },
          {
            name:       "BarterOpened",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "protected"
          },
          {
            name:       "_containerNpcLine",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_containerAnswers",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
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
        name:       "DialogController",
        file:       "include/level/dialog.hpp",
        decl:       "class",
        namespaces: ["DialogController","","DialogView","","UiBase"],
        methods: [
          {
              name:        "WithCharacter",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "ObjectCharacter",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "ExecuteAnswer",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetCurrentNode",
              params:      "const std::string& nodeName",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "OpenBarter",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_module",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_model",
            type:       "DialogModel",
            
            obj_type:   "DialogModel",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_character",
            type:       "ObjectCharacter",
            
            obj_type:   "ObjectCharacter",
            
            attrs:      1,
            visibility: "private"
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
        name:       "ObjectShelf",
        file:       "include/level/objects/shelf.hpp",
        decl:       "class",
        namespaces: ["ObjectShelf","","InstanceDynamicObject","","WaypointModifier","","AnimatedObject"],
        methods: [
          {
              name:        "LockWaypoints",
              params:      "bool",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetGoToData",
              params:      "InstanceDynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "InstanceDynamicObject::GoToData",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetInventory",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Inventory",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "CallbackActionUse",
              params:      "InstanceDynamicObject*",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_inventory",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      0,
            visibility: "private"
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
        name:       "ObjectLocker",
        file:       "include/level/objects/locker.hpp",
        decl:       "class",
        namespaces: ["ObjectLocker","","ObjectShelf","","InstanceDynamicObject","","WaypointModifier","","AnimatedObject","","Lockable"],
        methods: [
          {
              name:        "CallbackActionUse",
              params:      "InstanceDynamicObject*",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetKeyName",
              params:      "",
              attrs:       4,
              desc:        "",
              return_type: "string",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PendingActionOpen",
              params:      "InstanceDynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
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
        name:       "ObjectStair",
        file:       "include/level/objects/stair.hpp",
        decl:       "class",
        namespaces: ["ObjectStair","","InstanceDynamicObject","","WaypointModifier","","AnimatedObject","","Waypoint","Waypoint::ArcObserver"],
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
        name:       "ObjectItem",
        file:       "include/level/objects/dropped_object.hpp",
        decl:       "class",
        namespaces: ["ObjectItem","","InstanceDynamicObject","","WaypointModifier","","AnimatedObject"],
        methods: [
          {
              name:        "CallbackActionUse",
              params:      "InstanceDynamicObject* object",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ProcessCollisions",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_item",
            type:       "InventoryObject",
            
            obj_type:   "InventoryObject",
            
            attrs:      1,
            visibility: "private"
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
        name:       "SceneCamera",
        file:       "include/level/scene_camera.hpp",
        decl:       "class",
        namespaces: ["SceneCamera"],
        methods: [
          {
              name:        "Run",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetEnabledScroll",
              params:      "bool set",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SwapCameraView",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetLimits",
              params:      "unsigned int minX,unsigned int minY,unsigned int maxX,unsigned maxY",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SlideToHeight",
              params:      "float",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CenterCameraInstant",
              params:      "LPoint3f",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CenterCameraOn",
              params:      "NodePath np",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FollowNodePath",
              params:      "NodePath np",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "StopFollowingNodePath",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CenterOnObject",
              params:      "InstanceDynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FollowObject",
              params:      "InstanceDynamicObject*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetNodePath",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "NodePath",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RunScroll",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RunFollow",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "RunSlideHeight",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_graphicWindow",
            type:       "GraphicsWindow",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_camera",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_cameraPos",
            type:       "LPoint3f",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_cameraMovementSpeed",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_scrollEnabled",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_currentCameraAngle",
            type:       "char",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_currentHpr",
            type:       "LPoint3f",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_objectiveHpr",
            type:       "LPoint3f",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_minPosX",
            type:       "int",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_minPosY",
            type:       "int              ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_maxPosX",
            type:       "int",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_maxPosY",
            type:       "int",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_destHeight",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_centeringCamera",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_followingNodePath",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_toFollow",
            type:       "NodePath",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_currentPos",
            type:       "LPoint3f",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_objectivePos",
            type:       "LPoint3f",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "InteractMenu",
        file:       "include/level/interact_menu.hpp",
        decl:       "class",
        namespaces: ["InteractMenu","","UiBase"],
        methods: [
          {
              name:        "ButtonClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ButtonHovered",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "MouseButton",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ExecuteForButtonId",
              params:      "Rocket::Core::Event& event,std::function<bool (Rocket::Core::Event&,const std::string&,InstanceDynamicObject::Interaction*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_buttons",
            type:       "Buttons",
            
            obj_type:   "InteractMenu::Buttons",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_listeners",
            type:       "Listeners",
            
            obj_type:   "UiBase::Listeners",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_buttonListener",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_buttonHover",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_buttonClick",
            type:       "RocketListener               ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_obs",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_done",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
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
        name:       "WorldDiplomacy",
        file:       "include/level/diplomacy.hpp",
        decl:       "class",
        namespaces: ["WorldDiplomacy"],
        methods: [
          {
              name:        "AddFaction",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetFaction",
              params:      "const std::string& name",
              attrs:       0,
              desc:        "",
              return_type: "WorldDiplomacy::Faction",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "GetFaction",
              params:      "unsigned int flag",
              attrs:       0,
              desc:        "",
              return_type: "WorldDiplomacy::Faction",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "SetAsEnemy",
              params:      "bool set,const std::string& name1,const std::string& name2",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetAsEnemy",
              params:      "bool set,unsigned int flag1,unsigned int flag2",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Initialize",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetAsEnemy",
              params:      "bool set,Faction& first,Faction& second",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_data_engine",
            type:       "DataEngine",
            
            obj_type:   "DataEngine",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_factions",
            type:       "Factions",
            
            obj_type:   "WorldDiplomacy::Factions",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_next_flag",
            type:       "int",
            
            attrs:      8,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "WorldDiplomacy::Faction",
        file:       "include/level/diplomacy.hpp",
        decl:       "struct",
        namespaces: ["WorldDiplomacy","WorldDiplomacy::Faction"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "flag",
            type:       "int",
            
            attrs:      8,
            visibility: "public"
          },
          {
            name:       "enemyMask",
            type:       "int",
            
            attrs:      8,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Parser",
        file:       "include/shinylang.hpp",
        decl:       "class",
        namespaces: ["Parser"],
        methods: [
          {
              name:        "Run",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "DataTree",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "DataTree",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "ParseValue",
              params:      "DataBranch*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ParseString",
              params:      "DataBranch*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ParseOther",
              params:      "DataBranch*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ParseObject",
              params:      "DataBranch*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ParseArray",
              params:      "DataBranch*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_fileExist",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_source",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_str",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_it",
            type:       "int",
            
            attrs:      8,
            visibility: "private"
          },
          {
            name:       "_fileLoaded",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "WorldMap",
        file:       "include/worldmap/worldmap.hpp",
        decl:       "class",
        namespaces: ["WorldMap","","UiBase"],
        methods: [
          {
              name:        "Show",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetCityVisible",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddCity",
              params:      "const std::string&,float pos_x,float pos_y,float radius",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "HasCity",
              params:      "const std::string&",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "MapClicked",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PartyClicked",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CityClicked",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetInterrupted",
              params:      "bool set",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "MoveTowardsCoordinates",
              params:      "float x,float y",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "MoveTowardsPlace",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetDataEngine",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "DataEngine",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetCaseData",
              params:      "int x,int y",
              attrs:       4,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetCurrentPosition",
              params:      "float& x,float& y",
              attrs:       4,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "MapTileGenerator",
              params:      "Data map",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UpdatePartyCursor",
              params:      "float elapsedTime",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "UpdateClock",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "IsPartyInCity",
              params:      "std::string& ret",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GetCaseAt",
              params:      "int x,int y",
              attrs:       4,
              desc:        "",
              return_type: "Rocket::Core::Element",
              return_attrs: 1,
              visibility:  "private"
            },{
              name:        "GetCurrentCase",
              params:      "int&,int&",
              attrs:       4,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetCaseVisibility",
              params:      "int x,int y,char visibility",
              attrs:       4,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AddCityToList",
              params:      "Data",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SaveMapStatus",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "CheckRandomEncounter",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "CurrentWorldMap",
            type:       "WorldMap",
            
            obj_type:   "WorldMap",
            
            attrs:      17,
            visibility: "public"
          },
          {
            name:       "MapClickedEvent",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "PartyCursorClicked",
            type:       "RocketListener         ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "CityButtonClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonInventory",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonCharacter",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonPipbuck",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ButtonMenu",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_mapTree",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_dataEngine",
            type:       "DataEngine",
            
            obj_type:   "DataEngine",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_timeManager",
            type:       "TimeManager",
            
            obj_type:   "TimeManager",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_gameUi",
            type:       "GameUi",
            
            obj_type:   "GameUi",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_timer",
            type:       "Timer",
            
            obj_type:   "Timer",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_interrupted",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_size_x",
            type:       "int",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_size_y",
            type:       "int                    ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_tsize_x",
            type:       "int",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_tsize_y",
            type:       "int                    ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_current_pos_x",
            type:       "float",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_current_pos_y",
            type:       "float                  ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_goal_x",
            type:       "int",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_goal_y",
            type:       "int                    ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_cursor",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_cities",
            type:       "Cities",
            
            obj_type:   "WorldMap::Cities",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_cases",
            type:       "Cases",
            
            obj_type:   "WorldMap::Cases",
            
            attrs:      0,
            visibility: "private"
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
        name:       "WorldMap::City",
        file:       "include/worldmap/worldmap.hpp",
        decl:       "struct",
        namespaces: ["WorldMap","WorldMap::City"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "pos_x",
            type:       "int",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "pos_y",
            type:       "int",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "radius",
            type:       "int",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Filesystem",
        file:       "include/directory.hpp",
        decl:       "class",
        namespaces: ["Filesystem"],
        methods: [
          {
              name:        "FileContent",
              params:      "const std::string& filepath,std::string& out",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "FileCopy",
              params:      "const std::string& from,const std::string& dest",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Directory",
        file:       "include/directory.hpp",
        decl:       "class",
        namespaces: ["Directory"],
        methods: [
          {
              name:        "MakeDir",
              params:      "const std::string& str",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "RemoveDir",
              params:      "const std::string& str",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "OpenDir",
              params:      "const std::string& str",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Path",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "GetEntries",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Directory::Entries",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetEntries",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "Directory::Entries",
              return_attrs: 6,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_dirEntries",
            type:       "Entries",
            
            obj_type:   "Directory::Entries",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_dirName",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "UiLoad",
        file:       "include/saveuis.hpp",
        decl:       "class",
        namespaces: ["UiLoad","","UiBase"],
        methods: [
          {
              name:        "LoadSlotRml",
              params:      "const std::string& prefix,unsigned short slot",
              attrs:       16,
              desc:        "",
              return_type: "std::string",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "LoadSlots",
              params:      "const std::string& savepath",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "LoadGame",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ClickSlot",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Cancel",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Erase",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "EventLoadGame",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventClickSlot",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventCancel",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventErase",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_selectedSlot",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_savePath",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
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
        name:       "UiSave",
        file:       "include/saveuis.hpp",
        decl:       "class",
        namespaces: ["UiSave","","UiBase"],
        methods: [
          {
              name:        "LoadSlots",
              params:      "const std::string& savepath",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SaveGame",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ClickSlot",
              params:      "Rocket::Core::Event& event",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Cancel",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Erase",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "EventSaveGame",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventClickSlot",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventCancel",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventErase",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_selectedSlot",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_savePath",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Pipbuck",
        file:       "include/pipbuck.hpp",
        decl:       "class",
        namespaces: ["Pipbuck","","UiBase"],
        methods: [
          {
              name:        "Restart",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ReloadApps",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StartApp",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "QuitApp",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GoHome",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "DoQuitApp",
              params:      "App*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AppAskedFocus",
              params:      "App*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AppAskedUnfocus",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "AppAskedExit",
              params:      "App*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_data_engine",
            type:       "DataEngine",
            
            obj_type:   "DataEngine",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_module",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_running_apps",
            type:       "AppList",
            
            obj_type:   "Pipbuck::AppList",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_running_app",
            type:       "App",
            
            obj_type:   "Pipbuck::App",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_asked_unfocus",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_asked_focus",
            type:       "AppList",
            
            obj_type:   "Pipbuck::AppList",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_asked_exit",
            type:       "AppList",
            
            obj_type:   "Pipbuck::AppList",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventStartApp",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventQuitApp",
            type:       "RocketListener    ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventHome",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventHidePipbuck",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Pipbuck::App",
        file:       "include/pipbuck.hpp",
        decl:       "struct",
        namespaces: ["Pipbuck","Pipbuck::App"],
        methods: [
          {
              name:        "GetAppId",
              params:      "void",
              attrs:       68,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "RunAsMainTask",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RunAsBackgroundTask",
              params:      "DataEngine&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Started",
              params:      "DataEngine&",
              attrs:       64,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Exited",
              params:      "DataEngine&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unfocused",
              params:      "DataEngine&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Focused",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       64,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "PipbuckAppScript",
        file:       "include/pipbuck.hpp",
        decl:       "class",
        namespaces: ["PipbuckAppScript","","Pipbuck","Pipbuck::App"],
        methods: [
          {
              name:        "GetAppId",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "RunAsMainTask",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RunAsBackgroundTask",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Started",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Exited",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unfocused",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Focused",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_module",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_data",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
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
        name:       "PipbuckClockApp",
        file:       "include/pipbuck.hpp",
        decl:       "class",
        namespaces: ["PipbuckClockApp","","Pipbuck","Pipbuck::App"],
        methods: [
          {
              name:        "GetAppId",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "RunAsMainTask",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RunAsBackgroundTask",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Started",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Exited",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unfocused",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Focused",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "WaitChange",
              params:      "int chance",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Wait",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SelectTimeUnit",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetValue",
              params:      "Rocket::Core::Element* element,unsigned short value",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_appid",
            type:       "std::string",
            
            attrs:      4,
            visibility: "private"
          },
          {
            name:       "_time_manager",
            type:       "TimeManager",
            
            obj_type:   "TimeManager",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_inner_rml",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventWaitMore",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventWaitLess",
            type:       "RocketListener        ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventTimeUnitSelected",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventWait",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_year",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_month",
            type:       "Rocket::Core::Element ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_day",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_hour",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_minute",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_second",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_wait_days",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_wait_hours",
            type:       "Rocket::Core::Element ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_wait_minutes",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_selected_wait",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_wait_more",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_wait_less",
            type:       "Rocket::Core::Element ",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_do_wait",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_span_error",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_minutes_to_spend",
            type:       "int",
            
            attrs:      8,
            visibility: "private"
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
        name:       "PipbuckQuestApp",
        file:       "include/pipbuck.hpp",
        decl:       "class",
        namespaces: ["PipbuckQuestApp","","Pipbuck","Pipbuck::App"],
        methods: [
          {
              name:        "GetAppId",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 4,
              visibility:  "public"
            },{
              name:        "RunAsMainTask",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "RunAsBackgroundTask",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Started",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Exited",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Unfocused",
              params:      "DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Focused",
              params:      "Rocket::Core::Element*,DataEngine&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadQuestList",
              params:      "Rocket::Core::Element*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "LoadQuestView",
              params:      "Rocket::Core::Element*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ListQuestHovered",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ListQuestClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "EventQuestHovered",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventQuestClicked",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventBackClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_appid",
            type:       "std::string",
            
            attrs:      4,
            visibility: "private"
          },
          {
            name:       "_rml_index",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_rml_view",
            type:       "std::string       ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_data_engine",
            type:       "DataEngine",
            
            obj_type:   "DataEngine",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_current_view",
            type:       "Interfaces",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_current_quest",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_last_hovered",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_root",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_observer",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "private"
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
        name:       "Buff",
        file:       "include/gametask.hpp",
        decl:       "class",
        namespaces: ["Buff"],
        methods: [
          {
              name:        "GetTargetName",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "std::string",
              return_attrs: 6,
              visibility:  "public"
            },{
              name:        "GetStatistics",
              params:      "void",
              attrs:       4,
              desc:        "",
              return_type: "StatController",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "SetStatistics",
              params:      "StatController* v",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Refresh",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Save",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "InitScripts",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_context",
            type:       "asIScriptContext",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_module",
            type:       "asIScriptModule",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_refresh",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_buff",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_target_name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_target_stats",
            type:       "StatController",
            
            obj_type:   "StatController",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_tm",
            type:       "TimeManager",
            
            obj_type:   "TimeManager",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_task",
            type:       "TimeManager::Task",
            
            obj_type:   "TimeManager::Task",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_looping",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "BuffManager",
        file:       "include/gametask.hpp",
        decl:       "struct",
        namespaces: ["BuffManager"],
        methods: [
          {
              name:        "Save",
              params:      "Utils::Packet&,std::function<bool            (const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Load",
              params:      "Utils::Packet&,std::function<StatController* (const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CollectGarbage",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Cleanup",
              params:      "Buff*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "buffs",
            type:       "Buffs",
            
            obj_type:   "BuffManager::Buffs",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "tm",
            type:       "TimeManager",
            
            obj_type:   "TimeManager",
            
            attrs:      2,
            visibility: "public"
          },
          {
            name:       "garbage",
            type:       "Buffs",
            
            obj_type:   "BuffManager::Buffs",
            
            attrs:      0,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "GameTask",
        file:       "include/gametask.hpp",
        decl:       "class",
        namespaces: ["GameTask"],
        methods: [
          {
              name:        "MapOpenLevel",
              params:      "std::string name",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetLevel",
              params:      "Level* level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "do_task",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "AsyncTask::DoneStatus",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SaveGame",
              params:      "const std::string& savepath",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadGame",
              params:      "const std::string& savepath",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "OpenLevel",
              params:      "const std::string& savepath,const std::string& level,const std::string& entry_zone = ##",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "ExitLevel",
              params:      "const std::string& savepath",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CopySave",
              params:      "const std::string& savepath,const std::string& slotPath",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SaveToSlot",
              params:      "unsigned char slot",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadSlot",
              params:      "unsigned char slot",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadLastState",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Exit",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetLevelSpecialEncounter",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetLevelEncounter",
              params:      "const std::string& type,short n_creeps",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PushBuff",
              params:      "ObjectCharacter* character,Data buff",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PushBuff",
              params:      "const std::string& name,Data buff",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SaveLevelBuffs",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SavePartyBuffs",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadLevelBuffs",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "LoadPartyBuffs",
              params:      "Utils::Packet&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetTimeManager",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "TimeManager",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetDiplomacy",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "WorldDiplomacy",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetQuestManager",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "QuestManager",
              return_attrs: 2,
              visibility:  "public"
            },{
              name:        "GetPlayerParty",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Party",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "PlaySound",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "ISampleInstance",
              return_attrs: 1,
              visibility:  "public"
            },{
              name:        "FinishLoad",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "LoadClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SaveClicked",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SaveLevel",
              params:      "Level* level,const std::string& name",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "private"
            },{
              name:        "LoadLevel",
              params:      "WindowFramework* window,GameUi& gameUi,const std::string& path,const std::string& name,const std::string& entry_zone = ##,bool isSaveFile = false",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "DoLoadLevel",
              params:      "LoadLevelParams",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GameOver",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "DoCheckRandomEncounter",
              params:      "int x,int y",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "SetPlayerInventory",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "EraseSlot",
              params:      "unsigned char slot",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "CurrentGameTask",
            type:       "GameTask",
            
            obj_type:   "GameTask",
            
            attrs:      17,
            visibility: "public",
            doc:         {"name":"CurrentGameTask","desc":"Static accessor to the current running GameTask, if one exists. Among other things, it is what the \"game\" global variable points to in the scripting context"}
          },
          {
            name:       "_signals",
            type:       "Sync::Signals",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"_signals","desc":"Handle for registered observers"}
          },
          {
            name:       "_continue",
            type:       "bool",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"_continue","desc":"The game will stop at its next iteration if this variable is set to false"}
          },
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_window","desc":"Panda3D's WindowFramework instance"}
          },
          {
            name:       "_sound_manager",
            type:       "SoundManager",
            
            obj_type:   "SoundManager",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"SoundManager"}
          },
          {
            name:       "_gameUi",
            type:       "GameUi",
            
            obj_type:   "GameUi",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"GameUi"}
          },
          {
            name:       "_dataEngine",
            type:       "DataEngine",
            
            obj_type:   "DataEngine",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"DataEngine"}
          },
          {
            name:       "_timeManager",
            type:       "TimeManager",
            
            obj_type:   "TimeManager",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"TimeManager"}
          },
          {
            name:       "_buff_manager",
            type:       "BuffManager",
            
            obj_type:   "BuffManager",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"BuffManager"}
          },
          {
            name:       "_pipbuck",
            type:       "Pipbuck",
            
            obj_type:   "Pipbuck",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"Pipbuck"}
          },
          {
            name:       "_charSheet",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_charSheet","desc":"Pointer to the statistic sheet configuration file from the current player."}
          },
          {
            name:       "_playerParty",
            type:       "PlayerParty",
            
            obj_type:   "PlayerParty",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_playerParty"}
          },
          {
            name:       "_playerStats",
            type:       "StatController",
            
            obj_type:   "StatController",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_playerStats"}
          },
          {
            name:       "_playerInventory",
            type:       "Inventory",
            
            obj_type:   "Inventory",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_playerInventory"}
          },
          {
            name:       "_quest_manager",
            type:       "QuestManager",
            
            obj_type:   "QuestManager",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_quest_manager"}
          },
          {
            name:       "_worldMap",
            type:       "WorldMap",
            
            obj_type:   "WorldMap",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_worldMap"}
          },
          {
            name:       "_levelName",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"_levelName"}
          },
          {
            name:       "_level",
            type:       "Level",
            
            obj_type:   "Level",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_level"}
          },
          {
            name:       "_savePath",
            type:       "std::string",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"_savePath","desc":"Stores the savepath value from [[OptionsManager]]."}
          },
          {
            name:       "_uiSaveGame",
            type:       "UiSave",
            
            obj_type:   "UiSave",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_uiSaveGame"}
          },
          {
            name:       "_uiLoadGame",
            type:       "UiLoad",
            
            obj_type:   "UiLoad",
            
            attrs:      1,
            visibility: "private",
            doc:         {"name":"_uiLoadGame"}
          },
          {
            name:       "obs_level_unpersistent",
            type:       "Sync::ObserverId",
            
            attrs:      0,
            visibility: "private",
            doc:         {"name":"obs_level_unpersistent","desc":"Used in special encounters to handle the observers spawning creeps after the unpersistent level has been loaded."}
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "GameTask::LoadLevelParams",
        file:       "include/gametask.hpp",
        decl:       "struct",
        namespaces: ["GameTask","GameTask::LoadLevelParams"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "name",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "path",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "isSaveFile",
            type:       "bool",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "entry_zone",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "MouseCursor",
        file:       "include/mousecursor.hpp",
        decl:       "class",
        namespaces: ["MouseCursor","","UiBase"],
        methods: [
          {
              name:        "SetCursorTexture",
              params:      "const std::string& texture",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Update",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PullToFront",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Get",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "MouseCursor",
              return_attrs: 17,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_cursor",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_static",
            type:       "MouseCursor",
            
            obj_type:   "MouseCursor",
            
            attrs:      17,
            visibility: "private"
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
        name:       "OptionsManager",
        file:       "include/options.hpp",
        decl:       "class",
        namespaces: ["OptionsManager"],
        methods: [
          {
              name:        "Initialize",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Finalize",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Get",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "Data",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Refresh",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_data",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      17,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "UiNewGame",
        file:       "include/newgametask.hpp",
        decl:       "class",
        namespaces: ["UiNewGame","","UiBase"],
        methods: [
          {
              name:        "SelectedProfile",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "ClickedStartFromScratch",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GoToPreviousProfile",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GoToNextProfile",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GoToProfile",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "GoCancel",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "PreviousProfile",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "NextProfile",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventSelectProfile",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventStartFromScratch",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "EventCancel",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_data_profiles",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_current_profile",
            type:       "const_iterator",
            
            attrs:      0,
            visibility: "private"
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
        name:       "NewGameTask",
        file:       "include/newgametask.hpp",
        decl:       "class",
        namespaces: ["NewGameTask"],
        methods: [
          {
              name:        "SelectProfile",
              params:      "const std::string&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "StartFromScratch",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "Done",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_rocket",
            type:       "Rocket::Core::Context",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_ui_new_game",
            type:       "UiNewGame",
            
            obj_type:   "UiNewGame",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_stat_view",
            type:       "StatViewRocket",
            
            obj_type:   "StatViewRocket",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_stat_controller",
            type:       "StatController",
            
            obj_type:   "StatController",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_stat_sheet",
            type:       "DataTree",
            
            obj_type:   "DataTree",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "MainMenu",
        file:       "include/mainmenu.hpp",
        decl:       "class",
        namespaces: ["MainMenu",""],
        methods: [
          {
              name:        "Continue",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "EndGame",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "NewGame",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "StartGame",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CancelNewGame",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "do_task",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "DoneStatus",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AsyncCreateLevel",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "QuitGame",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "OpenUiLoad",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "LoadSlot",
              params:      "unsigned char slot",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "DisplayAlerts",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_window",
            type:       "WindowFramework",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_generalUi",
            type:       "GeneralUi",
            
            obj_type:   "GeneralUi",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_mouseCursor",
            type:       "MouseCursor",
            
            obj_type:   "MouseCursor",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_new_game_task",
            type:       "NewGameTask",
            
            obj_type:   "NewGameTask",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_levelTask",
            type:       "GameTask",
            
            obj_type:   "GameTask",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_view",
            type:       "View",
            
            obj_type:   "MainMenu::View",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_alerts",
            type:       "Alerts",
            
            obj_type:   "MainMenu::Alerts",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_uiLoad",
            type:       "UiLoad",
            
            obj_type:   "UiLoad",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_need_garbage_collect",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "quitGamePlz",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "createLevelPlz",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "slotToLoadPlz",
            type:       "short",
            
            attrs:      0,
            visibility: "private"
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
        name:       "MainMenu::View",
        file:       "include/mainmenu.hpp",
        decl:       "struct",
        namespaces: ["MainMenu","MainMenu::View","","UiBase"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "NewGameClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "QuitClicked",
            type:       "RocketListener ",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "LoadGameClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "OptionsClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "ContinueClicked",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "private"
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
        name:       "AstarPathfinding",
        file:       "include/astar.hpp",
        decl:       "class",
        namespaces: ["AstarPathfinding"],
        methods: [
          {
              name:        "CancelSearch",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SetStartAndGoalStates",
              params:      "UserState &Start,UserState &Goal",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SearchSuccedded",
              params:      "Node* node",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "SearchStep",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "State",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FreeSolutionNodes",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetStepCount",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "int",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "FreeAllNodes",
              params:      "",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "FreeUnusedNodes",
              params:      "bool onlyUnused = true",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "PedirNode",
              params:      "void",
              attrs:       32,
              desc:        "",
              return_type: "AstarPathfinding::Node",
              return_attrs: 1,
              visibility:  "private"
            },{
              name:        "ReleaseNode",
              params:      "Node* node",
              attrs:       32,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "NodeListIterator",
            type:       "iterator",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "_openList",
            type:       "NodeList",
            
            obj_type:   "AstarPathfinding::NodeList",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_closedList",
            type:       "NodeList",
            
            obj_type:   "AstarPathfinding::NodeList",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_state",
            type:       "State",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_cancelRequest",
            type:       "bool",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_nSteps",
            type:       "int",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_start",
            type:       "Node",
            
            obj_type:   "AstarPathfinding::Node",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_goal",
            type:       "Node",
            
            obj_type:   "AstarPathfinding::Node",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_allocateNodeIt",
            type:       "int",
            
            attrs:      8,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "AstarPathfinding::Node",
        file:       "include/astar.hpp",
        decl:       "struct",
        namespaces: ["AstarPathfinding","AstarPathfinding::Node"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "parent",
            type:       "Node",
            
            obj_type:   "AstarPathfinding::Node",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "child",
            type:       "Node",
            
            obj_type:   "AstarPathfinding::Node",
            
            attrs:      1,
            visibility: "public"
          },
          {
            name:       "g",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "h",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "f",
            type:       "float",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "userNode",
            type:       "UserState",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "AstarPathfinding::FunctorCompareNode",
        file:       "include/astar.hpp",
        decl:       "struct",
        namespaces: ["AstarPathfinding","AstarPathfinding::FunctorCompareNode"],
        methods: [
          {
              name:        "operator",
              params:      "",
              attrs:       4,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Quest",
        file:       "include/quest_manager.hpp",
        decl:       "class",
        namespaces: ["Quest","","Scriptable"],
        methods: [
          {
              name:        "Initialize",
              params:      "Level* level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Finalize",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "GetData",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "Data",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CompleteCondition",
              params:      "const std::string& objective,const std::string& condition",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsConditionCompleted",
              params:      "const std::string& objective,const std::string& condition",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "IsOver",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "CheckIfCompleted",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "bool",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "InitializeCondition",
              params:      "Data condition,Level* level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "WatcherCharacterInventory",
              params:      "Data condition,Level* level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "WatcherCharacterKill",
              params:      "Data condition,Level* level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            },{
              name:        "WatcherTime",
              params:      "Data condition,Level* level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "data",
            type:       "Data",
            
            obj_type:   "Data",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "_observers",
            type:       "Sync::ObserverHandler",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_update_hook",
            type:       "asIScriptFunction",
            
            attrs:      1,
            visibility: "private"
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
        name:       "Quest::WatcherInitializer",
        file:       "include/quest_manager.hpp",
        decl:       "struct",
        namespaces: ["Quest","Quest::WatcherInitializer"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "key",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "method",
            type:       "WatcherInitializerMethod",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "QuestManager",
        file:       "include/quest_manager.hpp",
        decl:       "class",
        namespaces: ["QuestManager"],
        methods: [
          {
              name:        "AddQuest",
              params:      "Data",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Initialize",
              params:      "Level* level",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "Finalize",
              params:      "void",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "QuestCompleted",
              params:      "Quest*",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "it",
            type:       "Quests::const_iterator",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "end",
            type:       "Quests::const_iterator",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "_data_engine",
            type:       "DataEngine",
            
            obj_type:   "DataEngine",
            
            attrs:      2,
            visibility: "private"
          },
          {
            name:       "_quests",
            type:       "Quests",
            
            obj_type:   "QuestManager::Quests",
            
            attrs:      0,
            visibility: "private"
          },
          {
            name:       "_stats_controller",
            type:       "StatController",
            
            obj_type:   "StatController",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_level",
            type:       "Level",
            
            obj_type:   "Level",
            
            attrs:      1,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Dices",
        file:       "include/dices.hpp",
        decl:       "struct",
        namespaces: ["Dices"],
        methods: [
          {
              name:        "Initialize",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Throw",
              params:      "unsigned int max",
              attrs:       16,
              desc:        "",
              return_type: "int",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Test",
              params:      "int successPercentage",
              attrs:       16,
              desc:        "",
              return_type: "bool",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Executor",
        file:       "include/executor.hpp",
        decl:       "class",
        namespaces: ["Executor"],
        methods: [
          {
              name:        "ExecuteLater",
              params:      "std::function<void (void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            },{
              name:        "Run",
              params:      "void",
              attrs:       16,
              desc:        "",
              return_type: "void",
              return_attrs: 16,
              visibility:  "public"
            }
        ],
        attributes: [
          
          {
            name:       "_lambdas",
            type:       "Lambdas",
            
            obj_type:   "Executor::Lambdas",
            
            attrs:      16,
            visibility: "private"
          }
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "Parser",
        file:       "include/json.hpp",
        decl:       "class",
        namespaces: ["Parser"],
        methods: [
          
        ],
        attributes: [
          
        ],
        ancestors: [
          
        ]
      },
      {
        name:       "UiDialog",
        file:       "include/ui_dialog.hpp",
        decl:       "class",
        namespaces: ["UiDialog","","UiBase"],
        methods: [
          {
              name:        "SetMessage",
              params:      "const std::string& message",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "AddChoice",
              params:      "const std::string& name,std::function<void (Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "public"
            },{
              name:        "PickedChoice",
              params:      "Rocket::Core::Event&",
              attrs:       0,
              desc:        "",
              return_type: "void",
              return_attrs: 0,
              visibility:  "private"
            }
        ],
        attributes: [
          
          {
            name:       "_button_container",
            type:       "Rocket::Core::Element",
            
            attrs:      1,
            visibility: "private"
          },
          {
            name:       "_buttons",
            type:       "Buttons",
            
            obj_type:   "UiDialog::Buttons",
            
            attrs:      0,
            visibility: "private"
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
        name:       "UiDialog::Button",
        file:       "include/ui_dialog.hpp",
        decl:       "struct",
        namespaces: ["UiDialog","UiDialog::Button"],
        methods: [
          
        ],
        attributes: [
          
          {
            name:       "id",
            type:       "std::string",
            
            attrs:      0,
            visibility: "public"
          },
          {
            name:       "listener",
            type:       "RocketListener",
            
            obj_type:   "RocketListener",
            
            attrs:      0,
            visibility: "public"
          }
        ],
        ancestors: [
          
        ]
      },
  ]
//};
,
  typedefs: [{ name: "Waypoint::Arcs", to: "std::list<Arc>" },
{ name: "Waypoint::ArcsWithdrawed", to: "std::vector<std::pair<Arc, unsigned short> >" },
{ name: "EntryZone", to: "Zone" },
{ name: "World::Waypoints", to: "std::vector<Waypoint>   " },
{ name: "World::MapObjects", to: "std::list<MapObject>    " },
{ name: "World::DynamicObjects", to: "std::list<DynamicObject>" },
{ name: "World::WorldLights", to: "std::list<WorldLight>   " },
{ name: "World::ExitZones", to: "std::list<ExitZone>     " },
{ name: "World::EntryZones", to: "std::list<EntryZone>    " },
{ name: "World::Floors", to: "std::vector<NodePath>   " },
{ name: "Party::DynamicObjects", to: "std::list<DynamicObject*>" },
{ name: "PlayerParty::DynamicObjects", to: "std::list<DynamicObject*>" },
{ name: "Semaphore::Handle", to: "sem_t          " },
{ name: "Semaphore::ThreadId", to: "std::thread::id" },
{ name: "Semaphore::Handle", to: "HANDLE" },
{ name: "Semaphore::ThreadId", to: "DWORD " },
{ name: "ObserverId", to: "void*" },
{ name: "Signal::FunctionObserver::Function", to: "std::function<void (void)>" },
{ name: "Signal::Observer::(ObserverClass::*Method)()", to: "void" },
{ name: "Signal::Observers", to: "std::list<InterfaceObserver*>" },
{ name: "Signal::RecordedCalls", to: "std::queue<RecordedCall>" },
{ name: "ObserverHandler::Observers", to: "std::list<IObserverPair*>" },
{ name: "TimeManager::Tasks", to: "std::list<Task*>" },
{ name: "DataBranch::Children", to: "std::list<DataBranch*>" },
{ name: "Data::Children", to: "DataBranch::Children" },
{ name: "AnimatedObject::MapAnims", to: "std::map<std::string, AnimControl*>" },
{ name: "WaypointModifier::WithdrawedArcs", to: "std::list<WithdrawedArc>       " },
{ name: "InstanceDynamicObject::InteractionList", to: "std::list<Interaction>" },
{ name: "ModuleManager::Modules", to: "std::list<LoadedModule>" },
{ name: "UiBase::Listeners", to: "std::list<Listener>" },
{ name: "InventoryObject::ActionsHooks", to: "std::vector<ActionHooks>" },
{ name: "Inventory::Content", to: "std::list<InventoryObject*>" },
{ name: "Scriptable::ScriptFuncPtr", to: "std::pair<asIScriptFunction**, std::string>" },
{ name: "Scriptable::ScriptFuncPtrs", to: "std::list<ScriptFuncPtr>                   " },
{ name: "LoadingScreen::StringQueue", to: "std::queue<std::string>" },
{ name: "SoundManager::Sounds", to: "std::list<Sound>           " },
{ name: "SoundManager::SoundsIterators", to: "std::list<Sounds::iterator>" },
{ name: "SoundManager::SoundInstances", to: "std::list<ISampleInstance*>" },
{ name: "SoundManager::SoundManagers", to: "std::list<SoundManager*>   " },
{ name: "Level::InstanceObjects", to: "std::list<InstanceDynamicObject*>" },
{ name: "Level::Characters", to: "std::list<ObjectCharacter*>      " },
{ name: "Level::ExitZones", to: "std::list<LevelExitZone*>        " },
{ name: "DialogAnswers::KeyValue", to: "std::pair<std::string, std::string>" },
{ name: "DialogAnswers::AnswerList", to: "std::list<KeyValue>                " },
{ name: "InteractMenu::Listeners", to: "std::vector<InstanceDynamicObject::Interaction*>" },
{ name: "InteractMenu::Buttons", to: "std::vector<Rocket::Core::Element*>             " },
{ name: "WorldDiplomacy::Factions", to: "std::list<Faction>" },
{ name: "WorldMap::Cities", to: "std::vector<City>                  " },
{ name: "WorldMap::Cases", to: "std::vector<Rocket::Core::Element*>" },
{ name: "Dirent", to: "struct dirent" },
{ name: "Directory::Entries", to: "std::list<Dirent>" },
{ name: "Pipbuck::AppList", to: "std::list<App*>" },
{ name: "BuffManager::Buffs", to: "std::list<Buff*>" },
{ name: "MainMenu::Alerts", to: "std::list<AlertUi*>" },
{ name: "AstarPathfinding::NodeList", to: "std::vector<Node*>                   " },
{ name: "AstarPathfinding::NodeListIterator", to: "typename std::vector<Node*>::iterator" },
{ name: "Quest::Level*)", to: "void (Quest::*WatcherInitializerMethod)(Data," },
{ name: "QuestManager::Quests", to: "std::list<Quest*>" },
{ name: "Executor::Lambdas", to: "std::queue<std::function<void (void)> >" },
{ name: "UiDialog::Buttons", to: "std::list<Button>" }]
};