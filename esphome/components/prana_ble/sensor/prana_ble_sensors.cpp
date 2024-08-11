#include "prana_ble_sensors.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace prana_ble {


void PranaBLESensors::dump_config() { ESP_LOGCONFIG("", "Prana BLE Sensors"); }
std::string PranaBLESensors::describe() { return "Prana BLE Sensors"; }


bool is_temp_valid(float temp)
{
  //TODO: test this
  if(abs(temp) > 60.0)
    return false;

  return true;
}

float convert_temp(short temp)
{
  return (byteswap(temp) & 0x3fff) / 10.0;
}

void PranaBLESensors::on_status(const PranaStatusPacket *data) {
  ESP_LOGV(TAG, " Handling on_status with data=%p",  (void *) data);
  if(data == nullptr)
    return;
  
  float temp = 0.0;
  if(this->temp_inside_out_ != nullptr)
  {
    temp = convert_temp(data->temp_inside_out);
    if(is_temp_valid(temp))
      this->temp_inside_out_->publish_state(temp);
  }

  if(this->temp_inside_in_ != nullptr)
  {
    temp = convert_temp(data->temp_inside_in);
    if(is_temp_valid(temp))
      this->temp_inside_in_->publish_state(temp);
  }

  if(this->temp_outside_out_ != nullptr)
  {
    temp = convert_temp(data->temp_outside_out);
    if(is_temp_valid(temp))
      this->temp_outside_out_->publish_state(temp);
  }

  if(this->temp_outside_in_ != nullptr)
  {
    temp = convert_temp(data->temp_outside_in);
    if(is_temp_valid(temp))
      this->temp_outside_in_->publish_state(temp);
  }

  if(this->humidity_ != nullptr)
    this->humidity_->publish_state(data->humidity - 128);

  //TODO: add validation
  if(this->pressure_ != nullptr)
    this->pressure_->publish_state(512 + data->pressure);

  if(this->tvoc_ != nullptr)
    this->tvoc_->publish_state(byteswap(data->voc) & 0x3fff);

  if(this->co2_ != nullptr)
    this->co2_->publish_state(byteswap(data->co2) & 0x3fff);

  if(this->voltage_ != nullptr)
    this->voltage_->publish_state(data->voltage);
    
  if(this->frequency_ != nullptr)
    this->frequency_->publish_state(data->frequency);
}


/** Attempts to update the fan device from the last received BedjetStatusPacket.
 *
 * This will be called from #on_status() when the parent dispatches new status packets,
 * and from #update() when the polling interval is triggered.
 *
 * @return `true` if the status has been applied; `false` if there is nothing to apply.
 */
/*
bool PranaBLESensors::update_status_() {
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
void PranaBLESensors::update() {
  // TODO: if the hub component is already polling, do we also need to include polling?
  //  We're already going to get on_status() at the hub's polling interval.
  //auto result = this->update_status_();
  //ESP_LOGD(TAG, " update_status result=%s", result ? "true" : "false");
}


}  // namespace prana_ble
}  // namespace esphome

#endif
