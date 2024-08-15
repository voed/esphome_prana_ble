#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/components/fan/fan.h"
#include "../prana_ble_child.h"
#include "../prana_ble_const.h"
#include "../prana_ble_hub.h"

#ifdef USE_ESP32

namespace esphome {
namespace prana_ble {

class PranaBLEFan : public fan::Fan, public PranaBLEClient, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  /* PranaClient status update */
  void on_status(const PranaStatusPacket *data) override;
  void on_prana_state(bool is_ready) override{};
  std::string describe() override;
  
  //fan::FanTraits get_traits() override { return fan::FanTraits(false, true, false, PRANA_FAN_SPEED_COUNT); }
  fan::FanTraits get_traits() override { return this->traits_; }

  void set_has_auto(bool has_auto) {has_auto_ = has_auto;}

  //void set_fan_type(PranaFan fan) { fan_type = PranaFan::FAN_BOTH; }
 protected:
  void control(const fan::FanCall &call) override;
  PranaFanMode fan_mode;
  fan::FanTraits traits_;
  //PranaFan fan_type;
  bool has_auto_{false};

 private:
  void reset_state_();
  bool update_status_();
};

}  // namespace prana_ble
}  // namespace esphome

#endif
