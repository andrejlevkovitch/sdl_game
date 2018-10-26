// player.hpp

#pragma once

#include <cstdint>
#include <map>
#include <string>

#include <SDL2/SDL.h>

namespace levi {
struct sound {
  sound();
  /**\param loop if you want, that sound play everytime, set true*/
  sound(uint8_t *buff_, int length_, float volume, bool loop);

  uint8_t *buff;
  uint8_t *cur_pos;
  int length;
  int cur_length;
  /**\brief relatively max volume*/
  float volume;
  bool loop;
  bool for_playing;
};

using sound_map = std::map<std::string, sound>;

class player {
public:
  /**\except if can't open audio device*/
  static player &instance();
  ~player();
  /**\brief parse xml file and load sounds from it
   * \param file_name file name + way to the file
   * \return count of loaded sounds*/
  size_t parse_file(const std::string &file_name);
  /**\return true if sound was storage, else - false
   * \warning if storage already have loaded sound with same alias, this sound
   * will not be loaded, but return value will be true!
   * \param loop if you want, that sound play everytime, set true*/
  bool load_audio_in_storage(const std::string &alias,
                             const std::string &file_name, float volume,
                             bool loop = false);
  /**\brief set sound to play
   * \return false if can't find sound
   * \except if can't load file or file haven't root*/
  bool play(const std::string &alias, bool status);
  /**\brief if you wan't pause all sounds, set true. By default audio is on
   * pause*/
  void pause(bool status);

private:
  player();
  player(const player &) = delete;
  player &operator=(const player &) = delete;

private:
  sound_map sound_map_;
  ::SDL_AudioSpec want_;
  ::SDL_AudioDeviceID device_;
};
}; // namespace levi
