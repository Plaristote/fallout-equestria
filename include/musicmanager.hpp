#ifndef  MUSICMANAGER_HPP
# define MUSICMANAGER_HPP

# ifndef AUDIO_BACKEND_SFML
#  ifndef AUDIO_BACKEND_PANDA3D
#    define AUDIO_BACKEND_SFML
#  endif
# endif

# include <string>
# include "datatree.hpp"
# include "timer.hpp"

# ifdef AUDIO_BACKEND_PANDA3D
#  include <panda3d/audioManager.h>
# endif
# ifdef AUDIO_BACKEND_SFML
#  include <SFML/Audio.hpp>
# endif

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
  void                 PlayNext(void);
  void                 Run(void);
  void                 SetVolume(float volume);

private:
  void                 FadeOut(float elapsed_time);
  void                 FadeVolume(float elapsed_time);

  Data                 _data;
  DataTree*            _data_tree;
  std::string          _current_category;
  Timer                _timer;
  bool                 _fading_out;
  float                _volume_goal;
  float                _volume_ref;
# ifdef AUDIO_BACKEND_PANDA3D
  PT(AudioManager)     _audio_manager;
  PT(AudioSound)       _current_music, _next_music;
# endif
# ifdef AUDIO_BACKEND_SFML
  sf::Music            *_current_music, *_next_music;
# endif
};

#endif
