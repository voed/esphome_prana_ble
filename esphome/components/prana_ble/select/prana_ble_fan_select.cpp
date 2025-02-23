#include "esphome/core/log.h"
#include "prana_ble_fan_select.h"

namespace esphome {
namespace prana_ble {


PranaFanMode get_mode_from_string(const std::string& mode_string)
{
  for(int i=0; i < PRANA_FAN_MODES.size(); i++)
  {
    if(PRANA_FAN_MODES[i] == mode_string)
      return static_cast<PranaFanMode>(i);
  }
  return PranaFanMode::Manual;
}

void PranaBLEFanSelect::control(const std::string &value) {
  if (!this->parent_->is_connected()) {
    ESP_LOGW(TAG, "Not connected, cannot handle control call yet.");
    return;
  }
  auto ival = get_mode_from_string(value);
  if(parent_->get_display_mode() != ival)
  {
    ESP_LOGW(TAG, "Setting value: %s %i", value.c_str(), ival);
    parent_->set_auto_mode(ival);
    this->publish_state(value);
  }
}

void PranaBLEFanSelect::on_status(const PranaStatusPacket *data) {
  if(data == nullptr)
    return;
  auto mode = data->fan_mode;
  if(mode > PRANA_FAN_MODES.size())
  {
    ESP_LOGE(TAG, "Incorrect fan mode: %i", mode);
    return;
  }

  this->publish_state(PRANA_FAN_MODES[mode]);
}
void PranaBLEFanSelect::dump_config() {
  LOG_SELECT("", describe().c_str(), this);
}
std::string PranaBLEFanSelect::describe() { return "Prana BLE Select Fan"; }
}  // namespace prana_ble
}  // namespace esphome