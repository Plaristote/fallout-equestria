var uml        = Joint.dia.uml;
var uml_height = 0;

function get_project_type(name)
{
  for (var i = 0 ; i < project.types.length ; i++)
  {
    var type = project.types[i];

    if (type.name == name)
      return (type);
  }
  return (null);
}

function generate_uml(type, visibility, start_pos)
{
  var color      = 'yellow';
  var methods    = [];
  var attributes = [];
  var object;
  var position   = typeof start_pos == 'undefined' ? { x: 0, y: 0 } : start_pos;
  var width      = 100;
  var height     = 50;

  if      (visibility == 'protected') { color = 'lightblue'; }
  else if (visibility == 'private')   { color = 'blue';      }

  for (var i = 0 ; i < type.methods.length ; i++)
  {
    var method = type.methods[i];
    var str;

    str = method.name + '(' + method.params + ')';
    methods.push(str);
    // Adjust size of the rect to the content
    if (str.length * 5 > width)
      width = 10 + str.length * 5;
    height += 13;
  }

  for (var i = 0 ; i < type.attributes.length ; i++)
  {
    var attribute = type.attributes[i];
    var str;

    str = attribute.name;
    attributes.push(str);
    if (str.length * 5 > width)
      width = 10 + str.length * 5;
    height += 13;
  }

  object = uml.Class.create({
    label: type.name,
    shadow: true,
    attrs: { fill: color },
    labelAttrs: { 'font-weight': 'bold' },
    methods: methods,
    attributes: attributes,
    rect: { x: position.x, y: position.y, width: width, height: height },
    interactive: false
  });

  if (position.y + height + 10 > uml_height)
    uml_height = position.y + height + 10;

  position.x += width + 50;
  var tmp  = position.x;
  var tmp2 = position.y;
  for (var i = 0 ; i < type.ancestors.length ; i++)
  {
    var ancestor      = type.ancestors[i];
    var ancestor_type = get_project_type(ancestor.name);
    var ancestor_pos  = position;

    if (ancestor_type != null)
    {
      var tmp2 = position.y;
      inherits_object   = generate_uml(ancestor_type, ancestor.visibility, ancestor_pos);
      position.y = tmp2;
      object.joint(inherits_object, uml.generalizationArrow);
      console.log(inherits_object);
      position.x  = tmp;
      position.y += inherits_object.getBBox().height + 10;
    }
  }
  return (object);
}

function GenerateHierarchyUml(id, classname)
{
  var joint  = Joint.paper(id, 1, 1);

  uml_height = 0;
  klass      = get_project_type(classname);
  if (klass != null)
    generate_uml(klass, 'public');
  else
    alert("UML: Couldn't find type '" + classname + "'");
  joint.setSize($('#' + id).parent().width(), uml_height);
}

window.uml.generate_hierarchy = GenerateHierarchyUml;