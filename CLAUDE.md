# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

@AGENTS.md

## What this is

The Tessera plugin `waveshare4b_audio` (scaffolded with `tools/new_plugin.py`). It gives the **Waveshare
ESP32-S3-Touch-LCD-4B** its microphone, speaker and media player, which Tessera's experimental board file
(`waveshare4b`) leaves unused.

### Why it is a hardware plugin and not a voice-assistant fork

Upstream `voice_assist` needs `requires.features: [microphone, speaker, media_player]` and uses
`ts_microphone` / `ts_speaker` / `ts_media_player` without making them. Boards with audio get these from the
Tessera core (`packages/features/audio.yaml`). The 4B board brings none, so `voice_assist` fit no 4B screen.
This plugin `provides` those three features for `boards: [waveshare4b]`. With it, upstream `voice_assist`
runs unchanged, and Tessera brings this plugin along when someone adds `voice_assist` to a 4B. Don't add wake word
or Assist logic here. That belongs to `voice_assist`.

## Repos beside this one (in `..\`)

- `../tessera-plugins`: rules, docs and tools (`AGENTS.md` above points there). Upstream `voice_assist` is in
  `plugins/voice_assist`.
- `../tessera-core`: a dev checkout of Tessera (`MaxGramser/homeassistant_espscreen`). It is the reference for
  the 4B board (`packages/boards/waveshare-esp32s3-4b.yaml`) and for how the core builds audio on one-I2S-bus boards.
  `plugin.yaml` mirrors these core files: `packages/features/audio.yaml`, `audio-player.yaml`,
  `audio-half-duplex.yaml`, `audio-amplifier.yaml`, and `packages/hardware/parts/es7210.yaml`, `es8311.yaml`,
  `amplifier.yaml`. When the core changes those, compare.
- `../esp32-s3-touch-lcd-4b`: the user's working plain-ESPHome config for this board, with Assist and the wake word
  working (`upstairs-panel/upstairs-panel.yaml`, plus its `CLAUDE.md`). It is the source of truth for pins, gain
  and audio behaviour.

## Commands

```sh
python ../tessera-plugins/tools/check.py .     # must end in "ok"; run after every change
```

The machine's Python is 3.9 (`python`, not `python3`), and there is no local ESPHome. The preview bench does not
build audio plugins, so `check.py` is the only local check. The real test is a build on the screen
(`../tessera-plugins/docs/TESTING.md`): push to GitHub and add the repo link in the Tessera app, or copy this folder
to Home Assistant's `config/tessera-plugins/waveshare4b_audio/`.

## How plugin.yaml fits the board

- It **reuses** the board file's `touch_bus` (I2C, GPIO47/48) and `expander` (TCA9554). It never declares them
  again.
- The only `ts_` ids it makes are the three features. Everything else is prefixed `waveshare4b_audio_` (enforced
  by `check.py`). `plugin.yaml` may not set `substitutions`, `packages`, `esphome`, `logger` or the like
  (`../tessera-plugins/docs/LIMITS.md`). That is why the core's audio files are copied in literally, with numbers
  instead of `${...}`.
- The core's own audio uses core-only hooks (`BOOT_AUDIO`, `audio_owner`, `live_view`, `settings_screen`). They
  are left out. The volume and microphone rows on the settings page come from this plugin's
  `settings()` instead.

## Board traps (details in `../esp32-s3-touch-lcd-4b/CLAUDE.md`)

- I2S: MCLK 5, BCLK 16, LRCLK 7, speaker DOUT 6, mic DIN 15. GPIO15/7 are audio on the 4B, never I2C.
- Amp enable is expander line 3 (EXIO3), high for on. Without it the DAC runs and nothing is heard. Here it follows
  playback (the 20 ms interval), not always-on as in the ESPHome config.
- Mic and speaker **share one I2S bus** and therefore one rate: everything runs at 16 kHz, and resamplers adapt
  other rates. The 50 ms interval stops the mic whenever the speaker wants the bus. In the plain-ESPHome config
  the same problem showed up as "wake word works exactly once", fixed there with a `rearm_wake_word` script.
- ES7210 `mic_gain: 24db`. At 33 dB, speech across the room set the wake word off.
- Check ESPHome options against the installed release tag, not `dev`/esphome.io. The user's HA add-on runs the
  beta channel. ESPHome rejects duplicate YAML keys that PyYAML would accept.
- `No API client connected` / `pipeline does not support speech-to-text` are setup problems on the Home Assistant
  side, not faults in the plugin.
