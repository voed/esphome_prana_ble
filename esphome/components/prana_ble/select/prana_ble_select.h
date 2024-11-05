#pragma once

#include "esphome/components/select/select.h"
#include "esphome/core/component.h"
#include "esphome/core/optional.h"
#include "../prana_ble_child.h"
#include "../prana_ble_const.h"
#include "../prana_ble_hub.h"

namespace esphome {
namespace prana_ble {

class PranaBLESelect : public select::Select, public PranaBLEClient, public Component {
 public:
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  /* PranaClient status update */
  void on_status(const PranaStatusPacket *data) override;
  void on_prana_state(bool is_ready) override{};
  std::string describe() override;

 protected:
  void control(const std::string &value) override;
};

}  // namespace prana_ble
}  // namespace esphome