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
 * \file bootrecord.h
 * \brief Boot record logging library for standardized boot time measurements
 */

#ifndef BOOT_RECORD_H
#define BOOT_RECORD_H

/* ========================================================================== */
/*                           Include Files                                    */
/* ========================================================================== */

#include <stdint.h>
#include <stddef.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * Error codes for API return values
 */
typedef int32_t boot_record_status_t;

/* Success code */
/* Operation completed successfully */
#define BOOT_RECORD_SUCCESS                 (0)
/* Error codes */
/* Invalid parameters */
#define BOOT_RECORD_ERR_INVALID_PARAMS      (-1)
/* Not enough memory */
#define BOOT_RECORD_ERR_INSUFFICIENT_MEM    (-2)
/* Record limit exceded */
#define BOOT_RECORD_ERR_OVERFLOW            (-3)

/* ========================================================================== */
/*                           Data Structures                                  */
/* ========================================================================== */

/**
 * Individual boot record profile information
 */
typedef struct
{
    /* Name of the record profile */
    char name[24];
    /* Time measurement for this profile */
    uint64_t time;
} boot_record_profile_t;

/**
 * Boot stage record structure
 */
typedef struct
{
    /* Unique identifier for this record */
    uint32_t record_id;
    /* Count of profile records in this boot stage */
    uint32_t record_count;
    /* Start time of this boot stage */
    uint64_t start_time;
    /* Array of profile records */
    boot_record_profile_t profiles[0];
} boot_stage_record_t;

/**
 * Complete boot records data structure
 */
typedef struct
{
    /* Base address of allocated memory */
    void *memory_base;
    /* Size of allocated memory */
    uint32_t memory_size;
    /* Number of possible record count */
    uint32_t possible_records;
    /* Array of boot stage records */
    boot_stage_record_t *records;
} boot_records_t;

/* ========================================================================== */
/*                          External Functions                                */
/* ========================================================================== */

/**
 * Get current timestamp - platform-dependent implementation
 * To be implemented by user for their specific platform
 *
 * \return Current timestamp value in microseconds
 */
__attribute__((weak)) uint64_t boot_record_get_timestamp(void);

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * Initialize the boot records system
 *
 * \param stage_id ID for this boot stage
 * \param memory_addr Base address for records storage
 * \param size Size of allocated memory
 * \return BOOT_RECORD_SUCCESS on success, error code on failure
 */
boot_record_status_t boot_record_init(uint32_t stage_id,
                                     void *memory_addr,
                                     uint32_t size);

/**
 * Log a profile record with the current timestamp
 *
 * \param name Name of the profile point
 * \return BOOT_RECORD_SUCCESS on success, error code on failure
 */
boot_record_status_t boot_record_log_profile(const char *name);
#endif /* BOOT_RECORD_H */