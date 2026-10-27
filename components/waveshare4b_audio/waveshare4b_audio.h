#pragma once

#include "esphome/core/component.h"
#include "esphome/components/media_player/media_player.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/smart_display/plugin_api.h"

namespace esphome::waveshare4b_audio {

// The plugin: the speaker's volume and the microphone's switch on the screen's settings page, as Tessera's own boards
// with audio have them. The audio is the components of plugin.yaml; these rows talk to them directly, so they work
// while Home Assistant is away.
class Waveshare4bAudio : public Component, public tessera::Plugin {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  bool settings(tessera::SettingsPage &page) override;

  void set_media_player(media_player::MediaPlayer *player) { player_ = player; }
  void set_microphone_on(switch_::Switch *microphone_on) { microphone_on_ = microphone_on; }

 protected:
  media_player::MediaPlayer *player_{nullptr};
  switch_::Switch *microphone_on_{nullptr};
};

}  // namespace esphome::waveshare4b_audio
