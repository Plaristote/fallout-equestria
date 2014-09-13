Data           datastore;
DynamicObject@ self;
bool           first_time;
string         force_field_name;

string HookInit(DynamicObject@ _self)
{
  @self            = @_self;
  datastore        = self.GetDataStore();
  first_time       = datastore["already-talked-to"].AsInt() != 1;
  force_field_name = get_forcefield_from_terminal();
  return ("welcome");
}

string get_forcefield_from_terminal()
{
  string prefix = "force-field#";

  if (self.GetName() == "computer-entry-hall#1")
    return (prefix + '2');
  return (prefix + '1');
}

bool if_force_field_enabled()
{
  DynamicObject@ force_field = level.GetObject(force_field_name);

  return (@force_field != null && !force_field.IsHidden());
}

bool if_force_field_disabled()
{
  return (!if_force_field_enabled());
}

string enable_force_field()
{
  set_force_field_enabled(true);
  return ("welcome");
}

string disable_force_field()
{
  set_force_field_enabled(false);
  return ("welcome");
}

void set_force_field_enabled(bool enabled)
{
  DynamicObject@ force_field = level.GetObject(force_field_name);

  if (@force_field != null)
  {
    force_field.SetVisible(enabled);
    Cout("Disabling force field " + force_field_name);
    level.GetDataEngine()["Variables"][force_field_name]["disabled"] = enabled ? 0 : 1;
    level.GetDataEngine()["Variables"].Output();
  }
}
