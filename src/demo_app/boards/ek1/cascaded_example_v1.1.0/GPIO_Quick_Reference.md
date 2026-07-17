# GPIO Quick Reference: Cascaded Example 3x CTRX8188

**Created:** 2026-07-17 17:30  
**Last Updated:** 2026-07-17 17:30  
**Purpose:** Quick lookup for GPIO pins used in cascaded radar operation

---

## Pin Summary Table

| Jetson Pin | GPIO ID | Port.Pin | Direction |   Chip    | Function      | Category |
| :--------: | :-----: | :------: | :-------: | :-------: | :------------ | :------- |
|     44     |  0x2D   |  PH.01   |    OUT    | gpiochip0 | EN_PMIC       | Power    |
|     49     |  0x31   |  PH.06   |    OUT    | gpiochip0 | CTRX_RESET_N  | Power    |
|     10     |  0x102  |  PBB.02  |    IN     | gpiochip1 | CTRX_OK       | Status   |
|    140     |  0x8C   |  PAC.02  |    IN     | gpiochip0 | PGOOD_PMIC_LV | Status   |
|    141     |  0x8D   |  PAC.03  |    IN     | gpiochip0 | PGOOD_PMIC_HV | Status   |
|     20     |  0x114  |  PDD.00  |    IN     | gpiochip1 | RFT_A         | Status   |
|    142     |  0x8E   |  PAC.04  |    OUT    | gpiochip0 | SPI_SS_A      | SPI      |
|     7      |  0x107  |  PAA.07  |    IN     | gpiochip1 | DMUX_1_A      | Sync ⭐   |
|     48     |  0x30   |  PH.05   |    IN     | gpiochip0 | DMUX_2_A      | Sync     |
|    139     |  0x8B   |  PAC.01  |    IN     | gpiochip0 | RFT_B         | Status   |
|    315     |  0x103  |  PCC.03  |    OUT    | gpiochip1 | SPI_SS_B      | SPI      |
|    319     |  0x107  |  PCC.07  |    IN     | gpiochip1 | DMUX_1_B      | Sync     |
|    100     |  0x64   |  PP.00   |    IN     | gpiochip0 | DMUX_2_B      | Sync     |
|     84     |  0x54   |  PN.00   |    IN     | gpiochip0 | RFT_C         | Status   |
|     4      |  0x104  |  PAA.04  |    OUT    | gpiochip1 | SPI_SS_C      | SPI      |
|     91     |  0x5B   |  PN.07   |    IN     | gpiochip0 | DMUX_1_C      | Sync     |
|    138     |  0x8A   |  PAC.00  |    IN     | gpiochip0 | DMUX_2_C      | Sync     |

---

## GPIO by Function

### Power Management (2)
```
EN_PMIC        = PH.01  (gpiochip0)  OUTPUT  → Powers all PMICs
CTRX_RESET_N   = PH.06  (gpiochip0)  OUTPUT  → Resets all CTRX chips
```

### SPI Chip Select (3)
```
SPI_SS_A       = PAC.04 (gpiochip0)  OUTPUT  → CTRX_A chip select
SPI_SS_B       = PCC.03 (gpiochip1)  OUTPUT  → CTRX_B chip select
SPI_SS_C       = PAA.04 (gpiochip1)  OUTPUT  → CTRX_C chip select
```

### Status Inputs (5)
```
CTRX_OK        = PBB.02 (gpiochip1)  INPUT   → Status indicator (all chips)
PGOOD_PMIC_LV  = PAC.02 (gpiochip0)  INPUT   → LV rail status
PGOOD_PMIC_HV  = PAC.03 (gpiochip0)  INPUT   → HV rail status
RFT_A          = PDD.00 (gpiochip1)  INPUT   → CTRX_A ready-for-TX
RFT_B          = PAC.01 (gpiochip0)  INPUT   → CTRX_B ready-for-TX
RFT_C          = PN.00  (gpiochip0)  INPUT   → CTRX_C ready-for-TX
```

### Synchronization (7)
```
⭐ DMUX_1_A    = PAA.07 (gpiochip1)  INPUT   → PRIMARY sync signal (actively polled)
  DMUX_2_A    = PH.05  (gpiochip0)  INPUT   → Configured but unused
  DMUX_1_B    = PCC.07 (gpiochip1)  INPUT   → Configured but unused
  DMUX_2_B    = PP.00  (gpiochip0)  INPUT   → Configured but unused
  DMUX_1_C    = PN.07  (gpiochip0)  INPUT   → Configured but unused
  DMUX_2_C    = PAC.00 (gpiochip0)  INPUT   → Configured but unused
```

---

## GPIO by Chip Controller

### gpiochip0 (Main, 10 pins)
```
EN_PMIC        = PH.01
CTRX_RESET_N   = PH.06
DMUX_2_A       = PH.05
PGOOD_PMIC_LV  = PAC.02
PGOOD_PMIC_HV  = PAC.03
SPI_SS_A       = PAC.04
RFT_B          = PAC.01
DMUX_2_C       = PAC.00
DMUX_2_B       = PP.00
RFT_C          = PN.00
DMUX_1_C       = PN.07
```

### gpiochip1 (Always-On, 7 pins)
```
CTRX_OK        = PBB.02
RFT_A          = PDD.00
DMUX_1_A       = PAA.07 ⭐ (critical)
SPI_SS_B       = PCC.03
DMUX_1_B       = PCC.07
SPI_SS_C       = PAA.04
```

---

## Critical Execution Path

### Power-Up (main.c lines 162-188)
```
1. PlatformGpio_set(GPIO_ID_CTRX_RESET_N, false)
   └─ Assert CTRX_RESET_N LOW

2. PlatformGpio_set(GPIO_ID_EN_PMIC, true)
   └─ Enable EN_PMIC HIGH (starts power-up)
   └─ Wait 10ms

3. [I2C transactions to enable spread spectrum]

4. [I2C transactions to reset PMIC outputs - workaround]

5. PlatformGpio_set(GPIO_ID_CTRX_RESET_N, true)
   └─ De-assert CTRX_RESET_N HIGH
   └─ Wait 10ms
```

### Ramp Execution (main.c lines 307-337)
```
1. PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, false, 10)
   └─ Block until DMUX_1_A LOW (ready for new ramp)

2. IfxRfe_startRampScenario()  on CTRX_C, CTRX_B, CTRX_A (reverse order)
   └─ All three chips start ramps simultaneously

3. PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, true, 15)
   └─ Block until DMUX_1_A HIGH (ramp complete)

4. IfxRfe_finishRampScenario()  on CTRX_A, CTRX_B, CTRX_C (forward order)
   └─ All three chips finish ramps
```

---

## Configuration States

### GPIO Initialization States
```
OUTPUT_DRIVE_LOW:  EN_PMIC, CTRX_RESET_N
OUTPUT_DRIVE_HIGH: SPI_SS_A, SPI_SS_B, SPI_SS_C
INPUT_ENABLE:      All others (CTRX_OK, RFT_A/B/C, DMUX_1_A/B/C, DMUX_2_A/B/C, PGOOD)
```

### Signal Polarity
```
Active LOW:   CTRX_RESET_N, SPI_SS_A/B/C
Active HIGH:  EN_PMIC, DMUX_1_A (when ramp complete)
```

---

## Synchronization Details

### DMUX_1_A State Machine
```
IDLE → LOW (ready) → wait for start → HIGH (executing) → finish → LOW (ready)
       ↑_________________↓                ↑________________↓
    PlatformGpio_gpioWait(false, 10ms)  PlatformGpio_gpioWait(true, 15ms)
```

### Three-Chip Synchronization via LOIN Links
```
CTRX_A (PRIMARY)  ═══LOIN2═══ CTRX_C (SECONDARY)
     │
  LOIN1
     │
  CTRX_B (SECONDARY)

GPIO Sync: DMUX_1_A only
Hardware Sync: LOIN1, LOIN2 (high-speed interconnects between chips)
```

---

## I2C Control Signals

These I2C addresses are accessed via `/dev/i2c-2`:

```
0x3E  → MAX20434 PMIC for CTRX_A and CTRX_B
0x3F  → MAX20434 PMIC for CTRX_C
```

Functions called:
```
Max20434_enableSpreadSpectrum(&pmic_ab, SSE_3Percent_PseudoRandom)
Max20434_enableSpreadSpectrum(&pmic_c, SSE_3Percent_PseudoRandom)
Max20434_setManualOutputEnable(&pmic_ab, false/true)  // Workaround
Max20434_setManualOutputEnable(&pmic_c, false/true)   // Workaround
```

---

## SPI Communication Sequence

```
Active Device Selection → SPI_SS_X driven LOW → Data transfer → SPI_SS_X driven HIGH

IfxRfe_selectDevice(0) → SPI_SS_A controls
IfxRfe_selectDevice(1) → SPI_SS_B controls
IfxRfe_selectDevice(2) → SPI_SS_C controls
```

All SPI on `/dev/spidev1.0` at 3.125 MHz with manual CS control

---

## Data Output Paths

```
CTRX_A → MIPI CSI-2 (4 lanes) → /dev/video0
CTRX_B → MIPI CSI-2 (4 lanes) → /dev/video1
CTRX_C → MIPI CSI-2 (4 lanes) → /dev/video2

Format: RAW12 (12-bit pixel data)
Clock: Non-continuous mode (NVIDIA padding applied)
```

---

## Common GPIO Operations in Code

| Operation | GPIO | Line | Purpose |
|-----------|------|------|---------|
| `configure()` | All 17 | 56-79 | Initialize all GPIO directions |
| `set(RESET, LOW)` | CTRX_RESET_N | 163 | Assert reset before power-up |
| `set(EN_PMIC, HIGH)` | EN_PMIC | 167 | Enable PMICs |
| `set(RESET, HIGH)` | CTRX_RESET_N | 187 | De-assert reset |
| `gpioWait(DMUX_1_A, LOW)` | DMUX_1_A | 305 | Wait for ready |
| `gpioWait(DMUX_1_A, HIGH)` | DMUX_1_A | 330 | Wait for completion |

---

## Default GPIO States

| Signal | Power-Up | Idle | During SPI | During Ramp |
|--------|----------|------|-----------|-------------|
| EN_PMIC | LOW | HIGH | HIGH | HIGH |
| CTRX_RESET_N | LOW | HIGH | HIGH | HIGH |
| SPI_SS_A | HIGH | HIGH | LOW* | HIGH |
| SPI_SS_B | HIGH | HIGH | LOW* | HIGH |
| SPI_SS_C | HIGH | HIGH | LOW* | HIGH |
| DMUX_1_A | ? | LOW | LOW | HIGH |

*Only when SPI transaction active on that device

---

## Differences from Standalone Example

| Aspect | Standalone | Cascaded |
|--------|-----------|----------|
| Total GPIOs | 17 (9 active + 8 idle) | 17 (all active) |
| CTRX Chips | 1 (CTRX_A only) | 3 (A primary, B+C secondary) |
| DMUX Polling | DMUX_1_A only | DMUX_1_A only (primary) |
| Synchronization | Single chip | LOIN1+LOIN2 + GPIO sync |
| PMICs | 1x PMIC (0x3E) | 2x PMICs (0x3E, 0x3F) |
| CSI-2 Streams | 1x `/dev/video0` | 3x `/dev/video0/1/2` |
| Ramp Execution | Sequential | Parallel (cascaded) |
| Init Sequence | Single init | 3x device init (PRIMARY→SECONDARY) |

---

## Document Version

**Version:** 1.0  
**Last Updated:** 2026-07-17  
**Format:** Quick reference lookup table  
**Related:** See GPIO_Analysis.md for detailed descriptions
