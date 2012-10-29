#include "soundmanager.hpp"
#include <algorithm>

using namespace std;

SoundManager::Sounds SoundManager::_sounds;
DataTree*            SoundManager::_data_audio = 0;

SoundManager::SoundManager()
{
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
    return (it->sample->NewInstance());
  return (0);
}

void             SoundManager::DeleteInstance(ISampleInstance* instance) const
{
  delete instance;
}