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

// Source
#define _POSIX_C_SOURCE 199309L

// Header Declarations
// ----------------------------------------------------------------
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/socket.h>
// ---
#include "include/logging.h"
#include "include/settings.h"
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
struct lsl_logging_instance_t
{
    /* The backend's file descriptor. */
    int file_descriptor;
    /* The backend's socket file descriptor. */
    int socket_descriptor;
    /* The backend's socket file path. */
    char socket_path[108];
    /* The backend's socket server thread. */
    pthread_t socket_server_thread;
    /* The backend's backend type. */
    enum LSL_LOGGING_BACKEND backend_type;
};

// Enum Definitions
/* Available logging formats for libStarlog. */
enum _lsl_formatting_option
{
    /* A standard logging message. */
    _LSL_FORMATTING_OPTION_LOG = 0,
    /* An informational message. */
    _LSL_FORMATTING_OPTION_INFO = 1,
    /* A warning message. */
    _LSL_FORMATTING_OPTION_WARN = 2,
    /* An error message. */
    _LSL_FORMATTING_OPTION_ERROR = 3,
};

// Type Definitions

// Variable Definitions

// Method Defintions
/**
 * Internal function. Handles the formatting of messages.
 *
 * @param user_message The message to format
 * @param user_formatting_args  A ``va_list`` of the formatting arguments
 * @param user_formatting_option The ``_lsl_formatting_option`` to use
 * @return A pointer to a buffer in memory else ``NULL``
 */
void *_lsl_internal_format_message(const char *user_message, va_list user_formatting_args, const enum _lsl_formatting_option user_formatting_option);

/**
 * The internal server logic for running
 * a libStarlog socket backend server.
 *
 * @param backend A pointer to a valid ``lsl_logging_instance_t``
 * @return ``void``
 */
void *_lsl_logging_backend_server(void *backend);

/**
 * Internal function. Handles the writing/sending of messages.
 *
 * @param backend A pointer to a valid ``lsl_logging_instance_t``
 * @param buffer A pointer to a valid buffer returned by ``_lsl_internal_format_message``
 * @return ``void``
 */
void _lsl_internal_write_message(const struct lsl_logging_instance_t *backend, void *buffer);

// Methods
struct lsl_logging_instance_t *lsl_instance_backend(const enum LSL_LOGGING_BACKEND backend_type, const char *file_path, const char *socket_path)
{
    // Temporary function scope variables
    int ret;

    // Switch on backend
    switch (backend_type)
    {
    case LSL_LOGGING_BACKEND_FILE:
    {
        // Check for required parameters
        if (file_path == NULL)
        {
            // Exit
            return NULL;
        }

        // Attempt to create/open a log file
        int file_descriptor = open(file_path, O_WRONLY | O_APPEND | O_CREAT | O_NONBLOCK, S_IRWXU);

        // Validate file operation
        if (file_descriptor < 0)
        {
            // Exit
            return NULL;
        }

        // Allocate a new backend instance
        struct lsl_logging_instance_t *backend = calloc(1, sizeof(struct lsl_logging_instance_t));

        // Validate allocation
        if (backend == NULL)
        {
            // Close the file
            close(file_descriptor);

            // Exit
            return NULL;
        }

        // Populate the struct
        backend->file_descriptor = file_descriptor;
        backend->socket_descriptor = -1;
        strncpy(backend->socket_path, "", (strlen("") + 1));
        backend->socket_server_thread = -1;
        backend->backend_type = backend_type;

        // Return the new backend
        return backend;
    }
    case LSL_LOGGING_BACKEND_SOCKET:
    {
        // Check for required parameters
        if (socket_path == NULL)
        {
            // Exit
            return NULL;
        }

        // Check if we're becoming a server or a client
        int is_server = access(socket_path, F_OK) != 0 ? 1 : 0;

        // Declare the socket struct
        struct sockaddr_un socket_name;

        // Prepare a new socket
        int socket_descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);

        // Validate the socket creation
        if (socket_descriptor < 0)
        {
            // Exit
            return NULL;
        }

        // Clear the socket struct
        memset(&socket_name, 0, sizeof(socket_name));

        // Populate the socket
        socket_name.sun_family = AF_UNIX;
        strncpy(socket_name.sun_path, socket_path, (strlen(socket_path) + 1));

        // Allocate a new backend instance
        struct lsl_logging_instance_t *backend = calloc(1, sizeof(struct lsl_logging_instance_t));

        // Validate allocation
        if (backend == NULL)
        {
            // Close the file
            close(socket_descriptor);

            // Exit
            return NULL;
        }

        // Finalize the connection
        if (is_server == 1)
        {
            // Check for required parameters
            if (file_path == NULL)
            {
                // Close the file
                close(socket_descriptor);

                // Free the backend
                free(backend);

                // Exit
                return NULL;
            }

            // Bind the socket
            ret = bind(socket_descriptor, (struct sockaddr *)&socket_name, sizeof(socket_name));

            // Validate the socket binding
            if (ret < 0)
            {
                // Close the socket
                close(socket_descriptor);

                // Exit
                return NULL;
            }

            // Attempt to create/open a log file
            int file_descriptor = open(file_path, O_WRONLY | O_APPEND | O_CREAT | O_NONBLOCK, S_IRWXU);

            // Validate file operation
            if (file_descriptor < 0)
            {
                // Close the socket
                close(socket_descriptor);

                // Unlink the socket
                unlink(socket_path);

                // Exit
                return NULL;
            }

            // Create a new background process
            pthread_create(&backend->socket_server_thread, NULL, _lsl_logging_backend_server, (void *)backend);

            // Populate the struct
            backend->file_descriptor = file_descriptor;
            backend->socket_descriptor = socket_descriptor;
            strncpy(backend->socket_path, socket_path, (strlen(socket_path) + 1));
            backend->backend_type = backend_type;
        }
        else
        {
            // Populate the struct
            backend->file_descriptor = -1;
            backend->socket_descriptor = socket_descriptor;
            backend->socket_server_thread = -1;
            strncpy(backend->socket_path, socket_path, (strlen(socket_path) + 1));
            backend->backend_type = backend_type;
        }

        // Return the new backend
        return backend;
    }
    case LSL_LOGGING_BACKEND_CONSOLE:
    {
        // Attempt to open /dev/console
        int file_descriptor = open("/dev/console", O_WRONLY | O_NONBLOCK);

        // Validate file operation
        if (file_descriptor < 0)
        {
            // Exit
            return NULL;
        }

        // Allocate a new backend instance
        struct lsl_logging_instance_t *backend = calloc(1, sizeof(struct lsl_logging_instance_t));

        // Validate allocation
        if (backend == NULL)
        {
            // Close the file
            close(file_descriptor);

            // Exit
            return NULL;
        }

        // Populate the struct
        backend->file_descriptor = file_descriptor;
        backend->socket_descriptor = -1;
        strncpy(backend->socket_path, "", (strlen("") + 1));
        backend->socket_server_thread = -1;
        backend->backend_type = backend_type;

        // Return the new backend
        return backend;
    }
    case LSL_LOGGING_BACKEND_STANDARD_OUTPUT:
    {
        // Allocate a new backend instance
        struct lsl_logging_instance_t *backend = calloc(1, sizeof(struct lsl_logging_instance_t));

        // Validate allocation
        if (backend == NULL)
        {
            // Exit
            return NULL;
        }

        // Populate the struct
        backend->file_descriptor = -1;
        backend->socket_descriptor = -1;
        strncpy(backend->socket_path, "", (strlen("") + 1));
        backend->socket_server_thread = -1;
        backend->backend_type = backend_type;

        // Return the new backend
        return backend;
    }
    default:
    {
        // Invalid enumerator
        return NULL;
    }
    }

    // Keep the compiler happy
    return NULL;
}

void lsl_free_backend(struct lsl_logging_instance_t *backend)
{
    // Check if the backend is initialized
    if (backend == NULL)
    {
        // Backend is not initialized
        return;
    }

    // Check for a valid server thread
    if (backend->socket_server_thread != -1)
    {
        // Close the server process
        pthread_cancel(backend->socket_server_thread);
        pthread_join(backend->socket_server_thread, NULL);

        // Unlink the socket
        unlink(backend->socket_path);
    }

    // Check for a valid socket connection
    if (backend->socket_descriptor != -1)
    {
        // Close the socket
        close(backend->socket_descriptor);
    }

    // Check for a valid file descriptor
    if (backend->file_descriptor != -1)
    {
        // Close the file
        close(backend->file_descriptor);
    }

    // Free the struct
    free(backend);

    // Set to NULL
    backend = NULL;
}

void lsl_log_message(const struct lsl_logging_instance_t *backend, const char *message, ...)
{
    // Check if the backend is initialized
    if (backend == NULL)
    {
        // Backend is not initialized
        return;
    }

    // Initialize the argument list
    va_list string_args;
    va_start(string_args, message);

    // Format the message
    void *buffer = _lsl_internal_format_message(message, string_args, _LSL_FORMATTING_OPTION_LOG);

    // Check the return buffer is valid
    if (buffer == NULL)
    {
        // Invalid buffer
        return;
    }

    // Write the message
    _lsl_internal_write_message(backend, buffer);

    // Free the buffer
    free(buffer);
}

void lsl_info_message(const struct lsl_logging_instance_t *backend, const char *message, ...)
{
    // Check if the backend is initialized
    if (backend == NULL)
    {
        // Backend is not initialized
        return;
    }

    // Initialize the argument list
    va_list string_args;
    va_start(string_args, message);

    // Format the message
    void *buffer = _lsl_internal_format_message(message, string_args, _LSL_FORMATTING_OPTION_INFO);

    // Check the return buffer is valid
    if (buffer == NULL)
    {
        // Invalid buffer
        return;
    }

    // Write the message
    _lsl_internal_write_message(backend, buffer);

    // Free the buffer
    free(buffer);
}

void lsl_warning_message(const struct lsl_logging_instance_t *backend, const char *message, ...)
{
    // Check if the backend is initialized
    if (backend == NULL)
    {
        // Backend is not initialized
        return;
    }

    // Initialize the argument list
    va_list string_args;
    va_start(string_args, message);

    // Format the message
    void *buffer = _lsl_internal_format_message(message, string_args, _LSL_FORMATTING_OPTION_WARN);

    // Check the return buffer is valid
    if (buffer == NULL)
    {
        // Invalid buffer
        return;
    }

    // Write the message
    _lsl_internal_write_message(backend, buffer);

    // Free the buffer
    free(buffer);
}

void lsl_error_message(const struct lsl_logging_instance_t *backend, const char *message, ...)
{
    // Check if the backend is initialized
    if (backend == NULL)
    {
        // Backend is not initialized
        return;
    }

    // Initialize the argument list
    va_list string_args;
    va_start(string_args, message);

    // Format the message
    void *buffer = _lsl_internal_format_message(message, string_args, _LSL_FORMATTING_OPTION_ERROR);

    // Check the return buffer is valid
    if (buffer == NULL)
    {
        // Invalid buffer
        return;
    }

    // Write the message
    _lsl_internal_write_message(backend, buffer);

    // Free the buffer
    free(buffer);
}

void *_lsl_internal_format_message(const char *user_message, va_list user_formatting_args, const enum _lsl_formatting_option user_formatting_option)
{
    // Grab the system uptime
    struct timespec ts;
    clock_gettime(CLOCK_BOOTTIME, &ts);

    // Declare the message lengths
    size_t message_prefix_length = 0;
    size_t message_suffix_length = 0;

    // Delcare the message prefix and color placeholders
    char *message_prefix_raw;
    char *message_prefix_color;

    // Calculate the message prefix length
    switch (user_formatting_option)
    {
    case _LSL_FORMATTING_OPTION_LOG:
    {
        // Calculate the message size
        message_prefix_length = snprintf(NULL, 0, "[ %s LOG %s @ %s %010ld.%03ld %s ] ", LSL_SETTINGS_LOGGING_MESSAGE_COLOR_LOG, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_TIMESTAMP, ts.tv_sec, ts.tv_nsec / 1000000, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR);

        // Set the prefix and color
        message_prefix_raw = "[ %s LOG %s @ %s %010ld.%03ld %s ] ";
        message_prefix_color = LSL_SETTINGS_LOGGING_MESSAGE_COLOR_LOG;

        // Exit
        break;
    }
    case _LSL_FORMATTING_OPTION_INFO:
    {
        // Calculate the message size
        message_prefix_length = snprintf(NULL, 0, "[ %s INF %s @ %s %010ld.%03ld %s ] ", LSL_SETTINGS_LOGGING_MESSAGE_COLOR_INFO, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_TIMESTAMP, ts.tv_sec, ts.tv_nsec / 1000000, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR);

        // Set the prefix and color
        message_prefix_raw = "[ %s INF %s @ %s %010ld.%03ld %s ] ";
        message_prefix_color = LSL_SETTINGS_LOGGING_MESSAGE_COLOR_INFO;

        // Exit
        break;
    }
    case _LSL_FORMATTING_OPTION_WARN:
    {
        // Calculate the message size
        message_prefix_length = snprintf(NULL, 0, "[ %s WRN %s @ %s %010ld.%03ld %s ] ", LSL_SETTINGS_LOGGING_MESSAGE_COLOR_WARN, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_TIMESTAMP, ts.tv_sec, ts.tv_nsec / 1000000, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR);

        // Set the prefix and color
        message_prefix_raw = "[ %s WRN %s @ %s %010ld.%03ld %s ] ";
        message_prefix_color = LSL_SETTINGS_LOGGING_MESSAGE_COLOR_WARN;

        // Exit
        break;
    }
    case _LSL_FORMATTING_OPTION_ERROR:
    {
        // Calculate the message size
        message_prefix_length = snprintf(NULL, 0, "[ %s ERR %s @ %s %010ld.%03ld %s ] ", LSL_SETTINGS_LOGGING_MESSAGE_COLOR_ERROR, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_TIMESTAMP, ts.tv_sec, ts.tv_nsec / 1000000, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR);

        // Set the prefix and color
        message_prefix_raw = "[ %s ERR %s @ %s %010ld.%03ld %s ] ";
        message_prefix_color = LSL_SETTINGS_LOGGING_MESSAGE_COLOR_ERROR;

        // Exit
        break;
    }
    default:
    {
        // Invalid enumerator
        return NULL;
    }
    }

    // Copy the formatting args
    va_list copied_formatting_args;
    va_copy(copied_formatting_args, user_formatting_args);

    // Calculate the message suffix length
    message_suffix_length = vsnprintf(NULL, 0, user_message, copied_formatting_args);

    // Re-copy the formatting args
    va_end(copied_formatting_args);
    va_copy(copied_formatting_args, user_formatting_args);

    // Allocate a new buffer of combined message lengths, a NL byte, and a NULL byte
    void *allocated_buffer = malloc((message_prefix_length + message_suffix_length + 2) * sizeof(char));

    // Format and store the messages
    sprintf(allocated_buffer, message_prefix_raw, message_prefix_color, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_TIMESTAMP, ts.tv_sec, ts.tv_nsec / 1000000, LSL_SETTINGS_LOGGING_MESSAGE_COLOR_CLEAR);
    vsnprintf((allocated_buffer + message_prefix_length), (message_suffix_length + 1), user_message, copied_formatting_args);
    memcpy((allocated_buffer + message_prefix_length + message_suffix_length), "\n\0", 2);

    // Return
    return allocated_buffer;
}

void *_lsl_logging_backend_server(void *backend)
{
    // Re-cast the backend argument
    struct lsl_logging_instance_t *backend_t = (struct lsl_logging_instance_t *)backend;

    // Run indefinitely
    for (;;)
    {
        // Declare the message struct
        struct iovec receiving_iov;
        struct msghdr receiving_msg;

        // Clear the structs
        memset(&receiving_iov, '\0', sizeof(receiving_iov));
        memset(&receiving_msg, '\0', sizeof(receiving_msg));

        // Prepare the IOVec
        receiving_iov.iov_base = NULL;
        receiving_iov.iov_len = 0;

        // Prepare the MsgHeader
        receiving_msg.msg_name = NULL;
        receiving_msg.msg_namelen = 0;
        receiving_msg.msg_iov = NULL;
        receiving_msg.msg_iovlen = 0;

        // Peak at the data and get the message length.
        size_t message_length = recvmsg(backend_t->socket_descriptor, &receiving_msg, MSG_PEEK | MSG_TRUNC);

        // Allocate a buffer on the stack
        char buffer[message_length];

        // Clear the structs
        memset(&receiving_iov, '\0', sizeof(receiving_iov));
        memset(&receiving_msg, '\0', sizeof(receiving_msg));

        // Prepare the iovec
        receiving_iov.iov_base = buffer;
        receiving_iov.iov_len = sizeof(buffer);

        // Prepare the message struct
        receiving_msg.msg_name = NULL;
        receiving_msg.msg_namelen = 0;
        receiving_msg.msg_iov = &receiving_iov;
        receiving_msg.msg_iovlen = 1;

        // Receive the message
        recvmsg(backend_t->socket_descriptor, &receiving_msg, 0);

        // Write
        write(backend_t->file_descriptor, buffer, message_length);
    }
}

void _lsl_internal_write_message(const struct lsl_logging_instance_t *backend, void *buffer)
{
    // Switch on backend
    switch (backend->backend_type)
    {
    case LSL_LOGGING_BACKEND_FILE:
    {
        // Write the buffer to the file
        write(backend->file_descriptor, buffer, (strlen(buffer) + 1));

        // Exit
        break;
    }
    case LSL_LOGGING_BACKEND_SOCKET:
    {
        // Declare the message struct
        struct iovec sending_iov;
        struct msghdr sending_msg;
        struct sockaddr_un receiving_socket;

        // Clear the structs
        memset(&sending_iov, '\0', sizeof(sending_iov));
        memset(&sending_msg, '\0', sizeof(sending_msg));
        memset(&receiving_socket, '\0', sizeof(receiving_socket));

        // Prepare the socket
        receiving_socket.sun_family = AF_UNIX;
        strncpy(receiving_socket.sun_path, backend->socket_path, (strlen(backend->socket_path) + 1));

        // Prepare the IOVec
        sending_iov.iov_base = buffer;
        sending_iov.iov_len = strlen(buffer) + 1;

        // Prepare the MsgHeader
        sending_msg.msg_name = &receiving_socket;
        sending_msg.msg_namelen = sizeof(receiving_socket);
        sending_msg.msg_iov = &sending_iov;
        sending_msg.msg_iovlen = 1;

        // Send the message
        sendmsg(backend->socket_descriptor, &sending_msg, 0);

        // Exit
        break;
    }
    case LSL_LOGGING_BACKEND_CONSOLE:
    {
        // Write the buffer to the console
        write(backend->file_descriptor, buffer, (strlen(buffer) + 1));

        // Exit
        break;
    }
    case LSL_LOGGING_BACKEND_STANDARD_OUTPUT:
    {
        // Write the buffer to STDOUT
        write(1, buffer, (strlen(buffer) + 1));

        // Exit
        break;
    }

    default:
    {
        // Invalid enumerator
        return;
    }
    }
}
