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
  
  if(this->timestamp_ != nullptr)
    this->timestamp_->publish_state(data->timestamp);
}


}  // namespace prana_ble
}  // namespace esphome

#endif
