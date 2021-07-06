#pragma once

#include "esphome.h"
using namespace esphome;


class OpenthermFloatOutput : public Component, public FloatOutput {
 public:
  float get_state() const { return state_; }
  void setup() override {
    // This will be called by App.setup()
    //pinMode(5, OUTPUT);
  }

 protected:
  float state_{0.0f};
  void write_state(float state) override { this->state_ = state; }

};


