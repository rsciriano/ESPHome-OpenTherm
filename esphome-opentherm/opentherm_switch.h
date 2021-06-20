#pragma once

#include "esphome.h"

class OpenthermSwitch : public Switch {
 public:
  void write_state(bool state) override {
    // This will be called every time the user requests a state change.

    ESP_LOGD("opentherm_switch", "write_state");

    // Acknowledge new state by publishing it
    publish_state(state);
  }
};
