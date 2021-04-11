import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor
from .. import Nilan, CONF_NILAN_ID
from esphome.const import (
    CONF_ID,
    CONF_SENSOR,
)
 
nilan_ns = cg.esphome_ns.namespace('nilan')
NilanClimate = nilan_ns.class_('NilanClimate', climate.Climate, cg.Component)
 
CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(NilanClimate),
    cv.GenerateID(CONF_NILAN_ID): cv.use_id(Nilan),
    cv.Required(CONF_SENSOR): cv.use_id(sensor.Sensor),
}).extend(cv.COMPONENT_SCHEMA)
 
def to_code(config):
    nilan = yield cg.get_variable(config[CONF_NILAN_ID])
    var = cg.new_Pvariable(config[CONF_ID], nilan)
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
 
    sens = yield cg.get_variable(config[CONF_SENSOR])
    cg.add(var.set_sensor(sens))
