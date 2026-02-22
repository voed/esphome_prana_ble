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



CONF_HAS_AUTO = "has_auto"
CONF_FAN_TYPE = "fan_type"

CONF_FAN_IN = "fan_in"
CONF_FAN_OUT = "fan_out"
CONF_FAN_BOTH = "fan_both"
CONF_DIRECT = "direct_control"
CONF_AUTO_LOCK = "auto_lock"
ICON_FAN_CHEVRON_UP = "mdi:fan-chevron-up"
ICON_FAN_CHEVRON_DOWN = "mdi:fan-chevron-down"

PRANA_FAN_TYPE = {
    CONF_FAN_IN: PranaFan.FAN_IN,
    CONF_FAN_OUT: PranaFan.FAN_OUT,
    CONF_FAN_BOTH: PranaFan.FAN_BOTH
}

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
            cv.Optional(CONF_DIRECT, default=False): cv.boolean,
            cv.Optional(CONF_AUTO_LOCK, default=True): cv.boolean,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)


async def to_code(config):
    for conf_key, fan_type in PRANA_FAN_TYPE.items():
        if conf_key not in config:
            continue

        var = await fan.new_fan(config[conf_key])
        await cg.register_component(var, config)

        cg.add(var.set_fan_type(fan_type))
        cg.add(var.set_speed_count(config[CONF_SPEED_COUNT]))
        cg.add(var.set_fan_direct(config[CONF_DIRECT]))
        cg.add(var.set_auto_lock(config[CONF_AUTO_LOCK]))

        await register_prana_child(var, config)