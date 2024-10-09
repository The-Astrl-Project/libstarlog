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
// Provides the logging utility for libStarlog.
//
// @author @MaxineToTheStars <https://github.com/MaxineToTheStars>
// ----------------------------------------------------------------

// Struct Definitions

// Enum Definitions

// Type Definitions

// Variable Definitions
char backend_file_metadata[1];
int backend_file_file_descriptor = -1;

// Main

// Methods
int instance_starlog_backend(const int backend_type, const char *output_filepath, const char *input_filepath)
{
    // Duplicate backend check
    if ((backend_file_file_descriptor > 0))
    {
        // Jump to failure
        goto failure;
    }

    // Match the backend type
    switch (backend_type)
    {
    case LSL_BACKEND_FILE:
        // Sanity check
        if ((output_filepath == NULL))
        {
            // Jump to failure
            goto failure;
        }

        // Store backend info
        backend_file_metadata[0] = *output_filepath;

        // Create a new file at filepath with RDWR flags
        backend_file_file_descriptor = open(output_filepath, O_CREAT | O_NONBLOCK | O_RDWR, S_IRWXU);

        // Validate creation
        if (backend_file_file_descriptor < 0)
        {
            // Jump to failure
            goto failure;
        }

        // Jump to success
        goto success;

    case LSL_BACKEND_SOCKET:
        // Jump to success
        goto success;

    case LSL_BACKEND_CONSOLE:
        // Jump to success
        goto success;

    default:
        break;
    }

    // Jump to failure
    goto failure;

success:
    // Return EXIT_SUCCESS
    return EXIT_SUCCESS;

failure:
    // Return EXIT_FAILURE
    return EXIT_FAILURE;
}

void log_message(const char *message, ...)
{
    // Temporary function scope variables
    va_list args_ptr;

    // Initialize the format list
    va_start(args_ptr, message);

    // Calculate the string length
    unsigned int string_length = vsnprintf(NULL, 0, message, args_ptr);

    // End the format list
    va_end(args_ptr);

    // Stack allocate a new buffer
    char string_buffer[string_length];

    // Re-initialize the format list
    va_start(args_ptr, message);

    // Format the string
    vsnprintf(string_buffer, string_length, message, args_ptr);

    // End the argument list
    va_end(args_ptr);

    // Debug
    printf(string_buffer);
}
