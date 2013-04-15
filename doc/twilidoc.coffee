ATTR_PTR      = 1
ATTR_REF      = 2
ATTR_CONST    = 4
ATTR_UNSIGNED = 8
ATTR_STATIC   = 16
ATTR_INLINE   = 32
ATTR_VIRTUAL  = 64

class View
  Display: () ->
    $("#page-content").empty()
    $("#page-content").append @elem
    window.current_view = @

class Widget
  Begin: (title, icon) ->
    "<div class='row-fluid sortable'>
        <div class='box span12'>
          <div class='box-header well' data-original-title>
            <h2>" + (if icon? then "<i class='" + icon + "'></i>" else '') + " #{title}</h2>
            <div class='box-icon'>
            </div>
          </div>
          <div class='box-content'>"
          
  End: () ->
    "</div></div></div>"

class Attribute
  TypeBox: (name, attrs, classname) ->
    html  = ''
    html += "<p class='btn-group'>"
    html +=   "<button class='btn btn-mini btn-info'>ptr</button>" if (attrs & ATTR_PTR)
    html +=   "<button class='btn btn-mini btn-info'>ref</button>" if (attrs & ATTR_REF)
    html +=   "<button class='btn btn-mini'>const</button>"        if (attrs & ATTR_CONST)
    html +=   "<button class='btn btn-mini'>unsigned</button>"     if (attrs & ATTR_UNSIGNED)
    if classname?
      html += "<button class='btn btn-mini btn-success' onclick='location.hash=\"#show-class-#{classname}\"'>#{name}</button>"
    else
      html += "<button class='btn btn-mini btn-inverse'>#{name}</button>"
    html += "</p>"
    html
##
## Class Widget
##
class Class extends View
  constructor: (classname) ->
    @view_type = 'class'
    for type in project.types
      if type.name == classname
        @type = type
        break
    unless @type?
      alert "Type #{classname} not found."
      throw "Type #{classname} not found."

    html  = Widget::Begin "#{type.decl} #{type.name}", 'icon-list-alt'
    html += "<div id='uml'></div>"
    html += Widget::End()
    
    if @type.methods.length > 0
      html += Widget::Begin "Methods", "icon-cog"
      html += "<div class='span12'></div>"
      for method in @type.methods
        obj_type = get_project_type method.return_type
        html += "<div class='span12' style='margin-top: 10px;'><div>"
        html += "<div class='method-descriptor'>"
        html += "<span class='span2'>"
        if obj_type?
          html += Attribute::TypeBox method.return_type, method.return_attrs, obj_type.name
        else
          html += Attribute::TypeBox method.return_type, method.return_attrs
        html += "</span>"
        html += "<span class='span2'><h4>" + method.name + "</h4></span>"
        html += "</div>"
        html += "</div>"
        html += "</div>"
      html += Widget::End()

    if @type.attributes.length > 0
      html += Widget::Begin "Attributes", "icon-asterisk"
      html += "<div class='span12'></div>"
      for attribute in @type.attributes
        obj_type = get_project_type attribute.obj_type if attribute.obj_type?
        html += "<div class='span12' style='margin-top: 10px;'><div>"
        html += "<div class='attribute-descriptor'>"
        html += "<span class='span2'>"
        if obj_type?
          html += Attribute::TypeBox attribute.type, attribute.attrs, obj_type.name
        else
          html += Attribute::TypeBox attribute.type, attribute.attrs
        html += "</span>"
        html += "<span class='span2'>"
        html += "<h4>" + attribute.name + "</h4>"
        html += "</span>"
        html += "</div>"
        html += "</div></div>"
        obj_type = null
      html += Widget::End()
    @elem = $(html)
    
  Display: () ->
    super
    window.uml.generate_hierarchy 'uml', @type.name

##
## Class List Widget
##
class ClassList extends View
  constructor: () ->
    html  = Widget::Begin "Class Index (#{project.types.length} classes)", 'icon-list'
    html += "<table class='table table-striped table-bordered bootstrap-datatable datatable dataTable' id='DataTables_Table_0' aria-describedby='DataTables_Table_0_info'>"
    html += "<thead>"
    html +=   "<tr role='row'>"
    html +=     "<th>File</th>"
    html +=     "<th>Name</th>"
    html +=     "<th>Overview</th>"
    html +=     "<th>Actions</th>"
    html +=   "</tr>"
    html += "</thead>"
    html += "<tbody role='alert' aria-live='polite' aria-relevant='all'>"
    i     = 0
    for type in project.types
      html += "<tr class='#{i % 2 == 0 ? 'even' : 'odd'}'>"
      html +=   "<td></td>"
      html +=   "<td>#{type.name}</td>"
      html +=   "<td></td>"
      html +=   "<td>"
      html +=     "<a class='btn btn-success' href='#show-class-#{type.name}'>"
      html +=       "<i class='icon-zoom-in icon-white' /> View"
      html +=     "</a>"
      html +=   "</td>"
      html += "</tr>"
    html += "</tbody>"
    html += "</table>"
    html += Widget::End()
    @elem = $(html)
    (@elem.find 'table').dataTable({
      "sDom": "<'row-fluid'<'span6'l><'span6'f>r>t<'row-fluid'<'span12'i><'span12 center'p>>",
      "sPaginationType": "bootstrap",
      "oLanguage": { "sLengthMenu": "_MENU_ records per page" } })

##
## Menu
##
class MenuEntry
  constructor: (menu, id, desc) ->
    icon     = if desc.type == 'search'
      'icon-eye-open'
    else if desc.type == 'class'
      'icon-list-alt'
    else if desc.type == 'method'
      'icon-cog'
    else if desc.type == 'attribute'
      'icon-asterisk'
    @menu    = menu
    html     = "<li id='#{id}'>"
    html    += "<a class='ajax-link' href='#{desc.url}'><i class='#{icon}'></i>"
    html    += "<span class='hidden-tablet'> #{desc.name}</span>"
    html    += '</a></li>'
    @elem    = $(html)
    @menu.results.append @elem
    @elem.click => @Clicked()

class Menu
  constructor: () ->
    @dom     = $('#main-menu')
    @search  = @dom.find '#menu-search'
    @search  = $("#main-menu").find("#menu-search")
    @results = $("#main-menu").find("#menu-results")
    @entries = []
    @search.keyup () => @SearchUpdate()

  SearchUpdate: () ->
    regex = new RegExp (@search.attr 'value')
    @ClearResults()
    return if (@search.attr 'value') == ''
    res_count = 0
    max_count = 10
    for type in project.types
      if type.name.match regex
        @AddMenuEntry type.name, { name: type.name, url: "#show-class-#{type.name}", type: 'class' }
        res_count++
      break if res_count > max_count
      for method in type.methods
        if method.name.match regex
          @AddMenuEntry method.name, { name: method.name, url: "#show-class-#{type.name}-method-#{method.name}", type: 'method' }
          res_count++
      break if res_count > max_count
      for attribute in type.attributes
        if attribute.name.match regex
          @AddMenuEntry attribute.name, { name: attribute.name, url: "#show-class-#{type.name}-attribute-#{attribute.name}", type: 'attribute' }
          res_count++
      break if res_count > max_count
    if res_count > max_count
      @AddMenuEntry 'more-results', { name: 'More results...', url: '#more-results', type: 'search' }

  AddMenuEntry: (id, desc) ->
    entry = new MenuEntry this, id, desc
    @entries.push entry
    
  ClearResults: () ->
    @results.empty()
    @entries = []

##
## Breadcrumb
##
class Breadcrumb
  constructor: () ->
    console.log 'Breadcrumb'
    @dom = $('#breadcrumb')

  Clear: () ->
    @dom.empty()
    this

  Add: (name, url) ->
    (@dom.find 'li').last().append "<span class='divider'>/</span>"
    html = "<li>"
    html += "<a href='#{url}'>" if url?
    html += name
    html += '</a>'              if url?
    html += "</li>"
    @dom.append html
    this
    
  PrefabClasses: () ->
    @Clear().Add('Project').Add('Class List', '#class-index')
    
  PrefabClass: (class_name) ->
    @PrefabClasses().Add('Class').Add(class_name, "#show-class-#{class_name}")

class AnchorHandle
  constructor: () ->
    callback = => @Refresh()
    setInterval callback, 50
    @anchor  = location.hash
    @routes  = []

  AddRoute: (regex, callback) ->
    @routes.push { exp: regex, callback: callback }
    
  Refresh: () ->
    if @anchor != location.hash
      @anchor = location.hash
      @Execute()

  Execute: () ->
    console.log 'Execute anchor path matching'
    for route in @routes
      if @anchor.match route.exp
        console.log 'Found matching route'
        route.callback @anchor
        return
    alert 'Not Found'

$(document).ready ->
  console.log '[Twilidoc] Initializing'
  window.menu          = new Menu()
  window.breadcrumb    = new Breadcrumb()
  window.anchor_handle = new AnchorHandle()

  window.anchor_handle.AddRoute /^$/, ->
    console.log 'Index'

  window.anchor_handle.AddRoute /#class-index/, ->
    console.log 'anchor handler executed'
    #unless window.class_list?
    window.class_list = new ClassList()
    window.class_list.Display()
    window.breadcrumb.PrefabClasses()

  window.anchor_handle.AddRoute /#show-class-[^-]+$/, ->
    matches = /#show-class-([^-]+)$/.exec window.anchor_handle.anchor
    class_view = new Class matches[1]
    class_view.Display()
    window.breadcrumb.PrefabClass(matches[1])

  window.anchor_handle.AddRoute /#show-class-[^-]+-method-[^-]+$/, ->
    matches = /#show-class-([^-]+)-method-([^-]+)$/.exec window.anchor_handle.anchor
    klass   = matches[1]
    method  = matches[2]
    if window.current_view? and window.current_view.view_type == 'class' and window.current_view.type.name == klass
      alert 'We\'re already in the good class !'
    else
      window.breadcrumb.PrefabClass(matches[1]).Add('Methods').Add(matches[2], "#show-class-#{matches[1]}-method-#{matches[2]}")

  window.anchor_handle.AddRoute /#show-class-[^-]+-attribute-[^-]+$/, ->
    matches = /#show-class-([^-]+)-attribute-([^-]+)$/.exec window.anchor_handle.anchor
    klass   = matches[1]
    attr    = matches[2]
    if window.current_view? and window.current_view.view_type == 'class' and window.current_view.type.name == klass
      alert 'We\'re already in the good class !'
    else
      window.breadcrumb.PrefabClass(matches[1]).Add('Attributes').Add(matches[2], "#show-class-#{matches[1]}-attribute-#{matches[2]}")

  window.anchor_handle.Execute()
  console.log '[Twilidoc] Finished initializing'
