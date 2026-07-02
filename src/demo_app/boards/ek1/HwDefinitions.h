/*
 * File: HwDefinitions.h
 * Description: Adapter header for the "Orin to P2S30 4xCTRX8191F v5" adapter board
 * Project: MIMOrad
 * Created Date: Friday, 10 January 2025, 10:01
 * Author: Daniel Klepatsch (daniel.klepatsch@silicon-austria.com)
 * ---------------
 * Last Modified: Thursday, 16 January 2025, 15:39
 * Modified By: Daniel Klepatsch
 * ---------------
 * Copyright (c) 2025 Silicon Austria Labs GmbH
 */

#ifndef HWDEFINITIONS_H
#define HWDEFINITIONS_H

#include "PlatformGpio.h"
#include "PlatformGpioI2cWrapper.h"

// GPIO Pin Offsets 
// https://docs.nvidia.com/jetson/archives/r35.2.1/DeveloperGuide/text/HR/JetsonModuleAdaptationAndBringUp/JetsonAgxOrinSeries.html#identifying-the-gpio-number
#define PORT_A_OFFSET 0
#define PORT_B_OFFSET 8
#define PORT_C_OFFSET 9
#define PORT_D_OFFSET 17
#define PORT_E_OFFSET 21
#define PORT_F_OFFSET 29
#define PORT_G_OFFSET 35
#define PORT_H_OFFSET 43
#define PORT_I_OFFSET 51
#define PORT_J_OFFSET 58
#define PORT_K_OFFSET 64
#define PORT_L_OFFSET 72
#define PORT_M_OFFSET 76
#define PORT_N_OFFSET 84
#define PORT_P_OFFSET 92
#define PORT_Q_OFFSET 100
#define PORT_R_OFFSET 108
#define PORT_X_OFFSET 114
#define PORT_Y_OFFSET 122
#define PORT_Z_OFFSET 130
#define PORT_AC_OFFSET 138
#define PORT_AD_OFFSET 146
#define PORT_AE_OFFSET 150
#define PORT_AF_OFFSET 152
#define PORT_AG_OFFSET 156
#define PORT_AA_OFFSET 0
#define PORT_BB_OFFSET 8
#define PORT_CC_OFFSET 12
#define PORT_DD_OFFSET 20
#define PORT_EE_OFFSET 23
#define PORT_GG_OFFSET 31

#define GPIO_OFFSET(portOffset, pinOffset) (portOffset + pinOffset)

#define GPIO_OFFSET_EN_PMIC             GPIO_OFFSET(PORT_H_OFFSET, 1) //PH.01 - gpiochip0
#define GPIO_OFFSET_CTRX_RESET_N        GPIO_OFFSET(PORT_H_OFFSET, 6) //PH.06 - gpiochip0
#define GPIO_OFFSET_CTRX_OK             GPIO_OFFSET(PORT_BB_OFFSET, 2) //PBB.02 - gpiochip1
#define GPIO_OFFSET_PGOOD_PMIC_LV       GPIO_OFFSET(PORT_AC_OFFSET, 2) //PAC.02 - gpiochip0
#define GPIO_OFFSET_PGOOD_PMIC_HV       GPIO_OFFSET(PORT_AC_OFFSET, 3) //PAC.03 - gpiochip0

#define GPIO_OFFSET_RFT_A               GPIO_OFFSET(PORT_DD_OFFSET, 0) //PDD.00 - gpiochip0
#define GPIO_OFFSET_SPI_SS_A            GPIO_OFFSET(PORT_AC_OFFSET, 4) //PAC.04 - gpiochip0
#define GPIO_OFFSET_DMUX_1_A            GPIO_OFFSET(PORT_AA_OFFSET, 7) //PAA.07 - gpiochip1
#define GPIO_OFFSET_DMUX_2_A            GPIO_OFFSET(PORT_H_OFFSET, 5) //PH.05 - gpiochip0

#define GPIO_OFFSET_RFT_B               GPIO_OFFSET(PORT_AC_OFFSET, 1) //PAC.01 - gpiochip0
#define GPIO_OFFSET_SPI_SS_B            GPIO_OFFSET(PORT_CC_OFFSET, 3) //PCC.03 - gpiochip1
#define GPIO_OFFSET_DMUX_1_B            GPIO_OFFSET(PORT_CC_OFFSET, 7) //PCC.07 - gpiochip1
#define GPIO_OFFSET_DMUX_2_B            GPIO_OFFSET(PORT_P_OFFSET, 0) //PP.00 - gpiochip0

#define GPIO_OFFSET_RFT_C               GPIO_OFFSET(PORT_N_OFFSET, 0) //PN.00 - gpiochip0
#define GPIO_OFFSET_SPI_SS_C            GPIO_OFFSET(PORT_AA_OFFSET, 4) //PAA.4 - gpiochip1
#define GPIO_OFFSET_DMUX_1_C            GPIO_OFFSET(PORT_N_OFFSET, 7) //PN.07 - gpiochip0
#define GPIO_OFFSET_DMUX_2_C            GPIO_OFFSET(PORT_AC_OFFSET, 0) //PAC.00 - gpiochip0




// GPIO Pin IDs
//sudo dmesg | grep gpiochip
//[    7.258086] gpiochip0: registered GPIOs 348 to 511 on tegra234-gpio
//[    7.265841] gpiochip1: registered GPIOs 316 to 347 on tegra234-gpio-aon

#define GPIO_ID(chipIdx, offset) ((GPIO_CHIP_##chipIdx##_ID << 8) | (offset & 0xFF))

#define GPIO_ID_EN_PMIC         GPIO_ID(0, GPIO_OFFSET_EN_PMIC)
#define GPIO_ID_CTRX_RESET_N    GPIO_ID(0, GPIO_OFFSET_CTRX_RESET_N)
#define GPIO_ID_CTRX_OK         GPIO_ID(1, GPIO_OFFSET_CTRX_OK)
#define GPIO_ID_PGOOD_PMIC_LV   GPIO_ID(0, GPIO_OFFSET_PGOOD_PMIC_LV)
#define GPIO_ID_PGOOD_PMIC_HV   GPIO_ID(0, GPIO_OFFSET_PGOOD_PMIC_HV)

#define GPIO_ID_RFT_A           GPIO_ID(1, GPIO_OFFSET_RFT_A)
#define GPIO_ID_SPI_SS_A        GPIO_ID(0, GPIO_OFFSET_SPI_SS_A)
#define GPIO_ID_DMUX_1_A        GPIO_ID(1, GPIO_OFFSET_DMUX_1_A)
#define GPIO_ID_DMUX_2_A        GPIO_ID(0, GPIO_OFFSET_DMUX_2_A)

#define GPIO_ID_RFT_B           GPIO_ID(0, GPIO_OFFSET_RFT_B)
#define GPIO_ID_SPI_SS_B        GPIO_ID(1, GPIO_OFFSET_SPI_SS_B)
#define GPIO_ID_DMUX_1_B        GPIO_ID(1, GPIO_OFFSET_DMUX_1_B)
#define GPIO_ID_DMUX_2_B        GPIO_ID(0, GPIO_OFFSET_DMUX_2_B)

#define GPIO_ID_RFT_C           GPIO_ID(0, GPIO_OFFSET_RFT_C)
#define GPIO_ID_SPI_SS_C        GPIO_ID(1, GPIO_OFFSET_SPI_SS_C)
#define GPIO_ID_DMUX_1_C        GPIO_ID(0, GPIO_OFFSET_DMUX_1_C)
#define GPIO_ID_DMUX_2_C        GPIO_ID(0, GPIO_OFFSET_DMUX_2_C)



#define CTRX_A_SPI_ID 0x10
#define CTRX_B_SPI_ID 0x11
#define CTRX_C_SPI_ID 0x12
#define CTRX_D_SPI_ID 0x13


#define I2C_ID 2        // I2C3 on the schematics, indexing 1-based, in driver 0-based

#endif  // HWDEFINITIONS_H
