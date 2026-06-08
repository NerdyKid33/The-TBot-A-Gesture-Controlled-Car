# The TBot: A Gesture Controlled Car

A car controlled entirely through hand gestures, with no physical controller required. TBot uses computer vision and Bluetooth Low Energy (BLE) to translate gestures into real-time movement commands across a three-device embedded system.


---

## How It Works

1. **Capture** — A camera module on the first Arduino Nano captures an image of the user's hand
2. **Inference** — The image is processed and run through an on-device ML model to classify the gesture
3. **Relay** — The result is transmitted via BLE to a second Nano acting as a relay
4. **Movement** — The car Nano receives the command and spins the wheels accordingly

---

## Gestures → Commands

| Gesture | Action |
|--------|--------|
| ✋ Open hand | Forward |
| ✊ Fist | Stop |
| 👈 Point left | Turn left |
| 👉 Point right | Turn right |

*(update with your actual gestures)*

---

## Hardware

- 3x Arduino Nano 33 BLE Sense
- Camera module (OV7675 or similar)
- DC motors + motor driver
- Car chassis

## Libraries Required

- `Arduino_OV767X`
- `ArduinoBLE`
- `Arduino_TensorFlowLite`

---

## Repository Structure

![Project Poster](Project_poster.png)
