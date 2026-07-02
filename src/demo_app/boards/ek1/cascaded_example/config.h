/*
 * (c) (2022-2026), Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees.
 */

#ifndef CONFIG_H
#define CONFIG_H 1

#include "IfxRfe.h"

typedef struct
{
    IfxRfe_clearResults_t clearResults;
    IfxRfe_configureRampScenario_t rampScenario;
    IfxRfe_configureRfFrequency_t rfFreq;
    IfxRfe_configureTxPower_t txpwr;
    IfxRfe_configureRx_t rxcfg;
    IfxRfe_executeCalibration_t calibration;
    IfxRfe_configureDmux_t dmux;
} IfxRfe_demoConfigParams_t;

/// \brief Initialization of demo application configuration parameters
/// \param configParams pointer of IfxRfe_demoConfigParams_t struct
/// \return error code if something went wrong (e.g. nmod calculation)
error_t IrfeDemoConfigInit(IfxRfe_demoConfigParams_t *configParams);

#endif  //CONFIG_H
