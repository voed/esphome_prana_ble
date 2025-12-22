#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/components/switch/switch.h"
#include "../prana_ble_child.h"
#include "../prana_ble_const.h"


#ifdef USE_ESP32

namespace esphome {
namespace prana_ble {

enum PranaSwitchType
{
  ENABLE,
  HEAT,
  WINTER,
  CONNECT,
  FAN_LOCK
};

class PranaBLESwitch : public switch_::Switch, public PranaBLEClient, public Component {
 public:
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  /* PranaClient status update */
  void on_status(const PranaStatusPacket *data) override;
  void on_prana_state(bool is_ready) override {};
  std::string describe() override;
  void set_switch_type(PranaSwitchType type) { switch_type_ = type;}
 protected:
  void write_state(bool state) override;

 private:
  PranaSwitchType switch_type_;
};

}  // namespace prana_ble
}  // namespace esphome

#endif
