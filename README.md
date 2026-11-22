# Smart Water Tank Monitoring and Overflow Prevention System

An Arduino-based system that monitors the water level in a tank, displays it in real time, and automatically controls a pump to prevent overflow and water wastage.

**Monitor • Alert • Save Water**

## Author
Sandhya Murugesan
B.Tech, Electronics and Communication Engineering (ECE Core), 1st Year, SRMIST, Kattankulathur, Chennai

## Objective
To design a smart system that monitors the water level in a tank, displays the remaining level, and prevents water wastage due to overflow.

## How It Works
1. An **HC-SR04 ultrasonic sensor**, mounted at the top of the tank, measures the distance to the water surface using sound-wave reflection (time-of-flight).
2. The **Arduino Uno** converts the distance into a water level percentage and shows it on a **16x2 I2C LCD**.
3. When the water reaches the **full level**, the relay switches the pump **OFF** and the buzzer beeps once.
4. When the water drops to the **low level**, the relay switches the pump **ON** automatically and the buzzer beeps once.
5. Between the two levels, the pump keeps its last state, so it does not switch on and off repeatedly.

## Components Used
| Component | Purpose |
|---|---|
| Arduino Uno | Main controller |
| HC-SR04 ultrasonic sensor | Measures the water level |
| 16x2 LCD with I2C module | Displays level and pump status |
| 5V relay module | Switches the pump |
| Buzzer module | Alert when the pump switches |
| Mini DC submersible pump | Fills the tank |
| Battery | Powers the pump |
| Breadboard and jumper wires | Connections |

## Pin Connections
| Component | Arduino Pin |
|---|---|
| Ultrasonic Trig | 9 |
| Ultrasonic Echo | 10 |
| Relay IN | 7 |
| Buzzer signal | 8 |
| LCD SDA | A4 |
| LCD SCL | A5 |
| Sensor, LCD, relay and buzzer VCC | 5V |
| Sensor, LCD, relay and buzzer GND | GND |

The pump is connected through the relay's **COM** and **NO** terminals, with its own battery supply.

## Physics Concepts
- Sound-wave reflection
- Time-of-flight measurement
- Distance and level calculation

## Setup
1. Install the **Arduino IDE**.
2. Install the **LiquidCrystal I2C** library (Tools → Manage Libraries).
3. Open `water_tank_sandhya.ino`.
4. Set `FULL_DIST` and `EMPTY_DIST` in the code to match your tank (distance from the sensor to the water surface, in cm).
5. Select **Board: Arduino Uno** and the correct COM port, then click **Upload**.

## Tuning
- If the pump runs when it should be off, swap `RELAY_ON` and `RELAY_OFF`.
- If the LCD stays blank, change the I2C address from `0x27` to `0x3F`, or adjust the contrast knob.

## Applications
- Homes and apartments
- College hostels
- Water storage tanks
- Small industries

## Future Improvements
- Dry-run protection for the pump
- IoT or mobile app monitoring
- Waterproof sensor housing

## Acknowledgements
The code was developed with AI assistance and tested by me on the hardware prototype.

## License
MIT License
