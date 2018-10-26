// player.cpp

#include "player.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <stdexcept>
#include <tinyxml.h>

namespace levi {
// I create this struct before I don't want include sdl in header file
struct audio_settings {
  ::SDL_AudioSpec want_;
  ::SDL_AudioDeviceID device_;
};
}; // namespace levi

levi::sound::sound()
    : buff{}, cur_pos{buff}, length{}, cur_length{length}, loop{false},
      for_playing{false} {}

levi::sound::sound(uint8_t *buff_, int length_, float volume_, bool loop_)
    : buff{buff_}, cur_pos{buff_}, length{length_},
      cur_length{length}, volume{volume_}, loop{loop_}, for_playing{false} {}

namespace levi {
void my_callback(void *data, Uint8 *stream, int length) {
  ::SDL_memset(stream, 0, length);
  for (auto &i : *reinterpret_cast<sound_map *>(data)) {
    if (i.second.for_playing) {
      if (length > i.second.cur_length) {
        i.second.cur_length = i.second.length;
        i.second.cur_pos = i.second.buff;
        if (!i.second.loop) {
          i.second.for_playing = false;
          continue;
        }
      }

      ::SDL_MixAudioFormat(stream, i.second.cur_pos, AUDIO_FORMAT, length,
                           SDL_MIX_MAXVOLUME * i.second.volume);

      i.second.cur_length -= length;
      i.second.cur_pos += length;
    }
  }
}

struct locker {
  locker(::SDL_AudioDeviceID &device) : device_{device} {
    ::SDL_LockAudioDevice(device_);
  };
  ~locker() { ::SDL_UnlockAudioDevice(device_); };
  locker(const locker &) = delete;
  locker &operator=(const locker &) = delete;

private:
  ::SDL_AudioDeviceID &device_;
};
}; // namespace levi

levi::player &levi::player::instance() {
  try {
    static player retval{};
    return retval;
  } catch (std::exception) {
    throw;
  }
}

levi::player::player() : audio_settings_{new audio_settings} {
  ::SDL_memset(&audio_settings_->want_, 0, sizeof(audio_settings_->want_));
  audio_settings_->want_.freq = 48000;
  audio_settings_->want_.format = AUDIO_FORMAT;
  audio_settings_->want_.channels = 2;
  audio_settings_->want_.samples = 4096;
  audio_settings_->want_.callback = my_callback;
  audio_settings_->want_.userdata = &sound_map_;

  audio_settings_->device_ = ::SDL_OpenAudioDevice(
      NULL, 0, &audio_settings_->want_, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
  if (!audio_settings_->device_) {
    throw std::runtime_error{::SDL_GetError()};
  }
}

levi::player::~player() {
  ::SDL_PauseAudioDevice(audio_settings_->device_, 1);
  ::SDL_CloseAudioDevice(audio_settings_->device_);
  for (auto &i : sound_map_) {
    ::SDL_FreeWAV(i.second.buff);
  }
  delete audio_settings_;
}

size_t levi::player::parse_file(const std::string &file_with_sounds) {
  size_t count{};

  std::string way_to_files = file_with_sounds;
  auto last_slesh = std::find(way_to_files.rbegin(), way_to_files.rend(), '/');
  way_to_files.erase(last_slesh.base(), way_to_files.rbegin().base());

  ::TiXmlDocument doc;
  if (!doc.LoadFile(file_with_sounds)) {
    throw std::runtime_error{"couldn't load file: " + file_with_sounds};
  }

  auto root = doc.RootElement();
  if (!root) {
    throw std::runtime_error{"couldn't get root in file: " + file_with_sounds};
  }

  for (auto i = root->FirstChildElement(); i != nullptr;
       i = i->NextSiblingElement()) {
    const char *pointer_to_text{};
    std::string alias;
    std::string file_name = way_to_files;
    int loop{};
    double volume{};

    pointer_to_text = i->Attribute("alias");
    if (pointer_to_text) {
      alias = pointer_to_text;
    }

    pointer_to_text = i->Attribute("file_name");
    if (pointer_to_text) {
      file_name += pointer_to_text;
    }

    i->Attribute("loop", &loop);
    i->Attribute("volume", &volume);

    try {
      if (load_audio_in_storage(alias, file_name, volume, loop)) {
        ++count;
      }
    } catch (std::exception) {
      continue;
    }
  }

  return count;
}

bool levi::player::load_audio_in_storage(const std::string &alias,
                                         const std::string &file_name,
                                         float volume, bool loop) {
  locker(audio_settings_->device_);
  if (sound_map_.find(alias) == sound_map_.end()) {
    Uint32 length{};
    Uint8 *temp_buf{nullptr};
    if (SDL_LoadWAV(file_name.c_str(), &audio_settings_->want_, &temp_buf,
                    &length) == nullptr) {
      std::cerr << ::SDL_GetError() << std::endl;
      return false;
    }
    sound_map_.insert(
        std::make_pair(alias, sound(temp_buf, length, volume, loop)));
    return true;
  }
  return false;
}

bool levi::player::play(const std::string &alias, bool status) {
  locker(audio_settings_->device_);
  auto iter = sound_map_.find(alias);
  if (iter == sound_map_.end()) {
    return false;
  } else {
    iter->second.for_playing = status;
    return true;
  }
}

void levi::player::pause(bool status) {
  ::SDL_PauseAudioDevice(audio_settings_->device_, status);
}
