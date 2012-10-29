#include "soundmanager.hpp"
#include <algorithm>

using namespace std;

SoundManager::Sounds SoundManager::_sounds;
DataTree*            SoundManager::_data_audio = 0;

SoundManager::SoundManager()
{
  _volume = 10.f;
  if (_data_audio == 0)
    _data_audio = DataTree::Factory::JSON("data/audio.json");
}

SoundManager::~SoundManager()
{
  for_each(_sounds_required.begin(), _sounds_required.end(), [this](Sounds::iterator it)
  {
    it->ref_count--;
    if (it->ref_count <= 0)
    {
      delete it->sample;
      _sounds.erase(it);
    }
  });
}

ISampleInstance* SoundManager::CreateInstance(const std::string& key)
{
  auto it = find(_sounds.begin(), _sounds.end(), key);
  
  if (it != _sounds.end())
  {
    ISampleInstance* instance = it->sample->NewInstance();

    instance->SetVolume(_volume);
    _sounds_playing.push_back(instance);
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