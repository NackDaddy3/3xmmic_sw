# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This repository contains the integration software for Infineon CTRX8188 MMIC radar chips with the NVIDIA Jetson Orin AGX platform (EK1 board). The project enables radar operations via SPI control, I2C power management, and MIPI CSI-2 data streaming through the Jetson camera subsystem.

**Hardware:** P2S40 board with 3x CTRX8188 radar chips connected to Jetson Orin via Camera Connector J509  
**Platform:** NVIDIA Jetson Orin AGX Developer Kit  
**Firmware:** Version 1.1.0-RamFw_1.0.0-rc3  

**Future Development:** This project will integrate GMSL SERDES links between the radar board and Orin for long-distance data transmission.

## Repository Structure

```
integration-ek1/
├── src/
│   ├── demo_app/                    # User-space radar applications
│   │   ├── boards/ek1/              # Board-specific applications
│   │   │   ├── HwDefinitions.h      # GPIO/SPI/I2C pin mappings for EK1
│   │   │   ├── standalone_example/  # Single CTRX_A operation
│   │   │   ├── cascaded_example/    # 3x CTRX cascaded operation
│   │   │   └── cascaded_example_v1.1.0/  # Updated cascaded example
│   │   ├── boardspecific/           # Board-specific wrappers (3xCtrx8188Wrapper)
│   │   ├── pmic_power/              # MAX20434 PMIC control drivers
│   │   └── submodules/
│   │       ├── iRfe/                # Infineon RFE (Radio Frequency Engine) middleware
│   │       └── platform-jetson/     # Jetson hardware abstraction layer
│   └── modules/
│       └── radar/                   # Linux kernel module for radar I2C device
├── artifacts/
│   ├── pinmux_config_ek1.sh        # Jetson pinmux configuration script
│   └── spi-tegra114.ko             # Patched SPI kernel driver
├── scripts/                         # Utility scripts
├── documentation/                   # Additional documentation
├── install_jetson_dependencies.sh  # One-time Jetson setup script
└── init_jetson.py                  # Runtime initialization script
```

## Architecture

### Three-Layer Software Stack

1. **Hardware Abstraction Layer** (`platform-jetson`)
   - Wraps Linux drivers for GPIO (libgpiod v1.6), SPI (spidev), I2C (SMBus)
   - Provides platform-agnostic interfaces for the middleware
   - Manual CS control for SPI to maintain chip-select assertion across multi-frame transactions

2. **Middleware Layer** (`iRfe`, `pmic_power`, `boardspecific`)
   - **IfxRfe**: Infineon's radar control API - handles sequencer programming, RF configuration, calibration
   - **Max20434Pec**: PMIC control for power management and spread spectrum
   - **3xCtrx8188Wrapper**: Board-specific SPI transfer overrides

3. **Application Layer** (`demo_app/boards/ek1/`)
   - Standalone/cascaded examples orchestrate initialization, configuration, and radar operation
   - Configuration parameters (RF frequency, TX power, RX settings) in `config.c`

### Data Flow

- **Control Path:** Application → IfxRfe → PlatformSpi → Linux spidev → CTRX chips
- **Power Path:** Application → Max20434 → PlatformI2c → Linux I2C → PMICs
- **Data Path:** CTRX chips → MIPI CSI-2 (4 lanes @ 1200 Mbps) → Jetson VI subsystem → `/dev/video0` (V4L2)

### Key Design Patterns

- **Error Propagation:** All functions return error codes; macros `EXIT_ON_PLATFORM_ERROR` and `EXIT_ON_IFXRFE_ERROR` ensure cleanup on failure
- **Hardware State Management:** Reset sequences and power-up delays are critical (see power-up workaround in main.c for PMIC output reset)
- **DMUX Synchronization:** GPIO DMUX_1 signals ramp scenario completion; code polls this before starting/finishing ramps

## Build System

The build system uses GNU Make with a hierarchical structure:

- Each application has a minimal `Makefile` setting `APP_NAME` and `CHIP_TYPE`
- Common build logic in `src/demo_app/boards/config.mk`
- IfxRfe middleware provides `makelist.mke` with chip-specific sources/includes

### Building Demo Applications

```bash
# Build standalone example (single CTRX_A)
cd src/demo_app/boards/ek1/standalone_example
make                  # Build application
make clean            # Clean build artifacts
make info             # Show build variables

# Build cascaded example (3x CTRX)
cd src/demo_app/boards/ek1/cascaded_example_v1.1.0
make

# Build with debug symbols
make DEBUG=1
```

Output: `bin/<app_name>` (e.g., `bin/p2s40_3x8188_standalone_example`)

### Building Kernel Module

```bash
cd src/modules/radar
make BOARD_TYPE=ek1   # Build radar kernel module
make install          # Load module into kernel
make clean            # Clean build artifacts
```

Output: `build/radar.ko`

## Jetson Setup

### One-Time Setup (run once after flashing Jetson)

```bash
./install_jetson_dependencies.sh
```

This script:
- Installs build tools (gcc, make, git)
- Builds and installs libgpiod v1.6 from source (required version)
- Installs runtime dependencies (devmem2, libi2c-dev, v4l-utils)
- Replaces SPI driver with patched version (fixes inter-transfer delays)
- Optionally installs Docker

**Important:** Reboot after running this script.

### Per-Session Initialization

```bash
sudo ./init_jetson.py
```

This script:
- Configures pinmux via `artifacts/pinmux_config_ek1.sh`
- Loads `spidev` kernel module (if not built-in)
- Builds and inserts `radar` kernel module

## Running Radar Applications

### Standalone Example (Single CTRX_A)

```bash
cd src/demo_app/boards/ek1/standalone_example
./bin/p2s40_3x8188_standalone_example
```

The application:
1. Initializes GPIO, SPI, I2C interfaces
2. Powers up PMICs with spread spectrum enabled
3. Resets and initializes CTRX_A via SPI
4. Loads sequencer program (512 ramps × 512 samples × 8 RX channels)
5. Configures radar parameters (80 GHz, 2.5 GHz BW, 12-bit data, 1200 Mbps)
6. Enters interactive loop: press 'c' to execute radar chirp, 'e' to exit

### Capturing Radar Data

In a separate terminal (after radar reaches operational mode):

```bash
cd src/demo_app/boards/ek1/standalone_example
./captureA_8x512x512.sh
```

This captures one frame from `/dev/video0` (RAW12 format, 4096×512 pixels) to `ctrx1_bin.raw`.

**Timing:** The CSI-2 receiver only activates when the radar is in operational mode with clock active. Start capture script after the radar application prints "Enter 'c' to continue".

## Hardware Pin Mapping

All pin mappings are defined in `src/demo_app/boards/ek1/HwDefinitions.h`:

- **GPIO Chips:** gpiochip0 (GPIOs 348-511), gpiochip1 (GPIOs 316-347)
- **SPI Bus:** `/dev/spidev1.0` at 3.125 MHz (CLK, MOSI, MISO shared; CS per chip via GPIO)
- **I2C Bus:** `/dev/i2c-2` (I2C3 in hardware)
- **MIPI CSI-2:** 4 differential data lanes + clock lane → Jetson VI subsystem

Individual CTRX chips (A, B, C) each have:
- SPI chip select (GPIO output, active LOW)
- RFT signal (GPIO input)
- DMUX_1, DMUX_2 (GPIO inputs for status/synchronization)

## Common Development Workflows

### Adding a New Radar Configuration

1. Edit `src/demo_app/boards/ek1/<example>/config.c`
2. Modify `IrfeDemoConfigInit()` to change:
   - RF frequency parameters (nmod, ncw, rampbw)
   - TX power levels (plvl[])
   - RX configuration (gain, data width, data rate)
   - Calibration parameters
3. Update sequencer program data in `main.c` if chirp sequence changes
4. Rebuild: `make clean && make`

### Modifying Hardware Pin Assignments

1. Edit `src/demo_app/boards/ek1/HwDefinitions.h`
2. Update GPIO offset macros and GPIO_ID macros
3. Update pinmux script: `artifacts/pinmux_config_ek1.sh`
4. Rebuild all applications: `cd src/demo_app/boards/ek1/<example> && make clean && make`
5. Run `sudo ./init_jetson.py` to apply new pinmux

### Debugging SPI Communication

Enable debug logging in platform layer:

```bash
cd src/demo_app/submodules/platform-jetson/src
# Uncomment #define DEBUG in PlatformSpi.c
cd ../../boards/ek1/<example>
make clean && make
```

### Testing with Different CTRX Chips

Standalone example uses CTRX_A. To use CTRX_B or CTRX_C:

1. Change SPI ID in `main.c`:
   ```c
   uint8_t spiIds[] = {CTRX_B_SPI_ID};  // or CTRX_C_SPI_ID
   uint16_t spiCsGpioIds[] = {GPIO_ID_SPI_SS_B};  // or GPIO_ID_SPI_SS_C
   ```
2. Update GPIO initialization to use CTRX_B or CTRX_C pins
3. Update IfxRfe GPIO definitions structure
4. Rebuild and test

## Important Constraints

- **SPI Clock Speed:** 3.125 MHz maximum for reliable communication
- **Power-Up Sequence:** CTRX reset must be LOW during PMIC power-up, then de-asserted after 10ms
- **PMIC Workaround:** Output disable/enable cycle required due to Jetson GPIO leakage issue
- **DMUX Timing:** Must wait for DMUX_1 LOW before starting ramp, HIGH during execution
- **CSI-2 Clock Mode:** Continuous clock mode required (configured in IRAM descriptors)
- **V4L2 Format:** RAW12 with NVIDIA padding (12-bit data padded to 16-bit)
- **libgpiod Version:** Must use v1.6.x (v2.x has incompatible API)

## Troubleshooting

### "spidev not found"
Run `sudo ./init_jetson.py` to load spidev module.

### "GPIO line busy"
Another process or previous run didn't release GPIO. Reboot or find and kill the process.

### "No data from /dev/video0"
Ensure radar is in operational mode before running capture script. CSI-2 receiver requires active clock.

### "SPI communication timeout"
Check SPI driver is patched version (installed by `install_jetson_dependencies.sh`). Verify SPI clock speed ≤ 3.125 MHz.

### "PMIC not responding"
Verify I2C bus ID is correct (`/dev/i2c-2`). Check I2C addresses: 0x3E (PMIC_AB), 0x3F (PMIC_C).

## Code Analysis Documentation

Comprehensive code analysis reports with execution flow, pin mappings, and architecture diagrams:

### Standalone Example (Single CTRX_A)
`src/demo_app/boards/ek1/standalone_example/CTRX8188_Code_Analysis.html`

Detailed analysis of single-chip radar operation with complete pin mappings and execution flow.

### Cascaded Example (3x CTRX8188 Synchronized)
`src/demo_app/boards/ek1/cascaded_example_v1.1.0/CTRX8188_Cascaded_Code_Analysis.html`

Detailed analysis of three-chip cascaded operation including:
- PRIMARY/SECONDARY synchronization architecture via LOIN signals
- Parallel calibration and data streaming patterns
- Complete pin mappings for all three chips (46 total interconnections)
- Synchronized ramp execution timing diagrams
- Triple CSI-2 data stream capture (/dev/video0, /dev/video1, /dev/video2)

Open in browser for detailed documentation with Mermaid flowcharts.
