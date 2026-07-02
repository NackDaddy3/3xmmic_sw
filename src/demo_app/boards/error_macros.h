/*
 * (c) (2022-2026), Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees.
 */

#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H 1

#include "IfxRfe_ErrorDefinitions.h"
#include "PlatformErrors.h"

#define EXIT_ON_IFXRFE_ERROR(expression, cleanup)                                                                                                     \
    {                                                                                                                                        \
        error_t const ret_code = expression;                                                                                                 \
        if (ret_code != IFXRFE_E_SUCCESS)                                                                                                    \
        {                                                                                                                                    \
            cleanup;                                                                                                              \
            printf("Error in function %s() in file %s:%d -> IfxRfe returned %d (0x%X)\n", __func__, __FILE__, __LINE__, ret_code, ret_code); \
            return ret_code;                                                                                                                 \
        }                                                                                                                                    \
    }

#define EXIT_ON_PLATFORM_ERROR(expression, cleanup)                                                                                                     \
    {                                                                                                                                        \
        error_t const ret_code = expression;                                                                                                 \
        if (ret_code != PLATFORM_JETSON_E_SUCCESS)                                                                                                    \
        {                                                                                                                                    \
            cleanup;                                                                                                              \
            printf("Error in function %s() in file %s:%d -> Platform returned %d (0x%X)\n", __func__, __FILE__, __LINE__, ret_code, ret_code); \
            return ret_code;                                                                                                                 \
        }                                                                                                                                    \
    }

#endif //ERROR_MACROS_H
