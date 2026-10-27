#include "waveshare4b_audio.h"

#include <cmath>

namespace esphome::waveshare4b_audio {

bool Waveshare4bAudio::settings(tessera::SettingsPage &page) {
  page.icon = "\U000F057E";  // mdi:volume-high
  media_player::MediaPlayer *player = player_;
  page.number(text("volume"), 0, 100, 5, "%", [player] { return (int) lroundf(player->volume * 100.0f); },
              [player](int value) { player->make_call().set_volume(value / 100.0f).perform(); });
  switch_::Switch *microphone_on = microphone_on_;
  page.toggle(text("microphone"), [microphone_on] { return microphone_on->state; },
              [microphone_on](bool on) { on ? microphone_on->turn_on() : microphone_on->turn_off(); });
  return true;
}

}  // namespace esphome::waveshare4b_audio
