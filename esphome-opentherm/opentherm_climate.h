#pragma once
#include "esphome.h"


class OpenthermClimate : public Climate, public Component {
private:
    const char *TAG = "opentherm_climate";
    bool supports_heat_cool_mode_ = false;
    bool supports_two_point_target_temperature_ = false;
    float minimum_temperature_ = 0;
    float maximum_temperature_ = 0;
    float default_target_ = 0;
public:
    void set_supports_heat_cool_mode(bool value) { supports_heat_cool_mode_ = value; }
    bool get_supports_heat_cool_mode() { return supports_heat_cool_mode_; }
    void set_supports_two_point_target_temperature(bool value) { supports_two_point_target_temperature_ = value; }    
    bool get_supports_two_point_target_temperature() { return supports_two_point_target_temperature_; }
    void set_temperature_settings(float min = 0, float max = 0, float target = 0) { 
        minimum_temperature_ = min;
        maximum_temperature_ = max,
        default_target_ = target;
    }

    void setup() override {
        // restore setpoints
        auto restore = this->restore_state_();
        if (restore.has_value()) {
            restore->apply(this);
        } else {
            // restore from defaults
            this->mode = climate::CLIMATE_MODE_OFF;
            // initialize target temperature to some value so that it's not NAN
            this->target_temperature = default_target_;

            if (supports_two_point_target_temperature_){
                this->target_temperature_low = minimum_temperature_;
                this->target_temperature_high = maximum_temperature_;
            }
        }
    }

    climate::ClimateTraits traits() override {

        auto traits = climate::ClimateTraits();
        traits.set_supports_current_temperature(true);
        // traits.set_supports_heat_cool_mode(supports_heat_cool_mode_);
        traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::CLIMATE_MODE_HEAT});
        traits.set_supports_two_point_target_temperature(supports_two_point_target_temperature_);
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
