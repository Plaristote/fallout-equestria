#ifndef  SOUNDMANAGER_HPP
# define SOUNDMANAGER_HPP

# include "globals.hpp"
# include <string>
# include <list>
# include "datatree.hpp"
# include <algorithm>

# include "musicmanager.hpp"

struct ISampleInstance
{
  virtual ~ISampleInstance() {}
  virtual void  Start(void)           = 0;
  virtual void  Stop(void)            = 0;
  virtual void  Pause(void)           = 0;
  virtual void  SetVolume(float)      = 0;
  virtual float GetVolume(void) const = 0;
  virtual bool  IsPlaying(void) const = 0;

  void           AddReference(void)            { ref_count++;        }
  void           DelReference(void)            { ref_count--;        }
  unsigned short GetReferenceCount(void) const { return (ref_count); }
  
private:
  unsigned short ref_count;
};

class SoundManager;

struct ISample
{
  ISample(SoundManager*) {}
  virtual ~ISample() {}
  virtual ISampleInstance* NewInstance(void) = 0;
  virtual bool             LoadFromFile(const std::string& filename)
  { this->filename = filename; return (true); }
  const std::string&       GetFilename(void) const { return (filename); }

protected:
  std::string   filename;  
};

class SoundManager
{
  struct Sound
  {
    Sound(const std::string& key, ISample* sample) : key(key), sample(sample), ref_count(0) {}
    bool operator==(const std::string& comp) { return (key == comp); }
    const std::string key;
    ISample*          sample;
    unsigned short    ref_count;
  };

  typedef std::list<Sound>            Sounds;
  typedef std::list<Sounds::iterator> SoundsIterators;
  typedef std::list<ISampleInstance*> SoundInstances;
  typedef std::list<SoundManager*>    SoundManagers;
public:

  SoundManager(void);
  ~SoundManager(void);
  
  static SoundManager* NewSoundManager(void) { return (new SoundManager); }
  void                 Release(void) { delete this; }

  ISampleInstance* CreateInstance(const std::string& key);
  void             DeleteInstance(ISampleInstance*);
  void             GarbageCollect(void);
  static void      GarbageCollectAll(void);

  void             SetVolume(float);
  float            GetVolume(void) const { return (_volume); }

  bool Require(const std::string& key);

  template<typename TSAMPLE>
  bool RequireSound(const std::string& key)
  {
    Data data_audio(_data_audio);
    Data data_file = data_audio[key];

    if (data_file.NotNil())
    {
      auto it      = find(_sounds.begin(), _sounds.end(), key);
      bool success = true;
      
      if (it == _sounds.end())
      {
        _sounds.push_back(Sound(key, new TSAMPLE(this)));
        it = --(_sounds.end());
        success = it->sample->LoadFromFile(data_file.Value());
	cout << "Loaded sample " << key << " with success: " << success << endl;
      }
      else if ((find(_sounds_required.begin(), _sounds_required.end(), it)) != _sounds_required.end())
      {
	cout << "Sample " << key << " already required" << endl;
        return (true);
      }
      it->ref_count++;
      _sounds_required.push_back(it);
      return (success);
    }
    cout << "Can't load any sample" << endl;
    return (false);
  }

private:
  static SoundManagers _sound_managers;
  static Sounds        _sounds;
  static DataTree*     _data_audio;
  SoundsIterators      _sounds_required;
  float                _volume;
  SoundInstances       _sounds_playing;
};

# ifdef AUDIO_BACKEND_PANDA3D
#  include <panda3d/audioManager.h>
#  define AUDIO_SAMPLE_TYPE p3_Sample

class p3_SampleInstance : public ISampleInstance
{
public:
  p3_SampleInstance(PT(AudioSound) sound) : sound(sound)
  {
  }

  void Start(void)
  {
    sound->play();
  }
  
  void Stop(void)
  {
    sound->stop();
  }
  
  void Pause(void)
  {
    sound->stop();
  }
  
  void SetVolume(float volume)
  {
    sound->set_volume(volume);
  }
  
  float GetVolume(void) const
  {
    return (sound->get_volume());
  }
  
  bool IsPlaying(void) const
  {
    return (sound->status() == AudioSound::PLAYING);
  }  
  
private:
  PT(AudioSound) sound;
};

class p3_Sample : public ISample
{
public:
  p3_Sample(SoundManager* sm) : ISample(sm)
  {
    if (audio_manager.is_null())
    {
      audio_manager = AudioManager::create_AudioManager();
      audio_manager->set_active(true);
      audio_manager->set_volume(1.0);
    }
  }

  ISampleInstance* NewInstance(void)
  {
    PT(AudioSound) sound = audio_manager->get_sound(filepath);
    
    return (new p3_SampleInstance(sound));
  }

  bool             LoadFromFile(const std::string& filename)
  {
    ISample::LoadFromFile(filename);
    filepath = filename;
    return (true);
  }  

private:
  static PT(AudioManager) audio_manager;
  std::string             filepath;
};

# endif

# ifdef AUDIO_BACKEND_SFML
#  include <SFML/Audio.hpp>
#  define AUDIO_SAMPLE_TYPE sf_Sample

class sf_SampleInstance : public ISampleInstance
{
public:
  sf_SampleInstance(sf::SoundBuffer& buffer)
  {
    instance.setBuffer(buffer);
  }
  
  void Start(void)
  {
    instance.play();
  }
  
  void Stop(void)
  {
    instance.stop();
  }
  
  void Pause(void)
  {
    instance.pause();
  }
  
  void SetVolume(float volume)
  {
    instance.setVolume(volume);
  }
  
  float GetVolume(void) const
  {
    return (instance.getVolume());
  }
  
  bool IsPlaying(void) const
  {
    return (instance.getStatus() == sf::SoundSource::Status::Playing);
  }

private:
  sf::Sound instance;
};

class sf_Sample : public ISample
{
public:
  sf_Sample(SoundManager* sm) : ISample(sm)
  {
  }

  ISampleInstance* NewInstance(void)
  {
    return (new sf_SampleInstance(buffer));
  }

  bool             LoadFromFile(const std::string& filename)
  {
    ISample::LoadFromFile(filename);
    return (buffer.loadFromFile(filename));
  }

private:
  sf::SoundBuffer buffer;
};

# endif

#endif
