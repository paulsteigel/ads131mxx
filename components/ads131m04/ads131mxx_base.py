import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, spi
from esphome.const import (
    CONF_ID,
    CONF_VOLTAGE,
    CONF_CURRENT,
    UNIT_VOLT,
    UNIT_AMPERE,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
)

DEPENDENCIES = ["spi"]
CODEOWNERS = ["Your Name Here"]

CONF_ADS131MXX_ID = "ads131mxx_id"
CONF_RESET_PIN = "reset_pin"
CONF_DRDY_PIN = "drdy_pin"  # Added DRDY pin config
CONF_GAIN = "gain"
CONF_REFERENCE_VOLTAGE = "reference_voltage"
CONF_DATA_RATE = "data_rate"

ads131mxx_ns = cg.esphome_ns.namespace("ads131mxx")
ADS131MXXComponent = ads131mxx_ns.class_("ADS131MXXComponent", cg.Component, spi.SPIDevice)  # Inherit from SPIDevice

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ADS131MXXComponent),
        cv.Required(CONF_VOLTAGE): cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(sensor.Sensor),
            }
        ),
        cv.Required(CONF_CURRENT): cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(sensor.Sensor),
            }
        ),
        cv.Optional(CONF_RESET_PIN): cv.use_id(cg.GPIOPin),
        cv.Optional(CONF_DRDY_PIN): cv.use_id(cg.GPIOPin),  # Added DRDY pin
        cv.Optional(CONF_GAIN, default=1): cv.int_range(min=1),  # Add validation for gain values (consult datasheet)
        cv.Optional(CONF_REFERENCE_VOLTAGE, default=2.5): cv.float_,  # Example: 2.5V reference
        cv.Optional(CONF_DATA_RATE, default=1000): cv.int_, # Example: 1000 SPS
    }
).extend(cv.COMPONENT_SCHEMA).extend(spi.spi_device_schema(cs_pin_required=True))  # Extend with SPI device schema



async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await spi.register_spi_device(var, config) # register spi device

    # Voltage Sensor
    voltage_config = config[CONF_VOLTAGE]
    sens = cg.new_Pvariable(voltage_config[CONF_ID], sensor.Sensor)
    cg.add(var.set_voltage_sensor(sens))
    await sensor.register_sensor(sens, voltage_config)
    cg.add(sens.set_unit_of_measurement(UNIT_VOLT))
    cg.add(sens.set_state_class(STATE_CLASS_MEASUREMENT))
    cg.add(sens.set_device_class(DEVICE_CLASS_VOLTAGE))

    # Current Sensor
    current_config = config[CONF_CURRENT]
    sens = cg.new_Pvariable(current_config[CONF_ID], sensor.Sensor)
    cg.add(var.set_current_sensor(sens))
    await sensor.register_sensor(sens, current_config)
    cg.add(sens.set_unit_of_measurement(UNIT_AMPERE))
    cg.add(sens.set_state_class(STATE_CLASS_MEASUREMENT))
    cg.add(sens.set_device_class(DEVICE_CLASS_CURRENT))

    # Reset Pin (If defined)
    if CONF_RESET_PIN in config:
      reset_pin = await cg.get_pin(config[CONF_RESET_PIN])
      cg.add(var.set_reset_pin(reset_pin))

    # DRDY Pin (If defined)
    if CONF_DRDY_PIN in config:
        drdy_pin = await cg.get_pin(config[CONF_DRDY_PIN])
        cg.add(var.set_drdy_pin(drdy_pin)) #Set the drdy_pin to the ADS131MXX component

    # Configuration Values
    cg.add(var.set_gain(config[CONF_GAIN]))
    cg.add(var.set_reference_voltage(config[CONF_REFERENCE_VOLTAGE]))
    cg.add(var.set_data_rate(config[CONF_DATA_RATE]))
