#include "prana_ble_fan.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace prana_ble {

using namespace esphome::fan;

PranaFanMode get_mode_from_string(const std::string& mode_string)
{
  for(int i=0; i < PRANA_FAN_MODES.size(); i++)
  {
    if(PRANA_FAN_MODES[i] == mode_string)
      return static_cast<PranaFanMode>(i);
  }
  return PranaFanMode::Manual;
}
void PranaBLEFan::setup()
{
  this->traits_ = fan::FanTraits(false, true, false, speed_count_);
  if(has_auto_)
  {
    std::set<std::string> modes(PRANA_FAN_MODES.begin(), PRANA_FAN_MODES.end());
    this->traits_.set_supported_preset_modes(modes);
  }
    
}
void PranaBLEFan::dump_config() { LOG_FAN("", "Prana BLE Fan", this); }
std::string PranaBLEFan::describe() { return "Prana BLE Fan"; }

void PranaBLEFan::control(const fan::FanCall &call) {
  if (!this->parent_->is_connected()) {
    ESP_LOGW(TAG, "Not connected, cannot handle control call yet.");
    return;
  }
  auto fans_locked = parent_->get_fans_locked();
  if( (fans_locked && fan_type_ != PranaFan::FAN_BOTH) 
      || (!fans_locked && fan_type_ == PranaFan::FAN_BOTH) )
  {
    ESP_LOGW(TAG, "Cannot control this fan. Check fan lock switch");
    return;
  }
  bool did_change = false;
  if(call.get_state().has_value() && call.get_state() != this->state)
  {
    if(this->state)
      this->parent_->set_fan_off(fan_type_);
    else
      this->parent_->set_fan_on(fan_type_);
    this->state = !this->state;
    did_change = true;
  }
  // ignore speed changes if not on or turning on
  if (this->state && call.get_speed().has_value()) {
    auto speed = *call.get_speed();
    if (speed > 0) {
      this->speed = speed;
      ESP_LOGW(TAG, "Setting fan speed %d", speed);
      this->parent_->set_fan_speed(fan_type_, speed);

      //this->parent_->set_fan_index(this->speed - 1);
      did_change = true;
    }
  }
  if(!call.get_preset_mode().empty() || this->preset_mode != call.get_preset_mode())
  {
    ESP_LOGD(TAG, "Changing preset from %s to %s", this->preset_mode.c_str(), call.get_preset_mode().c_str());
    auto mode = get_mode_from_string(call.get_preset_mode());
    this->preset_mode = call.get_preset_mode();
    this->parent_->set_auto_mode(mode); 
    this->fan_mode = mode;
    did_change = true;
  }
  if (did_change) {
    this->publish_state();
  }
}

void PranaBLEFan::on_status(const PranaStatusPacket *data) {
  bool did_change = false;
  auto mode = data->fan_mode;
  if(this->fan_mode != mode || this->preset_mode.empty())
  {
    ESP_LOGD(TAG, "New fan mode: %i", mode);
    if(mode > PRANA_FAN_MODES.size())
    {
      ESP_LOGE(TAG, "Incorrect fan mode: %i", mode);
      return;
    }

    this->preset_mode = PRANA_FAN_MODES[mode];

    this->fan_mode = mode;
    did_change = true;
  }


  switch(fan_type_)
  {
    case PranaFan::FAN_IN:
    {
      auto data_speed = data->speed_in / 10;
      // BedjetStatusPacket.fan_step is in range 0-19, but Fan.speed wants 1-20.
      if (data_speed != this->speed) {
        this->speed = data_speed;
        did_change = true;
      }
      if (data->input_enabled != this->state) {
        this->state = data->input_enabled;
        did_change = true;
      }
      
      break;
    }
    case PranaFan::FAN_OUT:
    {
      auto data_speed = data->speed_out / 10;
      // BedjetStatusPacket.fan_step is in range 0-19, but Fan.speed wants 1-20.
      if (data_speed != this->speed) {
        this->speed = data_speed;
        did_change = true;
      }

      if (data->output_enabled != this->state) {
        this->state = data->output_enabled;
        did_change = true;
      }
      
      break;
    }
    case PranaFan::FAN_BOTH:
    {
      auto data_speed = data->speed / 10;
      // BedjetStatusPacket.fan_step is in range 0-19, but Fan.speed wants 1-20.
      if (data_speed != this->speed) {
        this->speed = data_speed;
        did_change = true;
      }

      if (data->enabled != this->state) {
        this->state = data->enabled;
        did_change = true;
      }

        
      break;
    }
  }


  if (did_change) {
    this->publish_state();
  }
}
}  // namespace prana_ble
}  // namespace esphome

#endif
