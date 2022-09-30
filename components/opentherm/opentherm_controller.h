#include "esphome.h"
//#include "esphome/components/sensor/sensor.h"
#include "OpenTherm.h"
//#include "opentherm_switch.h"
#include "opentherm_climate.h"
//#include "opentherm_binary.h"
//#include "opentherm_output.h"

namespace esphome {
namespace opentherm {

class OpenthermController: public PollingComponent {
private:
  const char *TAG = "opentherm_component";
  OpenTherm *ot;
  //OpenthermFloatOutput *pid_output_; 
protected:
  uint8_t in_pin_ = D4;
  uint8_t out_pin_ = D5;
  OpenthermClimate *hotWaterClimate_;
  OpenthermClimate *heatingWaterClimate_;
public:
  void set_in_pin(uint8_t in_pin) { in_pin_ = in_pin; }
  void set_out_pin(uint8_t out_pin) { out_pin_ = out_pin; }
  void set_hotWaterClimate(OpenthermClimate *hotWaterClimate) { hotWaterClimate_ = hotWaterClimate; }
  void set_heatingWaterClimate(OpenthermClimate *heatingWaterClimate) { heatingWaterClimate_ = heatingWaterClimate; }
  //Switch *thermostatSwitch = new OpenthermSwitch();
  //Sensor *external_temperature_sensor = new Sensor();
  //Sensor *return_temperature_sensor = new Sensor();
  //Sensor *boiler_temperature = new Sensor();
  //Sensor *pressure_sensor = new Sensor();
  //Sensor *modulation_sensor = new Sensor();
  //Sensor *heating_target_temperature_sensor = new Sensor();
  //BinarySensor *flame = new OpenthermBinarySensor();
  
  // Set 3 sec. to give time to read all sensors (and not appear in HA as not available)
  OpenthermController(): PollingComponent(3000) {
  }

  //void set_pid_output(OpenthermFloatOutput *pid_output) { pid_output_ = pid_output; }


  void setup() override {
    // This will be called once to set up the component
    // think of it as the setup() call in Arduino
      ESP_LOGD("opentherm_component", "Setup");

      
      ot = new OpenTherm(this->in_pin_, this->out_pin_, false);
      ot->begin([this](unsigned long response, OpenThermResponseStatus status) {
        this->processResponse(response, status);
      });

      /*thermostatSwitch->add_on_state_callback([=](bool state) -> void {
        ESP_LOGD ("opentherm_component", "termostatSwitch_on_state_callback %d", state);    
      });*/

      // Adjust HeatingWaterClimate depending on PID
      // heatingWaterClimate->set_supports_heat_cool_mode(this->pid_output_ != nullptr);
      //heatingWaterClimate->set_supports_two_point_target_temperature(this->pid_output_ != nullptr);

      hotWaterClimate_->set_temperature_settings(5, 6, 0);
      heatingWaterClimate_->set_temperature_settings(0, 0, 30);
      //hotWaterClimate->setup();
      //heatingWaterClimate->setup();
  }

  float getHotWaterTemperature() {
      unsigned long response = ot->sendRequest(ot->buildRequest(OpenThermRequestType::READ, OpenThermMessageID::Tdhw, 0));
      return ot->isValidResponse(response) ? ot->getFloat(response) : -1;
  }

  bool setHotWaterTemperature(float temperature) {
	    unsigned int data = ot->temperatureToData(temperature);
      unsigned long request = ot->buildRequest(OpenThermRequestType::WRITE, OpenThermMessageID::TdhwSet, data);
      unsigned long response = ot->sendRequest(request);
      return ot->isValidResponse(response);
  }

  /*
  float getExternalTemperature() {
      unsigned long response = ot->sendRequest(ot->buildRequest(OpenThermRequestType::READ, OpenThermMessageID::Toutside, 0));
      return ot->isValidResponse(response) ? ot->getFloat(response) : -1;
  }

  float getReturnTemperature() {
      unsigned long response = ot->sendRequest(ot->buildRequest(OpenThermRequestType::READ, OpenThermMessageID::Tret, 0));
      return ot->isValidResponse(response) ? ot->getFloat(response) : -1;
  }
  
  float getModulation() {
    unsigned long response = ot->sendRequest(ot->buildRequest(OpenThermRequestType::READ, OpenThermMessageID::RelModLevel, 0));
    return ot->isValidResponse(response) ? ot->getFloat(response) : -1;
  }

  float getPressure() {
    unsigned long response = ot->sendRequest(ot->buildRequest(OpenThermRequestType::READ, OpenThermMessageID::CHPressure, 0));
    return ot->isValidResponse(response) ? ot->getFloat(response) : -1;
  }
  */

  void update() override {

    //ESP_LOGD("opentherm_component", "update heatingWaterClimate: %i", heatingWaterClimate_->mode);
    ESP_LOGD("opentherm_component", "update hotWaterClimate: %i", hotWaterClimate_->mode);
    
    bool enableHotWater = hotWaterClimate_->mode == climate::ClimateMode::CLIMATE_MODE_HEAT;
    bool enableCentralHeating = heatingWaterClimate_->mode == climate::ClimateMode::CLIMATE_MODE_HEAT;
    bool enableCooling = false; // this boiler is for heating only

    
    //Set/Get Boiler Status
    
    auto response = ot->setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
    bool isFlameOn = ot->isFlameOn(response);
    bool isCentralHeatingActive = ot->isCentralHeatingActive(response);
    bool isHotWaterActive = ot->isHotWaterActive(response);    
    
    // Set hot water temperature
    setHotWaterTemperature(hotWaterClimate_->target_temperature);
    // Publish status of thermostat that controls hot water
    float hotWater_temperature = getHotWaterTemperature();
    hotWaterClimate_->current_temperature = hotWater_temperature;
    hotWaterClimate_->action = isHotWaterActive ? climate::ClimateAction::CLIMATE_ACTION_HEATING : climate::ClimateAction::CLIMATE_ACTION_OFF;
    hotWaterClimate_->publish_state();
 


    // Set heating water temperature depending on room thermostat
    /*
    float heating_target_temperature;
    if (this->pid_output_ != nullptr) {
      float pid_output = pid_output_->get_state();
      if (pid_output == 0.0f) {
        heating_target_temperature = 10.0f;
      }
      else {
        heating_target_temperature =  pid_output * (heatingWaterClimate->target_temperature_high - heatingWaterClimate->target_temperature_low) 
        + heatingWaterClimate->target_temperature_low;      
      }
      ESP_LOGD("opentherm_component", "setBoilerTemperature  at %f °C (from PID Output)", heating_target_temperature);
    }
    else if (thermostatSwitch->state) {
      heating_target_temperature = heatingWaterClimate->target_temperature;
      ESP_LOGD("opentherm_component", "setBoilerTemperature  at %f °C (from heating water climate)", heating_target_temperature);
    }
    else {
      // If the room thermostat is off, set it to 0
      heating_target_temperature = 0;
      ESP_LOGD("opentherm_component", "setBoilerTemperature at %f °C (default low value)", heating_target_temperature);
    }*/
    
    float heating_target_temperature = heatingWaterClimate_->target_temperature;
    ot->setBoilerTemperature(heating_target_temperature);

    // Publish status of thermostat that controls heating
    float boilerTemperature = ot->getBoilerTemperature();
    heatingWaterClimate_->current_temperature = boilerTemperature;
    heatingWaterClimate_->action = isCentralHeatingActive && isFlameOn ? climate::ClimateAction::CLIMATE_ACTION_HEATING : climate::ClimateAction::CLIMATE_ACTION_OFF;
    heatingWaterClimate_->publish_state();
    


    //float ext_temperature = getExternalTemperature();
    //float pressure = getPressure();
    //float modulation = getModulation();

    // Publish sensor values
    /*
    flame->publish_state(isFlameOn); 
    external_temperature_sensor->publish_state(ext_temperature);
    return_temperature_sensor->publish_state(return_temperature);
    boiler_temperature->publish_state(boilerTemperature);
    pressure_sensor->publish_state(pressure);
    modulation_sensor->publish_state(modulation);
    
    heating_target_temperature_sensor->publish_state(heating_target_temperature);
    */    

  }

  void processResponse(unsigned long response, OpenThermResponseStatus status) {
    ESP_LOGD("opentherm_component", "Opentherm adapter comunication [response: %d, status %s", response, ot->statusToString(status));
  }

  void loop() override {
    ot->process();
  }


};

}
}