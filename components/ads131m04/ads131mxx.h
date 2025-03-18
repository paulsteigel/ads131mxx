// ads131mxx.h
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/gpio/gpio_pin.h"


namespace esphome {
namespace ads131mxx {

class ADS131MXXComponent : public Component, public spi::SPIDevice {
 public:
  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_reset_pin(gpio::GPIOPin *reset_pin) { reset_pin_ = reset_pin; }
  void set_drdy_pin(gpio::GPIOPin *drdy_pin) { drdy_pin_ = drdy_pin; }  // Added DRDY pin setter
  void set_gain(int gain) { gain_ = gain; }
  void set_reference_voltage(float reference_voltage) { reference_voltage_ = reference_voltage; }
  void set_data_rate(int data_rate) { data_rate_ = data_rate; }

  float get_setup_priority() const override { return setup_priority::DATA; }

  void setup() override;
  void loop() override;

 protected:
  virtual int32_t read_adc(uint8_t channel) = 0; // Abstract method for reading ADC data

  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  gpio::GPIOPin *reset_pin_{nullptr};
  gpio::GPIOPin *drdy_pin_{nullptr};  // Added DRDY pin member
  int gain_{1};
  float reference_voltage_{2.5f};
  int data_rate_{1000};
};

}  // namespace ads131mxx
}  // namespace esphome
