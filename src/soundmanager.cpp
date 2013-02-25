#include "soundmanager.hpp"
#include <algorithm>

using namespace std;

SoundManager::SoundManagers SoundManager::_sound_managers;
SoundManager::Sounds        SoundManager::_sounds;
DataTree*                   SoundManager::_data_audio = 0;

SoundManager::SoundManager()
{
  _volume = 10.f;
  if (_data_audio == 0)
    _data_audio = DataTree::Factory::JSON("data/audio.json");
  _sound_managers.push_back(this);
  cout << "Create SoundManager " << this << endl;
}

SoundManager::~SoundManager()
{
  cout << "Delete SoundManager " << this << endl;
  for_each(_sounds_required.begin(), _sounds_required.end(), [this](Sounds::iterator it)
  {
    it->ref_count--;
    if (it->ref_count <= 0)
    {
      delete it->sample;
      _sounds.erase(it);
    }
  });
  _sound_managers.remove(this);
  if (_sound_managers.size() == 0 && _data_audio)
  {
    delete _data_audio;
    _data_audio = 0;
  }
}

void             SoundManager::GarbageCollectAll(void)
{
  ForEach(_sound_managers, [](SoundManager* sm) { sm->GarbageCollect(); });
}

void             SoundManager::GarbageCollect(void)
{
  auto it  = _sounds_playing.begin();
  auto end = _sounds_playing.end();
  
  while (it != end)
  {
    ISampleInstance* sound = *it;

    if (!(sound->IsPlaying()) && (sound->GetReferenceCount() == 0))
    {
      delete sound;
      it = _sounds_playing.erase(it);
    }
    else
      ++it;
  }
}

bool             SoundManager::Require(const std::string& key)
{
  return (RequireSound<AUDIO_SAMPLE_TYPE>(key));
}

ISampleInstance* SoundManager::CreateInstance(const std::string& key)
{
  cout << "Create instance with " << this << endl;
  auto it = find(_sounds.begin(), _sounds.end(), key);
  
  if (it != _sounds.end())
  {
    cout << "NewInstance" << endl;
    ISampleInstance* instance = it->sample->NewInstance();

    instance->SetVolume(_volume);
    _sounds_playing.push_back(instance);
    cout << "Returning Safe and Sound" << endl;
    return (instance);
  }
  else
    cout << "[SoundManager] " << key << " hasn't been required" << endl;
  return (0);
}

void             SoundManager::DeleteInstance(ISampleInstance* instance)
{
  auto it = find(_sounds_playing.begin(), _sounds_playing.end(), instance);
  
  if (it != _sounds_playing.end())
  {
    _sounds_playing.erase(it);
    delete instance;
  }
}

void             SoundManager::SetVolume(float volume)
{
  for_each(_sounds_playing.begin(), _sounds_playing.end(), [this, volume](ISampleInstance* instance)
  {
    float current_volume = instance->GetVolume();
    float factor         = current_volume / volume;
    
    instance->SetVolume(volume * factor);
  });
}