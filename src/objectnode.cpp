#include "objectnode.hpp"
#include <panda3d/nodePathCollection.h>

using namespace std;
using namespace Observatory;

extern PandaFramework framework;

ObjectNode::ObjectNode(WindowFramework* window, Tilemap& map, Data data) : _window(window), _map(map)
{
  static unsigned int charId = 0;
  NodePath            root = window->get_render();

  _map.AddMapElement(this);

  // Load our panda
  _root = window->load_model(_root, "models/" + data["model"].Value());
  _root.set_hpr(0, 90, 0);
  _root.set_pos(0, 0, 3.5);
  if (data["scale"].Nil())
    _root.set_scale(0.5);
  else
    _root.set_scale(((float)data["scale"]) * WORLD_SCALE);
  _root.reparent_to(root);

  // Set the UnitID
  std::stringstream stream;
  stream << (++charId);
  _root.set_tag("character", stream.str());

  ForceCurrentCase(data["pos"]["x"], data["pos"]["y"]);

  // Load the walk animation
//   window->load_model(_root, "panda-walk4");
//   window->loop_animations(0);

  if (!(data["interactions"].Nil()))
    LoadInteractions(data["interactions"]);
}

void ObjectNode::ForceCurrentCase(int x, int y)
{
  const Tilemap::MapTile& tile =_map.GetTile(x, y);

  LPoint3 tilePos = tile.nodePath.get_pos();
  LPoint3 charPos = _root.get_pos();

  charPos.set_x(tilePos.get_x());
  charPos.set_y(tilePos.get_y());
  _root.set_pos(charPos);
  _mapPos.set_x(x);
  _mapPos.set_y(y);
}

void ObjectNode::Run(float elapsedTime)
{
}

//
// Interactions
//
Signal<void (ObjectNode*)> ObjectNode::ActionUse;
Signal<void (ObjectNode*)> ObjectNode::ActionUseObjectOn;
Signal<void (ObjectNode*)> ObjectNode::ActionUseSkillOn;
Signal<void (ObjectNode*)> ObjectNode::ActionTalkTo;

struct InteractionCallback
{
  InteractionCallback(string name, Signal<void (ObjectNode*)>& signal) : name(name), signal(signal)
  {}

  string                       name;
  Signal<void (ObjectNode*)>&  signal;
};

InteractionCallback interactionCallbacks[] =
{
  InteractionCallback("use",        ObjectNode::ActionUse),
  InteractionCallback("use_object", ObjectNode::ActionUseObjectOn),
  InteractionCallback("use_skill",  ObjectNode::ActionUseSkillOn),
  InteractionCallback("talk_to",    ObjectNode::ActionTalkTo),
  InteractionCallback("",           ObjectNode::ActionUse)
};

void ObjectNode::LoadInteractions(Data data)
{
  for_each(data.begin(), data.end(), [this](Data dataInt)
  {
    Interaction interaction;

    interaction.name          = dataInt.Key();
    interaction.objectNode    = this;

    for (unsigned short i = 0 ; interactionCallbacks[i].name != "" ; ++i)
    {
      if (interactionCallbacks[i].name == interaction.name)
      {
        interaction.triggered = &(interactionCallbacks[i].signal);
        break ;
      }
    }

    _interactions.push_back(interaction);
  });
}