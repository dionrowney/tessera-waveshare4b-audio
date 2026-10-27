"""Waveshare 4B audio: the speaker's volume and the microphone's switch on the screen's own settings page.

The audio itself is ESPHome's components in plugin.yaml; this component only knows the media player and the switch, so
the settings page can change them on the glass. register_plugin() takes the id, version and the screen's texts from the
manifest and translations/.
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import media_player, smart_display, switch
from esphome.const import CONF_ID

DEPENDENCIES = ["smart_display", "media_player", "switch"]

CONF_MEDIA_PLAYER = "media_player"
CONF_MICROPHONE_ON = "microphone_on"

waveshare4b_audio_ns = cg.esphome_ns.namespace("waveshare4b_audio")
Waveshare4bAudio = waveshare4b_audio_ns.class_("Waveshare4bAudio", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Waveshare4bAudio),
        cv.Required(CONF_MEDIA_PLAYER): cv.use_id(media_player.MediaPlayer),
        cv.Required(CONF_MICROPHONE_ON): cv.use_id(switch.Switch),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await smart_display.register_plugin(var, __file__)
    cg.add(var.set_media_player(await cg.get_variable(config[CONF_MEDIA_PLAYER])))
    cg.add(var.set_microphone_on(await cg.get_variable(config[CONF_MICROPHONE_ON])))
