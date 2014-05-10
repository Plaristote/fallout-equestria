#include "musicmanager.hpp"
#include "options.hpp"

using namespace std;

MusicManager* MusicManager::_global_ptr = 0;

MusicManager::MusicManager()
{
  _fading_out    = false;
  _volume_ref    = 10.0;
  _volume_goal   = _volume_ref;
  _data_tree     = DataTree::Factory::JSON("data/musics.json");
  if (_data_tree)
    _data = Data(_data_tree);
#ifdef AUDIO_BACKEND_PANDA3D
  _audio_manager = AudioManager::create_AudioManager();
  _audio_manager->set_active(false);
  if (!(_audio_manager->is_valid()))
    cout << "[MusicManager][Fatal Errror] AudioManager isn't valid" << endl;
  _audio_manager->set_volume(1.0);
#endif
#ifdef AUDIO_BACKEND_SFML
  _current_music = 0;
  _next_music    = 0;
#endif
  Connect(OptionsManager::Updated, *this, &MusicManager::SetVolumeToDefault);
  SetVolumeToDefault();
}

MusicManager::~MusicManager()
{
  _data = Data();
  if (_data_tree)
    delete _data_tree;
}

void MusicManager::Play(const string& category)
{
  if (_current_category == category) return ;
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
#ifdef AUDIO_BACKEND_PANDA3D
  if (_current_music && _current_music->status() == AudioSound::PLAYING)
  {
    _fading_out = true;
    _next_music = _audio_manager->get_sound("audio/" + song.Value());
  }
  else
  {
    _current_music = _audio_manager->get_sound("audio/" + song.Value());
    _current_music->play();
    _current_music->set_volume(_volume_ref);
  }
#endif
#ifdef AUDIO_BACKEND_SFML
  if (_current_music && _current_music->getStatus() == sf::SoundSource::Status::Playing)
  {
    if (!(_fading_out))
      _next_music = new sf::Music;
    _next_music->openFromFile("audio/" + song.Value());
    _fading_out = true;
  }
  else
  {
    if (_current_music) delete _current_music;
    _current_music = new sf::Music;
    _current_music->openFromFile("audio/" + song.Value());
    _current_music->play();
    _current_music->setVolume(_volume_ref);
  }
#endif
}

void MusicManager::PlayNext(void)
{
  Data           category = _data[_current_category];
  unsigned short max      = category.Count();
  unsigned short selected;

  if (max == 0)
  {
    if (_current_music)
      _current_music->play();
  }
  else
  {
    selected = rand() % max;
    Play(_current_category, category[selected].Key());
  }
}

void MusicManager::Run(void)
{
#ifdef AUDIO_BACKEND_PANDA3D
  _audio_manager->update();
#endif
  if (_current_music)
  {
    float elapsed_time  = _timer.GetElapsedTime();
    bool  not_playing;
    bool  volume_change;
    bool  is_valid;

#ifdef AUDIO_BACKEND_PANDA3D
    not_playing   = _current_music->status()     != AudioSound::PLAYING;
    volume_change = _current_music->get_volume() != _volume_goal;
    is_valid      = _audio_manager->is_valid();
#endif
#ifdef AUDIO_BACKEND_SFML
    not_playing   = _current_music->getStatus()  != sf::SoundSource::Status::Playing;
    volume_change = _current_music->getVolume()  != _volume_goal;
    is_valid      = true;
#endif
    if (is_valid)
    {
      if (volume_change)
        FadeVolume(elapsed_time);
      if (not_playing)
        PlayNext();
      else if (_fading_out)
        FadeOut(elapsed_time);
    }
    _timer.Restart();
  }
}

void MusicManager::FadeVolume(float elapsed_time)
{
#ifdef AUDIO_BACKEND_PANDA3D
  float volume = _current_music->get_volume();
#endif
#ifdef AUDIO_BACKEND_SFML
  float volume = _current_music->getVolume();
#endif

  if (volume > _volume_goal)
  {
    volume -= (elapsed_time * 4);
    if (volume < _volume_goal)
      volume = _volume_goal;
  }
  else
  {
    volume += (elapsed_time * 4);
    if (volume > _volume_goal)
      volume = _volume_goal;
  }
#ifdef AUDIO_BACKEND_PANDA3D
  _current_music->set_volume(volume);
#endif
#ifdef AUDIO_BACKEND_SFML
  _current_music->setVolume(volume);
#endif
}

void MusicManager::FadeOut(float elapsed_time)
{
#ifdef AUDIO_BACKEND_PANDA3D
  float volume = _current_music->get_volume();
#endif
#ifdef AUDIO_BACKEND_SFML
  float volume = _current_music->getVolume();
#endif

  _volume_goal = 0;
  if (volume == _volume_goal)
  {
#ifdef AUDIO_BACKEND_PANDA3D
    _current_music->stop();
    _current_music = _next_music;
    _next_music    = 0;
    _current_music->play();
    _current_music->set_volume(1);
    _current_music->set_balance(0);
#endif
#ifdef AUDIO_BACKEND_SFML
    _current_music->stop();
    delete _current_music;
    _current_music = _next_music;
    _next_music    = 0;
    _current_music->play();
    _current_music->setVolume(_volume_ref);
#endif
    _volume_goal   = _volume_ref;
    _fading_out    = false;
  }
}

void MusicManager::SetVolume(float volume)
{
  _volume_ref  = volume;
  _volume_goal = volume;
}

void MusicManager::SetVolumeToDefault()
{
  float volume = OptionsManager::Get()["music"]["volume"].Or(5.f);

  SetVolume(volume);
}
