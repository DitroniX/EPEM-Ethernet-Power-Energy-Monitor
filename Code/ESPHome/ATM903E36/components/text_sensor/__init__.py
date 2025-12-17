import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PHASE_A, CONF_PHASE_B, CONF_PHASE_C

from ..sensor import ATM90E36Component

CONF_PHASE_STATUS = "phase_status"
CONF_FREQUENCY_STATUS = "frequency_status"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(ATM90E36Component),
        cv.Optional(CONF_PHASE_STATUS): text_sensor.text_sensor_schema(
            icon="mdi:flash-alert"
        ),
        cv.Optional(CONF_FREQUENCY_STATUS): text_sensor.text_sensor_schema(
            icon="mdi:lightbulb-alert"
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ID])

    if phase_cfg := config.get(CONF_PHASE_STATUS):
        sens = await text_sensor.new_text_sensor(phase_cfg)
        cg.add(parent.set_phase_status_text_sensor(sens))

    if freq_status_config := config.get(CONF_FREQUENCY_STATUS):
        sens = await text_sensor.new_text_sensor(freq_status_config)
        cg.add(parent.set_freq_status_text_sensor(sens))
