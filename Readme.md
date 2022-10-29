# ESPHome OpenTherm external component [WIP]

> **Warning!!!** *This is a very preliminary version, it is incomplete and not yet functional.*

[ESPHome external component](https://esphome.io/components/external_components.html) to connect a OpenTherm boiler to Home Assistant using the [Ihor Melnyk](http://ihormelnyk.com/opentherm_adapter) or [DIYLESS](https://diyless.com/product/esp8266-thermostat-shield) OpenTherm Adapter 



## Configuration

This is a example of a configuration file


```yaml
substitutions:
  devicename: opentherm
  upper_devicename: Opentherm

esphome:
  name: $devicename
  platform: ESP8266
  board: d1_mini

external_components:
  - source: github://rsciriano/ESPHome-OpenTherm@external-component
    components: [ opentherm ]

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  ap:
    ssid: "${devicename} Fallback"
    password: !secret esphome_fallback_password

captive_portal:
logger:
api:
ota:

opentherm:
  in_pin: D5
  out_pin: D6

climate:
  - platform: opentherm
    hotwater:
      id: hot_water
      name: 'Hot water'
    heatingwater:
      id: heating_water
      name: 'Heating water'

sensor:
  - platform: opentherm

```

### OpenTherm adapter configuration:

```yaml
opentherm:
  in_pin: D5
  out_pin: D6
```

Configuration variables:

  - **in_pin** (Required, [Pin](https://esphome.io/guides/configuration-types.html#config-pin)): The pin connected to the OpenTherm Adapter input
  - **out_pin** (Required, [Pin](https://esphome.io/guides/configuration-types.html#config-pin)): The pin connected to the OpenTherm Adapter output

### Climates

```yaml
climate:
  - platform: opentherm
    hotwater:
      id: hot_water
      name: 'Hot water'
    heatingwater:
      id: heating_water
      name: 'Heating water'
```
Configuration variables:

- **hotwater**: Climate to control the hot water temperature
  - **name** (Required, string): The name of the hot water climate
  - **id** (Optional, [ID](https://esphome.io/guides/configuration-types.html#config-id)): Manually specify the ID used for code generation.
- **heatingwater**: Climate to control the heatting water temperature
  - **name** (Required, string): The name of the heatting water climate
  - **id** (Optional, [ID](https://esphome.io/guides/configuration-types.html#config-id)): Manually specify the ID used for code generation.

### Sensors [WIP]

Here we will have a property for each one of the sensors in order to configure its name, id, ...

```yaml
sensor:
  - platform: opentherm
```

## Roadmap

v1.0:

- [x] Rewrite as ESPHome **External component**
- [x] Configure **OpenTherm In/Out pins**
- [x] **Climate** to manually control the **hot water** temperature
- [x] **Climate** to manually control the **heating water** temperature
- [ ] **Float output** to control the **heating** with a PID thermostat like [ESPHome PID Climate](https://esphome.io/components/climate/pid.html) or [HASmartThermostat](https://github.com/ScratMan/HASmartThermostat) 
- [ ] **Switch** to control the heating with a On/Off thermostat like [Home Assistant Generic Thermostat](https://www.home-assistant.io/integrations/generic_thermostat/)
- [ ] Sensors
  - [ ] Boiler temperature
  - [ ] Flame (binary)
  - [ ] External Temperature
  - [ ] Return Temperature
  - [ ] Heating Water Pressure
  - [ ] Boiler Modulation
  - [ ] Heating Target Temperature
- [ ] Detailed documentation with many examples

vNext:

- [ ] Asynchronous communication with the OpenTherm bus
- [ ] External temperature compensation
- [ ] ...