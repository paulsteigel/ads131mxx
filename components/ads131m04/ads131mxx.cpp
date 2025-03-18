// ads131mxx.cpp
#include "ads131mxx.h"
#include "esphome/core/log.h"
#include <numeric>

namespace esphome {
namespace ads131mxx {

static const char *const TAG = "ads131mxx";

void ADS131MXXComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ADS131MXX...");
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(true); // Take ADS131MXX out of reset
  }

  if (this->drdy_pin_ != nullptr) {
    this->drdy_pin_->setup();
    this->drdy_pin_->set_mode(gpio::INPUT);
  }
  // Perform initial ADS131MXX configuration here (SPI commands, etc.)
  // For example: set gain, data rate, etc.
}

void ADS131MXXComponent::loop() {
  // Check DRDY pin if it's configured
  if (this->drdy_pin_ != nullptr) {
    if (this->drdy_pin_->digital_read()) { // DRDY is active (HIGH or LOW, check datasheet)
      // Read data from ADS131MXX
      int32_t voltage_adc_value = this->read_adc(0); // Assuming channel 0 is voltage
      int32_t current_adc_value = this->read_adc(1); // Assuming channel 1 is current

      // Convert ADC values to voltage and current
      float voltage = (float) voltage_adc_value * this->reference_voltage_ / (this->gain_ * 8388608.0f); //(2^23)
      float current = (float) current_adc_value * this->reference_voltage_ / (this->gain_ * 8388608.0f);

      // Publish sensor values
      if (this->voltage_sensor_ != nullptr) {
        this->voltage_sensor_->publish_state(voltage);
      }
      if (this->current_sensor_ != nullptr) {
        this->current_sensor_->publish_state(current);
      }
    }
  } else {
    // No DRDY pin configured, read data periodically
    // Implement a timer or other mechanism to control the read frequency
    // For example, read every 1 second:
    static uint32_t last_read_time = 0;
    if ((millis() - last_read_time) >= 1000) {
      last_read_time = millis();

      int32_t voltage_adc_value = this->read_adc(0); // Assuming channel 0 is voltage
      int32_t current_adc_value = this->read_adc(1); // Assuming channel 1 is current

      // Convert ADC values to voltage and current
      float voltage = (float) voltage_adc_value * this->reference_voltage_ / (this->gain_ * 8388608.0f); //(2^23)
      float current = (float) current_adc_value * this->reference_voltage_ / (this->gain_ * 8388608.0f);

      // Publish sensor values
      if (this->voltage_sensor_ != nullptr) {
        this->voltage_sensor_->publish_state(voltage);
      }
      if (this->current_sensor_ != nullptr) {
        this->current_sensor_->publish_state(current);
      }
    }
  }
}

}  // namespace ads131mxx
}  // namespace esphome
