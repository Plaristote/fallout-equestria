#include "level/characters/object_outline.hpp"
#include "level/objects/character.hpp"
#include <panda3d/depthTestAttrib.h>

using namespace std;

/*
 * TargetOutliner
 */
void TargetOutliner::UsePerspectiveOfCharacter(ObjectCharacter* subject)
{
  this->subject = subject;
  if (outlines.size() > 0)
    EnableOutline();
}

void TargetOutliner::EnableOutline(void)
{
  FieldOfView& field_of_view = subject->GetFieldOfView();

  DisableOutline();
  MakeOutlinesForListWithColor(field_of_view.GetDetectedAllies(),     LVector4f(0, 255, 0, 0.5));
  MakeOutlinesForListWithColor(field_of_view.GetDetectedEnemies(),    LVector4f(255, 0, 0, 0.5));
  MakeOutlinesForListWithColor(field_of_view.GetDetectedNonHostile(), LVector4f(255, 0, 0, 0.5));
  field_of_view.GetDetectedCharacters();
}

void TargetOutliner::DisableOutline(void)
{
  for_each(outlines.begin(), outlines.end(), [](Outline& outline) { outline.Finalize(); });
  outlines.clear();
}

void TargetOutliner::MakeOutlinesForListWithColor(const std::vector<ObjectCharacter*>& targets, LVector4f color)
{
  for_each(targets.begin(), targets.end(), [this, color](ObjectCharacter* character)
  {
    if (character->IsAlive())
    {
      Outline outline(character);

      outline.SetColor(color);
      outlines.push_back(outline);
      outlines.rbegin()->Initialize();
      outlines.rbegin()->Show();
    }
  });
}

/*
 * ObjectOutline
 */
TargetOutliner::Outline::Outline(InstanceDynamicObject* object)
{
  target = object->GetNodePath();
  Initialize();
}

void TargetOutliner::Outline::Finalize(void)
{
  if (!(outline.is_empty()))
    outline.remove_node();
}

void TargetOutliner::Outline::Initialize(void)
{
  if (outline.is_empty())
  {
    outline = target.copy_to(target);
    outline.set_name("outline");
    outline.set_texture_off();
    outline.set_light_off(10);
    outline.set_attrib(RenderModeAttrib::make(RenderModeAttrib::M_wireframe));
    outline.set_attrib(DepthTestAttrib::make(DepthTestAttrib::M_always));
    outline.set_transparency(TransparencyAttrib::M_alpha);
    outline.reparent_to(target);
    outline.set_pos(0, 0, 0);
    outline.set_hpr(0, 0, 0);
    outline.set_scale(1.05);
  }
}

void TargetOutliner::Outline::SetColor(LVector4f color)
{
  if (!(outline.is_empty()))
    outline.set_color(color);
}

void TargetOutliner::Outline::Show(void)
{
  if (!(outline.is_empty()))
    outline.show();
}

void TargetOutliner::Outline::Hide(void)
{
  if (!(outline.is_empty()))
    outline.hide();
}
