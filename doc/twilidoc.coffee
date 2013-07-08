ATTR_PTR      = 1
ATTR_REF      = 2
ATTR_CONST    = 4
ATTR_UNSIGNED = 8
ATTR_STATIC   = 16
ATTR_INLINE   = 32
ATTR_VIRTUAL  = 64
ATTR_TYPEDEF  = 128

escape_html = (text) ->
  result = text
  result = result.replace /</g, '&lt;'
  result = result.replace />/g, '&gt;'

class String
  constructor: (str) ->
    @string = str

  Substr: (i, end) ->
    end    = if end? then end + i else @string.length
    result = ''
    while i < @string.length and i < end
      result += @string[i]
      i++
    result

  Split: (sep) ->
    i         = 0
    parts     = []
    last_part = 0
    while i + sep.length < @string.length
      to_comp = @Substr i, sep.length
      if to_comp == sep
        parts.push (@Substr last_part, i - last_part)
        last_part = i + sep.length
        i         = last_part
        continue
      i++
    if last_part != i
      parts.push (@Substr last_part, @string.length)
    parts

window.String = String

Array::Join = (sep) ->
  string = ''
  string += item for item in @
  string

Array::Includes = (item) ->
  (@indexOf item) != -1

class Project
  #public
  GetTypedef: (name, parent) ->
    candidates = CandidatesFromType name, parent
    for typedef in project.typedefs
      if candidates.Includes? typedef.name
        return typedef
    null

  GetType: (name, parent) ->
    name = name.string if name.string?
    if (name.match /</)?
      name = new String name
      name = (name.Split '<')[0]
    candidates = CandidatesFromType name, parent
    if candidates.length > 0
      console.log candidates
      for type in project.types
        continue unless type.name?
        if candidates.Includes type.name
          return type
    null

  #private
  CandidatesFromType = (name, parent) ->
    if parent? and name?
      parent     = new String parent
      parts      = parent.Split '::'
      candidates = []
      for part in parts
        merge    = (candidates.Join '::') + part
        candidates.push merge
      i          = 0
      candidates[i++] += '::' + name while i < candidates.length
      candidates.push name
      candidates
    else if name?
      [ name ]
    else
      []

class View
  AfterFilter: () ->
    @RefreshVisibility()
    @InitRoutables()
    (@elem.find '[data-rel="popover"]').popover { trigger: 'hover' }

  Display: () ->
    $("#page-content").fadeOut =>
      $("#page-content").empty()
      $("#page-content").append @elem
      $("#page-content").fadeIn => @AfterFilter()
      window.current_view.Destroy() if window.current_view?
      window.current_view = @
      
  RefreshVisibility: () ->
    public_elems      = @elem.find '.visibility-public'
    console.log public_elems
    protected_elems   = @elem.find '.visibility-protected'
    private_elems     = @elem.find '.visibility-private'
    if window.vision_handle.DisplayPublic()
      public_elems.show()
    else
      public_elems.hide()
    if window.vision_handle.DisplayProtected()
      protected_elems.show()
    else
      protected_elems.hide()
    if window.vision_handle.DisplayPrivate()
      private_elems.show()
    else
      private_elems.hide()
      
  Destroy: () ->
    $(".popover").fadeOut -> $(@).remove()

  InitRoutables: () ->
    routables = @elem.find '[data-route]'
    routables.each ->
      $(@).click ->
        location.hash = $(@).attr 'data-route'

class Widget
  Begin: (title, icon, controls) ->
    "<div class='row-fluid sortable'>
        <div class='box span12'>
          <div class='box-header well' data-original-title>
            <h2>" + (if icon? then "<i class='" + icon + "'></i>" else '') + " #{title}</h2>
            <div class='box-icon'>
            " + (if controls? then controls else '') + "
            </div>
          </div>
          <div class='box-content'>"
          
  End: () ->
    "</div></div></div>"

class Attribute
  TypeBox: (name, attrs, type, is_typedef, name_only) ->
    html  = ''
    html += "<p class='btn-group'>" unless name_only == true
    html +=   "<button class='btn btn-mini btn-info'>ptr</button>" if (attrs & ATTR_PTR)
    html +=   "<button class='btn btn-mini btn-info'>ref</button>" if (attrs & ATTR_REF)
    html +=   "<button class='btn btn-mini'>const</button>"        if (attrs & ATTR_CONST)
    html +=   "<button class='btn btn-mini'>unsigned</button>"     if (attrs & ATTR_UNSIGNED)
    name        = (name.replace /</g, '[').replace />/g,']'
    if type? or is_typedef == true
      klass     = if is_typedef == true then 'btn-warning' else 'btn-success'
      html     += "<button class='btn btn-mini #{klass}'"
      html     += " data-route='#show-class-#{type.name}'" unless (is_typedef and type == null)
      # Popover
      desc      = if type == null or !(type.doc?) then 'Undocumented type' else type.doc.overview.replace "'", "&#39;"
      #classname = (classname.replace /</g, '&lt;').replace />/g, '&gt;'
      classname = if type? then type.name else name
      classname = (classname.replace /</g, '[').replace />/g,']'
      html     += "data-rel='popover' data-content='#{desc}' title='#{classname}'"
      # End popover
      html += ">#{name}</button>"
    else
      html     += "<button class='btn btn-mini btn-inverse'>#{name}</button>"
    html       += "</p>" unless name_only == true
    html
    
  Attribufy: (html, classname) ->
    html.replace /\[(((const|unsigned)\s*)*([a-z0-9_]+(::)?)*)\]/ig, (match, content, offset, s) ->
      attrs = 0
      while (content.match /^(const|unsigned)\s/)?
        if (content.substring 0, 5) == 'const'
          content = content.substring 6, content.length
          attrs  |= ATTR_CONST
        else if (content.substring 0, 8) == 'unsigned'
          content = content.substring 9, content.length
          attrs  |= ATTR_UNSIGNED
        else
          break
      type = Project::GetType content, classname
      if type?
        html = Attribute::TypeBox content, attrs, type, false, true
        html = html.replace '<button', '<a'
        html = html.replace 'button>', 'a>'
        html
      else
        match

##
## Single Element View
##
class Member extends View
  constructor: (classname, type, element) ->
    icon = ''
    icon = 'icon-cog'      if type == 'method'
    icon = 'icon-asterisk' if type == 'attribute'
    html = Widget::Begin "#{classname}::#{element.name}", icon
    if type == 'method'
      html += @RenderMethod element, classname
    else if type == 'attribute'
      html += @RenderAttribute element, classname
    else
      alert "Unimplemented type #{type} for Member View"
      throw "Unimplemented type #{type} for Member View"
    html += Widget::End()
    @elem = $(html)

  AfterFilter: () ->
    super
    sh_highlightDocument()
  
  RenderMethod: (method, classname) ->
    html  = ''
    html += "<div class='span12' style='margin-top: 10px;'><div>"
    html += "<div class='method-descriptor'>"
    html += "<span class='span3'>"

    if method.return_type? # Is not a constructor/destructor
      name            = new String method.return_type
      template_parts  = (name.Split '<')
      if template_parts.length > 1
        name          = template_parts[0]
      console.log "Classname is: #{classname}!"
      return_type = Project::GetType name, classname
      #return_type = get_project_type method.return_type, classname
      if not return_type?

        typedef       = Project::GetTypedef name, classname
        _typedef      = typedef
        while (not return_type?) and (_typedef?)
          return_type = Project::GetType    _typedef.to, classname
          if not return_type?
            _typedef  = Project::GetTypedef _typedef.to, classname

        if typedef?
          if return_type?
            html += Attribute::TypeBox typedef.name, method.return_attrs, return_type, true
          else
            html += Attribute::TypeBox typedef.name, method.return_attrs, { name: typedef.to }, true
        else
          html += Attribute::TypeBox method.return_type, method.return_attrs
      else if return_type?
        html += Attribute::TypeBox method.return_type, method.return_attrs, return_type
    else # Is a constructor/destructor
      if method.name[0] != '~'
        html += '<span class="label label-info"><i class="icon-cogs"></i> Constructor</span>'
      else
        html += '<span class="label label-info"><i class="icon-trash"></i> Destructor</span>'

    visibility = {
      class:   '<span class="label label-warning"><i class="icon-tag"></i> Class</span>',
      object:  '<span class="label label-primary"><i class="icon-tag"></i> Object</span>',
      virtual: '<span class="label label-inverse"><i class="icon-random"></i> Virtual</span>'
    }
    visibility = if method.attrs & ATTR_STATIC then visibility.class else visibility.object
    if method.attrs & ATTR_CONST
      visibility = '<span class="label label-info"><i class="icon-ban-circle"></i> Const</span> ' + visibility
    if method.attrs & ATTR_VIRTUAL
      visibility = '<span class="label label-inverse"><i class="icon-random"></i> Virtual</span> ' + visibility

    params = method.params
    params = params.replace /^\(\s*(((const|unsigned)\s*)*([a-z0-9_<>]+(::)?)*)/ig, (match, content, offset, s) ->
      match    = match.replace '(', '['
      match   += ']'
      '(' + match
    params = params.replace /,\s*(((const|unsigned)\s*)*([a-z0-9_<>]+(::)?)*)(\s*|,|)/ig, (match, content, offset, s) ->
      ", [#{content}]"
    params = Attribute::Attribufy params, classname

    html += "</span>"
    html += "<div class='span9'>"
    html +=   "<span class='span3'><h4 class='method-name'>" + method.name + "</h4></span>"
    html +=   "<span class='span6'><div class='box box-params'>" + params + "</div></span>"
    html +=   "<span class='span3'><div style='float:right;'>#{visibility}</div></span>"
    html += "</div>"
    html += @RenderDoc method if method.doc?
    html += "</div>"
    html += "</div>"
    html += "</div>"
    html  = html.replace '<pre>', '<pre class="sh_cpp">'
    html

  RenderAttribute: (attribute, classname) ->
    html  = ''
    obj_type = Project::GetType attribute.type, classname
    #obj_type = window.get_project_type attribute.type if attribute.type?
    html += "<div class='span12' style='margin-top: 10px;'><div>"
    html += "<div class='attribute-descriptor'>"
    html += "<span class='span3'>"
    if not obj_type?
      typedef        = Project::GetTypedef attribute.type, classname
      _typedef       = typedef
      while (not obj_type?) and (_typedef?)
        obj_type   = Project::GetType       _typedef.to, classname
        if not obj_type?
          _typedef = Project::GetTypedef _typedef.to, classname
      if typedef?
        if obj_type?
          html += Attribute::TypeBox typedef.name, attribute.attrs, obj_type, true
        else
          html += Attribute::TypeBox typedef.name, attribute.attrs, { name: typedef.to }, true
      else
        html += Attribute::TypeBox attribute.type, attribute.attrs
    else
      html += Attribute::TypeBox attribute.type, attribute.attrs, obj_type
    html += "</span>"
    html += "<span class='span6'>"
    html += "<h4>" + attribute.name + "</h4>"
    html += @RenderDoc attribute if attribute.doc?
    html += "</span>"
    visibility = {
      class:  '<span class="label label-warning"><i class="icon-tag"></i> Class</span>',
      object: '<span class="label label-primary"><i class="icon-tag"></i> Object</span>'
    }
    visibility = if attribute.attrs & ATTR_STATIC then visibility.class else visibility.object
    html += "<span class='span3'><div style='float:right;'>#{visibility}</div></span>"
    html += "</div>"
    html += "</div></div>"
    html
    
  RenderDoc: (attribute) ->
    has_doc   = false
    doc       = ''
    doc      += '<div class="well foldable" style="padding: 0 0 0 0; margin: 0 0 0 0; padding-left: 5px;"><dl>'
    if attribute.doc.short? and attribute.doc.short != ''
      doc    += "<dt>Overview</dt><dd>#{attribute.doc.short}</dd>"
      has_doc = true
    if attribute.doc.desc? and attribute.doc.desc != ''
      doc    += "<dt>Details</dt><dd>#{attribute.doc.desc}</dd>"   
      has_doc = true
    doc      += '</dl></div>'
    doc       = doc.replace '<pre>', '<pre class="sh_cpp">'
    doc       = Attribute::Attribufy doc
    if has_doc then doc else ''

##
## Namespace Widget
##
class Namespace extends View
  constructor: (namespace) ->
    @view_type = 'namespace'
    types      = []
    for type in project.types
      continue unless type.name?
      if type.name == namespace
        @namespace = type
        continue
      regexp   = new RegExp "^#{namespace}::"
      if (type.name.match regexp)?
       types.push type
    unless @namespace?
      alert "Namespace #{namespace} not found."
      throw "Namespace #{namespace} not found."

    html      = Widget::Begin "#{@namespace.decl} #{@namespace.name}", 'icon-list-alt'
    for member in types
      url     = "#go-to-class-#{member.name}"
      html   += "<div class='span2>#{member.decl}</div>"
      html   += "<div class='span10'> <button class='btn btn-success btn-mini' data-route='#{url}'>#{member.name}</button></div>"
      if member.doc? and member.doc.overview?
        html += '<div class="well foldable" style="padding: 0 0 0 0; margin: 0 0 0 0; padding-left: 5px;">'
        html += member.doc.overview
        html += '</div>'
      html   += '<br/>'
    html     += Widget::End()
    html      = Attribute::Attribufy html, @namespace.name
    @elem     = $(html)
 
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
        html += "<div class='visibility-#{method.visibility}'>"
        html += Member::RenderMethod method, classname
        html += "</div>" # Visibility
      html += Widget::End()

    if @type.attributes.length > 0
      html += Widget::Begin "Attributes", "icon-asterisk"
      html += "<div class='span12'></div>"
      for attribute in @type.attributes
        html += "<div class='visibility-#{attribute.visibility}'>"
        html += Member::RenderAttribute attribute, classname
        html += "</div>" # visibility
        obj_type = null
      html += Widget::End()

    $.ajax {
        url:      "docs/#{classname}.html",
        dataType: 'html',
        async:     false,
        success: (data) ->
          html += Widget::Begin "Documentation", "icon-file"
          html += Attribute::Attribufy data
          html += Widget::End()
      }

    @elem = $(html)
    
  AfterFilter: () ->
    super
    sh_highlightDocument()
    window.uml.generate_hierarchy 'uml', @type.name

##
## Class List Widget
##
class ClassList extends View
  constructor: (object_types) ->
    object_types = [ 'class', 'struct' ] unless object_types?
    title = if object_types.Includes 'class' then 'Class Index' else 'Namespace Index'
    html  = Widget::Begin title, 'icon-list'
    html += "<table class='table table-striped table-bordered bootstrap-datatable datatable dataTable'>"
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
      continue unless object_types.Includes type.decl
      html += "<tr class='#{i % 2 == 0 ? 'even' : 'odd'}'>"
      html +=   "<td>#{type.file}</td>"
      html +=   "<td>#{type.name}</td>"
      html +=   "<td>"
      if type.doc? and type.doc.overview != ''
        html += if type.doc.overview.length > 40
          type.doc.overview[0..40] + "..."
        else
          type.doc.overview
      html +=   "</td>"
      html +=   "<td>"
      html +=     "<a class='btn btn-success' href='#show-#{object_types[0]}-#{type.name}'>"
      html +=       "<i class='icon-zoom-in icon-white' /> View"
      html +=     "</a>"
      if type.doc? and type.doc.overview != ''
        html += "<button style='margin-left: 2px;' class='btn btn-primary' data-rel='popover' data-content='#{type.doc.overview}' title='#{type.name}'><i class='icon-eye-open'></i> Overview</button>"
      else
        html += "<button style='margin-left: 2px;' class='btn btn-primary disabled'><i class='icon-eye-open'></i> Overview</button>"
      html +=   "</td>"
      html += "</tr>"
    html += "</tbody>"
    html += "</table>"
    html += Widget::End()
    @elem  = $(html)
    @table = @elem.find 'table'

  AfterFilter: () ->
    update_popovers = =>
      (@elem.find '[data-rel="popover"]').popover { trigger: 'click', placement: 'left' }
    @table.dataTable().fnDestroy()
    @table.dataTable({
      "sDom": "<'row-fluid'<'span6'l><'span6'f>r>t<'row-fluid'<'span12'i><'span12 center'p>>",
      "sPaginationType": "bootstrap",
      "oLanguage": { "sLengthMenu": "_MENU_ records per page" },
      "fnDrawCallback": update_popovers })

class Homepage extends View
  constructor: () ->
    html  = Widget::Begin "#{project.name}"
    html += project.desc.homepage
    html += Widget::End()
    html  = Attribute::Attribufy html
    @elem = $(html)
    
  AfterFilter: () ->
    super
    sh_highlightDocument()

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
    @menu     = menu
    disp_name = desc.name
    title     = ''
    if disp_name.length >= 21
      disp_name = disp_name[0..16] + '...' 
      title     = " title='#{desc.name}' data-rel='tooltip'"
    html      = "<li id='#{id}'>"
    html     += "<a class='ajax-link' href='#{desc.url}' #{title}><i class='#{icon}'></i>"
    html     += "<span class='hidden-tablet'> #{disp_name}</span>"
    html     += '</a></li>'
    @elem     = $(html)
    @menu.results.append @elem
    #elem.click => @Clicked()

class Menu
  constructor: () ->
    @dom     = $('#main-menu')
    @search  = @dom.find '#menu-search'
    @search  = $("#main-menu").find("#menu-search")
    @results = $("#main-menu").find("#menu-results")
    @entries = []
    @search.keyup () => @SearchUpdate()

  MaxElems: () ->
    # body height - top bar - menu header - a little margin
    height = $(window).height() - 45 - (74 - 18) - 200
    entry  = 18 * 2
    (height / entry) - 1

  SearchUpdate: () ->
    regex = new RegExp (@search.attr 'value')
    @ClearResults()
    return if (@search.attr 'value') == ''
    res_count = 0
    max_count = @MaxElems()
    for type in project.types
      if type.name.match regex
        @AddMenuEntry type.name, { name: type.name, url: "#show-class-#{type.name}", type: 'class' }
        res_count++
      break if res_count > max_count
      for method in type.methods
        if method.name.match regex
          @AddMenuEntry method.name, { name: "#{type.name}::#{method.name}", url: "#show-class-#{type.name}-method-#{method.name}", type: 'method' }
          res_count++
      break if res_count > max_count
      for attribute in type.attributes
        if attribute.name.match regex
          @AddMenuEntry attribute.name, { name: "#{type.name}::#{attribute.name}", url: "#show-class-#{type.name}-attribute-#{attribute.name}", type: 'attribute' }
          res_count++
      break if res_count > max_count
    if res_count > max_count
      @AddMenuEntry 'more-results', { name: 'More results...', url: '#more-results', type: 'search' }
    (@dom.find '[data-rel="tooltip"]').tooltip { placement: 'right' }

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
    @loading_handle = $("#loading-indicator")

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

  PrefabNamespaces: () ->
    @Clear().Add('Project').Add('Namespaces', '#namespaces-index')

  PrefabNamespace: (namespace) ->
    @PrefabNamespaces().Add('Namespace').Add(namespace, "#show-namespace-#{namespace}")

  PrefabClasses: () ->
    @Clear().Add('Project').Add('Class List', '#class-index')
    
  PrefabClass: (class_name) ->
    @PrefabClasses().Add('Class').Add(class_name, "#show-class-#{class_name}")
    
  SetLoading: (set) ->
    if set
      @loading_handle.fadeIn()
    else
      @loading_handle.fadeOut()

class Foldables
  constructor: () ->
    @control = { fold_all: $('#fold-all'), unfold_all: $('#unfold-all') }
    @control.fold_all.click   => @SetVisible false
    @control.unfold_all.click => @SetVisible true

  SetVisible: (set_visible) ->
    if set_visible
      $('.foldable').slideDown()
    else
      $('.foldable').slideUp()

class VisibilityHandle
  PUBLIC    = 1
  PROTECTED = 2
  PRIVATE   = 4

  constructor: (default_val) ->
    @base             = $('#visibility-filter')
    @flag             = 0
    @toggle_public    = @base.find '.visibility-public'
    @toggle_protected = @base.find '.visibility-protected'
    @toggle_private   = @base.find '.visibility-private'
    @toggle_public.click    => @DisplayPublic    !(@DisplayPublic())
    @toggle_protected.click => @DisplayProtected !(@DisplayProtected())
    @toggle_private.click   => @DisplayPrivate   !(@DisplayPrivate())
    @toggle_public.click    => @Refresh()
    @toggle_protected.click => @Refresh()
    @toggle_private.click   => @Refresh()
    @toggle_public.click()    if (default_val & PUBLIC)    != 0
    @toggle_protected.click() if (default_val & PROTECTED) != 0
    @toggle_private.click()   if (default_val & PRIVATE)   != 0

  DisplayPublic:    (param) ->
    @FlagGetSet PUBLIC,    param

  DisplayProtected: (param) ->
    @FlagGetSet PROTECTED, param

  DisplayPrivate:   (param) ->
    @FlagGetSet PRIVATE,   param
    
  FlagGetSet: (value, param) ->
    if param?
      if param == true and (@flag & value) == 0
        @flag += value
      else if param == false and (@flag & value) != 0
        @flag -= value
    @flag & value
    
  Refresh: () ->
    if window.current_view?
      window.current_view.RefreshVisibility()

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
        window.breadcrumb.SetLoading true
        route.callback @anchor
        window.breadcrumb.SetLoading false
        return
    alert 'Not Found'

window.project_chart = ->
  meth_count   = 0
  meth_counted = 0
  data         = []

  for type in project.types
    meth_count += type.methods.length
  for type in project.types
    if type.methods.length > meth_count / 75
      data.push { label: type.name, data: type.methods.length }

  $.plot $("#piechart"), data, {
      series: { pie: { show: true } },
      grid:   { hoverable: true, clickable: true },
      legend: { show: false }
    }

  $("#piechart").bind("plothover", pieHover);

$(document).ready ->
  console.log '[Twilidoc] Initializing'
  window.menu          = new Menu()
  window.breadcrumb    = new Breadcrumb()
  window.anchor_handle = new AnchorHandle()
  window.vision_handle = new VisibilityHandle(1)
  window.foldables     = new Foldables()

  window.anchor_handle.AddRoute /^$/, ->
    console.log 'Nothing'
    
  window.anchor_handle.AddRoute /#project/, ->
    unless window.homepage?
      window.homepage = new Homepage()
    window.homepage.Display()
    window.breadcrumb.Clear().Add('Project')

  window.anchor_handle.AddRoute /#class-index/, ->
    console.log 'anchor handler executed'
    unless window.class_list?
      window.class_list = new ClassList()
    window.class_list.Display()
    window.breadcrumb.PrefabClasses()

  window.anchor_handle.AddRoute /#namespaces-index/, ->
    unless window.namespaces_list?
      window.namespaces_list = new ClassList [ 'namespace' ]
    window.namespaces_list.Display()
    window.breadcrumb.PrefabNamespaces()

  window.anchor_handle.AddRoute /#show-namespace-[^-]+$/, ->
    matches = /#show-namespace-([^-]+)$/.exec window.anchor_handle.anchor
    namespace_view = new Namespace matches[1]
    namespace_view.Display()
    window.breadcrumb.PrefabNamespace(matches[1])

  window.anchor_handle.AddRoute /#go-to-class-[^-]+$/, ->
    matches = /#go-to-class-([^-]+)$/.exec window.anchor_handle.anchor
    class_view = new Class matches[1]
    class_view.Display()
    window.breadcrumb.Add('Class')
    window.breadcrumb.Add(matches[1], "show-class-#{matches[1]}")

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
      alert 'Opening a method in an already opened class is not implement yet :('
    else
      window.breadcrumb.PrefabClass(matches[1]).Add('Methods').Add(matches[2], "#show-class-#{matches[1]}-method-#{matches[2]}")
      obj_type = window.get_project_type klass
      obj_meth = null
      for item in obj_type.methods
        if item.name == method
          obj_meth = item
          break
      view     = new Member klass, 'method', obj_meth
      view.Display()

  window.anchor_handle.AddRoute /#show-class-[^-]+-attribute-[^-]+$/, ->
    matches = /#show-class-([^-]+)-attribute-([^-]+)$/.exec window.anchor_handle.anchor
    klass   = matches[1]
    attr    = matches[2]
    if window.current_view? and window.current_view.view_type == 'class' and window.current_view.type.name == klass
      alert 'Opening an attribute in an already opened class is not implement yet :('
    else
      window.breadcrumb.PrefabClass(matches[1]).Add('Attributes').Add(matches[2], "#show-class-#{matches[1]}-attribute-#{matches[2]}")
      obj_type = window.get_project_type klass
      obj_attr = null
      for item in obj_type.attributes
        if item.name == attr
          obj_attr = item
          break
      view     = new Member klass, 'attribute', obj_attr
      view.Display()

  window.anchor_handle.anchor = '#project'
  window.anchor_handle.Execute()
  console.log '[Twilidoc] Finished initializing'
