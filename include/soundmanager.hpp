#ifndef  SOUNDMANAGER_HPP
# define SOUNDMANAGER_HPP

# include <string>
# include <list>
# include "datatree.hpp"
# include <algorithm>

struct ISampleInstance
{
  virtual ~ISampleInstance() {}
  virtual void Start(void) = 0;
  virtual void Stop(void)  = 0;
};

class SoundManager;

struct ISample
{
  ISample(SoundManager*) {}
  virtual ~ISample() {}
  virtual ISampleInstance* NewInstance(void) = 0;
  virtual bool             LoadFromFile(const std::string& filename)
  { this->filename = filename; }
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
public:

  SoundManager(void);
  ~SoundManager(void);

  ISampleInstance* CreateInstance(const std::string& key);
  void             DeleteInstance(ISampleInstance*) const;

  template<typename TSAMPLE>
  bool RequireSound(const std::string& key)
  {
    Data data_audio(_data_audio);
    Data data_file = data_audio[key];

    if (data_file.NotNil())
    {
      auto it      = find(_sounds.begin(), _sounds.end(), key);
      bool success = true;
      
      if (it != _sounds.end())
      {
        _sounds.push_back(Sound(key, new TSAMPLE(this)));
        it = --(_sounds.end());
        success = it->sample->LoadFromFile(data_file.Value());
      }
      else if ((find(_sounds_required.begin(), _sounds_required.end(), it)) != _sounds_required.end())
        return (true);
      it->ref_count++;
      _sounds_required.push_back(it);
      return (success);
    }
    return (false);
  }

private:
  static Sounds    _sounds;
  static DataTree* _data_audio;
  SoundsIterators  _sounds_required;
};

#endif
