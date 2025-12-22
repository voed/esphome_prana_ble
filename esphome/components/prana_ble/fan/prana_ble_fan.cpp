#include "prana_ble_fan.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace prana_ble {

using namespace esphome::fan;


void PranaBLEFan::setup()
{
  this->traits_ = fan::FanTraits(false, true, false, speed_count_);
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
    if(auto_lock_) {
      ESP_LOGD(TAG, "Switching fan lock state");
      this->parent_->set_fans_locked(!fans_locked);
      delay(20);
    }
    else {
      ESP_LOGW(TAG, "Cannot control this fan, use fan_lock switch first.");
      return;
    }
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
      ESP_LOGD(TAG, "Setting fan speed %d", speed);
      this->parent_->set_fan_speed(fan_type_, speed, direct_);
      did_change = true;
    }
  }

  if (did_change) {
    this->publish_state();
  }
}

void PranaBLEFan::on_status(const PranaStatusPacket *data) {
  uint8_t data_speed = this->speed;
  bool enabled = this->state;
  bool need_publish = false;
  switch(fan_type_)
  {
    case PranaFan::FAN_IN:
    {
      data_speed = data->speed_in / 10;
      enabled = data->input_enabled;
      break;
    }
    case PranaFan::FAN_OUT:
    {
      data_speed = data->speed_out / 10;
      enabled = data->output_enabled;
      break;
    }
    case PranaFan::FAN_BOTH:
    {
      data_speed = data->speed / 10;
      enabled = data->enabled;
      break;
    }
  }

  if (data_speed != this->speed) {
    this->speed = data_speed;
    need_publish = true;
  }
  if (enabled != this->state) {
    this->state = enabled;
    need_publish = true;
  }
  if(need_publish)
    this->publish_state();
}
}  // namespace prana_ble
}  // namespace esphome

#endif
