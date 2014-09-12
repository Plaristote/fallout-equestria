#include "level/characters/visibility.hpp"
#include "level/characters/field_of_view.hpp"

using namespace std;

ObjectVisibility::ObjectVisibility(WindowFramework* window) : AnimatedObject(window)
{
  _fading_in = _fading_off = false;
}

void ObjectVisibility::Initialize()
{
  base_color = GetNodePath().get_color();
  base_color.set_w(GetDynamicObject()->base_color.get_w());
  GetNodePath().set_transparency(TransparencyAttrib::M_alpha);
  GetNodePath().set_color(base_color);
}

void ObjectVisibility::SetVisible(bool do_set)
{
  LColor color = GetNodePath().get_color();

  if (do_set == true && (color.get_w() < base_color.get_w() || _fading_off == true))
  {
    _fading_off = false;
    _fading_in  = true;
  }
  else if (do_set == false && (color.get_w() > 0 || _fading_in == true))
  {
    _fading_off = true;
    _fading_in  = false;
  }
}

bool ObjectVisibility::IsHidden(void) const
{
  return (_fading_off || (!_fading_in && GetNodePath().get_color().get_w() <= 0));
}

void ObjectVisibility::Run(float elapsed_time)
{
  RunFade(elapsed_time);
  TaskAnimation();
}

void ObjectVisibility::RunFade(float) // TODO must use elapsedTime in ObjectCharacter::Fading
{
  if (_fading_in || _fading_off)
    Fading();
}

void ObjectVisibility::Fading(void)
{
  LColor color = GetNodePath().get_color();

  if (_fading_off)
  {
    GetNodePath().set_transparency(TransparencyAttrib::M_alpha);
    if (color.get_w() > 0)
      color.set_w(color.get_w() - 0.05);
    else
      _fading_off = false;
  }
  else if (_fading_in)
  {
    float max_alpha = HasFlag(FLAG_CHARACTER_SNEAK) ? 0.5 : base_color.get_w();

    if (color.get_w() < max_alpha)
      color.set_w(color.get_w() + 0.05);
    else
      _fading_in = false;
    if (_fading_in == false) // Quick hack for characters not fading back in completely
    {
      color.set_w(max_alpha);
      if (max_alpha == 1)
        GetNodePath().set_transparency(TransparencyAttrib::M_none);
    }
  }
  GetNodePath().set_color(color);
  if (color.get_w() == 0)
    GetNodePath().hide();
  else if (GetNodePath().is_hidden())
    GetNodePath().show();
}

void ObjectVisibility::Serialize(Utils::Packet& packet)
{
  unsigned char is_fading_in  = _fading_in  ? 1 : 0;
  unsigned char is_fading_out = _fading_off ? 1 : 0;
  float         alpha         = GetNodePath().get_color().get_w();
  float         red           = base_color.get_x();
  float         green         = base_color.get_y();
  float         blue          = base_color.get_z();

  packet << is_fading_in << is_fading_out << alpha;
  packet << red << green << blue;
}

void ObjectVisibility::Unserialize(Utils::Packet& packet)
{
  unsigned char is_fading_in;
  unsigned char is_fading_out;
  float         alpha, red, green, blue;

  packet >> is_fading_in >> is_fading_out >> alpha;
  packet >> red >> green >> blue;
  _fading_in  = is_fading_in  == 1;
  _fading_off = is_fading_out == 1;
  GetNodePath().set_color(red, green, blue, alpha);
}
