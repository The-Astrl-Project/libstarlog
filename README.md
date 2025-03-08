<!-- Move text down -->
<br>

<!-- Header -->
<h1 align="center">libStarlog</h1>

<!-- Subheading -->
<h3 align="center">A simple to use logging utility</h3>

<!-- Installing -->

## Installing

```bash
# Clone the repository
git clone https://github.com/The-Astrl-Project/libstarlog.git -b development --recurse-submodules && cd ./libstarlog

# Build the project
./scripts/build.sh -b

# Run tests (optional)
./scripts/build.sh -t

# Install
./scripts/build.sh -i
```

<!-- Example -->

## Example

```c
// Header files
#include <stdio.h>
#include <stdlib.h>
#include <astrl/libstarlog/logging.h>

int main()
{
    // Instance a couple backends
    struct lsl_logging_instance_t *backend_file = lsl_instance_backend(LSL_LOGGING_BACKEND_FILE, "file.log", NULL);
    struct lsl_logging_instance_t *backend_socket_server = lsl_instance_backend(LSL_LOGGING_BACKEND_SOCKET, "socket.log", "logging.sock");
    struct lsl_logging_instance_t *backend_socket_client = lsl_instance_backend(LSL_LOGGING_BACKEND_SOCKET, NULL, "logging.sock");

    // Log a standard message...
    lsl_log_message(backend_socket_client, "Hello from the client!");

    //...or maybe a critical error
    lsl_error_message(backend_file, "Something bad happened...");

    // Cleanup and exit
    lsl_free_backend(backend_file);
    lsl_free_backend(backend_socket_server);
    lsl_free_backend(backend_socket_client);
    return 0;
}
```
