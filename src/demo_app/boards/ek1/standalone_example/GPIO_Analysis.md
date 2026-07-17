# GPIO Analysis: Standalone Example CTRX8188 Radar Interface

**Analysis Date:** 2026-07-16  
**Code Location:** `C:\Users\agondali\gmsl\radar\sockeye\sw_package\integration-ek1\src\demo_app\boards\ek1\standalone_example`  
**Target Hardware:** NVIDIA Jetson Orin AGX with P2S40 radar board (single CTRX_A operation)

---

## Executive Summary

**Total GPIOs Required: 9 pins**

The standalone example code uses **9 GPIO pins** to interface with the radar board for single CTRX_A operation. These GPIOs control power management, chip reset, SPI chip select, and radar status/synchronization signals.

---

## Complete GPIO Pin List

### General Control GPIOs (5 pins)

| Function | GPIO ID | Direction | Jetson Pin | Description |
|----------|---------|-----------|------------|-------------|
| **EN_PMIC** | PH.01 (gpiochip0) | OUTPUT | GPIO 44 | PMIC enable - powers up MAX20434 PMICs |
| **CTRX_RESET_N** | PH.06 (gpiochip0) | OUTPUT | GPIO 49 | Active-low reset for all CTRX chips |
| **CTRX_OK** | PBB.02 (gpiochip1) | INPUT | GPIO 10 | CTRX status indicator (shared across all chips) |
| **PGOOD_PMIC_LV** | PAC.02 (gpiochip0) | INPUT | GPIO 140 | Low-voltage PMIC power-good indicator |
| **PGOOD_PMIC_HV** | PAC.03 (gpiochip0) | INPUT | GPIO 141 | High-voltage PMIC power-good indicator |

### CTRX_A Specific GPIOs (4 pins)

| Function | GPIO ID | Direction | Jetson Pin | Description |
|----------|---------|-----------|------------|-------------|
| **RFT_A** | PDD.00 (gpiochip1) | INPUT | GPIO 20 | Ready-for-transmission status from CTRX_A |
| **SPI_SS_A** | PAC.04 (gpiochip0) | OUTPUT | GPIO 142 | SPI chip select for CTRX_A (active LOW) |
| **DMUX_1_A** | PAA.07 (gpiochip1) | INPUT | GPIO 7 | DMUX_1 status signal (ramp scenario completion) |
| **DMUX_2_A** | PH.05 (gpiochip0) | INPUT | GPIO 48 | DMUX_2 status signal (unused in config) |

---

## GPIO Configuration Details

### Initialization Sequence (from main.c)

```c
// 1. General Control (5 GPIOs)
PlatformGpio_configure(GPIO_ID_EN_PMIC, GPIO_FLAG_OUTPUT_DRIVE_LOW);
PlatformGpio_configure(GPIO_ID_CTRX_RESET_N, GPIO_FLAG_OUTPUT_DRIVE_LOW);
PlatformGpio_configure(GPIO_ID_CTRX_OK, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_PGOOD_PMIC_LV, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_PGOOD_PMIC_HV, GPIO_FLAG_INPUT_ENABLE);

// 2. CTRX_A Specific (4 GPIOs)
PlatformGpio_configure(GPIO_ID_RFT_A, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_SPI_SS_A, GPIO_FLAG_OUTPUT_DRIVE_HIGH);
PlatformGpio_configure(GPIO_ID_DMUX_1_A, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_DMUX_2_A, GPIO_FLAG_INPUT_ENABLE);
```

### Critical GPIO Usage in Code Execution

#### Power-Up Sequence
```c
// Step 1: Assert CTRX reset (keep chips in reset during power-up)
PlatformGpio_set(GPIO_ID_CTRX_RESET_N, false);  // Active LOW

// Step 2: Enable PMICs
PlatformGpio_set(GPIO_ID_EN_PMIC, true);
PlatformTime_waitTime(10);  // 10ms delay

// Step 3: De-assert CTRX reset after power stabilizes
PlatformGpio_set(GPIO_ID_CTRX_RESET_N, true);
PlatformTime_waitTime(10);
```

#### Ramp Scenario Execution (uses DMUX_1_A)
```c
// Before starting ramp: wait for DMUX_1_A to go LOW
PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, false, 10);

// Start ramp scenario
IfxRfe_startRampScenario();

// Wait for DMUX_1_A to go HIGH (signals ramp completion)
PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, true, 15);

// Finish ramp scenario
IfxRfe_finishRampScenario(NULL);
```

#### SPI Chip Select (SPI_SS_A)
- Controlled by `PlatformSpi` layer
- Driven LOW during SPI transactions (active LOW)
- Driven HIGH when idle (default state)

---

## GPIO Chip Assignment

### Jetson Orin GPIO Controller Mapping
```
gpiochip0: GPIOs 348-511 (tegra234-gpio)      - Main GPIO controller
gpiochip1: GPIOs 316-347 (tegra234-gpio-aon)  - Always-on GPIO controller
```

### Pin Distribution

**gpiochip0 (7 pins):**
- EN_PMIC (PH.01)
- CTRX_RESET_N (PH.06)
- PGOOD_PMIC_LV (PAC.02)
- PGOOD_PMIC_HV (PAC.03)
- SPI_SS_A (PAC.04)
- DMUX_2_A (PH.05)

**gpiochip1 (2 pins):**
- CTRX_OK (PBB.02)
- RFT_A (PDD.00)
- DMUX_1_A (PAA.07)

---

## GPIO Functional Roles

### Power Management (2 GPIOs)
- **EN_PMIC** - Master enable for MAX20434 PMICs
- **CTRX_RESET_N** - Active-low reset for radar chips

### Status Monitoring (3 GPIOs)
- **CTRX_OK** - Radar chip operational status
- **PGOOD_PMIC_LV** - Low-voltage rail status (monitoring only)
- **PGOOD_PMIC_HV** - High-voltage rail status (monitoring only)

### SPI Communication (1 GPIO)
- **SPI_SS_A** - Manual chip select control for CTRX_A

### Synchronization & Timing (3 GPIOs)
- **RFT_A** - Ready-for-transmission flag
- **DMUX_1_A** - **CRITICAL** - Ramp scenario synchronization (actively polled)
- **DMUX_2_A** - Configured as input but not actively used in code

---

## DMUX Configuration Analysis

From `config.c`:
```c
configParams->dmux.config_mask = ConfigMask_DMUX1;             // Update only DMUX_1
configParams->dmux.dmux1_alt_signal = AltSignal_DmuxALevel;   // Map level, not pulse
configParams->dmux.dmux1_dir = DmuxDir_out;                   // CTRX drives this as OUTPUT
configParams->dmux.dmux1_pulse_duration_ext = 63;

// DMUX_2 and DMUX_3 configured as inputs but not used
configParams->dmux.dmux2_dir = DmuxDir_in;
configParams->dmux.dmux3_dir = DmuxDir_in;
```

**Key Point:** DMUX_1_A is the **critical synchronization signal**. The code actively polls this GPIO before and after ramp execution:
- **LOW** → ready to start new ramp
- **HIGH** → ramp scenario executing/completed

---

## Unused GPIOs in Standalone Configuration

The code initializes **but does not actively use** the following GPIO configurations:

### CTRX_B GPIOs (4 pins) - Configured but Unused
- GPIO_ID_RFT_B
- GPIO_ID_SPI_SS_B
- GPIO_ID_DMUX_1_B
- GPIO_ID_DMUX_2_B

### CTRX_C GPIOs (4 pins) - Configured but Unused
- GPIO_ID_RFT_C
- GPIO_ID_SPI_SS_C
- GPIO_ID_DMUX_1_C
- GPIO_ID_DMUX_2_C

**Reason:** These GPIOs are initialized in main.c lines 68-78, but the standalone example only operates CTRX_A. The CTRX_B and CTRX_C pins remain idle but configured.

**Impact:** If strict GPIO resource counting is needed (e.g., for pinmux conflicts with GMSL integration), these 8 additional pins could be freed for other uses.

---

## Total GPIO Count Summary

### Actively Used (Required for Operation)
| Category | Count | Pins |
|----------|-------|------|
| Power Management | 2 | EN_PMIC, CTRX_RESET_N |
| Status Monitoring | 3 | CTRX_OK, PGOOD_PMIC_LV, PGOOD_PMIC_HV |
| SPI Control | 1 | SPI_SS_A |
| Synchronization | 3 | RFT_A, DMUX_1_A, DMUX_2_A |
| **Total Active** | **9** | |

### Initialized but Unused
| Category | Count | Pins |
|----------|-------|------|
| CTRX_B | 4 | RFT_B, SPI_SS_B, DMUX_1_B, DMUX_2_B |
| CTRX_C | 4 | RFT_C, SPI_SS_C, DMUX_1_C, DMUX_2_C |
| **Total Idle** | **8** | |

### Grand Total Configured
**17 GPIOs** are configured in code, but only **9 GPIOs** are functionally required for standalone CTRX_A operation.

---

## Additional Hardware Interfaces (Non-GPIO)

These interfaces are also required but use dedicated hardware peripherals, not general-purpose GPIOs:

### SPI Bus (4 signals)
- **CLK** - SPI clock at 3.125 MHz
- **MOSI** - Master-out-slave-in data
- **MISO** - Master-in-slave-out data
- **CS** - Chip select (controlled by GPIO_ID_SPI_SS_A, counted above)

Configuration: `/dev/spidev1.0` at 3.125 MHz with manual CS control

### I2C Bus (2 signals)
- **SDA** - I2C data
- **SCL** - I2C clock

Configuration: `/dev/i2c-2` (I2C3 hardware), used for MAX20434 PMIC control

### MIPI CSI-2 Interface (10 signals)
- **4x Data Lanes** (differential pairs = 8 signals)
- **1x Clock Lane** (differential pair = 2 signals)

Configuration: 1200 Mbps per lane, RAW12 format, continuous clock mode

---

## Critical Timing Constraints

### Power-Up Sequence
1. Assert CTRX_RESET_N LOW (before PMIC enable)
2. Enable PMICs (EN_PMIC HIGH)
3. Wait 10ms for power stabilization
4. De-assert CTRX_RESET_N HIGH
5. Wait 10ms before SPI communication

### DMUX_1_A Polling
- **Pre-ramp:** Block until DMUX_1_A = LOW (10ms timeout)
- **Post-start:** Block until DMUX_1_A = HIGH (15ms timeout)

---

## Recommendations for GMSL Integration

### GPIO Resource Planning
When integrating GMSL SERDES (MAX96723/MAX96726):

1. **Keep these 9 GPIOs:** Required for radar operation
2. **Consider freeing 8 GPIOs:** CTRX_B and CTRX_C pins if cascaded operation is not planned
3. **Review pinmux conflicts:** Check if GMSL I2C/SPI tunneling requires pins that overlap with current assignments
4. **Validate CSI-2 routing:** Ensure MIPI CSI-2 lanes can be routed through GMSL SERDES without affecting GPIO assignments

### Questions for GMSL Design
- Does GMSL I2C tunneling replace direct I2C access to PMICs?
- Does GMSL SPI tunneling affect the manual CS control strategy?
- Are additional GPIOs needed for GMSL link control (lock status, error flags)?
- Can DMUX_1_A synchronization signal route through GMSL GPIO tunneling?

---

## Code References

### Primary Files
- **main.c** (lines 52-78) - GPIO initialization and usage
- **HwDefinitions.h** (lines 56-107) - Pin mappings and GPIO ID definitions
- **config.c** (lines 54-61) - DMUX configuration parameters

### Platform Layer
- **PlatformGpio.h** - GPIO abstraction API (libgpiod v1.6)
- **PlatformGpio.c** - GPIO implementation with polling support

---

## Document Version

**Version:** 1.0  
**Last Updated:** 2026-07-16  
**Analyzed By:** Claude (Claudian) AI Assistant  
**Vault Location:** [[C:\Users\agondali\gmsl\radar\sockeye\sw_package\integration-ek1\src\demo_app\boards\ek1\standalone_example\GPIO_Analysis.md]]
