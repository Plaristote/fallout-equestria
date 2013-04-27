$(document).ready ->
  uml        = Joint.dia.uml
  uml_height = 0

  visibility_string2uml = (obj) ->
    switch obj.visibility
      when 'public'    then '+'
      when 'protected' then '#'
      when 'private'   then '-'
      else                  '?'

  get_project_type = (name) ->
    for type in project.types
      if type.name == name
        return (type);
    null

  generate_uml = (type, visibility, start_pos) ->
    color      = 'yellow'
    methods    = []
    attributes = []
    object     = null
    default_pos= { x: 0, y: 0 }
    position   = unless start_pos? then default_pos else start_pos
    width      = 100
    height     = 50

    if      visibility == 'protected'
      color = 'lightgreen'
    else if visibility == 'private'
      color = 'lightblue'
    for method in type.methods
      str  = visibility_string2uml method
      str += ' ' + method.name + '(' + method.params + ') -> ' + method.return_type
      methods.push str
      # Adjust size of the rect to the content
      if str.length * 5 > width
        width = 10 + str.length * 5
      height += 13
    for attribute in type.attributes
      str = visibility_string2uml attribute
      str += ' ' + attribute.name + ' -> ' + attribute.type
      attributes.push str
      if (str.length * 5 > width)
        width = 10 + str.length * 5
      height += 13
    object = uml.Class.create {
        label:       type.name,
        shadow:      true,
        attrs:       { fill: color },
        labelAttrs:  { 'font-weight': 'bold' },
        methods:     methods,
        attributes:  attributes,
        rect:        { x: position.x, y: position.y, width: width, height: height },
        interactive: false
      }
    object.draggable false

    if (position.y + height + 10 > uml_height)
      uml_height = position.y + height + 10

    position.x += width + 50
    tmp         = position.x
    for ancestor in type.ancestors
      ancestor_type = get_project_type(ancestor.name)
      ancestor_pos  = position
      if ancestor_type != null
        tmp2            = position.y
        inherits_object = generate_uml ancestor_type, ancestor.visibility, ancestor_pos
        position.y      = tmp2
        object.joint inherits_object, uml.generalizationArrow
        console.log(inherits_object);
        position.x  = tmp
        position.y += inherits_object.getBBox().height + 10
    return object

  GenerateHierarchyUml = (id, classname) ->
    joint  = Joint.paper(id, 1, 1);
    uml_height = 0;
    klass      = get_project_type classname
    if (klass != null)
      generate_uml(klass, 'public');
    else
      alert("UML: Couldn't find type '" + classname + "'");
    joint.setSize($('#' + id).parent().width(), uml_height);

  window.get_project_type       = get_project_type
  window.uml                    = {}
  window.uml.generate_hierarchy = GenerateHierarchyUml
