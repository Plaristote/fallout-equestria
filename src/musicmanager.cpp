#include "musicmanager.hpp"

using namespace std;

MusicManager* MusicManager::_global_ptr = 0;

MusicManager::MusicManager()
{
  _fading_out    = false;
  _audio_manager = AudioManager::create_AudioManager();
  _data_tree     = DataTree::Factory::JSON("data/musics.json");
  if (_data_tree)
    _data = Data(_data_tree);
  _audio_manager->set_active(true);
  if (!(_audio_manager->is_valid()))
    cout << "[MusicManager][Fatal Errror] AudioManager isn't valid" << endl;
  _audio_manager->set_volume(1.0);
}

MusicManager::~MusicManager()
{
  _data = Data();
  if (_data_tree)
    delete _data_tree;
}

void MusicManager::Play(const string& category)
{
  _current_category = category;
  PlayNext();
}

void MusicManager::Play(const string& scategory, const string& name)
{
  Data category     = _data[scategory];
  Data song;

  if (category.Nil()) return ;
  song              = category[name];
  if (song.Nil())     return ;
  if (_current_music && _current_music->status() == AudioSound::PLAYING)
  {
    _fading_out = true;
    _next_music = _audio_manager->get_sound("audio/" + song.Value());
    cout << "File audio/" << song.Value() << "' on queue" << endl;
  }
  else
  {
    _current_music = _audio_manager->get_sound("audio/" + song.Value());
    _current_music->play();
    if (_current_music->status() == AudioSound::BAD)
      cout << "Failed to start audio file" << endl;
    else if (_current_music->status() == AudioSound::PLAYING)
      cout << "Now playing file 'audio/" << song.Value() << "'" << endl;
    else
      cout << "LOL WTF" << endl;
    cout << "Current music length is " << _current_music->length() << endl;
   }
  _current_music->set_play_rate(5.0);
}

void MusicManager::PlayNext(void)
{
  Data           category = _data[_current_category];
  unsigned short max      = category.Count();
  unsigned short selected;
  string         filename;

  if (category.Nil()) return ;
  selected = rand() % max;
  filename = category[selected].Value();
  if (_current_music && _current_music->status() == AudioSound::PLAYING)
  {
    _fading_out    = true;
    _next_music    = _audio_manager->get_sound("audio/" + filename);
    cout << "File 'audio/" << filename << "' on queue" << endl;
  }
  else
  {
    _current_music = _audio_manager->get_sound("audio/" + filename);
    _current_music->play();
    if (_current_music->status() == AudioSound::BAD)
      cout << "Failed to start audio file" << endl;
    else if (_current_music->status() == AudioSound::PLAYING)
      cout << "Now playing file 'audio/" << filename << "'" << endl;
    else
      cout << "LOL WTF" << endl;
  }
}

void MusicManager::Run(void)
{
  _audio_manager->update();
  if (_current_music)
  {
    float elapsed_time = _timer.GetElapsedTime();

    if (_current_music->status() != AudioSound::PLAYING)
    {
      PlayNext();
      cout << "Playing Next" << endl;
    }
    else if (_fading_out)
      FadeOut(elapsed_time);
    _timer.Restart();
  }
}

void MusicManager::FadeOut(float elapsed_time)
{
  float volume = _current_music->get_volume();

  volume -= (elapsed_time / 2);
  if (volume <= 0)
  {
    _current_music->stop();
    _current_music = _next_music;
    _next_music    = 0;
    _current_music->play();
    _current_music->set_volume(1);
    _current_music->set_balance(0);
    volume = 0;
  }
  else
    _current_music->set_volume(volume);
}
