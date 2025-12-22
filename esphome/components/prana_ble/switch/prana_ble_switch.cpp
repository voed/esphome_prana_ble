#include "prana_ble_switch.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace prana_ble {


void PranaBLESwitch::dump_config() { LOG_SWITCH("", "Prana BLE Switch", this); }
std::string PranaBLESwitch::describe() { return str_sprintf("Prana BLE Switch %s", this->get_name().c_str()); }

void PranaBLESwitch::write_state(bool state) {
  if (switch_type_ != PranaSwitchType::CONNECT && !this->parent_->is_connected()) {
    ESP_LOGW(TAG, "Not connected, cannot handle control call yet.");
    return;
  }
  switch(switch_type_)
  {
    case PranaSwitchType::ENABLE:
    {
      if(!state)
        this->parent_->command_poweroff();
      else
        this->parent_->command_poweron();
      break;
    }

    case PranaSwitchType::HEAT:
    {
      parent_->set_heating(state);
      break;
    }
    case PranaSwitchType::WINTER:
    {
      parent_->set_winter_mode(state);
      break;
    }
    case PranaSwitchType::CONNECT:
    {
      if(state)
      {
        this->parent_->command_connect();
      }
      else
        this->parent_->command_disconnect();

      this->publish_state(state);
      break;
    }
    case PranaSwitchType::FAN_LOCK:
    {
      parent_->set_fans_locked(state);

      break;
    }
  }
}

void PranaBLESwitch::on_status(const PranaStatusPacket *data) {
  if(data == nullptr)
    return;
  switch(switch_type_)
  {
    case PranaSwitchType::ENABLE:
    {
      this->publish_state(data->enabled);
      break;
    }

    case PranaSwitchType::HEAT:
    {
      this->publish_state(data->heating_on);
      break;
    }
    case PranaSwitchType::WINTER:
    {
      this->publish_state(data->winter_mode);
      break;
    }
    case PranaSwitchType::CONNECT:
    {
      this->publish_state(this->parent_->is_connection_enabled());
      break;
    }

    case PranaSwitchType::FAN_LOCK:
    {
      this->publish_state(this->parent_->get_fans_locked());
      break;
    }
  }
}

}  // namespace prana_ble
}  // namespace esphome

#endif
