/*
 * (c) (2022-2026), Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees.
 */

#ifndef _3XCTRX8188WRAPPER_H
#define _3XCTRX8188WRAPPER_H

#include <stdint.h>
#include <stdbool.h>

int PlatformSpi_transfer_override(uint8_t spiId, uint32_t count, const uint32_t bufWrite[], uint32_t bufRead[], bool keepSel);
int PlatformSpi_write_override(uint8_t spiId, uint32_t count, const uint32_t buffer[], bool keepSel);

#endif // _3XCTRX8188WRAPPER_H
