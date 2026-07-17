# GPIO Quick Reference: Standalone CTRX_A Operation

## **Total Required: 9 GPIOs**

---

## Pin Assignment Table

| #   | Signal Name   | Jetson Pin        | Direction | Chip  | Purpose                | Critical?      |
| --- | ------------- | ----------------- | --------- | ----- | ---------------------- | -------------- |
| 1   | EN_PMIC       | PH.01 (GPIO 44)   | OUTPUT    | chip0 | Power enable for PMICs | ✅ YES          |
| 2   | CTRX_RESET_N  | PH.06 (GPIO 49)   | OUTPUT    | chip0 | Active-low reset       | ✅ YES          |
| 3   | CTRX_OK       | PBB.02 (GPIO 10)  | INPUT     | chip1 | CTRX status flag       | ⚠️ Monitor     |
| 4   | PGOOD_PMIC_LV | PAC.02 (GPIO 140) | INPUT     | chip0 | LV power good          | ⚠️ Monitor     |
| 5   | PGOOD_PMIC_HV | PAC.03 (GPIO 141) | INPUT     | chip0 | HV power good          | ⚠️ Monitor     |
| 6   | SPI_SS_A      | PAC.04 (GPIO 142) | OUTPUT    | chip0 | SPI chip select        | ✅ YES          |
| 7   | RFT_A         | PDD.00 (GPIO 20)  | INPUT     | chip1 | Ready-for-TX           | ℹ️ Info        |
| 8   | DMUX_1_A      | PAA.07 (GPIO 7)   | INPUT     | chip1 | Ramp sync (polled)     | ✅ **CRITICAL** |
| 9   | DMUX_2_A      | PH.05 (GPIO 48)   | INPUT     | chip0 | Configured, unused     | ❌ No           |
|     |               |                   |           |       |                        |                |

---

## Functional Grouping

### 🔌 Power Control (2 pins)
```
EN_PMIC --------> MAX20434 PMICs (enable power supplies)
CTRX_RESET_N ---> CTRX chips (active-low reset)
```

### 📊 Status Monitoring (3 pins)
```
CTRX_OK ---------> Radar operational status
PGOOD_PMIC_LV --> Low-voltage rail OK
PGOOD_PMIC_HV --> High-voltage rail OK
```

### 🔄 Communication (1 pin)
```
SPI_SS_A --------> SPI chip select (active LOW)
```

### ⏱️ Synchronization (3 pins)
```
RFT_A -----------> Ready-for-transmission flag
DMUX_1_A --------> **Ramp completion sync** (actively polled!)
DMUX_2_A --------> Initialized but not used
```

---

## Critical Execution Flow

### 1. Power-Up Sequence
```
[1] Set CTRX_RESET_N = LOW    (hold reset)
[2] Set EN_PMIC = HIGH         (power on)
[3] Wait 10ms
[4] Set CTRX_RESET_N = HIGH    (release reset)
[5] Wait 10ms
```

### 2. Ramp Scenario Loop
```
[1] Poll DMUX_1_A until LOW     (ready for new ramp)
[2] IfxRfe_startRampScenario()  (begin chirp)
[3] Poll DMUX_1_A until HIGH    (ramp executing)
[4] IfxRfe_finishRampScenario() (complete)
```

---

## GPIO Chip Distribution

**gpiochip0 (GPIOs 348-511):** 7 pins  
- EN_PMIC, CTRX_RESET_N, PGOOD_PMIC_LV, PGOOD_PMIC_HV  
- SPI_SS_A, DMUX_2_A

**gpiochip1 (GPIOs 316-347):** 2 pins  
- CTRX_OK, RFT_A, DMUX_1_A

---

## Additional Hardware Interfaces (Not GPIOs)

### SPI Bus: `/dev/spidev1.0` @ 3.125 MHz
- CLK, MOSI, MISO (shared)
- CS controlled by SPI_SS_A GPIO

### I2C Bus: `/dev/i2c-2` (I2C3)
- SDA, SCL (2 dedicated pins)
- Used for PMIC configuration

### MIPI CSI-2: 4 lanes @ 1200 Mbps
- 4x data lanes (differential)
- 1x clock lane (differential)
- Total: 10 signals (5 differential pairs)

---

## GMSL Integration Notes

### GPIOs to Keep (9 pins)
All 9 listed above are required for basic radar operation.

### Optional GPIOs to Reclaim (8 pins)
CTRX_B and CTRX_C pins are initialized but unused in standalone mode:
- RFT_B, SPI_SS_B, DMUX_1_B, DMUX_2_B
- RFT_C, SPI_SS_C, DMUX_1_C, DMUX_2_C

These could be freed for GMSL control signals if cascaded operation is not planned.

### GMSL Considerations
- **I2C Tunneling:** Will GMSL replace direct PMIC I2C access?
- **SPI Tunneling:** Can manual CS control work through GMSL?
- **DMUX_1_A Sync:** Can this critical polling signal route through GMSL GPIO tunneling with acceptable latency?
- **Additional GPIOs:** Does GMSL require lock status, error flags, or control pins?

---

## Quick Troubleshooting

### Problem: Ramp scenario hangs
**Check:** DMUX_1_A polling timeout  
**Solution:** Verify DMUX configuration in config.c (dmux1_alt_signal, dmux1_dir)

### Problem: SPI communication fails
**Check:** SPI_SS_A not toggling correctly  
**Solution:** Verify manual CS control in PlatformSpi_transfer_override()

### Problem: Power-up sequence fails
**Check:** CTRX_RESET_N timing relative to EN_PMIC  
**Solution:** Ensure reset is LOW during power-up, then HIGH after 10ms

---

## File Locations

**Pin Definitions:** `HwDefinitions.h` (lines 56-107)  
**GPIO Init:** `main.c` (lines 52-78)  
**DMUX Config:** `config.c` (lines 54-61)  
**Full Analysis:** `GPIO_Analysis.md` (same directory)

---

**Last Updated:** 2026-07-16  
**Target:** NVIDIA Jetson Orin AGX + P2S40 (CTRX_A standalone)
