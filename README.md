# 🏗️ Ball Transport & Detection System (Arduino Uno)

This project brings together multiple sensors, motors, and an LCD display to create a **fully automated ball tracking and handling system**.  
It uses a **photoresistor**, **ultrasonic sensor**, **trapdoor switch**, **DC motor**, and **stepper motor**, all coordinated through an **Arduino Uno** with an **I²C LCD display** and **74HC595 shift register**.

---

## ⚙️ Hardware Overview

| Component | Function | Arduino Pins |
|------------|-----------|--------------|
| **DC Motor** | Pulley motor to lift balls | ENA = D8, IN1 = D13, IN2 = D12 |
| **Stepper Motor** | Trapdoor motor (opens/closes mechanism) | D3, D5, D6, D9 |
| **Ultrasonic Sensor (HC-SR04)** | Detects passing balls | Trig = Q7 (shift reg), Echo = A0 |
| **Photoresistor (LDR)** | Detects when a ball interrupts the light | A1 |
| **Trapdoor Switch** | Signals trapdoor completion | A2 |
| **Entry Switch** | Tracks balls entering the system | A3 |
| **Shift Register (74HC595N)** | Controls trigger signal to ultrasonic | Data = D2, Latch = D7, Clock = D4 |
| **LCD (1602 I²C)** | Displays total balls passed | SDA = A4, SCL = A5 |

---

## 🧠 System Behavior

The system tracks a ball’s journey through the mechanism using light, distance, and switch sensors:
1. **Entry Detection:**  
   When the entry switch is triggered, the LCD updates the total number of balls that have passed through.
2. **Pulley Control:**  
   When the LDR detects a shadow (light < `lightUnobstructed`), the DC motor starts running to pull the ball up.
3. **Trapdoor Operation:**  
   The trapdoor opens via the stepper motor when the trapdoor switch (`sensorPin2`) detects a ball ready to be released.
4. **Ultrasonic Tracking:**  
   An ultrasonic pulse triggered through the shift register confirms when the ball has been seen leaving the top of the system.
5. **Automatic Stop:**  
   Once the number of balls seen matches the number through the system, the pulley motor stops automatically.

---

## 🧾 Key Variables

| Variable | Description |
|-----------|-------------|
| `ballsThroughGSpot` | Total balls processed through the system |
| `ballsGrabbed` | Balls detected by the LDR (picked up) |
| `ballsTrapped` | Balls released by the trapdoor |
| `ballsSeen` | Balls confirmed by ultrasonic sensor |
| `timePulley1Ball`, `timePulley2Balls` | Travel time calibration for one or two balls |
| `lightUnobstructed` | Threshold for LDR detection |
| `shiftOutState` | Current bit state of shift register |
| `motor1ON` | Boolean flag for pulley motor status |

---

## 💡 Display Output

The I²C LCD shows:
Balls Through: X
where `X` increments each time the entry switch is triggered.

The **Serial Monitor** logs live sensor values, motor activations, and timing data for debugging:
Ultrasonic Distance: 7 cm
S1 Light Value: 421 - S2 Voltage Status: 1
B: 2 - T: 1 - S: 1

Pulley is ON!
Trapdoor Opening...


---

## 🧰 Libraries Used

- `Wire.h` — I²C communication  
- `LiquidCrystal_PCF8574.h` — LCD over I²C interface  
- `Stepper.h` — Stepper motor control  
- `74HC595` shift register logic (manual bit manipulation)  

---

## 🧩 Highlights

- Combines **analog + digital + timing logic** in one loop  
- Uses **shift register** to free up pins for sensors  
- Employs **millis()** for dynamic timing calibration  
- Integrates **LCD display feedback** for real-time visualization  
- Implements **modular helper functions** for clarity (`pingUltrasonic()`, `stopMotor()`, etc.)

---

## 🚀 Future Enhancements

- Replace blocking `delay()` calls with non-blocking timing  
- Implement adaptive travel time averaging  
- Add LED bar visualization for “balls in system” status  
- Integrate EEPROM or SD card logging for run tracking  
- Wireless version using ESP8266 or Arduino Nano IoT  

---

## 🛠️ Requirements

- Arduino Uno  
- HC-SR04 Ultrasonic Sensor  
- 74HC595 Shift Register  
- LDR (Photoresistor)  
- Push Button / Micro Switches/Ball-Completion Circuit
- 28BYJ-48 Stepper Motor + ULN2003 Driver  
- DC Motor + Driver Circuit (L298N or equivalent)  
- 1602 I²C LCD Display  
