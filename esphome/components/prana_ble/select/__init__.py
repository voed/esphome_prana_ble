import esphome.codegen as cg # type: ignore
from esphome.components import select
import esphome.config_validation as cv
from .. import (
    PRANA_BLE_CLIENT_SCHEMA,
    prana_ble_ns,
    register_prana_child,
)

DEPENDENCIES = ["prana_ble"]
CODEOWNERS = ["@voed"]

CONF_DISPLAY_MODE = "display_mode"
CONF_FAN_MODE = "fan_mode"

ICON_DISPLAY = "mdi:unfold-more-vertical"
ICON_FAN_AUTO="mdi:fan-auto"
PranaBLEDisplaySelect = prana_ble_ns.class_("PranaBLEDisplaySelect", select.Select, cg.Component)
PranaBLEFanSelect = prana_ble_ns.class_("PranaBLEFanSelect", select.Select, cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Optional(CONF_DISPLAY_MODE): select.select_schema(
                PranaBLEDisplaySelect,
                icon=ICON_DISPLAY
            ),
            cv.Optional(CONF_FAN_MODE): select.select_schema(
                PranaBLEFanSelect,
                icon=ICON_FAN_AUTO
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(PRANA_BLE_CLIENT_SCHEMA)
)

async def to_code(config):
    if CONF_DISPLAY_MODE in config:
        sel = await select.new_select(
            config[CONF_DISPLAY_MODE],
            options=["Fan", "Temp inside", "Temp outside", "CO2", "VOC", "Humidity", "Air quality", "Pressure", "Date", "Time"]
        )
        await cg.register_component(sel, config)
        await register_prana_child(sel, config)

    if CONF_FAN_MODE in config:
        sel = await select.new_select(
            config[CONF_FAN_MODE],
            options=["Manual", "Auto", "Auto+"]
        )
        await cg.register_component(sel, config)
        await register_prana_child(sel, config)

