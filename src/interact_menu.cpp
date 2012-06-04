#include "interact_menu.hpp"
#include <panda3d/mouseButton.h>
#include <panda3d/mouseWatcher.h>
#include "level.hpp"

using namespace std;

extern PandaFramework framework;

InteractMenu::InteractMenu(WindowFramework* window, InstanceDynamicObject& object)
{
  InstanceDynamicObject::InteractionList& interactions = object.GetInteractions();

  LPoint3       position;
  MouseWatcher* mouseWatcher = dynamic_cast<MouseWatcher*>(window->get_mouse().node());
  LPoint2f      cursorPoint  = mouseWatcher->get_mouse();

  position.set_x(cursorPoint.get_x());
  position.set_y(cursorPoint.get_y());
  cout << position.get_x() << "/" << position.get_y() << endl;

  for_each(interactions.begin(), interactions.end(), [this, window, &position](InstanceDynamicObject::Interaction& interaction)
  {
    PGButton*    button = new PGButton("ButtonInteraction" + interaction.name);
    PGFrameStyle style;

    Texture*     texture = TexturePool::load_texture("textures/buttons/" + interaction.name + "-normal.png");

    float ratio = texture->get_orig_file_y_size() / ((float)texture->get_orig_file_x_size());
    button->set_frame(-0.1, 0.1, -ratio / 10, ratio / 10);

    style = button->get_frame_style(0);
    style.set_type(PGFrameStyle::T_flat);
    style.set_texture(TexturePool::load_texture("textures/buttons/" + interaction.name + "-normal.png"));
    button->set_frame_style(0, style);
    style.set_texture(TexturePool::load_texture("textures/buttons/" + interaction.name + "-active.png"));
    button->set_frame_style(1, style);
    style.set_texture(TexturePool::load_texture("textures/buttons/" + interaction.name + "-pressed.png"));
    button->set_frame_style(2, style);
    style.set_texture(TexturePool::load_texture("textures/buttons/" + interaction.name + "-inactive.png"));
    button->set_frame_style(3, style);

    // This prevents anything else to react to mouse events over the button
    button->get_region()->set_suppress_flags(MouseWatcherRegion::SF_mouse_button);

    NodePath defButNp = window->get_aspect_2d().attach_new_node(button);

    // Set the button position (y axis is unused in aspect2d)
    defButNp.set_pos(position.get_x(), 0, position.get_y());
    // Set the position for the next iteration
    position.set_y(position.get_y() - ratio / 5);
    framework.define_key(button->get_click_event(MouseButton::one()), "button press", &InteractMenu::ButtonClicked, &interaction);
    _buttons.push_back(InteractMenu::ButtonStorage(button, defButNp));
  });
  cout << "Done opening menu" << endl;
}

InteractMenu::~InteractMenu()
{
  for_each(_buttons.begin(), _buttons.end(), [](ButtonStorage data)
  {
    data.node.remove_node();
  });
}

void InteractMenu::ButtonClicked(const Event*, void* data)
{
  InstanceDynamicObject::Interaction* interaction = reinterpret_cast<InstanceDynamicObject::Interaction*>(data);

  interaction->signal->Emit(interaction->instance);
  cout << "Signal emitted" << endl;
}