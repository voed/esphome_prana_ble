import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_SPEED_COUNT
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
CONFIG_SCHEMA = (

    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PranaBLEFan),
            cv.Optional(CONF_HAS_AUTO) : cv.boolean,
            cv.Optional(CONF_SPEED_COUNT, default=10) : cv.int_range(1, 10),
            cv.Required(CONF_FAN_TYPE) : cv.enum(PRANA_FAN_TYPE, upper=True)
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    f = await fan.register_fan(var, config)
    await cg.register_component(var, config)
    cg.add(var.set_has_auto(config[CONF_HAS_AUTO]))
    cg.add(var.set_fan_type(config[CONF_FAN_TYPE]))
    cg.add(var.set_speed_count(config[CONF_SPEED_COUNT]))
    await register_prana_child(var, config)

