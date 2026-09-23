## ESPHome custom components

My creation of a nilan component for ESPHome.

### Example of minimal configuration yaml
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: main
    files: [components/nilan/basic.yaml]
    refresh: 0s

uart:
  rx_pin: GPIO16
  tx_pin: GPIO17
  parity: EVEN
  baud_rate: 19200
  id: uart_modbus
  stop_bits: 1
  
modbus:
  id: modbus_id
  uart_id: uart_modbus
  
modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
```

### Example of a yaml configuration with sensors all cts602 based models have in common
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: main
    files: [components/nilan/all.yaml]
    refresh: 0s

uart:
  rx_pin: GPIO16
  tx_pin: GPIO17
  parity: EVEN
  baud_rate: 19200
  id: uart_modbus
  stop_bits: 1
  
modbus:
  id: modbus_id
  uart_id: uart_modbus
  
modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
```

### Model packages and optional hardware

The model packages provide a complete register set for a specific Nilan controller. Add exclusion files after the model package to remove entities for hardware that is not installed. The optional `display.yaml` package can be added when display registers are needed.

```yaml
packages:
  nilan:
    url: https://github.com/Jopand/esphome_components
    ref: main
    files:
      - components/nilan/cts602.yaml
      # - components/nilan/display.yaml

      # Remove the features that are not installed in this unit.
      - components/nilan/exclude/air_damper.yaml
      - components/nilan/exclude/air_heater.yaml
      - components/nilan/exclude/compressor.yaml
      - components/nilan/exclude/co2_sensor.yaml
      - components/nilan/exclude/ek.yaml
      - components/nilan/exclude/hot_gas.yaml
      - components/nilan/exclude/hot_water.yaml
    refresh: 0s

uart:
  rx_pin: GPIO16
  tx_pin: GPIO17
  parity: EVEN
  baud_rate: 19200
  id: uart_modbus
  stop_bits: 1

modbus:
  id: modbus_id
  uart_id: uart_modbus

modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
```

The existing `basic.yaml`, `all.yaml`, `light.yaml`, `comfort.yaml`, and `full.yaml` package paths remain available for existing configurations.

### Example of a Nilan light compatible configuration yaml
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: main
    files: [components/nilan/light.yaml]
    refresh: 0s

uart:
  rx_pin: GPIO16
  tx_pin: GPIO17
  parity: EVEN
  baud_rate: 19200
  id: uart_modbus
  stop_bits: 1
  
modbus:
  id: modbus_id
  uart_id: uart_modbus
  
modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
```

### Example of a Nilan CTS400 based configuration yaml. Remember to set the proper output for user function 1 and 2
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: main
    files: [components/nilan/cts400.yaml]
    refresh: 0s

substitutions:
  user_function_1_output: '3'
  user_function_2_output: '4'

uart:
  rx_pin: GPIO16
  tx_pin: GPIO17
  parity: EVEN
  baud_rate: 19200
  id: uart_modbus
  stop_bits: 1
  
modbus:
  id: modbus_id
  uart_id: uart_modbus
  
modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
```

### NOTE: The following examples might not work well with ESP8266 controllers. An ESP32 is recommended ###
### If you only have a ESP8266 controller, use the all.yaml example and add some extra sensors manually by copy-pasting from comfort.yaml or full.yaml ###

### Example of a Nilan Comfort compatible configuration yaml
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: main
    files: [components/nilan/comfort.yaml]
    refresh: 0s

uart:
  rx_pin: GPIO16
  tx_pin: GPIO17
  parity: EVEN
  baud_rate: 19200
  id: uart_modbus
  stop_bits: 1
  
modbus:
  id: modbus_id
  uart_id: uart_modbus
  
modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
```
