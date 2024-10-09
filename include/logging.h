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
#define LSL_BACKEND_FILE 1
#define LSL_BACKEND_SOCKET 2
#define LSL_BACKEND_CONSOLE 3

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

// Main

// Methods
/**
 * Instances a new backend based on the value of ``backend_type``. Instancing multiple
 * backends of the same type results in ``EXIT_FAILURE``.
 *
 * @param backend_type The backend type to instance
 * @param output_filepath The /path/to/filename of the output file
 * @param input_filepath The /path/to/filename of the input file
 * @see ``include/logging.h - LSL_BACKEND_XXX`` for the available backends
 * @return ``EXIT_SUCCESS`` or ``EXIT_FAILURE``
 */
int instance_starlog_backend(const int backend_type, const char *output_filepath, const char *input_filepath);

/**
 * Logs the given ``message``. Internally this method uses ``vsnprintf`` for formatting
 * so it is fully compliant with string formatting flags and options.
 *
 * @param message The message to log
 * @return void - Will silently fail on error
 */
void log_message(const char *message, ...);

/**
 * Logs the given ``message``. Internally this method uses ``vsnprintf`` for formatting
 * so it is fully compliant with string formatting flags and options.
 *
 * @param message The message to log
 * @return void - Will silently fail on error
 */
void log_warning(const char *message, ...);

/**
 * Logs the given ``message``. Internally this method uses ``vsnprintf`` for formatting
 * so it is fully compliant with string formatting flags and options.
 *
 * @param message The message to log
 * @return void - Will silently fail on error
 */
void log_error(const char *message, ...);
