#pragma once

#include "esphome/core/component.h"
#include "esphome/components/atm90e36/atm90e36.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace atm90e36 {

class ATM90E36Number : public number::Number, public Parented<ATM90E36Component> {
 public:
  void control(float value) override { this->publish_state(value); }
};

}  // namespace atm90e32
}  // namespace esphome
