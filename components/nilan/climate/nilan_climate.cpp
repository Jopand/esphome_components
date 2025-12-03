#include "nilan_climate.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace nilan {
static const char *TAG = "nilan.climate";

void NilanClimate::setup() {
  current_temp_sensor_->add_on_state_callback([this](float state) {
    // ESP_LOGD(TAG, "CURRENT TEMP SENSOR CALLBACK: %f", state);
    this->current_temperature = state;
    publish_state();
  });
  temp_setpoint_number_->add_on_state_callback([this](float state) {
    // ESP_LOGD(TAG, "TEMP SETPOINT SENSOR CALLBACK: %f", state);
    this->target_temperature = state;
    publish_state();
  });
  mode_select_->add_on_state_callback([this](std::string state, size_t index) {
    // ESP_LOGD(TAG, "OPERATION MODE CALLBACK: %s", state.c_str());
    nilanmodetext_to_climatemode(index);
    publish_state();
  });
  fan_speed_number_->add_on_state_callback([this](float state) {
    // ESP_LOGD(TAG, "FAN SPEED SENSOR CALLBACK: %f", state);
    nilanfanspeed_to_fanmode(state);
    publish_state();
  });

  this->current_temperature = current_temp_sensor_->state;
  this->target_temperature  = temp_setpoint_number_->state;
  size_t current_mode_index = static_cast<size_t>(mode_select_->active_index().value());
  nilanmodetext_to_climatemode(current_mode_index);
  nilanfanspeed_to_fanmode(fan_speed_number_->state); // Will update either fan_mode or custom_fan_mode
}

void NilanClimate::control(const climate::ClimateCall& call) {
  if (call.get_target_temperature().has_value())
  {
    this->target_temperature = *call.get_target_temperature();
    
    ESP_LOGD(TAG, "Target temperature changed to: %f", this->target_temperature);
    temp_setpoint_number_->make_call().set_value(this->target_temperature).perform();
  }

  if (call.get_mode().has_value())
  {
    auto new_mode = *call.get_mode();
    mode = new_mode;
    int operation_mode = climatemode_to_nilanoperationmode(new_mode);

    ESP_LOGD(TAG, "Operation mode changed to: %d", operation_mode);
    mode_select_->make_call().set_index(operation_mode).perform();
  }

  if (call.get_fan_mode().has_value())
  {
    // The only valid fan mode that is not custom is "OFF"
    auto new_fan_mode = *call.get_fan_mode();
    this->set_custom_fan_mode_("");

    ESP_LOGD(TAG, "Custom Fan mode set to: 0");
    fan_speed_number_->make_call().set_value(0).perform();
  }

  if (call.get_custom_fan_mode() != nullptr)
  {
    const char *new_custom_fan_mode = call.get_custom_fan_mode();
    this->set_custom_fan_mode_(new_custom_fan_mode);
    this->fan_mode.reset();
    auto optional_nilan_fan_mode = parse_number<float>(new_custom_fan_mode);
    if(optional_nilan_fan_mode.has_value())
    {
      auto nilan_fan_mode = optional_nilan_fan_mode.value();
      ESP_LOGD(TAG, "Custom Fan mode set to: %i", static_cast<int>(nilan_fan_mode));
      fan_speed_number_->make_call().set_value(nilan_fan_mode).perform();
    }
  }
  this->publish_state();
}

climate::ClimateTraits NilanClimate::traits() {
  auto traits = climate::ClimateTraits();

  traits.set_supported_custom_fan_modes({
    "1",
    "2",
    "3",
    "4"
  });

  traits.set_supported_fan_modes({ 
    climate::ClimateFanMode::CLIMATE_FAN_OFF 
  });

  traits.set_supported_modes({
    climate::ClimateMode::CLIMATE_MODE_COOL,
    climate::ClimateMode::CLIMATE_MODE_HEAT,
    climate::ClimateMode::CLIMATE_MODE_HEAT_COOL
   });

  traits.set_supports_current_temperature(true);
  traits.set_visual_temperature_step(1);
  traits.set_visual_min_temperature(5);
  traits.set_visual_max_temperature(30);

  return traits;
}

void NilanClimate::dump_config() {
  LOG_CLIMATE("", "Nilan Climate", this);
}

void NilanClimate::nilanfanspeed_to_fanmode(const int state)
{
  this->set_custom_fan_mode_("");
  this->fan_mode.reset();

  switch (state) {
  case 1:
  case 2:
  case 3:
  case 4:
    this->set_custom_fan_mode_(esphome::to_string(state).c_str());
    break;
  case 0:
  default: 
    this->fan_mode = climate::CLIMATE_FAN_OFF; 
    break;
  }
}

int NilanClimate::climatemode_to_nilanoperationmode(const climate::ClimateMode mode)
{
  int return_value;

  switch (mode) {
    case climate::CLIMATE_MODE_OFF: return_value = 0; break;
    case climate::CLIMATE_MODE_HEAT: return_value = 1; break;
    case climate::CLIMATE_MODE_COOL: return_value = 2; break;
    case climate::CLIMATE_MODE_HEAT_COOL: return_value = 3; break;
    default: return_value = 4; break;
  }

  return return_value;
}

void NilanClimate::nilanmodetext_to_climatemode(const size_t index)
{
  switch(index) {
    case 0: this->mode = climate::CLIMATE_MODE_OFF; break;
    case 1: this->mode = climate::CLIMATE_MODE_HEAT; break;
    case 2: this->mode = climate::CLIMATE_MODE_COOL; break;
    default: this->mode = climate::CLIMATE_MODE_HEAT_COOL; break;
  }
}

} // namespace nilan
} // namespace esphome
