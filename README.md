# The TBot: A Gesture Controlled Car

A car controlled entirely through hand gestures, with no physical controller required. TBot uses computer vision and Bluetooth Low Energy (BLE) to translate gestures into real-time movement commands across a three-device embedded system.

![Project Poster](Project_poster.png)
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
| Like (Thumbs up) | Forward |
| Dislike (Thumbs down) | Backward |
| Fist | Turn clockwise |
| Ok | Turn counterclockwise |
| Open Palm | Stop |

---

## Hardware

- 3x Arduino Nano 33 BLE Sense
- 2x Solderless Breadboard
- Jumper Wires
- Camera module (OV7670 or OV7675)
- 4x TT 1:48 DC Motors
- 2x TB6612FNG Motor Drivers
- Car chassis

## Libraries Required

- `Arduino_OV767X`
- `ArduinoBLE`
- `Arduino_TensorFlowLite`

---

## Repository Structure

---

## Demo

📹 [Watch TBot in action](YOUR_GOOGLE_DRIVE_LINK_HERE)

---

## Background

Built as a high school engineering project. The goal was to explore edge ML inference and multi-device BLE communication on low-power embedded hardware.


