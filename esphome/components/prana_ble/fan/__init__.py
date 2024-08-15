import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import (
    CONF_ID,
)
from .. import (
    PRANA_BLE_CLIENT_SCHEMA,
    prana_ble_ns,
    register_prana_child,
)

_LOGGER = logging.getLogger(__name__)
CODEOWNERS = ["@voed"]
DEPENDENCIES = ["prana_ble"]

PranaBLEFan = prana_ble_ns.class_("PranaBLEFan", fan.Fan, cg.PollingComponent)


CONF_FANS = "fans"
CONF_FAN_IN = "inlet_fan"
CONF_FAN_OUT = "outlet_fan"
CONF_HAS_AUTO = "has_auto"

CONFIG_SCHEMA = (
    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PranaBLEFan),
            cv.Optional(CONF_HAS_AUTO, default=False) : cv.boolean
        }
    )
    .extend(cv.polling_component_schema("1s"))
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    await register_prana_child(var, config)

    if CONF_HAS_AUTO in config:
        cg.add(var.set_has_auto(config[CONF_HAS_AUTO]))
