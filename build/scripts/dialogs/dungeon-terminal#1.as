DynamicObject@ self = null;
Data           datastore;

string HookInit(DynamicObject@ _self)
{
  Data terminal_unlocked;

  @self             = @_self;
  datastore         = self.GetDataStore();
  terminal_unlocked = datastore["terminal-unlocked"];
  if (terminal_unlocked.NotNil() && terminal_unlocked.AsInt() == 1)
    return ("command-input");
  return ("authentication-required");
}

string left_password_guesses()
{
  Data data          = datastore["password-guess-count"];
  int  left_attempts = data.Nil() ? 3 : 3 - data.AsInt();

  return ("" + left_attempts);
}

int password_guess_count()
{
  Data data = datastore["password-guess-count"];

  return (data.Nil() ? 0 : data.AsInt());
}

void add_password_guess_attempt()
{
  Data data = datastore["password-guess-count"];

  data = data.Nil() ? 1 : data.AsInt() + 1;  
}

void set_player_detected()
{
  Data data_engine      = level.GetDataEngine();
  Data security_enabled = data_engine["dungeon"]["player-position"];

  security_enabled = level.GetPlayer().GetCurrentWaypoint();
}

void set_security_enabled(bool enabled)
{
  Data data_engine      = level.GetDataEngine();
  Data security_enabled = data_engine["dungeon"]["security-enabled"];

  security_enabled = enabled ? 1 : 0;
  set_player_detected();
}

string wrong_password()
{
  add_password_guess_attempt();
  if (password_guess_count() >= 3)
  {
    set_security_enabled(true);
    return ("intruder-detected");
  }
  return ("authentication-required");
}

string unlock_elevator()
{
  DynamicObject@ door_object = level.GetObject("door-elevator");
  Door@          door        = @door_object != null ? door_object.AsDoor() : null;

  if (@door != null)
    door.SetLocked(false);
  return ("elevator-unlocked");
}

string good_password()
{
  datastore["terminal-unlocked"] = 1;
  return ("command-input");
}
