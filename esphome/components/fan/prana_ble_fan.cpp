#include "prana_ble_fan.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace prana_ble {

using namespace esphome::fan;

void PranaBLEFan::dump_config() { LOG_FAN("", "Prana BLE Fan", this); }
std::string PranaBLEFan::describe() { return "Prana BLE Fan"; }

void PranaBLEFan::control(const fan::FanCall &call) {
  ESP_LOGD(TAG, "Received PranaBLEFan::control");
  if (!this->parent_->is_connected()) {
    ESP_LOGW(TAG, "Not connected, cannot handle control call yet.");
    return;
  }
  bool did_change = false;
  auto call_state = call.get_state();
  if(call_state.has_value() && call_state != this->state)
  {
    if(!call_state)
      this->parent_->set_fan_off(PranaFan::FAN_BOTH);
    else
      this->parent_->set_fan_on(PranaFan::FAN_BOTH);
    delay(100);
  }
  // ignore speed changes if not on or turning on
  if (this->state && call.get_speed().has_value()) {
    auto speed = *call.get_speed();
    if (speed > 0) {
      this->speed = speed;
      ESP_LOGW(TAG, "Setting fan speed %d", speed);
      this->parent_->set_fan_speed(PranaFan::FAN_BOTH, speed);

      //this->parent_->set_fan_index(this->speed - 1);
      did_change = true;
    }
  }
  if (did_change) {
    this->publish_state();
  }

}

void PranaBLEFan::on_status(const PranaStatusPacket *data) {
  ESP_LOGV(TAG, "[%s] Handling on_status with data=%p", this->get_name().c_str(), (void *) data);
  bool did_change = false;


  if (data->enabled != this->state) {
    this->state = data->enabled;
    did_change = true;
  }
  auto data_speed = data->speed / 10;
  // BedjetStatusPacket.fan_step is in range 0-19, but Fan.speed wants 1-20.
  if (data_speed != this->speed) {
    this->speed = data_speed;
    did_change = true;
  }

  if (did_change) {
    this->publish_state();
  }
}

/** Attempts to update the fan device from the last received BedjetStatusPacket.
 *
 * This will be called from #on_status() when the parent dispatches new status packets,
 * and from #update() when the polling interval is triggered.
 *
 * @return `true` if the status has been applied; `false` if there is nothing to apply.
 */

bool PranaBLEFan::update_status_() {
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

void PranaBLEFan::update() {
  ESP_LOGD(TAG, "[%s] update()", this->get_name().c_str());
  // TODO: if the hub component is already polling, do we also need to include polling?
  //  We're already going to get on_status() at the hub's polling interval.
  auto result = this->update_status_();
  ESP_LOGD(TAG, "[%s] update_status result=%s", this->get_name().c_str(), result ? "true" : "false");
}

/** Resets states to defaults. */
void PranaBLEFan::reset_state_() {
  this->state = false;
  this->publish_state();
}
}  // namespace prana_ble
}  // namespace esphome

#endif
