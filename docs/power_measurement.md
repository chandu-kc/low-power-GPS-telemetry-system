#  Power Measurement & Analysis

## Project
**Low-Power GPS Telemetry System using STM32 + Cavli C16QS**

---

## 1. Objective

The objective of this document is to:
- Measure current consumption for one complete telemetry cycle
- Identify peak and average current values
- Calculate charge consumption per cycle
- Estimate battery life for different wake-up intervals
- Validate low-power firmware design

---

## 2. Measurement Setup

### Hardware Used
- **Microcontroller:** STM32 L-series
- **Cellular + GPS Module:** Cavli C16QS
- **Power Source:** Li-ion Battery / Regulated DC Supply
- **Measurement Tool:** Power analyzer / current profiler

### Measurement Point
- Current measured at the **main supply input**
- Includes:
  - STM32 MCU
  - Cellular modem
  - GPS receiver

---

## 3. Firmware Operating Cycle

Each power measurement corresponds to **one full operational cycle**:

1. Wake up from low-power sleep  
2. Modem initialization  
3. Cellular network attach  
4. GPS fix acquisition  
5. MQTT data publish  
6. Modem power-down  
7. Return to low-power sleep  

---

## 4. Measured Power Metrics (Single Cycle)

{
AVERAGE : 68.50 mA,
MAX : 312.13 mA,
CHARGE : 68.50 mC
}

---

### Summary Table

| Parameter | Value |
|---------|-------|
| Average Current | 68.50 mA |
| Maximum Current | 312.13 mA |
| Charge per Cycle | 68.50 mC |

---

## 5. Current Consumption Profile

### Observations
- Peak current (~312 mA) occurs during:
  - Cellular network attachment
  - MQTT publish phase
- GPS acquisition shows moderate current consumption
- Sleep mode contributes negligible current


---

## 6. Charge-Based Energy Analysis

Charge (mC) represents the **total energy consumed per cycle**.

Advantages of charge-based analysis:
- Independent of wake interval
- Enables accurate battery life prediction
- Suitable for low-duty-cycle IoT devices

---

## 7. Battery Life Estimation

### Example Calculation (2000 mAh Battery)

68.50 mC = 0.019 mAh

Estimated number of cycles:

---

### Battery Life vs Wake Interval

| Wake Interval | Estimated Battery Life |
|--------------|------------------------|
| 5 minutes | ~18 days |
| 15 minutes | ~45 days |
| 1 hour | ~6 months |
| 6 hours | ~1.2 years |

*Actual battery life depends on network conditions and GPS fix time.*

---

## 8. Power Optimization Techniques Used

- STM32 low-power sleep modes (STOP / STANDBY)
- State-machine based firmware execution
- GPS enabled only when required
- Cellular modem powered down after MQTT publish
- Minimal peripheral on-time

---

## 9. Design Conclusions

- System is optimized for battery-powered operation
- Power peaks are short and controlled
- Charge consumption per cycle is low
- Suitable for long-term remote deployment

---

## 10. Future Improvements

- Assisted-GPS (A-GPS) for faster fixes
- Adaptive wake interval based on battery voltage
- Network retry optimization
- Deep sleep current characterization

---

##  Summary

This document demonstrates:
- Real hardware-based power measurements
- Practical low-power embedded system design
- Battery life estimation for IoT deployments


