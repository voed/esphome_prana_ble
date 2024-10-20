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


PranaBLENumber = prana_ble_ns.class_("PranaBLENumber", cg.PollingComponent)


CONFIG_SCHEMA = cv.All(
    number.number_schema(PranaBLENumber)
    .extend(
        {
#            cv.GenerateID(): cv.use_id(PranaBLENumber),
        }
    )
    .extend(cv.polling_component_schema("1min"))
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await number.register_number(
        var,
        config,
        min_value=1,
        max_value=6,
        step=1,
    )
    await register_prana_child(var, config)