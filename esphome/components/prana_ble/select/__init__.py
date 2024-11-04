import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
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



PranaBLESelect = prana_ble_ns.class_("PranaBLESelect", select.Select, cg.Component)


CONFIG_SCHEMA = (
    select.select_schema(PranaBLESelect)
    .extend(cv.COMPONENT_SCHEMA)
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)


async def to_code(config):
    var = await select.new_select(
        config,
        options=["Fan", "Temp inside", "Temp outside", "CO2", "VOC", "Humidity", "Air quality", "Pressure", "Date", "Time"]
    )
    await cg.register_component(var, config)
    await register_prana_child(var, config)