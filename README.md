# Arduino Multimeter Project

This is a DIY multimeter built using an **Arduino UNO**, an **LCD display**, and basic electronic components. It can measure:

- **Current (mA)**
- **Voltage (V)**
- **Resistance (Ohms/kOhms)**
- **Capacitance (uF)**

## 📷 Project Overview

![Multimeter Project](multimeter.jpg)

`🔧 Hardware Used`

- Arduino UNO
- 16x2 LCD Display (with RS, EN, D4-D7)
- DIP Switches (to select mode)
- 10kΩ resistor (for charging capacitor)
- 950Ω known resistor (for resistance calculation)
- 0.5Ω shunt resistor (for current sensing)
- Breadboard, jumper wires, basic passive components

`📐 Circuit Description`

| DIP Switch | Mode |
|------------|------|
| Pin 9      | Current |
| Pin 10     | Voltage |
| Pin 11     | Resistance |
| Pin 12     | Capacitance |

- **A5**: Voltage divider (resistance)
- **A1**: Current sensing
- **A2**: Voltage measurement (via divider)
- **A0**: Capacitance reading

`💻 How It Works`

- Select a mode using DIP switch.
- Arduino reads the relevant analog pins.
- Data is processed and displayed on LCD.
- Serial Monitor also prints the results.

### Measurement Techniques:
- **Voltage**: Read through a voltage divider.
- **Current**: Use a shunt resistor to measure voltage drop.
- **Resistance**: Voltage divider method.
- **Capacitance**: Time-based charging of capacitor.

