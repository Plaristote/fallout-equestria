#ifndef  PANDAGUI_HPP
# define PANDAGUI_HPP

# include <panda3d/texture.h>
# include <panda3d/windowFramework.h>
# include <panda3d/cardMaker.h>
# include <panda3d/texturePool.h>

# include <iostream>
# include <panda3d/meshDrawer2D.h>

namespace PandaUi
{
  class Widget
  {
  public:
    typedef std::list<Widget*> Widgets;

    Widget(WindowFramework* window, const std::string& name) : _cardMaker(name), _window(window)
    {
      _parent = 0;
      _mesh.set_budget(1000);
      _np     = _mesh.get_root();
      //_np     = NodePath(_cardMaker.generate());
      _np.reparent_to(window->get_pixel_2d());
    }

    Widget(Widget* parent, const std::string& name) : _cardMaker(name), _parent(parent)
    {
      _window = _parent->_window;
      _mesh.set_budget(1000);
      _np     = _mesh.get_root();
      //_np     = NodePath(_cardMaker.generate());
      _np.reparent_to(_window->get_pixel_2d());
    }

    void             SetBackground(const std::string& source)
    {
      _background = TexturePool::load_texture(source);
      _background->set_magfilter(Texture::FT_nearest);
      _background->set_minfilter(Texture::FT_nearest);
      _np.set_texture(_background);
      SetSize(LVector2f(_background->get_orig_file_x_size(), _background->get_orig_file_y_size()));
    }

    void             SetPosition(LVector2f position)
    {
      LVector2f absolute = position;

      if (_parent)
        absolute += _parent->GetAbsolutePosition();
      _np.set_pos(absolute.get_x(), 0, -(absolute.get_y() + _size.get_y()));
      _position = position;
    }

    LVector2f        GetAbsolutePosition(void) const
    {
      if (_parent)
        return (_parent->GetAbsolutePosition() + _position);
      return (_position);
    }

    LVector2f        GetPosition(void) const
    {
      return (_position);
    }

    void             SetSize(LVector2f size)
    {
      _np.set_scale(size.get_x(), 0, size.get_y());
      _size = size;
    }

    LVector2f        GetSize(void) const
    {
      return (_size);
    }

  private:
    MeshDrawer2D     _mesh;
    CardMaker        _cardMaker;
    WindowFramework* _window;
    Widget*          _parent;
    Widgets          _children;
    NodePath         _np;
    PT(Texture)      _background;
    LVector2f        _position;
    LVector2f        _size;
  };
}

#endif