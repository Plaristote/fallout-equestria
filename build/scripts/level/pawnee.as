
Zone@ entry_zone;

void entered_entry_zone(DynamicObject@ object)
{
  Cout("OBJECT ENTERED ENTRY ZONE");
}

void exited_entry_zone(DynamicObject@ object)
{
  Cout("OBJECT EXITED ENTRY ZONE");
}

void initialize()
{
  @entry_zone = Zone_Factory("worldmap", "void entered_entry_zone(DynamicObject@)");
  entry_zone.SetExitCallback("void exited_entry_zone(DynamicObject@)");
}

void finalize()
{
  entry_zone.Delete();
}

void run(float elapsed_time)
{
}
