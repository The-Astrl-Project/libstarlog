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
    STARLOG_BACKEND_FILE = 1,
    STARLOG_BACKEND_SOCKET = 2,
    STARLOG_BACKEND_CONSOLE = 3,
};

// Variable Definitions

// Main

// Methods
/**
 * Instances a new backend based on the value of ``backend_type``. Instancing multiple
 * backends of the same type results in ``EXIT_FAILURE``.
 *
 * @param backend_type The backend type to instance
 * @param filepath The filepath of the output file
 * @param socket_path The filepath of the socket
 * @return ``EXIT_SUCCESS`` or ``EXIT_FAILURE``
 */
int instance_starlog_backend(const enum STARLOG_BACKEND backend_type, const char *filepath, const char *socket_path);

/**
 * Logs the given ``message`` to all instanced backends. Internally this method uses
 * ``vsnprintf`` for formatting so it is fully compliant with string formatting flags
 * and options.
 *
 * @param message The message to log
 * @return void - On error this method will silently fail
 */
void log_message(const char *message, ...);
