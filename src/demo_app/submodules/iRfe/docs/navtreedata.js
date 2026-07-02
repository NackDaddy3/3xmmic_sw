/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "iRFE", "index.html", [
    [ "CTRX iRFE (Radar Frontend) driver", "index.html", null ],
    [ "About CTRX Device", "md_docs_doc_pages__about__c_t_r_x.html", [
      [ "Application interfaces", "md_docs_doc_pages__about__c_t_r_x.html#Application_interfaces", null ],
      [ "Subsystems", "md_docs_doc_pages__about__c_t_r_x.html#Subsystems", [
        [ "Sequencer", "md_docs_doc_pages__about__c_t_r_x.html#Sequencer", null ]
      ] ],
      [ "Firmware functions in RAM", "md_docs_doc_pages__about__c_t_r_x.html#Firmware_functions", null ],
      [ "Device Configuration", "md_docs_doc_pages__about__c_t_r_x.html#Device_configuration", [
        [ "State Diagram", "md_docs_doc_pages__about__c_t_r_x.html#State_diagram", null ]
      ] ]
    ] ],
    [ "About CTRX iRFE Driver", "md_docs_doc_pages__about_page.html", [
      [ "Introduction to iRFE", "md_docs_doc_pages__about_page.html#Introduction_iRFE", [
        [ "Scope of the Driver", "md_docs_doc_pages__about_page.html#Scope", null ],
        [ "Non-scope of the driver", "md_docs_doc_pages__about_page.html#Non_scope", null ],
        [ "Requirements to develop radar application using the driver", "md_docs_doc_pages__about_page.html#Development_requirements", null ]
      ] ],
      [ "Overview of the driver", "md_docs_doc_pages__about_page.html#Overview", [
        [ "Access to the configuration interface", "md_docs_doc_pages__about_page.html#Access_config_interface", null ],
        [ "Error Macros", "md_docs_doc_pages__about_page.html#Error_macros", null ],
        [ "Convenience functions", "md_docs_doc_pages__about_page.html#Conv_functions", null ],
        [ "Additional functions", "md_docs_doc_pages__about_page.html#Add_functions", null ]
      ] ]
    ] ],
    [ "Additional Functions", "md_docs_doc_pages__additional_functions.html", [
      [ "GPIO Wrapper", "md_docs_doc_pages__additional_functions.html#GPIO_Wrapper", null ],
      [ "SPI Wrapper", "md_docs_doc_pages__additional_functions.html#SPI_Wrapper", null ]
    ] ],
    [ "CTRX Configuration interface", "md_docs_doc_pages__configuration_interface.html", [
      [ "SPI protocol", "md_docs_doc_pages__configuration_interface.html#SPI_Prtocol", [
        [ "RFT: Ready for Transfer", "md_docs_doc_pages__configuration_interface.html#RFT", null ],
        [ "Message from MCU to MMIC", "md_docs_doc_pages__configuration_interface.html#MCU_MMIC", null ],
        [ "Message from MMIC to MCU", "md_docs_doc_pages__configuration_interface.html#MMIC_MCU", null ],
        [ "Cyclic redundancy check (CRC)", "md_docs_doc_pages__configuration_interface.html#CRC", null ]
      ] ]
    ] ],
    [ "Convenience functions", "md_docs_doc_pages__convenience_functions.html", null ],
    [ "Error Handling", "md_docs_doc_pages__error__handling.html", null ],
    [ "Example Code Description", "md_docs_doc_pages__example_description.html", [
      [ "Example 1 : Infinite Loop Ramp scenario", "md_docs_doc_pages__example_description.html#Example1", null ]
    ] ],
    [ "Firmware command calls", "md_docs_doc_pages__firmware_command_calls.html", [
      [ "Available Firmware command calls with their driver interfaces", "md_docs_doc_pages__firmware_command_calls.html#Available_FW_Commands", [
        [ "Additional Firmware function calls in iRFE", "md_docs_doc_pages__firmware_command_calls.html#Additional_FW_functions", null ]
      ] ],
      [ "Programming methods using Firmware command calls", "md_docs_doc_pages__firmware_command_calls.html#Methods_of_using_FWcmd_Calls", [
        [ "Executing the firmware commands directly and blocking", "md_docs_doc_pages__firmware_command_calls.html#FW_cmds_directly", null ],
        [ "Executing the firmware commands using non blocking calls", "md_docs_doc_pages__firmware_command_calls.html#FW_cmds_non_blocking", null ],
        [ "Configuring and executing firmware command Handles", "md_docs_doc_pages__firmware_command_calls.html#FW_cmds_using_handles", null ]
      ] ]
    ] ],
    [ "Firmware commands for fw version 0.0.9-dr", "md_docs_doc_pages__firmware_commands_8181__e_s.html", null ],
    [ "Firmware commands for fw version 1.0.0-qsr.rc1", "md_docs_doc_pages__firmware_commands_8188.html", null ],
    [ "Firmware commands for fw version 0.2.0-dr", "md_docs_doc_pages__firmware_commands_8191__a11.html", null ],
    [ "Firmware commands for fw version 3.0.0-qsr.rc2", "md_docs_doc_pages__firmware_commands_8191__b11.html", null ],
    [ "Low level commands", "md_docs_doc_pages__low_level_commands.html", null ],
    [ "Operating CTRX", "md_docs_doc_pages__operating__c_t_r_x.html", [
      [ "Power up Sequence and initializing the MMIC", "md_docs_doc_pages__operating__c_t_r_x.html#Power_up_Initialize", null ],
      [ "Radar Operation", "md_docs_doc_pages__operating__c_t_r_x.html#Radar_operation", [
        [ "Low power State and configuration of MMIC", "md_docs_doc_pages__operating__c_t_r_x.html#Low_power_Config", null ],
        [ "Going to Operation(before sequencing)", "md_docs_doc_pages__operating__c_t_r_x.html#Operation", null ],
        [ "Sequencing state", "md_docs_doc_pages__operating__c_t_r_x.html#Sequencing", null ],
        [ "Operation(after sequencing)", "md_docs_doc_pages__operating__c_t_r_x.html#Operation_post_sequencing", null ],
        [ "Low power (subsequent application cycles)", "md_docs_doc_pages__operating__c_t_r_x.html#Low_power_second_cycle", null ]
      ] ],
      [ "Power Down Sequence", "md_docs_doc_pages__operating__c_t_r_x.html#Power_down", null ]
    ] ],
    [ "Spi communication setup", "md_docs_doc_pages__spi_communication_setup.html", [
      [ "SPI communication using iLLD", "md_docs_doc_pages__spi_communication_setup.html#SPI_using_iLLD", null ],
      [ "Example using PlatformSpi", "md_docs_doc_pages__spi_communication_setup.html#Example_SPI", [
        [ "Preparation", "md_docs_doc_pages__spi_communication_setup.html#Preparation", [
          [ "Include files", "md_docs_doc_pages__spi_communication_setup.html#autotoc_md6", null ],
          [ "Variables", "md_docs_doc_pages__spi_communication_setup.html#autotoc_md7", null ],
          [ "Interrupt Handler installation(with DMA use)", "md_docs_doc_pages__spi_communication_setup.html#autotoc_md8", null ],
          [ "module initialization (with dma)", "md_docs_doc_pages__spi_communication_setup.html#autotoc_md9", null ],
          [ "SPI channel Initialization", "md_docs_doc_pages__spi_communication_setup.html#autotoc_md10", null ],
          [ "Data Transfers", "md_docs_doc_pages__spi_communication_setup.html#autotoc_md11", null ]
        ] ]
      ] ]
    ] ],
    [ "Modules", "modules.html", "modules" ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Structure Index", "classes.html", null ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Variables", "functions_vars.html", "functions_vars" ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", "globals_eval" ],
        [ "Macros", "globals_defs.html", "globals_defs" ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"8181__es_2_ifx_rfe___command_param_enums_8h.html",
"8181__es_2_ifx_rfe___command_param_enums_8h.html#a5ac96f79aa0ee67af2bb2a1d7ea38446a794d7dbb5234e10466c530dfeec75c70",
"8181__es_2_ifx_rfe___command_param_enums_8h.html#aa8a28ccc07f96001fe849be37fdde70ca68148b26a459a904051271fc9e011d1e",
"8181__es_2_ifx_rfe___firmware_commands_8c.html#a14ca357b70febc706024659416f3fa2a",
"8181__es_2_ifx_rfe___firmware_commands_8c.html#af2ae963f187633f0513f3243dedaafc6",
"8181__es_2_ifx_rfe___firmware_commands_8h.html#ad02b9669fe4ccdabaf4a9dc421f9bee8",
"8188_2_ifx_rfe___command_param_enums_8h.html#a1aa2847ec1dc43e4879d0d6cf559c931a15cde410e00f29110a4d70f8b9748ec6",
"8188_2_ifx_rfe___command_param_enums_8h.html#a6bb43ceeff28f05c7df6e39554713f30",
"8188_2_ifx_rfe___command_param_enums_8h.html#ae3636a448b7a33a931721aa047805867a424b33ab3348c303a5dd303c29aaa707",
"8188_2_ifx_rfe___firmware_commands_8c.html#a4cd60333563078426c5ed9f446d55c05",
"8188_2_ifx_rfe___firmware_commands_8h.html#a1522277f79689228cb3b87d61c03fade",
"8188_2_ifx_rfe___firmware_commands_8h.html#ad79dd9bf237808ca3e02c834820cff89",
"8191__a11_2_ifx_rfe___command_param_enums_8h.html#a0402e481048e7e44e54494a3db4eb4bbaec108e2196189ea918c04f1c1ac289b4",
"8191__a11_2_ifx_rfe___command_param_enums_8h.html#a5ac96f79aa0ee67af2bb2a1d7ea38446ac582217d341b2c9cbc0e88fcbfc49444",
"8191__a11_2_ifx_rfe___command_param_enums_8h.html#abe02f9c7772a3a79de491e7483d6c9acad8c5a5d992ab2643972083e38d351844",
"8191__a11_2_ifx_rfe___firmware_commands_8c.html#a10483a266c5e0c4f50b60e4007112c60",
"8191__a11_2_ifx_rfe___firmware_commands_8c.html#ad02b9669fe4ccdabaf4a9dc421f9bee8",
"8191__a11_2_ifx_rfe___firmware_commands_8h.html#a87374d34f42a301210be44d383635662",
"8191__a11_2_ifx_rfe___firmware_commands___serdes_8h.html#a88dffe438eafdafa98260f00bd54a89f",
"8191__b11_2_ifx_rfe___command_param_enums_8h.html#a40566035c264219624f872a5e31d4cb8a7ef4e4d0c8aec441bcc928de9db5a4d3",
"8191__b11_2_ifx_rfe___command_param_enums_8h.html#a7fe61def5d1fb7b8150b9a6546ed9ee4aa0366ada8ef337705a0181bd441ef558",
"8191__b11_2_ifx_rfe___command_param_enums_8h.html#afb5bc3fc5f3b624f3e1d9cf2ab87a5cba8445ecf8fb8a291de1f72bc4dfd50ef9",
"8191__b11_2_ifx_rfe___firmware_commands_8c.html#a83ca3684d8e98a01af136a4bc7eb3bb1",
"8191__b11_2_ifx_rfe___firmware_commands_8h.html#a4752b4afde07ba28dde9a35be5db6cdb",
"8191__b11_2_ifx_rfe___firmware_commands___serdes_8c.html#a28223f9ec55a9e4e8dfa1f0067f65ade",
"_ifx_rfe___error_definitions_8h.html#a24da12d06cfde51503788295ae1b12b1",
"functions_vars.html",
"group___ifx_lld___qspi___std___enum.html#gga5902768dd3dc8004eed63c7dbd4661abaa0d632d9d76dcaa56972aaffbbea6ebc",
"struct_ifx_qspi___spi_master___config.html#ac47d2299f57cd283cbfd18a4c6cc541a",
"struct_ifx_rfe__execute_calibration_result__t.html#ab5bbaf69f6d5b94103831a30ea5c3964",
"struct_ifx_rfe__trigger_reset_result__t.html#a417171e9ef34513f55fe7444f329f11c"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';