/**
 * Copyright (c) 2024 Astrl.
 *
 * This file is part of libStarlog. It is subject to the license terms in
 * the LICENSE file found in the top-level directory of this project and at
 * https://github.com/The-Astrl-Project/libstarlog/blob/HEAD/LICENSE.
 *
 * This file may not be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE file.
 */

// Header Declarations
// ----------------------------------------------------------------
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
// ---
#include "include/logging.h"
// ---

// ----------------------------------------------------------------

// Macro Definitions

// File Docstring
// --------------------------------
// libStarlog || src/logging.c <-> include/logging.h
//
// A feature rich logging utility.
//
// @author @MaxineToTheStars <https://github.com/MaxineToTheStars>
// ----------------------------------------------------------------

// Struct Definitions

// Enum Definitions

// Type Definitions
/* Internal implementation of a starlog backend as a type struct */
typedef struct _starlog_backend_info_t
{
    int file_descriptor;
    char filepath[256];
    char socket_path[256];
    enum STARLOG_BACKEND backend_type;
};

// Variable Definitions
/* Internal implementation used to hold the amount of allocated backends */
struct _starlog_backend_info_t _allocated_backends[3];

// Main

// Methods
int instance_starlog_backend(const enum STARLOG_BACKEND backend_type, const char *filepath, const char *socket_path)
{
    // Match the specified backend type
    switch (backend_type)
    {
    case STARLOG_BACKEND_FILE:
        // Check for valid inputs and non-duplicate backend instance
        if (filepath == NULL || _allocated_backends[0].filepath != NULL)
        {
            // Jump to failure
            goto failure;
        }

        // Declare a new backend into type
        struct _starlog_backend_info_t backend_file_info;

        // Attempt to create a new log file with RWX permission at given filepath
        int fd = open(filepath, O_CREAT | O_NONBLOCK | O_RDWR, S_IRWXU);

        // Validate log file creation
        if (fd < 0)
        {
            // Jump to failure
            goto failure;
        }

        // Store backend info
        backend_file_info.file_descriptor = fd;
        strcpy(backend_file_info.filepath, filepath);
        strcpy(backend_file_info.socket_path, "");
        backend_file_info.backend_type = STARLOG_BACKEND_FILE;

        // Save to allocation array
        _allocated_backends[0] = backend_file_info;

        // Jump to success
        goto success;

    case STARLOG_BACKEND_SOCKET:
        // Jump to success
        goto success;

    case STARLOG_BACKEND_CONSOLE:
        // Jump to success
        goto success;

    default:
        // Jump to failure
        goto failure;
    };

success:
    // Return EXIT_SUCCESS
    return EXIT_SUCCESS;

failure:
    // Return EXIT_FAILURE
    return EXIT_FAILURE;
};

void log_message(const char *message, ...)
{
    // TODO: Implement
    return;
};
