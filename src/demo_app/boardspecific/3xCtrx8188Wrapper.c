/*
 * (c) (2022-2026), Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees.
 */

#include "3xCtrx8188Wrapper.h"
#include "IfxRfe_ErrorDefinitions.h"
#include <unistd.h>
#include "PlatformSpi.h"

// For EK1 a workaround is needed: some commands (e.g. trigger reset, trigger LBIST, finish firmware download) trigger a soft reset
//   causing the RFT pin to be pulled high too early. Therefore we add a delay after the spi transfer call of these commands.
const uint32_t triggerResetHeader = 0x80000000;  ///< Header for Trigger_Reset command
const uint32_t fwCommandHeader    = 0xD0000000;  ///< Header for fw command
const uint32_t triggerLBISTId     = 0x3B000000;  ///< ID for Trigger_LBIST command
const uint32_t finishFwDownloadId = 0x39000000;  ///< ID for Finish_FW_Download command
// SPI readout delay in milliseconds
#define IFXRFE_SPI_RECEIVE_DELAY 2

int PlatformSpi_transfer_override(uint8_t spiId, uint32_t count, const uint32_t bufWrite[], uint32_t bufRead[], bool keepSel)
{
    error_t retVal = PlatformSpi_transfer(spiId, count, bufWrite, bufRead, keepSel);

    const bool isTriggerReset = (count == 1) && ((bufWrite[0] & 0xF0000000) == triggerResetHeader);

    const bool isTriggerLBIST = (count == 5) && ((bufWrite[0] & 0xF0000000) == fwCommandHeader) && ((bufWrite[1] & 0xFF000000) == triggerLBISTId);

    const bool isTriggerFinishFwDownload = (count == 3) && ((bufWrite[0] & 0xF0000000) == fwCommandHeader) && ((bufWrite[1] & 0xFF000000) == finishFwDownloadId);

    if (isTriggerReset || isTriggerLBIST || isTriggerFinishFwDownload)
    {
        // Wait longer for the RFT pin to stabilize after a reset command
        usleep(IFXRFE_SPI_RECEIVE_DELAY * 1000);
    }

    return retVal;
}

int PlatformSpi_write_override(uint8_t spiId, uint32_t count, const uint32_t buffer[], bool keepSel)
{
    return PlatformSpi_transfer_override(spiId, count, buffer, NULL, keepSel);
}
