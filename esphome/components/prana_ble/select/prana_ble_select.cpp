#include "esphome/core/log.h"
#include "prana_ble_select.h"

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

void PranaBLESelect::control(const std::string &value) {
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


void PranaBLESelect::on_status(const PranaStatusPacket *data) {
  if(data == nullptr)
    return;

  this->publish_state(PRANA_DISPLAY_MODES[data->display_mode]);
}
void PranaBLESelect::dump_config() {
  LOG_SELECT("", "PranaBLE Select", this);
}
std::string PranaBLESelect::describe() { return "Prana BLE Select"; }
}  // namespace prana_ble
}  // namespace esphome