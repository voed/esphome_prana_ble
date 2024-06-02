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
      this->parent_->command_heating();
      break;
    }
    case PranaSwitchType::WINTER:
    {
      this->parent_->command_winter_mode();
      break;
    }
    case PranaSwitchType::AUTO:
    {
      this->parent_->command_auto_mode();
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
      
    }
  }

  this->publish_state(state);
}

void PranaBLESwitch::on_status(const PranaStatusPacket *data) {
  ESP_LOGD(TAG, "[%s] Handling on_status with data=%p", this->get_name().c_str(), (void *) data);
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
    case PranaSwitchType::AUTO:
    {
      this->publish_state(data->auto_mode);
      break;
    }
    case PranaSwitchType::CONNECT:
    {
      this->publish_state(this->parent_->is_connection_enabled());
    }
  }
}

/** Attempts to update the fan device from the last received PranaStatusPacket.
 *
 * This will be called from #on_status() when the parent dispatches new status packets,
 * and from #update() when the polling interval is triggered.
 *
 * @return `true` if the status has been applied; `false` if there is nothing to apply.
 */
/*
bool PranaBLESwitch::update_status_() {
  if (!this->parent_->is_connected())
    return false;
  if (!this->parent_->has_status())
    return false;
  auto *status = this->parent_->get_status_packet();

  if (status == nullptr)
    return false;

  this->on_status(status);
  return true;
}
*/
void PranaBLESwitch::update() {
  ESP_LOGD(TAG, "[%s] update()", this->get_name().c_str());
  // TODO: if the hub component is already polling, do we also need to include polling?
  //  We're already going to get on_status() at the hub's polling interval.
  //auto result = this->update_status_();
  //ESP_LOGD(TAG, "[%s] update_status result=%s", this->get_name().c_str(), result ? "true" : "false");
}


}  // namespace prana_ble
}  // namespace esphome

#endif
