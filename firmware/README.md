# Wokwi firmware

Target: `board-esp32-s3-devkitc-1`.

## Pins

- I2C SDA GPIO8
- I2C SCL GPIO9
- Buzzer GPIO10
- Status LEDs GPIO11/12/13
- Potentiometers: GPIO1..GPIO6
- Scenario buttons: GPIO14..GPIO19

The firmware deliberately does not name unsupported virtual sensor parts as if they existed. Potentiometers and deterministic generators stand in for the listed medical/environmental sensors.
