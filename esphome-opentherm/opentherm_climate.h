#pragma once
#include "esphome.h"


class OpenthermClimate : public Climate {
private:
    const char *TAG = "opentherm_climate";
    bool supports_heat_cool_mode_ = false;
    bool supports_two_point_target_temperature_ = false;
public:
    void set_supports_heat_cool_mode(bool value)
    {
        supports_heat_cool_mode_ = value;
    }    
    bool get_supports_heat_cool_mode()
    {
        return supports_heat_cool_mode_;
    }

    void set_supports_two_point_target_temperature(bool value)
    {
        supports_two_point_target_temperature_ = value;
    }    
    bool get_supports_two_point_target_temperature()
    {
        return supports_two_point_target_temperature_;
    }


    climate::ClimateTraits traits() override {

        auto traits = climate::ClimateTraits();
        traits.set_supports_current_temperature(true);
        // traits.set_supports_heat_cool_mode(supports_heat_cool_mode_);
        traits.set_supports_heat_cool_mode(false);
        traits.set_supports_cool_mode(false);
        traits.set_supports_heat_mode(true);
        traits.set_supports_two_point_target_temperature(supports_two_point_target_temperature_);
        traits.set_supports_away(false);
        traits.set_supports_action(true);

        traits.set_visual_min_temperature(5);
        traits.set_visual_max_temperature(80);
        traits.set_visual_temperature_step(1);

        return traits;    
    }

  void control(const ClimateCall &call) override {

    if (call.get_mode().has_value()) {
        // User requested mode change
        ClimateMode mode = *call.get_mode();
        // Send mode to hardware
        // ...
        ESP_LOGD(TAG, "get_mode");    

        // Publish updated state
        this->mode = mode;
        this->publish_state();
    }
    if (call.get_target_temperature().has_value()) {
        // User requested target temperature change
        float temp = *call.get_target_temperature();
        // Send target temp to climate
        // ...
        ESP_LOGD(TAG, "get_target_temperature");    

        this->target_temperature = temp;
        this->publish_state();
    }
    if (call.get_target_temperature_low().has_value()) {
        // User requested target temperature change
        float temp = *call.get_target_temperature_low();
        // Send target temp to climate
        // ...
        ESP_LOGD(TAG, "get_target_temperature_low");    

        this->target_temperature_low = temp;
        this->publish_state();
    }
    if (call.get_target_temperature_high().has_value()) {
        // User requested target temperature change
        float temp = *call.get_target_temperature_high();
        // Send target temp to climate
        // ...
        ESP_LOGD(TAG, "get_target_temperature_high");    

        this->target_temperature_high = temp;
        this->publish_state();
    }

  }
};