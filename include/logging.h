/**
 * Copyright (c) 2025 Astrl.
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
// A simple to use logging utility.
//
// @author @MaxineToTheStars <https://github.com/MaxineToTheStars>
// ----------------------------------------------------------------

// Struct Definitions
/* libStarlog's representation of a logging instance type. */
struct lsl_logging_instance_t;

// Enum Definitions
/* Available logging backends supported by libStarlog. */
enum LSL_LOGGING_BACKEND
{
    /* Write log messages to a file. */
    LSL_LOGGING_BACKEND_FILE = 0,
    /* Write log messages to a UNIX Socket that then writes the log message to a file. */
    LSL_LOGGING_BACKEND_SOCKET = 1,
    /* Write log messages to the console. */
    LSL_LOGGING_BACKEND_CONSOLE = 2,
    /* Write log messages to stdout. */
    LSL_LOGGING_BACKEND_STANDARD_OUTPUT = 3,
};

// Type Definitions

// Variable Definitions

// Main

// Methods
/**
 * Instances a new backend based on the value of ``backend_type``.
 *
 * @param backend_type The backend type to instance
 * @param file_path The file path of the output file
 * @param socket_path The file path of the socket
 * @return A ``lsl_logging_instance_t`` pointer else ``NULL``
 */
struct lsl_logging_instance_t *lsl_instance_backend(const enum LSL_LOGGING_BACKEND backend_type, const char *file_path, const char *socket_path);

/**
 * Frees the given ``backend``.
 *
 * @param backend An initialized ``lsl_logging_instance_t``
 * @return ``void``
 */
void lsl_free_backend(struct lsl_logging_instance_t *backend);

/**
 * Logs a given ``message`` to the given ``backend``.
 * This method supports formatting as well.
 *
 * @param backend An initialized ``lsl_logging_instance_t``
 * @param message The message to log
 * @return ``void``
 * @note This method will silently fail is the backend is not initialized
 */
void lsl_log_message(const struct lsl_logging_instance_t *backend, const char *message, ...);

/**
 * Logs a given info ``message`` to the given ``backend``.
 * This method supports formatting as well.
 *
 * @param backend An initialized ``lsl_logging_instance_t``
 * @param message The message to log
 * @return ``void``
 * @note This method will silently fail is the backend is not initialized
 */
void lsl_info_message(const struct lsl_logging_instance_t *backend, const char *message, ...);

/**
 * Logs a given warning ``message`` to the given ``backend``.
 * This method supports formatting as well.
 *
 * @param backend An initialized ``lsl_logging_instance_t``
 * @param message The message to log
 * @return ``void``
 * @note This method will silently fail is the backend is not initialized
 */
void lsl_warning_message(const struct lsl_logging_instance_t *backend, const char *message, ...);

/**
 * Logs a given error ``message`` to the given ``backend``.
 * This method supports formatting as well.
 *
 * @param backend An initialized ``lsl_logging_instance_t``
 * @param message The message to log
 * @return ``void``
 * @note This method will silently fail is the backend is not initialized
 */
void lsl_error_message(const struct lsl_logging_instance_t *backend, const char *message, ...);
