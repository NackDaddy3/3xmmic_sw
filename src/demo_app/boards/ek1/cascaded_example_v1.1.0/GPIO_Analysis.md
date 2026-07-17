# GPIO Analysis: Cascaded Example 3x CTRX8188 Radar Interface

**Created:** 2026-07-17 17:30  
**Last Updated:** 2026-07-17 17:30  
**Analysis Date:** 2026-07-17  
**Code Location:** `C:\Users\agondali\gmsl\radar\sockeye\sw_package\integration-ek1\src\demo_app\boards\ek1\cascaded_example_v1.1.0`  
**Target Hardware:** NVIDIA Jetson Orin AGX with P2S40 radar board (3x CTRX synchronized operation)

---

## Executive Summary

**Total GPIOs Required: 17 pins**

The cascaded example code uses **17 GPIO pins** to interface with three radar chips (CTRX_A, CTRX_B, CTRX_C) in a synchronized cascaded configuration. These GPIOs control power management, chip reset, SPI chip select, and radar status/synchronization signals for all three CTRX devices.

**Key Difference from Standalone:** All 17 pins are actively utilized (vs. 9 active + 8 idle in standalone). The cascaded example operates CTRX_A as PRIMARY and CTRX_B/C as SECONDARY devices in synchronized operation.

---

## Complete GPIO Pin List

### General Control GPIOs (5 pins) - Shared

| Function | GPIO ID | Direction | Jetson Pin | Description |
|----------|---------|-----------|------------|-------------|
| **EN_PMIC** | PH.01 (gpiochip0) | OUTPUT | GPIO 44 | PMIC enable - powers up MAX20434 PMICs (AB and C) |
| **CTRX_RESET_N** | PH.06 (gpiochip0) | OUTPUT | GPIO 49 | Active-low reset for ALL CTRX chips (A, B, C) |
| **CTRX_OK** | PBB.02 (gpiochip1) | INPUT | GPIO 10 | CTRX status indicator (shared across all three chips) |
| **PGOOD_PMIC_LV** | PAC.02 (gpiochip0) | INPUT | GPIO 140 | Low-voltage PMIC power-good indicator |
| **PGOOD_PMIC_HV** | PAC.03 (gpiochip0) | INPUT | GPIO 141 | High-voltage PMIC power-good indicator |

### CTRX_A Specific GPIOs (4 pins)

| Function     | GPIO ID            | Direction | Jetson Pin | Description                                                     |
| ------------ | ------------------ | --------- | ---------- | --------------------------------------------------------------- |
| **RFT_A**    | PDD.00 (gpiochip1) | INPUT     | GPIO 20    | Ready-for-transmission status from CTRX_A (PRIMARY)             |
| **SPI_SS_A** | PAC.04 (gpiochip0) | OUTPUT    | GPIO 142   | SPI chip select for CTRX_A (active LOW)                         |
| **DMUX_1_A** | PAA.07 (gpiochip1) | INPUT     | GPIO 7     | **CRITICAL** - Ramp scenario synchronization signal for PRIMARY |
| **DMUX_2_A** | PH.05 (gpiochip0)  | INPUT     | GPIO 48    | DMUX_2 status signal (unused in config)                         |

### CTRX_B Specific GPIOs (4 pins)

| Function | GPIO ID | Direction | Jetson Pin | Description |
|----------|---------|-----------|------------|-------------|
| **RFT_B** | PAC.01 (gpiochip0) | INPUT | GPIO 139 | Ready-for-transmission status from CTRX_B (SECONDARY) |
| **SPI_SS_B** | PCC.03 (gpiochip1) | OUTPUT | GPIO 315 | SPI chip select for CTRX_B (active LOW) |
| **DMUX_1_B** | PCC.07 (gpiochip1) | INPUT | GPIO 319 | DMUX_1 status signal (configured as input but not actively polled) |
| **DMUX_2_B** | PP.00 (gpiochip0) | INPUT | GPIO 100 | DMUX_2 status signal (unused in config) |

### CTRX_C Specific GPIOs (4 pins)

| Function | GPIO ID | Direction | Jetson Pin | Description |
|----------|---------|-----------|------------|-------------|
| **RFT_C** | PN.00 (gpiochip0) | INPUT | GPIO 84 | Ready-for-transmission status from CTRX_C (SECONDARY) |
| **SPI_SS_C** | PAA.4 (gpiochip1) | OUTPUT | GPIO 4 | SPI chip select for CTRX_C (active LOW) |
| **DMUX_1_C** | PN.07 (gpiochip0) | INPUT | GPIO 91 | DMUX_1 status signal (configured as input but not actively polled) |
| **DMUX_2_C** | PAC.00 (gpiochip0) | INPUT | GPIO 138 | DMUX_2 status signal (unused in config) |

---

## GPIO Configuration Details

### Initialization Sequence (from main.c, lines 55-79)

```c
// 1. General Control GPIOs (5 pins)
PlatformGpio_configure(GPIO_ID_EN_PMIC, GPIO_FLAG_OUTPUT_DRIVE_LOW);
PlatformGpio_configure(GPIO_ID_CTRX_RESET_N, GPIO_FLAG_OUTPUT_DRIVE_LOW);
PlatformGpio_configure(GPIO_ID_CTRX_OK, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_PGOOD_PMIC_LV, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_PGOOD_PMIC_HV, GPIO_FLAG_INPUT_ENABLE);

// 2. CTRX_A GPIOs (4 pins)
PlatformGpio_configure(GPIO_ID_RFT_A, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_SPI_SS_A, GPIO_FLAG_OUTPUT_DRIVE_HIGH);
PlatformGpio_configure(GPIO_ID_DMUX_1_A, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_DMUX_2_A, GPIO_FLAG_INPUT_ENABLE);

// 3. CTRX_B GPIOs (4 pins)
PlatformGpio_configure(GPIO_ID_RFT_B, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_SPI_SS_B, GPIO_FLAG_OUTPUT_DRIVE_HIGH);
PlatformGpio_configure(GPIO_ID_DMUX_1_B, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_DMUX_2_B, GPIO_FLAG_INPUT_ENABLE);

// 4. CTRX_C GPIOs (4 pins)
PlatformGpio_configure(GPIO_ID_RFT_C, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_SPI_SS_C, GPIO_FLAG_OUTPUT_DRIVE_HIGH);
PlatformGpio_configure(GPIO_ID_DMUX_1_C, GPIO_FLAG_INPUT_ENABLE);
PlatformGpio_configure(GPIO_ID_DMUX_2_C, GPIO_FLAG_INPUT_ENABLE);
```

### Critical GPIO Usage in Code Execution

#### Power-Up Sequence (main.c, lines 162-188)
```c
// Step 1: Assert CTRX reset (keep all chips in reset during power-up)
PlatformGpio_set(GPIO_ID_CTRX_RESET_N, false);  // Active LOW
PlatformTime_waitTime(10);

// Step 2: Enable PMICs (powers CTRX_A, B, and C simultaneously)
PlatformGpio_set(GPIO_ID_EN_PMIC, true);
PlatformTime_waitTime(10);

// Step 3: Enable spread spectrum on both PMICs
Max20434_enableSpreadSpectrum(&pmic_ab, SSE_3Percent_PseudoRandom);   // PMIC_AB serves CTRX_A and CTRX_B
Max20434_enableSpreadSpectrum(&pmic_c, SSE_3Percent_PseudoRandom);    // PMIC_C serves CTRX_C

// Step 4: Workaround - reset PMIC output states (Jetson GPIO leakage issue)
Max20434_setManualOutputEnable(&pmic_ab, false);
Max20434_setManualOutputEnable(&pmic_c, false);
PlatformTime_waitTime(100);
Max20434_setManualOutputEnable(&pmic_ab, true);
Max20434_setManualOutputEnable(&pmic_c, true);
PlatformTime_waitTime(10);

// Step 5: De-assert CTRX reset after power stabilizes
PlatformGpio_set(GPIO_ID_CTRX_RESET_N, true);
PlatformTime_waitTime(10);
```

#### Device Selection and Initialization (main.c, lines 199-225)
```c
// CTRX_A initialized as PRIMARY
IfxRfe_selectDevice(0);
IfxRfe_ctrxInit(ctrxInitConfig);  // Primary configuration

// CTRX_B initialized as SECONDARY
IfxRfe_selectDevice(1);
IfxRfe_ctrxInit(ctrxInitConfig);  // Secondary configuration

// CTRX_C initialized as SECONDARY
IfxRfe_selectDevice(2);
IfxRfe_ctrxInit(ctrxInitConfig);  // Secondary configuration
```

#### DMUX_1_A Polling - Primary Synchronization Signal (main.c, lines 305 & 330)
```c
// Before starting any ramps: wait for DMUX_1_A to go LOW
PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, false, 10);

// Execute ramps on all three devices in reverse order (C, B, A)
for (int i = 2; i >= 0; i--) {
    IfxRfe_selectDevice(i);
    IfxRfe_startRampScenario();
}

// Wait for DMUX_1_A to go HIGH (signals all ramps completed)
PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, true, 15);

// Finish ramps on all three devices
for (int i = 0; i < 3; i++) {
    IfxRfe_selectDevice(i);
    IfxRfe_finishRampScenario(NULL);
}
```

#### SPI Chip Select Control
- **SPI_SS_A, SPI_SS_B, SPI_SS_C** controlled by `PlatformSpi` layer
- Each driven LOW during device-specific SPI transactions (active LOW)
- Driven HIGH when idle (default state)
- Allows independent SPI communication with each CTRX chip

---

## GPIO Chip Assignment

### Jetson Orin GPIO Controller Mapping
```
gpiochip0: GPIOs 348-511 (tegra234-gpio)      - Main GPIO controller
gpiochip1: GPIOs 316-347 (tegra234-gpio-aon)  - Always-on GPIO controller
```

### Pin Distribution by Chip Controller

**gpiochip0 (10 pins):**
- EN_PMIC (PH.01)
- CTRX_RESET_N (PH.06)
- PGOOD_PMIC_LV (PAC.02)
- PGOOD_PMIC_HV (PAC.03)
- SPI_SS_A (PAC.04)
- DMUX_2_A (PH.05)
- RFT_B (PAC.01)
- DMUX_2_B (PP.00)
- RFT_C (PN.00)
- DMUX_1_C (PN.07)
- DMUX_2_C (PAC.00)

**gpiochip1 (7 pins):**
- CTRX_OK (PBB.02)
- RFT_A (PDD.00)
- DMUX_1_A (PAA.07)
- SPI_SS_B (PCC.03)
- DMUX_1_B (PCC.07)
- SPI_SS_C (PAA.04)

---

## GPIO Functional Roles in Cascaded Configuration

### Power Management (2 GPIOs) - ALL CHIPS
- **EN_PMIC** - Master enable for both MAX20434 PMICs (affects all three CTRX chips)
- **CTRX_RESET_N** - Active-low reset for all three CTRX chips simultaneously

### Status Monitoring (5 GPIOs)
- **CTRX_OK** - Shared operational status indicator (same signal from all three chips)
- **PGOOD_PMIC_LV** - Low-voltage PMIC power-good indicator
- **PGOOD_PMIC_HV** - High-voltage PMIC power-good indicator
- **RFT_A, RFT_B, RFT_C** - Ready-for-transmission status for each chip (independent)

### SPI Communication (3 GPIOs)
- **SPI_SS_A** - Manual chip select for CTRX_A
- **SPI_SS_B** - Manual chip select for CTRX_B
- **SPI_SS_C** - Manual chip select for CTRX_C

### Synchronization & Timing (3 GPIOs)
- **DMUX_1_A** - **CRITICAL** - PRIMARY synchronization signal (actively polled)
- **DMUX_1_B** - Configured as input but not actively used in cascaded mode
- **DMUX_1_C** - Configured as input but not actively used in cascaded mode
- **DMUX_2_x** - All configured as inputs but not used

---

## DMUX Configuration Analysis

From `config.c` (lines 54-61):
```c
configParams->dmux.config_mask = ConfigMask_DMUX1;             // Update only DMUX_1
configParams->dmux.dmux1_alt_signal = AltSignal_DmuxALevel;    // Map level, not pulse
configParams->dmux.dmux1_dir = DmuxDir_out;                    // CTRX drives this as OUTPUT
configParams->dmux.dmux1_pulse_duration_ext = 63;

// DMUX_2 and DMUX_3 configured as inputs but not used
configParams->dmux.dmux2_dir = configParams->dmux.dmux3_dir = DmuxDir_in;
configParams->dmux.dmux2_alt_signal = configParams->dmux.dmux3_alt_signal = 0;  // 0 if input
configParams->dmux.dmux2_pulse_duration_ext = configParams->dmux.dmux3_pulse_duration_ext = 63;
```

**Key Points:**
- **Only CTRX_A (PRIMARY) DMUX_1_A is actively monitored** for synchronization
- DMUX_1_A signals ramp scenario completion for the entire cascade
- CTRX_B and CTRX_C are SECONDARY devices synchronized via LOIN interconnect signals (not GPIOs)
- The code actively polls DMUX_1_A before and after ramp execution to synchronize all three chips

---

## PRIMARY vs. SECONDARY Initialization

From `main.c` (lines 192-196):
```c
const uint32_t intitializeValuePrimary[] = {15, 2, 0, 0, 0};  
// Primary mode using Loin2, disable CRC32, 1 data packet, mode a, non-continuous clock

const uint32_t intitializeValueSecondary_B[] = {4, 2, 0, 0, 0};  
// Secondary mode using Loin1, disable CRC32, 1 data packet, mode a, non-continuous clock

const uint32_t intitializeValueSecondary_C[] = {12, 2, 0, 0, 0};  
// Secondary mode using Loin2, disable CRC32, 1 data packet, mode c, non-continuous clock
```

**CTRX_A (Primary):**
- Drives DMUX_1_A synchronization signal
- Synchronizes CTRX_B and CTRX_C via LOIN2 interconnect

**CTRX_B and CTRX_C (Secondary):**
- Synchronized to CTRX_A via LOIN signals (not GPIO-based)
- Do not actively control GPIO synchronization signals
- Still initialized and configured for parallel radar operation

---

## Total GPIO Count Summary

### Actively Used (Required for Operation)
| Category | Count | Details |
|----------|-------|---------|
| Power Management | 2 | EN_PMIC, CTRX_RESET_N |
| Status Monitoring | 5 | CTRX_OK, PGOOD_PMIC_LV, PGOOD_PMIC_HV, RFT_A/B/C |
| SPI Control | 3 | SPI_SS_A, SPI_SS_B, SPI_SS_C |
| Synchronization | 4 | DMUX_1_A (critical), DMUX_1_B, DMUX_1_C, DMUX_2_A |
| **Total Active** | **17** | All GPIOs in cascaded config are initialized and used |

### Actively Polled (Critical for Operation)
| Signal | Count | Details |
|--------|-------|---------|
| **DMUX_1_A** | 1 | Only this GPIO actively polled for synchronization |
| **SPI_SS_A/B/C** | 3 | Actively driven for SPI transactions |
| **EN_PMIC** | 1 | Actively driven during power-up |
| **CTRX_RESET_N** | 1 | Actively driven during power sequence |
| **Total Polled** | **6** | All others are inputs for status monitoring |

### Configured but Minimally Used
| Signal | Count | Details |
|--------|-------|---------|
| **DMUX_1_B/C** | 2 | Configured as inputs, not actively polled |
| **DMUX_2_A/B/C** | 3 | Configured as inputs, not used |
| **RFT_B/C** | 2 | Monitored for status but not actively polled |
| **PGOOD_PMIC_LV/HV** | 2 | Monitored for status but not actively polled |
| **CTRX_OK** | 1 | Shared status indicator, not actively polled |
| **Total Low-Use** | **10** | Initialized but not critical for operation |

### Grand Total Configured
**17 GPIOs** are configured and all are utilized in some capacity (vs. 9 active + 8 idle in standalone).

---

## Additional Hardware Interfaces (Non-GPIO)

### SPI Bus (4 signals per chip, time-multiplexed)
- **CLK** - SPI clock at 3.125 MHz (shared across all three chips)
- **MOSI** - Master-out-slave-in data (shared)
- **MISO** - Master-in-slave-out data (shared)
- **CS_A/CS_B/CS_C** - Individual chip selects (GPIO-controlled, only one LOW at a time)

Configuration: `/dev/spidev1.0` at 3.125 MHz with manual CS control per device

### I2C Bus (2 signals)
- **SDA** - I2C data
- **SCL** - I2C clock

Configuration: `/dev/i2c-2` (I2C3 hardware)
- Address 0x3E - MAX20434 PMIC for CTRX_A and CTRX_B
- Address 0x3F - MAX20434 PMIC for CTRX_C

### LOIN Interconnect (High-Speed Digital Links)
- **LOIN1** - Connects CTRX_B to CTRX_A (Primary-Secondary synchronization)
- **LOIN2** - Connects CTRX_C to CTRX_A (Primary-Secondary synchronization)
- These are dedicated chip-to-chip signals, not GPIO-based

### Triple MIPI CSI-2 Interfaces
- **CTRX_A → `/dev/video0`**: 4 differential data lanes + clock lane
- **CTRX_B → `/dev/video1`**: 4 differential data lanes + clock lane
- **CTRX_C → `/dev/video2`**: 4 differential data lanes + clock lane

Configuration: 1200 Mbps per lane, RAW12 format, non-continuous clock mode

---

## Critical Timing Constraints

### Power-Up Sequence
1. Assert CTRX_RESET_N LOW (before PMIC enable)
2. Enable EN_PMIC HIGH
3. Wait 10ms for power stabilization
4. Enable spread spectrum on both PMICs
5. Execute PMIC output enable workaround (disable → wait 100ms → enable)
6. De-assert CTRX_RESET_N HIGH
7. Wait 10ms before initialization

### Initialization Sequence
1. Initialize all three CTRX chips sequentially via SPI
2. Load sequencer programs (different for PRIMARY vs. SECONDARY)
3. Configure DMUX only on PRIMARY (CTRX_A)
4. Execute parallel RX calibration on CTRX_B and CTRX_C (async)
5. Execute RX calibration on CTRX_A (blocking)
6. Execute parallel TX calibration on CTRX_B and CTRX_C (async)
7. Execute TX calibration on CTRX_A (blocking)

### Ramp Execution Synchronization
- **Pre-ramp:** Block until DMUX_1_A = LOW (10ms timeout)
- **Start ramps:** Execute start on all three devices (C→B→A order)
- **Wait completion:** Block until DMUX_1_A = HIGH (15ms timeout)
- **Finish ramps:** Execute finish on all three devices (A→B→C order)

---

## Recommendations for GMSL Integration

### GPIO Resource Planning for GMSL Links
When integrating GMSL SERDES (MAX96723/MAX96725/MAX96726):

1. **Preserve all 17 GPIOs:** Required for complete cascaded radar operation
2. **DMUX_1_A is critical:** Must maintain GPIO polling capability for synchronization
3. **Review GMSL GPIO tunneling:** Consider which GPIO signals can traverse GMSL links vs. remaining local
4. **Check pinmux conflicts:** Validate no overlap between GMSL and radar GPIO assignments
5. **Plan LOIN signal routing:** LOIN1 and LOIN2 high-speed links may need re-routing through GMSL SERDES

### Design Questions for GMSL Integration
- How will LOIN synchronization signals (LOIN1, LOIN2) route through GMSL?
- Can DMUX_1_A synchronization be tunneled through GMSL or must it remain local?
- Which I2C transactions (PMIC control) should use GMSL I2C tunneling vs. local I2C?
- Which SPI transactions should use GMSL SPI tunneling vs. local SPI?
- Are additional GPIOs needed for GMSL link status (lock, error indicators)?

---

## Code References

### Primary Files
- **main.c** (lines 50-337) - Cascaded initialization, calibration, and ramp execution
- **config.c** (lines 12-64) - Configuration parameter initialization
- **config.h** - Configuration structure definitions
- **HwDefinitions.h** (lines 56-106) - All GPIO ID definitions (17 total)

### Key Functions and Line Ranges
- **GPIO initialization:** main.c lines 55-79
- **Power-up sequence:** main.c lines 162-188
- **Device initialization:** main.c lines 199-225
- **Sequencer programming:** main.c lines 231-237
- **Ramp execution loop:** main.c lines 307-337
- **DMUX polling:** main.c lines 305, 330

### Platform Layer
- **PlatformGpio.h/c** - GPIO abstraction API (libgpiod v1.6)
- **PlatformSpi.h/c** - SPI abstraction with manual CS control
- **PlatformI2c.h/c** - I2C abstraction for PMIC control
- **3xCtrx8188Wrapper.h/c** - Board-specific SPI transfer overrides

---

## Document Version

**Version:** 1.0  
**Last Updated:** 2026-07-17  
**Analyzed By:** Claude (Claudian) AI Assistant  
**Vault Location:** [[cascaded_example_v1.1.0/GPIO_Analysis.md]]
