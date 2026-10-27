# Waveshare 4B audio

The Waveshare ESP32-S3-Touch-LCD-4B has a microphone and a speaker on the board, but Tessera leaves them switched off
on this screen. This plugin turns them on. The screen then has the microphone, speaker and media player that other
plugins need, so the **Voice assistant** plugin works on it unchanged. Home Assistant can also play announcements and
text to speech on it.

## What you need

- A Waveshare ESP32-S3-Touch-LCD-4B running Tessera. This is not the older ESP32-S3-Touch-LCD-4 without the "B": that
  board has its audio on other pins.
- The speaker that comes attached to the board.

## Set up

1. In Tessera, open **Plugins**, choose **Waveshare 4B audio** and tick your 4B screen.
2. Add it. The screen builds once.
3. For a voice assistant, add the **Voice assistant** plugin to the same screen too, then follow its own set up.
   If you add Voice assistant first, Tessera brings this plugin along with it.
4. In Home Assistant the screen's device now has a **Media Player** and a **Microphone** switch.

## On the screen

Hold the top bar, then **Settings**, **Plugins**, **Audio**: the speaker's volume and the microphone's switch. These
work on the screen itself, so they also work while Home Assistant is away.

## Good to know

- The microphone and the speaker share one audio connection on this board, so they take turns. The microphone stops
  listening while the screen plays a sound and starts again half a second after it ends. A wake word is not heard
  while an answer is playing.
- The speaker's amplifier is on only while a sound plays, so the speaker does not hiss between sounds.
- The microphone gain is 24 dB. A higher gain let speech from across the room set the wake word off.
- Pins: I2S MCLK GPIO5, BCLK GPIO16, LRCLK GPIO7, speaker data GPIO6, microphone data GPIO15. The ES8311 (speaker,
  0x18) and the ES7210 (microphones, 0x40) are on the touch panel's I2C bus. The amplifier is switched on by the IO
  expander's line EXIO3.
- Tessera's own support for the 4B is still experimental.
