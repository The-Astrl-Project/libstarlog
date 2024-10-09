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
#include <stdlib.h>
// ---
#include "tests/dev_test.h"
#include "include/logging.h"
// ---

// ----------------------------------------------------------------

// Macro Definitions

// File Docstring
// --------------------------------
// libStarlog || tests/dev_test.c <-> tests/dev_test.h
//
// Crazy and wacky development testing. DO NOY USE AS AN ACTUAL
// TEST SUITE!
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
    // Instance a backend
    int didInit = instance_starlog_backend(LSL_BACKEND_FILE, "/home/maxine/testing.lsl", NULL);

    if (didInit == EXIT_FAILURE)
    {
        // Log
        printf("Failed to initialize logging backend!\n");

        // Exit
        return -1;
    }

    // Log a message
    log_message("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
}

// Methods
