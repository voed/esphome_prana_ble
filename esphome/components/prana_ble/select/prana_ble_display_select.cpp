#include "esphome/core/log.h"
#include "prana_ble_display_select.h"

namespace esphome {
namespace prana_ble {


PranaDisplayMode get_display_mode_from_string(const std::string& mode_string)
{
  for(int i=0; i < PRANA_DISPLAY_MODES.size(); i++)
  {
    if(PRANA_DISPLAY_MODES[i] == mode_string)
      return static_cast<PranaDisplayMode>(i);
  }
  return PranaDisplayMode::FAN;
}

void PranaBLEDisplaySelect::control(const std::string &value) {
  if (!this->parent_->is_connected()) {
    ESP_LOGW(TAG, "Not connected, cannot handle control call yet.");
    return;
  }
  short ival = get_display_mode_from_string(value);
  if(parent_->get_display_mode() != ival)
  {
    ESP_LOGW(TAG, "Setting value: %s %i", value.c_str(), ival);
    parent_->set_display_mode(ival);
    this->publish_state(value);
  }
}


void PranaBLEDisplaySelect::on_status(const PranaStatusPacket *data) {
  if(data == nullptr)
    return;
  if(data->display_mode > PRANA_DISPLAY_MODES.size())
  {
    ESP_LOGE(TAG, "Incorrect display mode: %i", data->display_mode);
    
    return;
  }
  
  this->publish_state(PRANA_DISPLAY_MODES[data->display_mode]);
}
void PranaBLEDisplaySelect::dump_config() {
  LOG_SELECT("", describe().c_str(), this);
}
std::string PranaBLEDisplaySelect::describe() { return "Prana BLE Select Display"; }
}  // namespace prana_ble
}  // namespace esphome