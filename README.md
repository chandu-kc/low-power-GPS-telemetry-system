# Low-Power GPS Telemetry System Using STM32 & Cavli Module

##  Project Overview
This project implements a **low-power GPS telemetry system** using an **STM32 L-series microcontroller** and a **Cavli GPS module**.

The system periodically:
1. Wakes up from low-power sleep
2. Acquires the current GPS location
3. Transmits the data
4. Returns to sleep

By using **low-power modes and a state-machine-based firmware architecture**, the system achieves an ultra-low sleep current of approximately **200 µA**, significantly extending battery life.

---

##  Objective
- Periodically transmit GPS location data
- Minimize power consumption for battery-operated systems
- Achieve reliable sleep–wake operation
- Use structured, maintainable firmware design

---

##  System Operation Flow

1. Device starts in **SLEEP state**
2. Wakes every **10 minutes** using RTC
3. Powers ON the Cavli GPS module
4. Acquires GPS fix
5. Transmits location data
6. Powers OFF GPS
7. Returns to **SLEEP state**

---

##  Firmware Architecture

The entire application is implemented using a **finite state machine (FSM)** for clarity, scalability, and reliability.

### States:
- `STATE_INIT`
- `STATE_SLEEP`
- `STATE_WAKEUP`
- `STATE_GPS_POWER_ON`
- `STATE_GPS_ACQUIRE`
- `STATE_DATA_TRANSMIT`
- `STATE_GPS_POWER_OFF`
- `STATE_RETURN_TO_SLEEP`

This approach ensures:
- Deterministic behavior
- Easy debugging
- Low CPU active time
- Better power control

---

##  Power Optimization

### Power Strategy
- STM32 low-power sleep modes
- Peripheral clock gating
- GPS module powered only when required
- UART disabled during sleep
- RTC used for timed wake-up

### Achieved Performance
- **One Complete Cycle IN**: ~**312 mA**
- **Sleep current**: ~**200 µA**
- **Active duration**: Only during GPS acquisition and    transmission
- **Duty cycle optimized** for maximum battery life

---

##  Hardware Used
- **Microcontroller**: STM32 L-series
- **GPS Module**: Cavli GPS
- **RTC**: Internal STM32 RTC
- **Communication**: UART
- **Power Source**: Battery-powered system

---

##  Data Transmission
- GPS latitude and longitude acquired
- Data transmitted periodically every **10 minutes**
- Transmission window minimized to reduce power consumption

---

##  Timing Configuration
| Parameter | Value |
|--------|------|
Wake-up interval | 10 minutes |
Active time | GPS fix + transmit |
Sleep current | ~200 µA |

---

##  Project Structure

```
low-power-gps-telemetry/
├── firmware/
│   ├── Core/
│   ├── Drivers/
│   ├── state_machine.c
│   └── main.c
├── docs/
│   └── power_measurement.md
├── images/
│   └── system_block_diagram.png
├── logs/
│   └── gps_transmission_log.txt
├── README.md
└── LICENSE

```
---
## Testing & Validation

Current consumption measured using multimeter and nRF PPK (Power Profiler Kit)

Sleep current verified at ~200 µA

GPS acquisition validated outdoors

Wake-up interval verified using RTC timing

---

## Industrial Relevance

Asset tracking devices

Battery-powered IoT products

Remote telemetry systems

Low-power embedded designs

Field-deployed monitoring devices

---
## Disclaimer

This project is developed for learning and demonstration purposes only.
No proprietary or confidential company code is included.

---

## Author
Chandan K C
Embedded Systems Engineer
Expertise in STM32, Low-Power Design, GPS, UART, and Embedded C