import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_PRESSURE,
    CONF_TEMPERATURE,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_HECTOPASCAL,
    CONF_IIR_FILTER,
    CONF_OVERSAMPLING,
)

bmp280_ns = cg.esphome_ns.namespace("bmp280")
BMP280Component = bmp280_ns.class_(
    "BMP280Component", cg.PollingComponent
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BMP280Component),
            #cv.Optional(CONF_HOT_WATER): climate.CLIMATE_SCHEMA.extend(
            #cv.Optional(CONF_HOT_WATER): cv.Schema({}).extend(
            #    {
            #        #cv.GenerateID(): cv.declare_id(OpenThermClimate),
            #    }
            #)
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)