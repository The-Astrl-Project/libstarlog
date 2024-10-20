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

// ---

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
/* Available logging backends supported by libStarlog */
typedef enum STARLOG_BACKEND
{
    /* Write log messages to a file */
    STARLOG_BACKEND_FILE = 0,
    /* Write log messages to a UNIX Socket that then writes the log message to a file */
    STARLOG_BACKEND_SOCKET = 1,
    /* Write log messages to the console */
    STARLOG_BACKEND_CONSOLE = 2,
} STARLOG_BACKEND;

// Variable Definitions

// Main

// Methods
/**
 * Instances a new backend based on the value of ``backend_type``. Instancing multiple
 * backends of the same type results in ``EXIT_FAILURE``.
 *
 * @param backend_type The backend type to instance
 * @param file_path The file path of the output file
 * @param socket_path The file path of the socket
 * @return ``EXIT_SUCCESS`` or ``EXIT_FAILURE``
 */
int starlog_instance_backend(const enum STARLOG_BACKEND backend_type, const char *file_path, const char *socket_path);

/**
 * Logs the given ``message`` to all instanced backends. Internally this method uses
 * ``vsnprintf`` for formatting so it is fully compliant with string formatting flags
 * and options.
 *
 * @param message The message to log
 * @return void - On error this method will silently fail
 */
void starlog_log_message(const char *message, ...);
