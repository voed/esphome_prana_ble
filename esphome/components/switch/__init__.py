import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
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

PranaBLESwitch = prana_ble_ns.class_("PranaBLESwitch", switch.Switch, cg.PollingComponent)
PranaSwitchType = prana_ble_ns.enum("PranaSwitchType")

CONF_ENABLE = "enable"
CONF_HEATING = "heating"
CONF_WINTER_MODE = "winter_mode"
CONF_AUTO_MODE = "auto_mode"
CONF_CONNECT = "connect"


CONFIG_SCHEMA = (
    cv.Schema(
        {
            #cv.GenerateID(): cv.declare_id(PranaBLESwitch),
            cv.Required(CONF_CONNECT): switch.switch_schema(PranaBLESwitch),
            cv.Optional(CONF_ENABLE): switch.switch_schema(PranaBLESwitch),
            cv.Optional(CONF_HEATING): switch.switch_schema(PranaBLESwitch),
            cv.Optional(CONF_WINTER_MODE): switch.switch_schema(PranaBLESwitch),
            cv.Optional(CONF_AUTO_MODE): switch.switch_schema(PranaBLESwitch)
        }
    )
    .extend(cv.polling_component_schema("1s"))
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)

async def to_code(config):

    if CONF_CONNECT in config:
        sw = await switch.new_switch(config[CONF_CONNECT])
        await cg.register_component(sw, config)
        cg.add(sw.set_switch_type(PranaSwitchType.CONNECT))
        await register_prana_child(sw, config)
    if CONF_ENABLE in config:
        sw = await switch.new_switch(config[CONF_ENABLE])
        await cg.register_component(sw, config)
        cg.add(sw.set_switch_type(PranaSwitchType.ENABLE))
        await register_prana_child(sw, config)

    if CONF_HEATING in config:
        sw = await switch.new_switch(config[CONF_HEATING])
        await cg.register_component(sw, config)
        cg.add(sw.set_switch_type(PranaSwitchType.HEAT))
        await register_prana_child(sw, config)

    if CONF_WINTER_MODE in config:
        sw = await switch.new_switch(config[CONF_WINTER_MODE])
        await cg.register_component(sw, config)
        cg.add(sw.set_switch_type(PranaSwitchType.WINTER))
        await register_prana_child(sw, config)