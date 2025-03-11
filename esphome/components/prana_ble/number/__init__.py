from esphome.components import number
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_ID,
)

from .. import (
    PRANA_BLE_CLIENT_SCHEMA,
    prana_ble_ns,
    register_prana_child,
)

DEPENDENCIES = ["prana_ble"]
CODEOWNERS = ["@voed"]

ICON_BRIGHTNESS = "mdi:television-ambient-light"

PranaBLENumber = prana_ble_ns.class_("PranaBLENumber", number.Number, cg.Component)


CONFIG_SCHEMA = (
    number.number_schema(
        PranaBLENumber,
        icon=ICON_BRIGHTNESS
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)


async def to_code(config):
    var = await number.new_number(
        config,
        min_value=1,
        max_value=6,
        step=1,
    )
    await cg.register_component(var, config)
    await register_prana_child(var, config)