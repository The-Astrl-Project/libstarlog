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
// Crazy and wacky development testing. DO NOT USE AS AN ACTUAL
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
    // Instance a new backend
    starlog_instance_backend(STARLOG_BACKEND_FILE, "~/testing.txt", NULL);

    // Log
    starlog_log_message("The mathematical constant pi, representing the ratio of a circle's circumference to its diameter, is approximately %.2f", 3.14);

    // Return
    return 0;
}

// Methods
