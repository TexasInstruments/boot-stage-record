/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file bootrecord.c
 * \brief Implementation of boot record logging library
 */

/* ========================================================================== */
/*                           Include Files                                    */
/* ========================================================================== */

#include "bootrecord.h"
#include <string.h>

/* ========================================================================== */
/*                          Global Variables                                  */
/* ========================================================================== */

static boot_records_t gboot_records_config;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 * Initialize the boot records system
 */
boot_record_status_t boot_record_init(uint32_t stage_id, void *memory_addr, uint32_t size)
{
    if (!memory_addr || size < (sizeof(boot_stage_record_t) +
                             sizeof(boot_record_profile_t)))
    {
        return BOOT_RECORD_ERR_INVALID_PARAMS;
    }

    /* Clear the memory area */
    memset(memory_addr, 0, size);
    memset(&gboot_records_config, 0, sizeof(gboot_records_config));

    /* Initialize the main boot records structure */
    gboot_records_config.memory_base = memory_addr;
    gboot_records_config.memory_size = size;
    gboot_records_config.records = (boot_stage_record_t *)memory_addr;

    /* Calculate number of profile records that can fit */
    gboot_records_config.possible_records = (size - \
                                            sizeof(boot_stage_record_t)) /
                                            sizeof(boot_record_profile_t);

    if (gboot_records_config.possible_records == 0)
    {
        return BOOT_RECORD_ERR_INSUFFICIENT_MEM;
    }

    /* Initialize the boot stage record */
    boot_stage_record_t *stage = gboot_records_config.records;
    stage->record_id = stage_id;
    stage->record_count = 0;
    stage->start_time = boot_record_get_timestamp();

    return BOOT_RECORD_SUCCESS;
}

/**
 * Log a profile record with the current timestamp
 */
boot_record_status_t boot_record_log_profile(const char *name)
{
    if (!name)
    {
        return BOOT_RECORD_ERR_INVALID_PARAMS;
    }

    /* Check if boot record is initialized */
    if (!gboot_records_config.records || !gboot_records_config.memory_base)
    {
        return BOOT_RECORD_ERR_INVALID_PARAMS;
    }

    boot_stage_record_t *stage = gboot_records_config.records;

    /* Check if we've reached the maximum number of profiles */
    if (stage->record_count >= gboot_records_config.possible_records)
    {
        return BOOT_RECORD_ERR_OVERFLOW;
    }

    /* Get pointer to the next available profile record */
    boot_record_profile_t *profile = &stage->profiles[stage->record_count];

    /* Copy profile name with length limit */
    strncpy(profile->name, name, sizeof(profile->name) - 1);
    profile->name[sizeof(profile->name) - 1] = '\0'; /* Ensure null termination */

    /* Store the current time */
    profile->time = boot_record_get_timestamp();

    /* Increment profile record counter */
    stage->record_count++;

    return BOOT_RECORD_SUCCESS;
}
