
// 0: file list
// 1: file text
// 2: file sound
int         current_interface = 0;
string      current_file;
RmlElement@ myroot;

// AudioFile Playing stuff
SoundManager@ pb_snd_manager;
Sound@        pb_sound_playing;
bool          pb_sound_paused;
bool          pb_mus_snd_handled;

bool start(Data data)
{
  Write("Helloworld Started");
  @pb_snd_manager = NewSoundManager();
  pb_sound_paused = false;
  return (true);
}

void exit(Data data)
{
  current_interface = 0;
  current_file      = "";
  if (@pb_sound_playing != null)
  {
    pb_snd_manager.DeleteInstance(pb_sound_playing);
    MusicHandle().SetVolume(10);
  }
  pb_snd_manager.Release();
  Write("Helloworld Exited");
}

void focused(RmlElement@ root, Data data)
{
  Write("FileManager Focused");
  string rml;

  @myroot = @root;
  rml     = "<h1>File Manager 2000</h1>";
  if (current_interface == 0)
    rml += display_file_list(data);
  else if (current_interface == 1)
    rml += display_file_text(data);
  else if (current_interface == 2)
    rml += display_file_audio(data);
  root.SetInnerRML(rml);
  Write("InnerRML Set");
  if (current_interface == 0)
    events_file_list(data);
  else if (current_interface == 2)
    events_file_audio(data);
  Write("Focused ended");
}

string display_file_list(Data data)
{
  Data   files   = data["Pipbuck"]["Files"];
  int    n_files = files.Count();
  int    it      = 0;
  string rml     = "";

  rml += "<div id='file_list'>";
  while (it < n_files)
  {
    Data   file = files[it];
    string icon = "../textures/pipbuck/icons/" + file["type"].AsString() + ".png";

    rml += "<datagrid id='file-" + file.Key() + "'>\n";
    rml += "  <col width='20%'><img src='" + icon + "' /></col>\n";
    rml += "  <col width='80%'>" + file["header"].AsString() + "</col>\n";
    rml += "</datagrid>\n";
    it  += 1;
  }
  rml += "</div>";
  return (rml);
}

void   events_file_list(Data data)
{
  Write("Executing events_file_list");
  Data   files   = data["Pipbuck"]["Files"];
  int    n_files = files.Count();
  int    it      = 0;

  while (it < n_files)
  {
    Data        file = files[it];
    RmlElement@ elem = myroot.GetElementById("file-" + file.Key());

    if (@elem != null)
    {
      elem.ClearEventListeners();
      elem.AddEventListener("click", "file_opened");
    }
    else
      Write("[Pipbuck][FileManager][Error] No element with id 'file-" + file.Key() + "'");
    it += 1;
  }
}

void   events_file_audio(Data data)
{
  Write("Executing events_file_audio");
  RmlElement@ elem_play  = myroot.GetElementById("pipbuck-audio-play");
  RmlElement@ elem_stop  = myroot.GetElementById("pipbuck-audio-stop");
  RmlElement@ elem_pause = myroot.GetElementById("pipbuck-audio-pause");

  elem_play.AddEventListener("click", "file_audio_play");
  elem_stop.AddEventListener("click", "file_audio_stop");
  elem_pause.AddEventListener("click", "file_audio_pause");
}

void file_audio_play(Data data, RmlElement@ current_element, string event)
{
  Write("FileAudioPlay");
  if (pb_sound_paused)
  {
    Write("Unpausing");
    pb_sound_playing.Play();
  }
  else
  {
    Write("Creating new instance of sound");
    Data   file  = data["Pipbuck"]["Files"][current_file];
    string key   = file["src"].AsString();

    if (@pb_sound_playing != null)
    {
      pb_sound_playing.DelReference();
      pb_snd_manager.DeleteInstance(pb_sound_playing);
    }
    pb_snd_manager.RequireSound(key);
    @pb_sound_playing = pb_snd_manager.CreateInstance(key);
    if (@pb_sound_playing != null)
    {
      pb_sound_playing.Play();
      pb_sound_playing.AddReference();
    }
  }
  pb_sound_paused = false;
}

void file_audio_stop(Data data, RmlElement@ current_element, string event)
{
  Write("FileAudioStop");
  if (@pb_sound_playing != null)
  {
    Write("Stoped");
    pb_sound_playing.Stop();
    pb_sound_playing.DelReference();
    pb_snd_manager.DeleteInstance(pb_sound_playing);
    @pb_sound_playing = null;
  }
}

void file_audio_pause(Data data, RmlElement@ current_element, string event)
{
  Write("FileAudioPause");
  if (@pb_sound_playing != null)
  {
    Write("Paused");
    pb_sound_paused = true;
    pb_sound_playing.Pause();
  }
}

void   release_events_file_audio(Data)
{
  RmlElement@ elem_play  = myroot.GetElementById("pipbuck-audio-play");
  RmlElement@ elem_stop  = myroot.GetElementById("pipbuck-audio-stop");
  RmlElement@ elem_pause = myroot.GetElementById("pipbuck-audio-pause");

  elem_play.ClearEventListeners();
  elem_stop.ClearEventListeners();
  elem_pause.ClearEventListeners();
}

void   file_opened(Data data, RmlElement@ current_element, string event)
{
  string rml;
  Data   files   = data["Pipbuck"]["Files"];
  int    it      = 0;
  int    n_files = files.Count();
  string elem_id = current_element.GetId();

  Write("[Pipbuck][FileManager] Loading file " + elem_id);
  while (it < n_files)
  {
    Data file = files[it];

    if ("file-" + file.Key() == elem_id)
    {
      if (file["type"].AsString() == "audio")
        current_interface = 2;
      else
        current_interface = 1;
      current_file      = file.Key();
      break ;
    }
    it += 1;
  }
  current_element.ClearEventListeners();
  if (current_interface == 1)
    rml = display_file_text(data);
  else if (current_interface == 2)
    rml = display_file_audio(data);
  myroot.SetInnerRML(rml);
  if (current_interface == 2)
    events_file_audio(data);
}

string display_file_text(Data data)
{
  Data   file = data["Pipbuck"]["Files"][current_file];
  string rml  = "";

  rml += "<div id='file-header'>" + file["header"].AsString() + "</div>";
  rml += "<div id='file-context'>" + file["content"].AsString() + "</div>";
  return (rml);
}

string display_file_audio(Data data)
{
  Data   file = data["Pipbuck"]["Files"][current_file];
  string rml  = "";

  rml += "<div id=file-header'>" + file["header"].AsString() + "</div>";
  rml += "<button id='pipbuck-audio-play' class='button'>Play</button>";
  rml += "<button id='pipbuck-audio-stop' class='button'>Stop</button>";
  rml += "<button id='pipbuck-audio-pause' class='button'>Pause</button>";
  return (rml);
}

void unfocused(Data data)
{
  Write("Helloworld Unfocused");
}

void handle_music_sound()
{
  Music@ music = MusicHandle();

  if (@pb_sound_playing != null)
  {
    music.SetVolume(1);
    pb_mus_snd_handled = false;
  }
  else if (pb_mus_snd_handled == false)
  {
    music.SetVolume(10);
    pb_mus_snd_handled = true;
  }
}

int main(RmlElement@ root, Data data)
{
  handle_music_sound();
  return (0);
}

int daemon(Data data)
{
  handle_music_sound();
  return (0);
}

