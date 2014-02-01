#include "level/character.hpp"
#include "level/level.hpp"

using namespace std;

/*
 * Buff related Character code
 */
void ObjectCharacter::PushBuff(Data data, ObjectCharacter* caster)
{
  CharacterBuff* buff = new CharacterBuff(_level, this, data);

  buff->Begin(caster);
  _buffs.push_back(buff);
}

void ObjectCharacter::DelBuff(CharacterBuff* to_del)
{
  std::list<CharacterBuff*>::iterator it = find(_buffs.begin(), _buffs.end(), to_del);
  
  if (it != _buffs.end())
  {
    _buffs.erase(it);
    delete to_del;
  }
}

/*
 * CharacterBuff class
 */
CharacterBuff::CharacterBuff(Level* level, ObjectCharacter* character, Data buff)
  : _timeManager(level->GetTimeManager())
{
  Initialize(level, character, buff);
}

CharacterBuff::CharacterBuff(Level* l) : _timeManager(l->GetTimeManager()) {}

void CharacterBuff::Initialize(Level* level, ObjectCharacter* character, Data buff)
{
  Data dataGraphics = buff["graphics"];

  _buff.Duplicate(buff);
  _buff.Output();
  _character = character;
  _name      = buff.Key();
  _duration  = buff["duration"];
  _begin     = _end = 0;
  _module    = 0;
  _task      = 0;
  _context   = Script::Engine::Get()->CreateContext();
  if (_context)
  {
    Data scriptName = buff["script"]["source"];

    if (!(scriptName.Nil()))
    {
      _module  = Script::ModuleManager::Require(scriptName.Value(), "scripts/buffs/" + scriptName.Value());
      if (_module)
      {
        Data   scriptBegin = buff["script"]["hookBegin"];
        Data   scriptEnd   = buff["script"]["hookEnd"];
        string declBegin   = "bool " + scriptBegin.Value() + "(Character@, Character@)";
        string declEnd     = "void " + scriptEnd.Value()   + "(Character@)";

        _begin = _module->GetFunctionByDecl(declBegin.c_str());
        _end   = _module->GetFunctionByDecl(declEnd.c_str());
      }
    }
  }
  if (!(dataGraphics.Nil()))
  {
    WindowFramework* window = level->GetWorld()->window;

    _graphicalEffect = window->load_model(window->get_panda_framework()->get_models(), "models/" + dataGraphics["model"].Value());
    _graphicalEffect.set_name("graphical_effect");
    if (!(dataGraphics["scale"].Nil()))
      _graphicalEffect.set_scale((float)dataGraphics["scale"]);
    if (dataGraphics["color"].NotNil())
    {
      Data color = dataGraphics["color"];
      
      if (color["alpha"].NotNil())
      {
        _graphicalEffect.set_transparency(TransparencyAttrib::M_alpha);
        _graphicalEffect.set_color(color["red"], color["green"], color["blue"], color["alpha"]);
      }
      else
        _graphicalEffect.set_color(color["red"], color["green"], color["alpha"]);
    }
  }
}

void CharacterBuff::Begin(ObjectCharacter* from, TimeManager::Task* task)
{
  if (_begin)
  {
    if (!task)
    {
      _context->Prepare(_begin);
      _context->SetArgAddress(0, _character);
      _context->SetArgAddress(1, from);
      _context->Execute();
      _task = _timeManager.AddTask(TASK_LVL_CITY, false, _duration);
    }
    else
      _task = task;
    _task->Interval.Connect(*this, &CharacterBuff::End);

    if (_graphicalEffect.node())
      _graphicalEffect.reparent_to(_character->GetNodePath());
  }
}

void CharacterBuff::End(void)
{
  if (_end)
  {
    _context->Prepare(_end);
    _context->SetArgAddress(0, _character);
    _context->Execute();

    if (_graphicalEffect.node())
      _graphicalEffect.remove_node();
  }
  _character->DelBuff(this);
}
