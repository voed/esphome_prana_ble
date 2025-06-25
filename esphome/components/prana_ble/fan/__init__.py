import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_SPEED_COUNT,
    CONF_ENTITY_CATEGORY,
    CONF_ICON
)
from .. import (
    PRANA_BLE_CLIENT_SCHEMA,
    CONF_PRANA_ID,
    prana_ble_ns,
    PranaBLEHub,
    register_prana_child,
)


_LOGGER = logging.getLogger(__name__)
CODEOWNERS = ["@voed"]
DEPENDENCIES = ["prana_ble"]

PranaBLEFan = prana_ble_ns.class_("PranaBLEFan", fan.Fan, cg.Component)
PranaFan = prana_ble_ns.enum("PranaFan")

PRANA_FAN_TYPE = {
    "INLET": PranaFan.FAN_IN,
    "OUTLET": PranaFan.FAN_OUT,
    "BOTH": PranaFan.FAN_BOTH
}

CONF_HAS_AUTO = "has_auto"
CONF_FAN_TYPE = "fan_type"

CONF_FAN_IN = "fan_in"
CONF_FAN_OUT = "fan_out"
CONF_FAN_BOTH = "fan_both"
CONF_DIRECT = "direct_control"
ICON_FAN_CHEVRON_UP = "mdi:fan-chevron-up"
ICON_FAN_CHEVRON_DOWN = "mdi:fan-chevron-down"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Optional(CONF_FAN_IN): fan.fan_schema(
                PranaBLEFan,
                icon=ICON_FAN_CHEVRON_DOWN
            ),
            cv.Optional(CONF_FAN_OUT): fan.fan_schema(
                PranaBLEFan,
                icon=ICON_FAN_CHEVRON_UP
            ),
            cv.Optional(CONF_FAN_BOTH): fan.fan_schema(PranaBLEFan),
            cv.Optional(CONF_SPEED_COUNT, default=10): cv.int_range(1, 10),
            cv.Optional(CONF_DIRECT, default=False): cv.boolean
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)



async def to_code(config):
    if CONF_FAN_IN in config:
        var = await fan.new_fan(config[CONF_FAN_IN]);
        await cg.register_component(var, config)
        cg.add(var.set_fan_type(PranaFan.FAN_IN))
        cg.add(var.set_speed_count(config[CONF_SPEED_COUNT]))
        cg.add(var.set_fan_direct(config[CONF_DIRECT]))
        await register_prana_child(var, config)

    if CONF_FAN_OUT in config:
        var = await fan.new_fan(config[CONF_FAN_OUT]);
        await cg.register_component(var, config)
        cg.add(var.set_fan_type(PranaFan.FAN_OUT))
        cg.add(var.set_speed_count(config[CONF_SPEED_COUNT]))
        cg.add(var.set_fan_direct(config[CONF_DIRECT]))
        await register_prana_child(var, config)

    if CONF_FAN_BOTH in config:
        var = await fan.new_fan(config[CONF_FAN_BOTH]);
        await cg.register_component(var, config)
        cg.add(var.set_fan_type(PranaFan.FAN_BOTH))
        cg.add(var.set_speed_count(config[CONF_SPEED_COUNT]))
        cg.add(var.set_fan_direct(config[CONF_DIRECT]))
        await register_prana_child(var, config)
