/*
 * (c) (2022-2026), Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees.
 */

#include "config.h"
#include "IfxRfe_CommandParamEnums.h"

error_t IrfeDemoConfigInit(IfxRfe_demoConfigParams_t *configParams)
{
    //clear results
    configParams->clearResults.clr_areas_msk = 0x10;

    //ramp Scenario
    configParams->rampScenario.startoffset = 0;  //Sequencer setup structure start address of sequencer program

    //RF Frequency
    //  TX configuration
    // const float f_static = 79.65f;                   // static frequency in GHz before ramp sequence starts
    // const float f_lock = 80.988002f;                 // Upper frequency of the RF modulation bandwidth in GHz
    // const float f_bw = 2.499999f;                    // RF modulation bandwidth in GHz

    //TODO: fix mismatch in calculation in P2SSW-1127
    // IFXRFE_RETURN_ON_ERROR(IfxRfe_calculateNmod(f_static, f_lock, &configParams->rfFreq.nmod));
    configParams->rfFreq.nmod = 62432208U;
    configParams->rfFreq.ncw = 283073584U; // IfxRfe_calculateNcw(f_lock);
    configParams->rfFreq.bc = 1;
    configParams->rfFreq.rampbw = 8738128U; //IfxRfe_calculateRampBW(f_bw);

    //configure the tx power
    configParams->txpwr.action_mask = TxPower_ConfigurePowerLevels | TxPower_ConfigureSlopeScalingFactor;
    configParams->txpwr.ch_mask = 0xFF;  
    configParams->txpwr.tx_pa_slope_scale_fact = 256;  
    configParams->txpwr.plvl[0] = 0 << 7;
    configParams->txpwr.plvl[1] = 0 << 7;
    configParams->txpwr.plvl[2] = 0 << 7;
    configParams->txpwr.plvl[3] = 0 << 7;

    //Configure Rx Frontend
    configParams->rxcfg.gain_sel             = GainSel_0dB;
    configParams->rxcfg.data_width_sel       = DataWidth_12bits;
    configParams->rxcfg.data_rate_sel        = DataRate_1200Mbitsps;
    configParams->rxcfg.start_mode           = StartMode_Immediate; 
    
    // Calibration Parameters
    configParams->calibration.calib_sub_func_id = 0x1B00; 
    configParams->calibration.tx_ch_pow_idx     = 0xFFFFFFFF;         // enable power calibration for all power levels at all TX channels
    configParams->calibration.ref_temp_idx      = 0;                  // no reference temperature. Execute_Calibration() is based only on the LimitTemp. The user has to determine both the reported MMIC temperature and the temperature after the previous calibration.
    configParams->calibration.limit_temp        = 0;                  // Calibration shall be called if |latest temperature - reference temperature| > LimitTemp (scaled in Q12.3 format), 0: calibrate regardless of current temperature.

    // Ctrx Dmux config
    configParams->dmux.config_mask = ConfigMask_DMUX1;             //update only DMUX_1
    configParams->dmux.dmux1_alt_signal = AltSignal_DmuxALevel;     //in the ramp scenario DMUX_A is set at the end, map level, not pulse to not miss it
    configParams->dmux.dmux1_dir = DmuxDir_out;
    configParams->dmux.dmux1_pulse_duration_ext = 63;
    configParams->dmux.dmux2_dir = configParams->dmux.dmux3_dir = DmuxDir_in;
    configParams->dmux.dmux2_alt_signal = configParams->dmux.dmux3_alt_signal = 0; //set to 0 if input
    configParams->dmux.dmux2_pulse_duration_ext = configParams->dmux.dmux3_pulse_duration_ext = 63;
 
    return IFXRFE_E_SUCCESS;
}
