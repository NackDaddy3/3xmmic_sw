#!/bin/bash

# Exit on any error
set -e

# Create silent wrapper for devmem2 to suppress all output
silent_devmem2() {
    command sudo devmem2 "$@" > /dev/null 2>&1
}

# ------------------------------------------------------------------------------
# This script configures the pinmux settings for the EK1 setup
# All the connected pins are configured for their respective functions
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# MFPs and pins connected to the Deserializer 
# ------------------------------------------------------------------------------

# Jetson PIN: E61
# Jetson Verilog Ball Name: SPI2_SCK
# Usage: CTRX_SPI_SCLK
silent_devmem2 0x0c302048 word 0x400

# Jetson PIN: D62
# Jetson Verilog Ball Name: SPI2_MISO
# Usage: CTRX_SPI_SO
silent_devmem2 0x0c302050 word 0x450

# Jetson PIN: F60
# Jetson Verilog Ball Name: SPI2_MOSI
# Usage:CTRX_SPI_SI
silent_devmem2 0x0c302028 word 0x0400

# Jetson PIN: D60
# Jetson Verilog Ball Name: SPI2_CS0
# Usage: CTRX_SPI_SS_B
silent_devmem2 0x0c302038 word 0x1

# Jetson PIN: J61
# Jetson Verilog Ball Name: GEN2_I2C_SCL
# Usage: CTRX_DMUX_1_B
silent_devmem2 0x0c302030 word 0x1071

# Jetson PIN: K61
# Jetson Verilog Ball Name: GEN2_I2C_SDA
# Usage: CTRX_RFT_A
silent_devmem2 0x0c302040 word 0x71

# Jetson PIN: E59
# Jetson Verilog Ball Name: CAN0_STB
# Usage: CTRX_SPI_SS_C
silent_devmem2 0x0c303020 word 0xC000

# Jetson PIN: F59
# Jetson Verilog Ball Name: CAN0_ERR
# Usage: CTRX_DMUX_1_A
silent_devmem2 0x0c303038 word 0xC050

# Jetson PIN: A62
# Jetson Verilog Ball Name: SOC_GPIO50
# Usage: CTRX_OK
silent_devmem2 0x0c303050 word 0xC050

# Jetson PIN: J54
# Jetson Verilog Ball Name: EXTPERIPH1_CLK
# Usage: CTRX_DMUX_2_B
silent_devmem2 0x02430008 word 0x51

# Jetson PIN: H53
# Jetson Verilog Ball Name: EXTPERIPH2_CLK
# Usage: None
silent_devmem2 0x02430000 word 0x415

# Jetson PIN: F53
# Jetson Verilog Ball Name: CAM_I2C_SCL
# Usage: I2C_SCL
silent_devmem2 0x02430018 word 0x1460

# Jetson PIN: E53
# Jetson Verilog Ball Name: CAM_I2C_SDA
# Usage: I2C_SDA
silent_devmem2 0x02430010 word 0x460

# Jetson PIN: H55
# Jetson Verilog Ball Name: SOC_GPIO32
# Usage: None
silent_devmem2 0x02430068 word 0x414

# Jetson PIN: A53
# Jetson Verilog Ball Name: DP_AUX_CH3_P
# Usage: CTRX_DMUX_1_C
silent_devmem2 0x02440070 word 0x1071

# Jetson PIN: C53
# Jetson Verilog Ball Name: DP_AUX_CH3_N
# Usage: CTRX_RFT_C
silent_devmem2 0x02440078 word 0x71

# Jetson PIN: L9
# Jetson Verilog Ball Name: SOC_GPIO22
# Usage: EN_PMIC
silent_devmem2 0x02434048 word 0x0

# Jetson PIN: L5
# Jetson Verilog Ball Name: UART4_TX
# Usage: None
silent_devmem2 0x02434020 word 0x6

# Jetson PIN: L4
# Jetson Verilog Ball Name: UART4_RTS
# Usage: CTRX_DMUX_2_A
silent_devmem2 0x02434010 word 0x412

# Jetson PIN: L49
# Jetson Verilog Ball Name: UART4_CTS
# Usage: CTRX_RESET_N
silent_devmem2 0x02434008 word 0x1

# Jetson PIN: F10
# Jetson Verilog Ball Name: SPI5_SCK
# Usage: CTRX_DMUX_2_C
silent_devmem2 0x02448058 word 0x1003

# Jetson PIN: F9
# Jetson Verilog Ball Name: SPI5_MISO
# Usage: CTRX_RFT_B
silent_devmem2 0x02448048 word 0x3

# Jetson PIN: G7
# Jetson Verilog Ball Name: SPI5_MOSI
# Usage: PGOOD_PMIC_LV
silent_devmem2 0x02448050 word 0x53

# Jetson PIN: L15
# Jetson Verilog Ball Name: SPI5_CS0
# Usage: PGOOD_PMIC_HV
silent_devmem2 0x02448040 word 0x53

# Jetson PIN: A7
# Jetson Verilog Ball Name: SOC_GPIO57
# Usage: CTRX_SPI_SS_A
silent_devmem2 0x02448020 word 0x1000

# Jetson PIN: E10
# Jetson Verilog Ball Name: SOC_GPIO60
# Usage: ?
silent_devmem2 0x02448038 word 0x414