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
    /* Backend file descriptor */
    int file_descriptor;
    /* Backend file path */
    char file_path[256];
    /* Backend socket path */
    char socket_path[256];
    /* Backend type */
    enum STARLOG_BACKEND backend_type;
} _starlog_backend_info_t;

/* Internal implementation that allows for quick lookup/storage of a particular backend */
typedef enum _starlog_backend_index_t
{
    /* File based backend */
    _starlog_backend_index_file = 0,
    /* UNIX Socket based backend */
    _starlog_backend_index_socket = 1,
    /* Console based backend */
    _starlog_backend_index_console = 2,
} _starlog_backend_index_t;

// Variable Definitions
/* Internal implementation used to hold the amount of allocated backends */
struct _starlog_backend_info_t _starlog_allocated_backends[3];

// Main

// Method Definitions
/**
 * Internal helper method. Checks if a given ``backend_index`` is instanced.
 *
 * @param backend_index Starlog backend index
 * @return ``boolean`` like value
 */
int _util_is_backend_instanced(const enum _starlog_backend_index_t backend_index);

/**
 * Internal helper method. Writes the given data to the specified ``backend_struct``.
 *
 * @param file_descriptor The backend's file descriptor
 * @param file_path The backends's file path
 * @param socket_path The backend's socket path
 * @param backend_type The backend's backend type
 * @param backend_struct The backend struct to write into
 * @return ``backend_struct``
 */
struct _starlog_backend_info_t _util_write_starlog_backend_info(const int file_descriptor, const char *file_path, const char *socket_path, const enum STARLOG_BACKEND backend_type, struct _starlog_backend_info_t backend_struct);

/**
 * Internal helper method. Appends the given ``backend_struct`` at the given ``backend_index``.
 *
 * @param backend_index Starlog backend index
 * @param backend_struct Starlog backend struct
 * @return void
 */
void _util_append_starlog_backend(const enum _starlog_backend_index_t backend_index, const struct _starlog_backend_info_t backend_struct);

// Methods
int _util_is_backend_instanced(const enum _starlog_backend_index_t backend_index)
{
    // Sick one-liner girl, hopefully it doesn't become an issue :P
    return strlen(_starlog_allocated_backends[backend_index].file_path) == 0 ? 0 : 1;
}

int starlog_instance_backend(const enum STARLOG_BACKEND backend_type, const char *file_path, const char *socket_path)
{
    // Match the specified backend type
    switch (backend_type)
    {
    case STARLOG_BACKEND_FILE:
        // Switch-case scope variables
        const int backend_index = _starlog_backend_index_file;

        // Check for valid inputs and non-duplicate backend instance
        if (file_path == NULL || _util_is_backend_instanced(backend_index) == 0)
        {
            // Jump to failure
            goto failure;
        }

        // Declare a new backend into type
        struct _starlog_backend_info_t backend_file_info;

        // Attempt to create a new log file with RWX permission at given file path
        int fd = open(file_path, O_CREAT | O_NONBLOCK | O_RDWR, S_IRWXU);

        // Validate log file creation
        if (fd < 0)
        {
            // Jump to failure
            goto failure;
        }

        // Store relevant backend info and save to backend array
        _util_append_starlog_backend(backend_index, _util_write_starlog_backend_info(fd, file_path, NULL, STARLOG_BACKEND_FILE, backend_file_info));

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

struct _starlog_backend_info_t _util_write_starlog_backend_info(const int file_descriptor, const char *file_path, const char *socket_path, const enum STARLOG_BACKEND backend_type, struct _starlog_backend_info_t backend_struct)
{
    // Write data to structure
    backend_struct.file_descriptor = file_descriptor;
    strcpy(backend_struct.file_path, (file_path != NULL) ? file_path : "");
    strcpy(backend_struct.socket_path, (socket_path != NULL) ? socket_path : "");
    backend_struct.backend_type = backend_type;

    // Return the newly written struct
    return backend_struct;
}

void _util_append_starlog_backend(const enum _starlog_backend_index_t backend_index, const struct _starlog_backend_info_t backend_struct)
{
    // Append to the allocated backends array
    _starlog_allocated_backends[backend_index] = backend_struct;

    // Return
    return;
}

void starlog_log_message(const char *message, ...)
{
    // Function scope variables
    va_list args_ptr;

    // Instance the argument list
    va_start(args_ptr, message);

    // Calculate the message length (+1 added to account for NULL terminator)
    const size_t message_length = vsnprintf(NULL, 0, message, args_ptr) + 1;

    // Allocate a new buffer on the stack
    char message_buffer[message_length];

    // End the argument list
    va_end(args_ptr);

    // Re-instance the argument list
    va_start(args_ptr, message);

    // Format the message
    vsnprintf(message_buffer, message_length, message, args_ptr);

    // End the argument list
    va_end(args_ptr);

    // Log to all available backends
    return;
};
