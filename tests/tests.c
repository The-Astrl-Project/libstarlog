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
#include <stdio.h>
#include <unistd.h>
// ---
#include "tests/tests.h"
#include "include/logging.h"
// ---

// ----------------------------------------------------------------

// Macro Definitions

// File Docstring
// --------------------------------
// libStarlog || tests/tests.c <-> tests/tests.h
//
// Sanity checks and tests for making sure everything works.
//
// @author @MaxineToTheStars <https://github.com/MaxineToTheStars>
// ----------------------------------------------------------------

// Struct Definitions

// Enum Definitions

// Type Definitions

// Variable Definitions

// Main
int main()
{
    // Instance backends
    struct lsl_logging_instance_t *backend_file = lsl_instance_backend(LSL_LOGGING_BACKEND_FILE, "/home/astrl/file.log", NULL);
    struct lsl_logging_instance_t *backend_socket_server = lsl_instance_backend(LSL_LOGGING_BACKEND_SOCKET, "/home/astrl/socket.log", "/home/astrl/log.socket");
    struct lsl_logging_instance_t *backend_socket_client = lsl_instance_backend(LSL_LOGGING_BACKEND_SOCKET, NULL, "/home/astrl/log.socket");
    struct lsl_logging_instance_t *backend_standard_out = lsl_instance_backend(LSL_LOGGING_BACKEND_STANDARD_OUTPUT, NULL, NULL);

    // Log
    lsl_log_message(backend_file, "Value: %-8d, Float: %.2f, String: %10s", 123, 3.14159, "FILE");
    lsl_log_message(backend_socket_server, "Value: %-8d, Float: %.2f, String: %10s", 123, 3.14159, "SOCKET - SERVER");
    lsl_log_message(backend_socket_client, "Value: %-8d, Float: %.2f, String: %10s", 123, 3.14159, "SOCKET - CLIENT");
    lsl_log_message(backend_standard_out, "Value: %-8d, Float: %.2f, String: %10s", 123, 3.14159, "STDOUT");

    // Sleep
    sleep(2);

    // Free
    lsl_free_backend(backend_file);
    lsl_free_backend(backend_socket_server);
    lsl_free_backend(backend_socket_client);
    lsl_free_backend(backend_standard_out);

    // Exit
    return 0;
}

// Methods
