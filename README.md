# This is a fork of Szczepan's custom esphome components that supports water metering using the Apator NAXOM: OP-04-1a, OP-04-1b, and OP-04-2.

This has been tested with ESP32 and CC1101.

[Original source](https://github.com/SzczepanLeon/esphome-components/tree/version_4/)


## 1. Usage

Use latest [ESPHome](https://esphome.io/)
with external components and add this to your `.yaml` definition:

```yaml
external_components:
  - source: github://MaciejR89/esphome-components@version_4
```

## 2. Example

```yaml
captive_portal:

time:
  - platform: sntp
    id: time_sntp

external_components:
  - source: github://MaciejR89/esphome-components@version_4
    refresh: 0d
    components: [ wmbus ]


wmbus:
  mosi_pin: GPIO23
  miso_pin: GPIO19
  clk_pin:  GPIO18
  cs_pin:   GPIO15
  gdo0_pin: GPIO4
  gdo2_pin: GPIO27

  led_pin: GPIO2
  led_blink_time: "1s"

  frequency: 868.950
  all_drivers: False
  sync_mode: True
  log_all: True

sensor:
  - platform: wmbus
    meter_id: 0x********
    # ^^ Replace the * with the ID of your water meter
    type: apator_op04
    key: "********************************"
    # ^^ Replace the * with the key assigned to your water meter

    sensors:
      - name: "Ciepła woda sygnał"
        field: "rssi"
        accuracy_decimals: 0
        unit_of_measurement: "dBm"
        device_class: "signal_strength"
        state_class: "measurement"
        entity_category: "diagnostic"

      - name: "Ciepła woda"
        field: "total"
        accuracy_decimals: 3
        unit_of_measurement: "m³"
        device_class: "water"
        state_class: "total_increasing"
        icon: "mdi:water"


  - platform: wmbus
    meter_id: 0x********
    # ^^ Replace the * with the ID of your water meter
    type: apator_op04
    key: "********************************"
    # ^^ Replace the * with the key assigned to your water meter

    sensors:
      - name: "Zimna woda sygnał"
        field: "rssi"
        accuracy_decimals: 0
        unit_of_measurement: "dBm"
        device_class: "signal_strength"
        state_class: "measurement"
        entity_category: "diagnostic"

      - name: "Zimna woda"
        field: "total"
        accuracy_decimals: 3
        unit_of_measurement: "m³"
        device_class: "water"
        state_class: "total_increasing"
        icon: "mdi:water"
```

Then in logs you will have trace with telegram:
```
[10:05:05.452][I][wmbus:100]: apator_op04 [0x********] RSSI: -80dBm T: 61440106164004101A078CC01E7AEF00508521B465B4525E63928C3A31E2A21E6F6A6A5F8B2B3C6554F02134937421191B58DBACAC8CC22DBF8011334C124B06E7955062EC36270760B2D45AF1A5DD8EE534A08F2B259241F3A3308EC65093AE9F45 (98) T1 A
[10:05:05.465][D][meters.cpp:1985]: (meter) created ESPHome apator_op04 10044016 encrypted
[10:05:05.470][D][meters.cpp:909]: (meter) ESPHome(0) apator_op04  handling telegram from 10044016.M=APA.V=1a.T=07
[10:05:05.489][D][Telegram.cpp:563]: (telegram) ELL CI=8c CC=c0 (bidir fast_resp) ACC=1e
[10:05:05.503][D][sensor:135]: 'Zimna woda sygnał': Sending state -80.00000 dBm with 0 decimals of accuracy
[10:05:05.509][D][sensor:135]: 'Zimna woda': Sending state 33.99400 m³ with 3 decimals of accuracy
```

```

## 3. Author & License

Szczepan, Maciejr GPL, 2026
