#ifndef  MUSICMANAGER_HPP
# define MUSICMANAGER

# include <string>
# include <panda3d/audioManager.h>
# include "datatree.hpp"
# include "timer.hpp"

class MusicManager
{
  static MusicManager* _global_ptr;

  MusicManager();
  ~MusicManager();

public:
  static void          Initialize(void)   { if (_global_ptr == 0) { _global_ptr = new MusicManager();    } }
  static void          Finalize(void)     { if (_global_ptr != 0) { delete _global_ptr; _global_ptr = 0; } }
  static MusicManager* Get(void)          { return (_global_ptr); }

  void                 Play(const std::string& category);
  void                 Play(const std::string& category, const std::string& name);
  void                 Run(void);

private:
  void                 PlayNext(void);
  void                 FadeOut(float elapsed_time);

  Data                 _data;
  DataTree*            _data_tree;
  PT(AudioManager)     _audio_manager;
  PT(AudioSound)       _current_music, _next_music;
  bool                 _fading_out;
  std::string          _current_category;
  Timer                _timer;
};

#endif
