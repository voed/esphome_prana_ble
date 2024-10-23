#include "esphome/core/log.h"
#include "prana_ble_number.h"

namespace esphome {
namespace prana_ble {

void PranaBLENumber::control(float value) {
  ESP_LOGV(TAG, "Setting number: %f", value);
  if (!this->parent_->is_connected()) {
    ESP_LOGW(TAG, "Not connected, cannot handle control call yet.");
    return;
  }
  short ival = round(value);
  if(parent_->get_brightness() != ival)
  {
    ESP_LOGW(TAG, "Setting number: %f %i", value, ival);
    parent_->set_brightness(ival);
    this->publish_state(value);
  }

  
}


void PranaBLENumber::on_status(const PranaStatusPacket *data) {
  if(data == nullptr)
    return;

  this->publish_state(parent_->get_brightness());
}
void PranaBLENumber::dump_config() {
  LOG_NUMBER("", "PranaBLE Number", this);
}
std::string PranaBLENumber::describe() { return "Prana BLE Number"; }
}  // namespace prana_ble
}  // namespace esphome