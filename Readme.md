# ESPHome OpenTherm

[![build](https://github.com/rsciriano/ESPHome-OpenTherm/actions/workflows/build.yml/badge.svg)](https://github.com/rsciriano/ESPHome-OpenTherm/actions/workflows/build.yml)

This is an example of a integration with a OpenTherm boiler using [ESPHome](https://esphome.io/) and the [Ihor Melnyk](http://ihormelnyk.com/opentherm_adapter) or [DIYLESS](https://diyless.com/product/esp8266-thermostat-shield) OpenTherm Adapter 

## Installation
- Copy the content of this repository to your ESPHome folder
- Make sure the pin numbers are right, check the file opentherm_component.h in the esphome-opentherm folder.
- Edit the opentherm.yaml file:
    - Make sure the board and device settings are correct for your device
    - Set the sensor entity_id with the external temperature sensor's name from Home Assistant. (The ESPHome sensor name is temperature_sensor).
- Flash the ESP and configure in Home Assistant. It should be auto-discovered by the ESPHome Integration.

## Additional info

> I am working in a new version of this integration, completely rewriting it as an [external component](https://esphome.io/components/external_components.html). You can see the progress in [this branch](https://github.com/rsciriano/ESPHome-OpenTherm/tree/external-component)

> I have pending to write a detailed documentation, I promise to do it for the new version. Until then you have lot of info in [this issue](https://github.com/rsciriano/ESPHome-OpenTherm/issues/37#issuecomment-1288779030), thanks to [@foltymat](https://github.com/foltymat)
