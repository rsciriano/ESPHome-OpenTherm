from symbol import import_as_name
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor
from esphome import pins, core
from esphome.const import (
    CONF_ID,
)
from esphome.core import coroutine_with_priority
from esphome.cpp_generator import RawExpression
import logging

from . import opentherm_component_schema, set_hotwater_climate

DEPENDENCIES = ["opentherm"]

_LOGGER = logging.getLogger(__name__)

CONF_HOT_WATER = "hotwater"

opentherm_ns = cg.esphome_ns.namespace("opentherm")
OpenThermClimate = opentherm_ns.class_("OpenthermClimate", climate.Climate, cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            #cv.GenerateID(): cv.declare_id(OpenThermClimate),
            #cv.Optional(CONF_HOT_WATER): climate.CLIMATE_SCHEMA.extend(
            cv.Optional(CONF_HOT_WATER): cv.Schema({}).extend(
                {
                    cv.GenerateID(): cv.declare_id(OpenThermClimate),
                }
            ).extend(opentherm_component_schema())
        }
    )
    .extend(opentherm_component_schema())
    .extend(cv.COMPONENT_SCHEMA)
)


@coroutine_with_priority(1.0)
async def to_code(config):

    _LOGGER.info("to_code: %s", config)


    if CONF_HOT_WATER in config:
        hotwater_climate = cg.new_Pvariable(config[CONF_HOT_WATER][CONF_ID])
#        await cg.register_component(hotwater_climate, config[CONF_HOT_WATER])
#        await climate.register_climate(hotwater_climate, config[CONF_HOT_WATER])
        await set_hotwater_climate(hotwater_climate, config[CONF_HOT_WATER])
