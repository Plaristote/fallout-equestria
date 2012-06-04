#include "globals.hpp"
#include "objectnode.hpp"
#include <panda3d/nodePathCollection.h>

using namespace std;
using namespace Observatory;

extern PandaFramework framework;

/*ObjectNode::ObjectNode(WindowFramework* window, Tilemap& map, Data data) : _window(window), _map(map)
{
  static unsigned int charId = 0;
  NodePath            root = window->get_render();

  //root.set_collide_mask(Object);
  
  _map.AddMapElement(this);

  // Load the model + textures

  // WARNING lpip.egg segfault
  if (data["model"].Value() == "lpip.egg")
    data["model"] = "horse.obj";
  // END lpip.egg segfault
  
  _root = window->load_model(_window->get_panda_framework()->get_models(), "models/" + data["model"].Value());
  if (!(data["texture"].Nil()))
  {
    _tex = TexturePool::load_texture(TEXTURE_PATH + data["texture"].Value());
    if (_tex)
      _root.set_texture(_tex);
    else
      cout << "/!\\ WARNING: Can't load texture from path '" << TEXTURE_PATH << data["texture"].Value() << "'" << endl;
  }

  _root.set_hpr(0, 0, 0);
  _root.set_pos(0, 0, 0);
  if (data["scale"].Nil())
    _root.set_scale(WORLD_SCALE);
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
  _dialog = data["dialog"].Value();
}

void ObjectNode::ForceCurrentCase(int x, int y)
{
  const Tilemap::MapTile& tile =_map.GetTile(x, y);

  LPoint3 tilePos = tile.position;
  LPoint3 charPos = _root.get_pos();

  charPos.set_x(tilePos.get_x());
  charPos.set_y(tilePos.get_y());
  _root.set_pos(charPos);
  _mapPos.set_x(x);
  _mapPos.set_y(y);
}

void ObjectNode::ForceClosestCase(void)
{
  LPoint3 currentPos =_root.get_pos();
  LPoint2 roundedPos;

  roundedPos.set_x(round(currentPos.get_x() / (WORLD_SCALE * TILE_UNIT)));
  roundedPos.set_y(round(currentPos.get_y() / (WORLD_SCALE * TILE_UNIT)));
  Tilemap::Tile& tile = _map.GetTile(roundedPos.get_x(), roundedPos.get_y());
  _root.set_pos(tile.position);
  _mapPos.set_x(roundedPos.get_x());
  _mapPos.set_y(roundedPos.get_y());
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
}*/