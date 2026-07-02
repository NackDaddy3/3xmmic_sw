/*
 * (c) (2022-2026), Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees.
 */

/*
 * File: main.c
 * Description: Example for using the P2S40_3xCTRX8188 board
 * Project: MIMOrad
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "IfxRfe.h"
#include "IfxRfe_Specific.h"
#include "IfxRfe_FirmwareCommands.h"
#include "Max20434Pec.h"

#include "PlatformGpio.h"
#include "PlatformI2c.h"
#include "PlatformLogCallbacks.h"
#include "PlatformSpi.h"
#include "PlatformTime.h"
#include "../HwDefinitions.h"
#include "3xCtrx8188Wrapper.h"
#include "../../error_macros.h"

#define CTRX_DEV_COUNT 1

void cleanup_platform()
{
    PlatformGpio_set(GPIO_ID_CTRX_RESET_N, false); /* without reset CTRX-s might remain in operation */
    PlatformI2c_deinit();
    PlatformSpi_deinit();
    PlatformGpio_deinit();
}

int main()
{

    printf(" === IfxRfe App - p2s40_3x8188_standalone_example ===\n");

    const Max20434Pec_t pmic_ab = {.devAddrI2c = 0x3e}; // PMIC for CTRX_A and CTRX_B
    const Max20434Pec_t pmic_c  = {.devAddrI2c = 0x3f}; // PMIC for CTRX_C

    // =============== GPIO Setup ===================

    // General GPIOs:
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_EN_PMIC, GPIO_FLAG_OUTPUT_DRIVE_LOW), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_CTRX_RESET_N, GPIO_FLAG_OUTPUT_DRIVE_LOW), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_CTRX_OK, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_PGOOD_PMIC_LV, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_PGOOD_PMIC_HV, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());


    // CTRX A:
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_RFT_A, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_SPI_SS_A, GPIO_FLAG_OUTPUT_DRIVE_HIGH), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_DMUX_1_A, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_DMUX_2_A, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());

    // CTRX B:
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_RFT_B, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_SPI_SS_B, GPIO_FLAG_OUTPUT_DRIVE_HIGH), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_DMUX_1_B, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_DMUX_2_B, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());

    // CTRX C:
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_RFT_C, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_SPI_SS_C, GPIO_FLAG_OUTPUT_DRIVE_HIGH), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_DMUX_1_C, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_configure(GPIO_ID_DMUX_2_C, GPIO_FLAG_INPUT_ENABLE), cleanup_platform());

    // =============== I2C Setup ===================
    EXIT_ON_PLATFORM_ERROR(PlatformI2c_init(I2C_ID), cleanup_platform());

    // =============== SPI Setup ===================

    uint8_t devIds[] = {0};

    uint8_t spiIds[] = {CTRX_A_SPI_ID};

    uint16_t spiCsGpioIds[] = {GPIO_ID_SPI_SS_A};

    PlatformSpi_init();
    PlatformSpi_setDevIdSpiIdMapping(devIds, spiIds, sizeof(devIds));
    PlatformSpi_setCsGpioIds(spiIds, spiCsGpioIds, sizeof(spiIds));


    // =============== IfxRfe Setup ===================

    IfxRfe_spiFunctions_t spiFncs = {
        .spiConfigure = PlatformSpi_configure,
        .spiTransfer  = PlatformSpi_transfer_override,
        .spiWrite     = PlatformSpi_write_override};

    IfxRfe_gpioFunctions_t gpioFncs = {
        .gpioGet            = PlatformGpio_get,
        .gpioSet            = PlatformGpio_set,
        .gpioConfigure      = PlatformGpio_configure,
        .isGpioConfigOutput = PlatformGpio_isGpioConfigOutput};

    IfxRfe_timeFunctions_t timeFncs = {
        .getDeadLine = PlatformTime_getDeadLine,
        .now         = PlatformTime_now,
        .waitTime    = PlatformTime_waitTime};

    IfxRfe_logCallbacks_t logInterface = {
        .errorLog   = PlatformLogCallbacks_error,
        .warningLog = PlatformLogCallbacks_warning,
        .infoLog    = PlatformLogCallbacks_info};

    uint16_t const GPIO_UNUSED = 0;

    IfxRfe_gpioDefinitions_t gpios[CTRX_DEV_COUNT] = {
        {
            .spiRftId = GPIO_ID_RFT_A,
            .okId     = GPIO_ID_CTRX_OK,
            .dmux1Id  = GPIO_ID_DMUX_1_A,
            .dmux2Id  = GPIO_UNUSED,
            .dmux3Id  = GPIO_UNUSED
        }
    };

    EXIT_ON_IFXRFE_ERROR(
        IfxRfe_init(
            CTRX_DEV_COUNT,
            gpios,
            spiFncs,
            gpioFncs,
            timeFncs,
            logInterface),
        cleanup_platform()
    );

    // Put CTRXs into reset before powering up the board    
    printf("Resetting CTRXs\n");
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_set(GPIO_ID_CTRX_RESET_N, false), cleanup_platform());
    PlatformTime_waitTime(10);

    printf("Enabling PMICs\n");
    PlatformGpio_set(GPIO_ID_EN_PMIC, true);
    PlatformTime_waitTime(10);

    printf("  enabling spread spectrum PMIC_AB\n");
    // Enable Spread Spectrum +-3% pseudo-random
    EXIT_ON_PLATFORM_ERROR(Max20434_enableSpreadSpectrum(&pmic_ab, SSE_3Percent_PseudoRandom), cleanup_platform());
    printf("  enabling spread spectrum PMIC_C\n");
    EXIT_ON_PLATFORM_ERROR(Max20434_enableSpreadSpectrum(&pmic_c, SSE_3Percent_PseudoRandom), cleanup_platform());

    printf("  resetting PMIC outputs - workaround\n");
    //workaround Nikola: because of the leakage from Jetson output the PMIC outputs are not powering up correctly (when connected to Jetson first and powered up afterwards)
    // disable and enable the outputs to reset internal state
    EXIT_ON_PLATFORM_ERROR(Max20434_setManualOutputEnable(&pmic_ab, false), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(Max20434_setManualOutputEnable(&pmic_c, false), cleanup_platform());
    PlatformTime_waitTime(100);
    EXIT_ON_PLATFORM_ERROR(Max20434_setManualOutputEnable(&pmic_ab, true), cleanup_platform());
    EXIT_ON_PLATFORM_ERROR(Max20434_setManualOutputEnable(&pmic_c, true), cleanup_platform());
    PlatformTime_waitTime(10);

    printf("Starting CTRXs\n");
    PlatformGpio_set(GPIO_ID_CTRX_RESET_N, true);
    PlatformTime_waitTime(10);

    // =============== CTRX_A Init ===================

    IfxRfe_getStatusResult_t status = {0};

    IfxRfe_demoConfigParams_t _configParams;
    EXIT_ON_IFXRFE_ERROR(IrfeDemoConfigInit(&_configParams), cleanup_platform());

     // Initialize CTRX
    static uint32_t intitializeIramDesc[] = {0x10000028, 0x1180000A, 0x1000000E};
    static uint32_t intitializeValue[]    = {1, 2, 1};  //LO-config: standalone / crc32 disabled / continuous CSI-2 clk mode

    IfxRfe_ctrxInitConfiguration_t ctrxInitConfig = prepareCtrxInitConfig(STANDALONE);
    ctrxInitConfig.spiClockSpeed = 3125000;

    ctrxInitConfig.initializeConfig = (IfxRfe_initialize_t) {
        .iram_descriptor = intitializeIramDesc,
        .value           = intitializeValue,
        .length          = sizeof(intitializeIramDesc) / sizeof(intitializeIramDesc[0])};
    
    printf("# Initializing CTRX_A ...\n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_selectDevice(0), cleanup_platform()); // this should be enable by default after init
    EXIT_ON_IFXRFE_ERROR(IfxRfe_ctrxInit(ctrxInitConfig), cleanup_platform());

    EXIT_ON_IFXRFE_ERROR(IfxRfe_getStatus(&status), cleanup_platform());
    printf("Status of CTRX after initialization: %#010x\n", status.curr_state);

    // Standalone Ramps = 512, Samples= 512, 8RX => v4l2-ctl -d /dev/video0 --set-fmt-video=width=4096,height=512,pixelformat='RG12' --set-ctrl bypass_mode=0 --stream-mmap
    // RampExample_8188_f80_b600_t10_r512_TX1.rdt
    const uint32_t sequencer_program_data[] = {0x33880024, 0x00000000, 0x01000100, 0x01000100, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xe0010200, 0x47800000, 0x00000190, 0x03b8a3d0, 0x00100000, 0x081a0000, 0x00000001, 0x46000000, 0x00000800, 0x0b060000, 0x00000001, 0x46000000, 0x00000008, 0x0a020000, 0x00000001, 0x47800000, 0x00000120, 0x03dee3d0, 0x3f781800, 0x00020001, 0x00000001, 0x47800000, 0x000004e4, 0x03b8a3d0, 0x00000000, 0x00020001, 0x00000001, 0xe0020000, 0x47000000, 0x00000008, 0x00000000, 0x80000000, 0x00000000, 0xffffffff};

    printf("  configuring sequencer memory \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_loadSequencerData(0, sequencer_program_data, sizeof(sequencer_program_data) / sizeof(uint32_t)), cleanup_platform());

    //configure DMUX as it is required to check whether ramp scenario finished
    printf("  configuring DMUX \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_configureDmux(_configParams.dmux), cleanup_platform());

    //clear results
    printf("  clear results \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_clearResults(_configParams.clearResults), cleanup_platform());

    //configure ramp scenario
    printf("  configure ramp scenario \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_configureRampScenario(_configParams.rampScenario), cleanup_platform());

    //configure TX power
    printf("  configure TX power \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_configureTxPower(_configParams.txpwr), cleanup_platform());

    //configure RX
    printf("  configure RX \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_configureRx(_configParams.rxcfg), cleanup_platform());

    //configure RF frequency
    printf("  configure RF frequency \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_configureRfFrequency(_configParams.rfFreq), cleanup_platform());

    //get temperature
    IfxRfe_getTemperatureResult_t tempRes;
    EXIT_ON_IFXRFE_ERROR(IfxRfe_getTemperature(&tempRes), cleanup_platform());
    printf("  Temperature Sensor 1: %.1f °C\n", ((float)tempRes.temp1) / 8.0f);
    printf("  Temperature Sensor 2: %.1f °C\n", ((float)tempRes.temp2) / 8.0f);
    
    //execute goto operation
    printf("  go to operation \n");
    EXIT_ON_IFXRFE_ERROR(IfxRfe_gotoOperation(), cleanup_platform());
    
    EXIT_ON_IFXRFE_ERROR(IfxRfe_getStatus(&status), cleanup_platform());
    EXIT_ON_IFXRFE_ERROR(0x10000000 != status.curr_state, cleanup_platform());    

    //execute TX calibration subfunctions
    printf("  execute TX calibration subfunctions \n");
    _configParams.calibration.calib_sub_func_id = 0x000F;
    EXIT_ON_IFXRFE_ERROR(IfxRfe_executeCalibration(_configParams.calibration, NULL), cleanup_platform());

    //make sure DMUX is in the right state before start
    EXIT_ON_PLATFORM_ERROR(PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, false, 10), cleanup_platform());

    char user_input;
    for(int i = 0; i < 10000; i++) {

        // Prompt the user to continue or exit
        printf("Enter 'c' to continue or 'e' to exit: ");
        scanf(" %c", &user_input);

        if (user_input == 'e') {
        printf("Exiting loop.\n");
        break;
        }

        printf("  executing ramp scenario \n");
        EXIT_ON_IFXRFE_ERROR(IfxRfe_startRampScenario(), cleanup_platform());
        
        EXIT_ON_PLATFORM_ERROR(PlatformGpio_gpioWait(GPIO_ID_DMUX_1_A, true, 15), cleanup_platform());

        EXIT_ON_IFXRFE_ERROR(IfxRfe_finishRampScenario(NULL), cleanup_platform());
        
    }

    // =============== Deinitialize ===================
    cleanup_platform();

    printf(" === SUCCESS ===\n");
    return EXIT_SUCCESS;
}
